
#include "April/Gui/Skillbar.h"

#include "April/Utility/TimeFormatting.h"

#include "GWCA/GWCA.hpp"

#include <array>
#include <string>

using namespace April;
using namespace GW::literals;
using Threshold = April::Gui::Skillbar::Config::Threshold;
using Thresholds = April::Gui::Skillbar::Config::Thresholds;


namespace {

	auto impl_to_string_secf( GW::ms32 const time ) -> std::string
	{
		using namespace std::chrono;
		using secf = duration<float>;
		// truncating the result, thus not in TimeFormatting.h

		auto const secs = duration_cast<secf>( time );

		auto buf = std::array<char, 16>{};
		snprintf(
			buf.data(), buf.size(),
			"%.1f",
			static_cast<int>( secs.count() * 10 ) / 10.f );

		return std::string{ buf.data() };
	}

	auto to_string( GW::ms32 const time ) -> std::string
	{
		if ( time >= 10_s )
			return to_string_ss( time );

		if ( time > 0_ms )
			return impl_to_string_secf( time );

		if ( time == 0_ms )
			return "";

		else // if ( cd < 0s ) // happens occasionally due to ping
			return "0.0";      // avoid negative numbers in overlay
	}

	auto weighted_average(
		float const left, float const right, float const right_weight )
	{
		return left * (1 - right_weight) + right * right_weight;
	}

	auto weighted_average(
		ImVec4 const& left, ImVec4 const& right, float const right_weight )
	{
		return ImVec4{
			weighted_average( left.x, right.x, right_weight ),
			weighted_average( left.y, right.y, right_weight ),
			weighted_average( left.z, right.z, right_weight ),
			weighted_average( left.w, right.w, right_weight ),
		};
	}

	auto uptime_to_color(
		GW::SkillID const skill_id, Thresholds const& thresholds ) -> ImVec4
	{
		if ( thresholds.empty() )
			return Invisible();

		if ( GW::GetSkillType( skill_id ) == GW::SkillType::Hex )
			return Invisible();

		auto const time = GW::GetTimeRemaining( skill_id );

		if ( time < -1_s ) // permanent effects
			return thresholds.back().color;

		// inactive effect and negative cooldown; can happen due to ping
		if ( time <= 0_s )
			return Invisible();

		if ( time > thresholds.front().uptime )
			return thresholds.front().color;

		// max threshold > uptime > min threshold
		for ( auto it = 1u; it < thresholds.size(); ++it )
		{
			auto const& upper = thresholds[it - 1];
			auto const& lower = thresholds[it];

			if ( time <= lower.uptime )
				continue;

			auto const time_between_thresholds = upper.uptime - lower.uptime;
			auto const time_until_threshold = time - lower.uptime;

			auto const fraction =
				static_cast<float>( time_until_threshold.count() )
				/ static_cast<float>( time_between_thresholds.count() );

			auto const color =
				weighted_average( lower.color, upper.color, fraction );

			return color;
		}

		// min threshold > uptime > 0ms
		return Invisible();
	}

}


April::Gui::Skillbar::Skillbar(
	std::shared_ptr<Config>                     config,
	std::shared_ptr<FontAtlas const>            fonts,
	std::shared_ptr<ReducedSkillRecharge const> recharges )
	: config{ config }, fonts{ fonts }, recharges{ recharges }
{
}

void April::Gui::Skillbar::Display()
{
	auto const skillbar = GW::GetPlayerSkillbar();
	if ( skillbar == nullptr )
		return;

	ImGui::SetNextWindowPosCentered( ImGuiCond_FirstUseEver );

	if ( ImGui::Begin( config->window ) )
	{
		ImGui::PushFont( fonts->Get( config->font ) );
		ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
		ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { -1, -1 } );
		ImGui::PushStyleVar( ImGuiStyleVar_FrameBorderSize, 1 );

		for ( auto it = 0u; it < std::size( skillbar->skills ); ++it )
		{
			auto const& skill = skillbar->skills[it];

			ImGui::PushID( &skill );

			// cooldown-shadow
			auto const cooldown_str = to_string( GW::GetRecharge( skill ) );
			auto const cursor = ImGui::GetCursorPos();
			ImGui::SetCursorPos( cursor + ImVec2{ 1, 1 } );
			ImGui::PushStyleColor( ImGuiCol_Border, White() );
			ImGui::PushStyleColor( ImGuiCol_Text, Black() );
			{
				ImGui::Button( cooldown_str, config->skill_size );
			}
			ImGui::PopStyleColor( 2 );
			ImGui::SetCursorPos( cursor );

			// cooldown
			auto const color_bg =
				uptime_to_color( GW::GetSkillID( skill ), config->thresholds );
			auto const color_cd = (*recharges)[it] ?
				config->color_reduced_cd : config->color_cooldown;
			ImGui::PushStyleColor( ImGuiCol_Button, color_bg );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, color_bg );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, color_bg );
			ImGui::PushStyleColor( ImGuiCol_Text, color_cd );
			{
				ImGui::Button( cooldown_str, config->skill_size );
			}
			ImGui::PopStyleColor( 4 );

			if ( (it + 1) % config->skills_per_row != 0 )
				ImGui::SameLine();

			ImGui::PopID();
		}

		ImGui::PopStyleVar( 2 );
		ImGui::PopStyleColor( 3 );
		ImGui::PopFont();
	}
	ImGui::End();
}

bool April::Gui::Skillbar::Config::Threshold::operator>(
	Threshold const& other ) const
{
	return this->uptime > other.uptime;
}
