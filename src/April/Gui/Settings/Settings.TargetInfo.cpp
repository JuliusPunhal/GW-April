
#include "April/Gui/Settings/Settings.TargetInfo.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include <array>
#include <type_traits>

using namespace GW::Constants::AgentModelID;
using namespace GW::Constants::ItemModelID;


namespace {

	constexpr auto input_flags = ImGuiInputTextFlags_ReadOnly;
	constexpr auto hex = ImGuiInputTextFlags_CharsHexadecimal;

	auto const helper_agentid =
		"Fixed ID to identify an agent. This will always be the same for this "
		"kind of agent, e.g. Xunlai Chest = " + std::to_string( XunlaiChest ) +
		".\n\n"
		"The name \"Model ID\" is not strictly correct:\n"
		"Agents that look the same can have different ModelIDs (e.g. ranger "
		"spirits).";

	auto const helper_itemid =
		"Fixed ID to identify an item. This will always be the same for this "
		"item, e.g. Ecto = " + std::to_string( GlobofEctoplasm ) + ".\n\n"
		"The name \"Model ID\" is not strictly correct:\n"
		"Items that look the same can have different ModelIDs (e.g. armor "
		"head pieces with different attribute buffs).\n"
		"Also, Item Model IDs are not exclusive, two items can share the same "
		"ID, e.g. Plant Fibers = " + std::to_string( PlantFiber ) +
		" = Bramble Recurve Bow.\n";


	template<typename T>
	void draw1(
		const char* label, T input, ImGuiInputTextFlags const flags = 0 )
	{
		if constexpr ( std::is_integral_v<T> )
		{
			auto cpy = static_cast<int>( input );
			ImGui::InputInt( label, &cpy, 0, 0, input_flags | flags );
		}
		else if constexpr ( std::is_same_v<T, float> )
		{
			ImGui::InputFloat(
				label, &input, 0, 0, "%.0f", input_flags | flags );
		}
	}

	template<typename T>
	void draw2(
		const char* label,
		T const* input,
		ImGuiInputTextFlags const flags = 0 )
	{
		if constexpr ( std::is_integral_v<T> )
		{
			auto cpy = std::array<int, 2>{ input[0], input[1] };
			ImGui::InputInt2( label, cpy.data(), input_flags | flags );
		}
		else if constexpr ( std::is_same_v<T, float> )
		{
			auto cpy = std::array<float, 2>{ input[0], input[1] };
			ImGui::InputFloat2(
				label, cpy.data(), "%.0f", input_flags | flags );
		}
	}


	void draw_agent_id( GW::Agent const& agent )
	{
		draw1( "Agent ID", agent.agent_id );
		ImGui::SameLine();
		ImGui::HelpMarker( "Temporary ID to identify agents in an instance." );
	}

	void draw_pos_dist( GW::Agent const& agent )
	{
		draw2( "Position", &agent.x );

		if ( auto const* player = GW::GetCharacter() )
		{
			auto const dist = GW::GetDistance( player->pos, agent.pos );
			draw1( "Distance to Player", dist );
		}
	}

	void draw_living( GW::AgentLiving const& target )
	{
		auto const space = ImGui::GetFrameHeight() * 0.5f;

		draw_agent_id( target );
		draw1( "Agent Model ID", target.player_number );
		ImGui::SameLine();
		ImGui::HelpMarker( helper_agentid );

		ImGui::MoveCursorPosY( space );

		draw_pos_dist( target );
		draw1( "Movement Speed %", GW::GetVelocity( target ) * 100 );

		ImGui::MoveCursorPosY( space );

		auto prof = std::array<int, 2>{ target.primary, target.secondary };
		draw2( "Professions", prof.data() );

		auto health = std::array<float, 3>{
			target.hp * 100,
			static_cast<float>( GW::GetHealthPoints( target ) ),
			static_cast<float>( target.max_hp )
		};
		ImGui::InputFloat3( "Health", health.data(), "%.0f", input_flags );
		ImGui::SameLine();
		ImGui::HelpMarker( "Current %\nCurrent\nMax" );

		auto energy = std::array<float, 3>{
			target.energy * 100,
			static_cast<float>( GW::GetEnergyPoints( target ) ),
			static_cast<float>( target.max_energy )
		};
		ImGui::InputFloat3( "Energy", energy.data(), "%.0f", input_flags );
		ImGui::SameLine();
		ImGui::HelpMarker( "Current %\nCurrent\nMax" );

		ImGui::MoveCursorPosY( space );

		draw1( "Effect Flags (hex)", target.effects, hex );
		draw1( "Model State (hex)", target.model_state, hex );
		draw1( "Type Map (hex)", target.type_map, hex );
	}

	void draw_item( GW::AgentItem const& target )
	{
		auto const* item = GW::GetItemArray()[target.item_id];
		if ( item == nullptr )
			return;

		draw_agent_id( target );
		draw1( "Item ID", item->item_id );
		ImGui::SameLine();
		ImGui::HelpMarker( "Temporary ID to identify items in an instance." );

		draw1( "Item Model ID", item->model_id );
		ImGui::SameLine();
		ImGui::HelpMarker( helper_itemid );

		draw1( "Item Extra ID", item->extra_id );
		ImGui::SameLine();
		ImGui::HelpMarker( "Mainly used to differentiate dyes." );

		ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );

		draw_pos_dist( target );
	}

	void draw_gadget( GW::AgentGadget const& target )
	{
		draw_agent_id( target );
		draw1( "Gadget ID", target.gadget_id );
		ImGui::SameLine();
		ImGui::HelpMarker(
			"Fixed ID to identify a gadget. This will always be the same for "
			"this gadget, e.g. Underworld Chest = 9285." );

		ImGui::MoveCursorPosY( ImGui::GetFrameHeight() * 0.5f );

		draw_pos_dist( target );
	}

}


void April::Gui::Settings_TargetInfo::Draw()
{
	ImGui::TextWrapped(
		"This widget shows more detailed information about your current "
		"target. The content changes depending on the type of agent." );

	ImGui::SeparatorSpaced();

	auto const* target = GW::GetTarget();
	if ( auto const* living = GW::GetAsAgentLiving( target ) )
	{
		draw_living( *living );
	}
	else if ( auto const* item = GW::GetAsAgentItem( target ) )
	{
		draw_item( *item );
	}
	else if ( auto const* gadget = GW::GetAsAgentGadget( target ) )
	{
		draw_gadget( *gadget );
	}
	else ImGui::Text( "No target found!" );
}
