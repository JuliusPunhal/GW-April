
#include "April/Gui/Settings/Settings.NotifyOnEffect.h"

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"

#include <vector>

using Notification = April::Module::NotifyOnEffect::Notification;
using Notifications = April::Module::NotifyOnEffect::Notifications;


namespace {

	void draw_existing( Notifications& notifications )
	{
		auto const& col = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
		ImGui::PushStyleColor( ImGuiCol_Button, col );

		auto const& space = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { space.y, space.y } );

		auto const x_button_w = ImGui::GetFrameHeight();
		auto const width_avail =
			ImGui::GetContentRegionAvail().x
			- 2 * ImGui::GetStyle().ItemSpacing.x
			- x_button_w;

		for (
			auto it = notifications.begin(); it != notifications.end(); /**/ )
		{
			ImGui::PushID( &*it );

			ImGui::PushItemWidth( width_avail * 0.25f );
			ImGui::InputIntegral( "##ID", it->skill_id, 0 );
			ImGui::PopItemWidth();

			ImGui::SameLine();

			ImGui::PushItemWidth( width_avail * 0.75f );
			ImGui::InputText( "##Message", &it->message );
			ImGui::PopItemWidth();

			ImGui::SameLine();
			if ( ImGui::Button( "X", { x_button_w, x_button_w } ) )
			{
				it = notifications.erase( it );
			}
			else ++it;

			ImGui::PopID();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	void draw_add_new(
		Notification& new_notification, Notifications& notifications )
	{
		auto const& space = ImGui::GetStyle().ItemInnerSpacing;
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { space.y, space.y } );

		auto const add_button_w = ImGui::GetFrameHeight();
		auto const width_avail =
			ImGui::GetContentRegionAvail().x
			- 2 * ImGui::GetStyle().ItemSpacing.x
			- add_button_w;

		ImGui::PushItemWidth( width_avail * 0.25f );
		ImGui::InputIntegral( "##ID", new_notification.skill_id, 0 );
		ImGui::PopItemWidth();

		auto const* skillbar = GW::GetPlayerSkillbar();
		if ( ImGui::IsItemHovered() && skillbar )
		{
			auto const skillid = skillbar->skills[0].skill_id;
			auto const str =
				"SkillID of first Skill in Skillbar: "
				+ std::to_string( static_cast<int>( skillid ) );

			ImGui::BeginTooltip();
			{
				ImGui::PushTextWrapPos( ImGui::GetFontSize() * 35.0f );
				ImGui::TextUnformatted( str.c_str() );
				ImGui::PopTextWrapPos();
			}
			ImGui::EndTooltip();
		}

		ImGui::SameLine();

		ImGui::PushItemWidth( width_avail * 0.75f );
		ImGui::InputText( "##Message", &new_notification.message );
		ImGui::PopItemWidth();

		ImGui::SameLine();

		if (
			ImGui::Button( "+", { add_button_w, add_button_w } )
			&& not new_notification.message.empty()
			&& new_notification.skill_id != GW::SkillID::No_Skill )
		{
			notifications.push_back( new_notification );
			new_notification = {};
		}

		ImGui::PopStyleVar();
	}

}


April::Gui::Settings_NotifyOnEffect::Settings_NotifyOnEffect(
	std::shared_ptr<Module::NotifyOnEffect::Config> config )
	: config { config }
{
}

void April::Gui::Settings_NotifyOnEffect::Draw()
{
	// TODO: prefer names over skill_ids

	ImGui::TextWrapped(
		"When activated, this module will write the provided message to party "
		"chat when the specified effect is applied or removed from you. This "
		"will be only visible to you." );

	ImGui::SeparatorSpaced();

	DrawActivateButton( config->active );

	ImGui::SeparatorSpaced();

	ImGui::PushID( "On Loss" );
	{
		ImGui::Text( "When effect is removed" );
		draw_existing( config->on_loss );
		ImGui::MoveCursorPosY( 0.5f * ImGui::GetFrameHeight() );
		draw_add_new( new_notification, config->on_loss );
	}
	ImGui::PopID();

	ImGui::SeparatorSpaced();

	ImGui::PushID( "On Gain" );
	{
		ImGui::Text( "When effect is added" );
		draw_existing( config->on_gain );
		ImGui::MoveCursorPosY( 0.5f * ImGui::GetFrameHeight() );
		draw_add_new( new_notification, config->on_gain );
	}
	ImGui::PopID();
}
