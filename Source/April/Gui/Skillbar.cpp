
#include "April/Gui/Skillbar.h"

#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <optional>

using April::RGBA;
using Config = April::Gui::Skillbar::Config;
using namespace GW::Packet::StoC;
using namespace std::chrono;


namespace {

	auto can_be_reduced_by_fast_casting( GW::Skill const& info )
	{
		auto const profession = static_cast<GW::Profession>( info.profession );
		auto const type = static_cast<GW::SkillType>( info.type );

		if ( profession != GW::Profession::Mesmer )
			return false;

		if ( type != GW::SkillType::Spell
			&& type != GW::SkillType::Hex
			&& type != GW::SkillType::Enchantment )
		{
			return false;
		}

		return true;
	}

	auto get_player_fast_casting( GW::AgentID const player_id )
	{
		auto const& attrs = GW::GameContext::instance()->world->attributes;

		auto const player =
			std::find_if(
				attrs.begin(), attrs.end(),
				[player_id]( GW::PartyAttribute const& attr )
				{
					return attr.agent_id == player_id;
				} );
		if ( player == std::end( attrs ) )
			return 0u;

		return player->attribute[0].level;
	}

	auto expected_recharge(
		GW::SkillID const skill_id, GW::AgentLiving const& player )
	{
		auto const& info = GW::GetSkillConstantData( skill_id );

		auto const fc_reduction = [&player, &info]()
		{
			auto const prof = static_cast<GW::Profession>( player.primary );
			if ( prof != GW::Profession::Mesmer )
				return 1.0;

			if ( not can_be_reduced_by_fast_casting( info ) )
				return 1.0;

			return 1 - get_player_fast_casting( player.agent_id ) * 0.03;
		}();

		auto const essence =
			GW::Effects::GetPlayerEffectById(
				GW::SkillID::Essence_of_Celerity_item_effect );
		auto const bu_reduction = essence ? 0.8 : 1.0;

		// std::nearbyint() rounds half-even like GW; requires double here
		return std::nearbyint( info.recharge * fc_reduction * bu_reduction );
	}

	auto get_skill_slot( GW::SkillID const id, uint32_t const instance )
		-> std::optional<unsigned>
	{
		auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
		if ( skillbar == nullptr )
			return std::nullopt;

		auto const& skills = skillbar->skills;
		auto const found =
			std::find_if(
				std::begin( skills ),
				std::end( skills ),
				[id, instance]( GW::SkillbarSkill const& skill )
				{
					return static_cast<GW::SkillID>( skill.skill_id ) == id
						&& skill.event == instance;
				} );

		if ( found == std::end( skills ) )
			return std::nullopt;

		auto const slot = std::distance( std::begin( skills ), found );
		return slot;
	}

	auto ms_to_string_secf( milliseconds const time, const char* fmt = "%.1f" )
		-> std::string
	{
		// truncating the result, thus not in TimeFormatting.h

		auto const secs = duration_cast<duration<float>>(time);

		char buf[16];
		snprintf(
			buf, sizeof( buf ),
			fmt, static_cast<int>(secs.count() * 10) / 10.f	);

		return buf;
	}

	auto cooldown_to_string( milliseconds const time ) -> std::string
	{
		if ( time >= 10s )
			return to_string( time, April::TimeFormat<seconds>{ "%02d" } );

		if ( time > 0s )
			return ms_to_string_secf( time );

		if ( time == 0s )
			return "";

		else // if ( cd < 0s )	// happens occasionally due to ping
			return "0.0";		// avoid negative numbers in overlay
	}

	auto get_longest_effect_duration( GW::SkillID const skill_id )
		-> milliseconds
	{
		auto longest = 0ms;
		for ( auto const& effect : GW::Effects::GetPlayerEffectArray() )
		{
			if ( static_cast<GW::SkillID>( effect.skill_id ) != skill_id )
				continue;

			auto const current = GW::GetTimeRemaining( effect );
			longest = std::max( current, longest );
		}
		return longest;
	}

	constexpr auto weighted_average(
		float const left, float const right, float const right_weight )
	{
		return left * (1 - right_weight) + right * right_weight;
	}

	constexpr auto weighted_average(
		RGBA const& left, RGBA const& right, float const right_weight )
	{
		auto const r = weighted_average( left.r, right.r, right_weight );
		auto const g = weighted_average( left.g, right.g, right_weight );
		auto const b = weighted_average( left.b, right.b, right_weight );
		auto const a = weighted_average( left.a, right.a, right_weight );

		return RGBA{ r, g, b, a };
	}

	auto get_color( milliseconds const time, Config const& config )
		-> RGBA
	{
		if ( config.thresholds.size() == 0 )
			return config.color_inactive;

		// account for permanently active effects
		if ( time < -1s )
			return config.thresholds[0].color;

		// account for negative cooldown; can happen due to ping
		if ( time <= 0ms )
			return config.color_inactive;

		// uptime > max threshold
		if ( time > config.thresholds[0].time )
			return config.thresholds[0].color;

		// max threshold > uptime > min threshold
		for ( auto it = 1u; it < config.thresholds.size(); ++it )
		{
			auto const& upper = config.thresholds[it - 1];
			auto const& lower = config.thresholds[it];

			if ( time <= lower.time ) continue;

			auto const time_between_thresholds = upper.time - lower.time;
			auto const time_until_threshold = time - lower.time;

			auto const fraction =
				static_cast<float>(time_until_threshold.count())
				/ static_cast<float>(time_between_thresholds.count());

			auto const color =
				weighted_average( lower.color, upper.color, fraction );

			return color;
		}

		// min threshold > uptime > 0ms
		return config.color_inactive;
	}

}


April::Gui::Skillbar::Skillbar( Config const& config )
	:
	font_cooldown{ LoadFont( config.font_cooldown ) },
	font_uptime{ LoadFont( config.font_uptime ) }
{
}

void April::Gui::Skillbar::Display( Config const& config ) const
{
	// Get GW-data
	auto const skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
	if ( skillbar == nullptr ) return;

	// transform
	struct Slot {
		std::string cooldown;
		std::string uptime;
		RGBA		color;
	};

	auto slots = std::array<Slot, 8>{};
	for ( auto it = 0; it < 8; ++it )
	{
		// cooldown
		auto const cooldown = GetRecharge( skillbar->skills[it] );
		slots[it].cooldown = cooldown_to_string( cooldown );

		// effect-uptime
		auto const raw_id = skillbar->skills[it].skill_id;
		auto const& data = GW::SkillbarMgr::GetSkillConstantData( raw_id );
		if ( data.type == static_cast<uint32_t>( GW::SkillType::Hex ) )
		{
			slots[it].color = Invisible();
			continue;
		}

		auto const skill_id = static_cast<GW::SkillID>( raw_id );
		auto const longest = get_longest_effect_duration( skill_id );
		slots[it].uptime =
			config.show_uptime ? cooldown_to_string( longest ) : "";
		slots[it].color = get_color( longest, config );
	}

	// Draw
	if ( ImGui::Begin( config.window ) )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, config.spacing );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
		ImGui::PushStyleColor( ImGuiCol_Text, config.text_color );
		ImGui::PushStyleColor( ImGuiCol_Border, config.border_color );

		auto const rows_per_col = 8/config.skills_per_row;
		auto const height = ImGui::GetContentRegionAvail().y / rows_per_col;
		for ( auto it = 0; it < 8; ++it )
		{
			auto const& skill = slots[it];

			ImGui::PushID( &skill );
			ImGui::PushStyleColor( ImGuiCol_Button, skill.color );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, skill.color );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, skill.color );
			{
				auto const cursor_start = ImGui::GetCursorPos();
				ImGui::PushFont( font_cooldown );

				// shadow
				ImGui::SetCursorPos( cursor_start + XY{ 1, 1 } );
				ImGui::PushStyleColor( ImGuiCol_Text, Black() );
				ImGui::Button( skill.cooldown, { height, height } );
				ImGui::PopStyleColor();
				ImGui::SetCursorPos( cursor_start );

				// label
				if ( hsr[it] )
				{
					ImGui::PushStyleColor( ImGuiCol_Text, config.hsr_color );
					ImGui::Button( skill.cooldown, { height, height } );
					ImGui::PopStyleColor();
				}
				else ImGui::Button( skill.cooldown, { height, height } );
				ImGui::PopFont();

				if ( (it + 1) % config.skills_per_row != 0 )
					ImGui::SameLine();

				if ( config.show_uptime )
				{
					auto const cursor_next = ImGui::GetCursorPos();
					ImGui::SetCursorPos( cursor_start );
					ImGui::PushFont( font_uptime );
					ImGui::SetCursorPos( cursor_start + config.uptime_offset );
					ImGui::Text( skill.uptime );
					ImGui::PopFont();

					ImGui::SetCursorPos( cursor_next );
				}
			}
			ImGui::PopStyleColor( 3 );
			ImGui::PopID();
		}
		ImGui::PopStyleColor( 2 );
		ImGui::PopStyleVar( 2 );
	}
	ImGui::End();
}

void April::Gui::Skillbar::UpdateHSR( SkillRecharge const& packet )
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return;

	auto const skill_id = static_cast<GW::SkillID>( packet.skill_id );
	if ( packet.recharge < expected_recharge( skill_id, *player ) )
	{
		auto const slot = get_skill_slot( skill_id, packet.skill_instance );

		if ( slot == std::nullopt )
			return;

		hsr[*slot] = true;
	}
}

void April::Gui::Skillbar::UpdateHSR( SkillRecharged const& packet )
{
	auto const* player = GW::Agents::GetCharacter();
	if ( player == nullptr || player->agent_id != packet.agent_id )
		return;

	auto const skill_id = static_cast<GW::SkillID>( packet.skill_id );
	auto const slot = get_skill_slot( skill_id, packet.skill_instance );

	if ( slot == std::nullopt )
		return;

	hsr[*slot] = false;
}

void April::Gui::Skillbar::UpdateHSR( MapLoaded const& )
{
	hsr = {};
}

auto April::Gui::Skillbar::Config::LoadDefault() -> Config
{
	constexpr auto window_flags =
		ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoBackground
		| ImGuiWindowFlags_NoMouseInputs
		| ImGuiWindowFlags_NoFocusOnAppearing
		| ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoNavInputs
		| ImGuiWindowFlags_NoNavFocus;

	return Config{
		{ "C:\\Windows\\Fonts\\Gothic.ttf", 30 },
		{ "C:\\Windows\\Fonts\\Gothic.ttf", 14 },
		false,
		{ 3, -2 },

		White(),
		Green(),
		White(),
		{ -1, -1 },
		8,

		{
			{ 4s, Green( 0.25f ) },
			{ 3s, Yellow( 0.25f ) },
			{ 1s, Red( 0.25f ) },
			{ 0s, Red( 0.25f ) }
		},
		Invisible(),

		{ "Skillbar", true, window_flags }
	};
}
