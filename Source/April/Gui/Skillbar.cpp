
#include "April/Gui/Skillbar.h"

#include "April/Utility/TimeFormatting.h"

#include "Dependencies/GWCA.hpp"
#include "Dependencies/ImGui.hpp"

#include <array>

using April::RGBA;
using Config = April::Gui::Skillbar::Config;
using namespace std::chrono;


namespace {
		
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
	: config{ config }
{
}

void April::Gui::Skillbar::Display() const
{
	// Get GW-data
	auto const skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
	if ( skillbar == nullptr ) return;

	// transform
	struct Skill {
		std::string label;
		RGBA		color;
	};

	auto skills = std::array<Skill, 8>{};
	for ( auto it = 0; it < 8; ++it )
	{
		// label (cooldown)
		auto const cooldown = GetRecharge( skillbar->skills[it] );
		skills[it].label = cooldown_to_string( cooldown );

		// color (effect-uptime)
		auto const raw_id = skillbar->skills[it].skill_id;
		auto const& data = GW::SkillbarMgr::GetSkillConstantData( raw_id );
		if ( data.type == static_cast<uint32_t>( GW::SkillType::Hex ) )
		{
			skills[it].color = Invisible();
			continue;
		}

		auto const skill_id = static_cast<GW::SkillID>( raw_id );
		auto const longest = get_longest_effect_duration( skill_id );
		skills[it].color = get_color( longest, config );
	}

	// Draw
	ImGui::Begin( config.window_name, config.window_flags );
	ImGui::PushFont( config.font );
	ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, config.spacing );
	ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );
	ImGui::PushStyleColor( ImGuiCol_Text, config.text_color );
	ImGui::PushStyleColor( ImGuiCol_Border, config.border_color );
	{
		auto const height = ImGui::GetContentRegionAvail().y;
		for ( auto const& skill : skills )
		{
			ImGui::PushID( &skill );
			ImGui::PushStyleColor( ImGuiCol_Button, skill.color );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, skill.color );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, skill.color );
			{
				ImGui::Button( skill.label, { height, height } );
				ImGui::SameLine();
			}
			ImGui::PopStyleColor( 3 );
			ImGui::PopID();
		}
	}
	ImGui::PopStyleColor( 2 );
	ImGui::PopStyleVar( 2 );
	ImGui::PopFont();
	ImGui::End();
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

	auto const config = Config{
		LoadFont( "C:\\Windows\\Fonts\\Gothic.ttf", 30 ),
		White(),
		White(),
		{ -1, -1 },

		{
			{ 4s, Green( 0.25f ) },
			{ 3s, Yellow( 0.25f ) },
			{ 1s, Red( 0.25f ) },
			{ 0s, Red( 0.25f ) }
		},
		Invisible(),

		"Skillbar",
		window_flags
	};

	return config;
}
