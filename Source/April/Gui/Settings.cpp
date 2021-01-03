
#include "April/Gui/Settings.h"

#include "April/Framework/Modules.h"
#include "April/Framework/WndProc.h"
#include "April/Utility/FileIO.h"

#include "Dependencies/ImGui.hpp"

#include <algorithm>
#include <chrono>
#include <limits>
#include <string>

using namespace std::chrono;


namespace {

	void draw_visibility( April::Window& window )
	{
		ImGui::PushID( &window );
		ImGui::Checkbox( "##visibility", &window.visible );
		ImGui::PopID();
	}

	void draw_settings( April::Window& window )
	{
		ImGui::PushID( &window );

		if ( auto* wnd = ImGui::FindWindowByName( window.name.c_str() ); wnd )
		{
			// Window Position
			ImGui::AlignTextToFramePadding();
			ImGui::DragFloat2( "Position", &wnd->Pos.x, 1, 0, 0, "%.0f" );

			// Window Size
			if ( (window.flags & ImGuiWindowFlags_AlwaysAutoResize) == 0 )
			{
				ImGui::AlignTextToFramePadding();
				ImGui::DragFloat2( "Size", &wnd->SizeFull.x, 1, 0, 0, "%.0f" );
			}
			else ImGui::Text( "Turn off Auto Resize to define size" );
		}
		else
		{
			ImGui::Text(
				"Cannot find Size- and Position-Info for Window %s!",
				window.name.c_str() );
		}

		// Window Flags
		auto const draw = [&window]( const char* str, unsigned const flag )
		{
			auto const is_set = (window.flags & flag) == flag;

			auto checked = is_set;
			if ( ImGui::Checkbox( str, &checked ) )
			{
				is_set ? window.flags &= ~flag : window.flags |= flag;
			}
		};

		ImGui::Columns( 2, 0, false );
		draw( "Auto Resize",		ImGuiWindowFlags_AlwaysAutoResize );
		draw( "No Movement",		ImGuiWindowFlags_NoMove );
		draw( "No Resizing",		ImGuiWindowFlags_NoResize );
		draw( "No Titlebar",		ImGuiWindowFlags_NoTitleBar );
		draw( "No Collapsing",		ImGuiWindowFlags_NoCollapse );
		draw( "No Scrolling",		ImGuiWindowFlags_NoScrollWithMouse );
		ImGui::NextColumn();
		draw( "No Scrollbar",		ImGuiWindowFlags_NoScrollbar );
		draw( "Force Scrollbar",	ImGuiWindowFlags_AlwaysVerticalScrollbar );
		draw( "Enable H-Scrollbar", ImGuiWindowFlags_HorizontalScrollbar );
		draw( "Disable Mouse",		ImGuiWindowFlags_NoMouseInputs );
		draw( "Disable Keyboard",	ImGuiWindowFlags_NoNavInputs );
		draw( "Opaque Background",	ImGuiWindowFlags_NoBackground );
		ImGui::Columns();

		ImGui::PopID();
	}

	void draw_window_settings_collapsing_header( April::Window& window )
	{
		ImGui::PushID( &window );
		if ( ImGui::CollapsingHeader( "Window Settings" ) )
		{
			ImGui::Indent();
			draw_settings( window );
			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	template<typename Config_t>
	void draw_save_discard( Config_t& config )
	{
		ImGui::PushID( &config );

		if ( ImGui::Button( "Save" ) )
		{
			April::IO::to_file( config, config.path );
		}

		ImGui::SameLine();

		if ( ImGui::Button( "Discard" ) )
		{
			if ( auto cfg = April::IO::from_file<Config_t>( config.path ); cfg )
			{
				config = *std::move( cfg );
			}
		}

		ImGui::SameLine();

		if ( ImGui::Button( "Restore Default" ) )
		{
			config = Config_t::LoadDefault();
		}

		ImGui::PopID();
	}

	template<typename Config_t>
	void draw_window_settings( Config_t& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();
			draw_save_discard( config );
			draw_window_settings_collapsing_header( config.window );
			ImGui::Unindent();
		}
		ImGui::PopID();
	}


	void draw( April::ConsumablesMgr::Config& config )
	{
		constexpr auto consumables_mgr_warning =
			"CAUTION! If this is defined as (close to) zero, April will "
			"spam your consumable once per frame until the client recieves "
			"the info from the server that the effect has been activated!"
			"\n\n"
			"Explanation: This timeout does not define the seemingly "
			"equivalent setting in GWToolboxpp: \n"
			"April does not use all activated consumables at once. This "
			"timeout will define how much time shall pass after successfully "
			"activating one consumable before trying to use the next one. "
			"This check happens once every frame. \n"
			"Therefore, if this timeout has expired before the server "
			"notifies your client that the effect has been activated, the "
			"same consumable will be used again. At 0ms timeout, with 60FPS "
			"and a ping of 200, this can be approximately 24 consumables.";

		if ( ImGui::CollapsingHeader( "Consumables" ) )
		{
			ImGui::Indent();
			draw_save_discard( config );

			auto buf = static_cast<int>( config.timeout.count() );
			if ( ImGui::InputInt( "Timeout (ms)", &buf ) )
			{
				config.timeout = milliseconds{ buf };
			}

			ImGui::TextWrapped( consumables_mgr_warning );
			ImGui::Unindent();
		}
	}

	void draw( April::AgentFilter::Config& config )
	{
		auto const draw = []( auto& rarities, auto& ids )
		{
			auto const box = [&]( auto const label, auto const rarity )
			{
				auto const it = std::find( rarities, rarity );

				auto visible = (it != rarities.end());
				if ( ImGui::Checkbox( label, &visible ) )
				{
					if ( it == rarities.end() )
						rarities.emplace_back( rarity );
					else
						rarities.erase( it );
				}
			};

			box( "White", GW::Rarity::White );		ImGui::SameLine();
			box( "Blue", GW::Rarity::Blue );		ImGui::SameLine();
			box( "Purple", GW::Rarity::Purple );	ImGui::SameLine();
			box( "Gold", GW::Rarity::Gold );		ImGui::SameLine();
			box( "Green", GW::Rarity::Green );


			for ( auto it = ids.begin(); it != ids.end(); /**/ )
			{
				ImGui::PushID( &*it );

				auto id = static_cast<int>( *it );
				if ( ImGui::InputInt( "", &id ) )
				{
					*it = id;
				}

				ImGui::SameLine();
				if ( ImGui::Button( "Remove" ) )
				{
					it = ids.erase( it );
				}
				else ++it;

				ImGui::PopID();
			}

			ImGui::Text( "\n" );
			static auto new_id = 0;

			if ( ImGui::Button( "Add ID" ) && new_id > 0 )
			{
				ids.emplace_back( static_cast<GW::ItemID>( new_id ) );
				new_id = 0;
			}
			ImGui::SameLine();
			ImGui::InputInt( "", &new_id );

		};


		if ( ImGui::CollapsingHeader( "Agent Filter" ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			ImGui::Checkbox( "Activate Agent Filter", &config.active );

			if ( ImGui::CollapsingHeader( "Visible Player Items" ) )
			{
				ImGui::Indent();
				ImGui::PushID( "Player" );
				draw(
					config.visible_user_rarities,
					config.visible_user_items );

				auto const* item = GW::Items::GetItemBySlot( 1, 1 );
				ImGui::Text(
					"ID in first Inventory Slot: %d",
					item ? item->model_id : 0 );

				ImGui::PopID();
				ImGui::Unindent();
			}
			if ( ImGui::CollapsingHeader( "Visible Party Items" ) )
			{
				ImGui::Indent();
				ImGui::PushID( "Party" );
				draw(
					config.visible_party_rarities,
					config.visible_party_items );

				auto const* item = GW::Items::GetItemBySlot( 1, 1 );
				ImGui::Text(
					"ID in first Inventory Slot: %d",
					item ? item->model_id : 0 );

				ImGui::PopID();
				ImGui::Unindent();
			}

			ImGui::Unindent();
		}
	}

	void draw( April::ChatCommands::Config& config )
	{
		if ( ImGui::CollapsingHeader( "Chat Commands" ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			ImGui::PushItemWidth( ImGui::GetContentRegionAvail().x * 0.25f );

			auto const draw_cmd = []( char const* label, auto& str )
			{
				char buf[128] = {};
				str.copy( buf, sizeof( buf ) );
				buf[127] = '\0';

				if ( ImGui::InputText(
					label,
					buf, sizeof( buf ),
					ImGuiInputTextFlags_CharsNoBlank ) )
				{
					str = std::string{ std::begin( buf ), std::end( buf ) };
				}
			};

			draw_cmd( "Sent to Teamchat", config.sendchat );
			draw_cmd( "Write to Chat", config.writechat );
			draw_cmd( "Open Xunlai", config.openxunlai );
			draw_cmd( "Activate Consumables", config.activate_pcons );
			draw_cmd( "Deactivate Consumables", config.deactivate_pcons );
			draw_cmd( "Activate permanently", config.activate_persistent );
			draw_cmd( "Dectivate permanently", config.deactivate_persistent );
			draw_cmd( "Set objective", config.set_deactivating_objective );
			draw_cmd( "Toggle Gui", config.toggle_gui );
			draw_cmd( "Shut Down", config.exit );

			ImGui::Separator();

			for (
				auto it = config.abbreviations.begin();
				it != config.abbreviations.end();
				/**/ )
			{
				ImGui::PushID( &*it );

				auto const remove = ImGui::Button( "Remove" );

				char abbrev[128] = {};
				it->abbreviation.copy( abbrev, sizeof( abbrev ) );
				abbrev[127] = '\0';

				ImGui::SameLine();
				if ( ImGui::InputText( "", abbrev, sizeof( abbrev ) ) )
				{
					it->abbreviation = abbrev;
				}

				char original[128] = {};
				it->original.copy( original, sizeof( original ) );
				original[127] = '\0';

				ImGui::SameLine();
				if (
					ImGui::InputText(
						"Abbreviation|Expanded",
						original, sizeof( original ) ) )
				{
					it->original = original;
				}

				remove ? it = config.abbreviations.erase( it ) : ++it;

				ImGui::PopID();
			}

			ImGui::Text( "\n" );
			auto const add = ImGui::Button( "Add new Abbreviation" );
			static char new_abbrev[128] = {};
			static char new_original[128] = {};

			ImGui::InputText( "", new_abbrev, sizeof( new_abbrev ) );
			ImGui::SameLine();
			ImGui::InputText(
				"Abbreviation|Expanded", new_original, sizeof( new_original ) );

			if ( add )
			{
				using Abbrev = April::ChatCommands::Config::Abbreviation;
				config.abbreviations.emplace_back(
					Abbrev{ new_abbrev, new_original } );

				std::fill_n( new_abbrev, sizeof( new_abbrev ), '\0' );
				std::fill_n( new_original, sizeof( new_original ), '\0' );
			}

			ImGui::PopItemWidth();
			ImGui::Unindent();
		}
	}

	void draw( April::ChatFilter::Config& config )
	{
		if ( ImGui::CollapsingHeader( "Chat Filter" ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			ImGui::Text( "Block checked messages" );
			ImGui::Checkbox( "Rare drops for player", &config.self_drop_rare );
			ImGui::Checkbox( "Common drops for player", &config.self_drop_common );
			ImGui::Checkbox( "Player pciking up item", &config.self_pickup );

			ImGui::Checkbox( "Common drop for ally", &config.ally_drop_common );
			ImGui::Checkbox( "Rare drop for ally", &config.ally_drop_rare );
			ImGui::Checkbox( "Ally picks up common", &config.ally_pickup_common );
			ImGui::Checkbox( "Ally picks up rare", &config.ally_pickup_rare );

			ImGui::Checkbox( "Player is away", &config.away );
			ImGui::Checkbox( "Player is dnd", &config.dnd );
			ImGui::Checkbox( "No one hears you", &config.noone_hears_you );

			ImGui::Checkbox( "Player joined the game", &config.player_joined );
			ImGui::Checkbox( "Player achieved title", &config.player_achieved_title );
			ImGui::Checkbox( "You have been playing for", &config.you_have_been_playing_for );
			ImGui::Checkbox( "Favor of gods", &config.favor_of_gods );
			ImGui::Checkbox( "Screenshot taken", &config.screenshot_taken );

			ImGui::Checkbox( "Gained experience", &config.gain_exp );
			ImGui::Checkbox( "Player gained skillpoint", &config.self_gain_skillpoint );
			ImGui::Checkbox( "Ally gained skillpoint", &config.ally_gain_skillpoint );
			ImGui::Checkbox( "Quest updated", &config.quest_updated );
			ImGui::Checkbox( "Quest reward accepted", &config.quest_reward );

			ImGui::Checkbox( "Invalid Target", &config.invalid_target );
			ImGui::Checkbox( "Not enough Energy", &config.not_enough_energy );

			ImGui::Checkbox( "Item cant be used", &config.item_cannot_be_used );
			ImGui::Checkbox( "Summon already in instance", &config.summon_in_instance );
			ImGui::Checkbox( "Inventory is full", &config.inventory_is_full );
			ImGui::Checkbox( "Item already identified", &config.item_already_identified );
			ImGui::Checkbox( "Salvage results", &config.salvage_results );

			ImGui::Checkbox( "Challenge-Mission messages", &config.challenge_mission_messages );
			ImGui::Checkbox( "Faction gained", &config.faction_gain );
			ImGui::Checkbox( "Skill updated for pvp", &config.skill_to_pvp );
			ImGui::Checkbox( "XYZ won battle in hall of heros", &config.hall_of_heros );

			ImGui::Checkbox( "Emotes", &config.emotes );
			ImGui::Checkbox( "Guild annoucement", &config.guild_announcement );

			ImGui::Checkbox( "Opening chest revealed...", &config.chest_reveal );
			ImGui::Checkbox( "Locked chest messages", &config.opening_chest_messages );

			ImGui::Checkbox( "Lunar messages", &config.lunars );
			ImGui::Checkbox( "Nine Rings messages", &config.ninerings );

			ImGui::Unindent();
		}
	}

	void draw( April::NotifyEffectLoss::Config& config )
	{
		if ( ImGui::CollapsingHeader( "Notify Effect Lost" ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			for (
				auto it = config.notifications.begin();
				it != config.notifications.end();
				/**/ )
			{
				ImGui::PushID( &*it );

				char label[64] = {};
				snprintf(
					label, sizeof( label ),
					"%5d", static_cast<int>( it->skill_id ) );

				ImGui::InputText(
					label, it->message.data(), it->message.capacity() );

				ImGui::SameLine();
				if ( ImGui::Button( "Remove" ) )
				{
					it = config.notifications.erase( it );
				}
				else ++it;

				ImGui::PopID();
			}

			ImGui::Text( "Add another Effect" );
			static int new_id = 0;
			static char msg[140] = {};

			ImGui::InputInt( "SkillID", &new_id );
			ImGui::InputText( "Message", msg, sizeof( msg ) );
			if ( ImGui::Button( "Add" ) && new_id != 0 )
			{
				using Notification =
					April::NotifyEffectLoss::Config::Notification;

				config.notifications.emplace_back(
					Notification{ static_cast<GW::SkillID>( new_id ), msg } );

				new_id = 0;
				std::fill_n( msg, sizeof( msg ), '\0' );
			}

			auto const* skillbar = GW::SkillbarMgr::GetPlayerSkillbar();
			if ( skillbar )
			{
				auto const skillid = skillbar->skills[0].skill_id;
				auto const id = static_cast<int>( skillid );
				ImGui::Text( "SkillID of first Skill in Skillbar: %d", id );
			}

			ImGui::Unindent();
		}
	}

	void draw( April::ReturnToOutpost::Config& config )
	{
		if ( ImGui::CollapsingHeader( "Return to Outpost" ) )
		{
			ImGui::Indent();
			draw_save_discard( config );

			ImGui::Checkbox(
				"Automatically return to outpost when party is defeated",
				&config.active );
			ImGui::Unindent();
		}
	}

	void draw( April::ShowKitUses::Config& config )
	{
		constexpr auto kit_uses_warning =
			"Warning!\n"
			"This changes the quantity the client assumes this item has. "
			"Therefore, it will also show the incorrect quantity at e.g. a "
			"merchant. If you want to sell the kit sure to turn this off "
			"and reset the client by reloading the map. \n"
			"When selling stacked items, a different packet is sent to the "
			"server, therefore you'd be sending packets a client could not "
			"normally send.\n"
			"Also, the server does not accept the packet so this could not "
			"be abused as a money-hack anyway.";

		if ( ImGui::CollapsingHeader( "Show Kit Uses" ) )
		{
			ImGui::Indent();
			draw_save_discard( config );

			ImGui::Checkbox(
				"Show remaining uses of Ident- & Salvage-Kits as quantity",
				&config.active );

			ImGui::TextWrapped( kit_uses_warning );
			ImGui::Unindent();
		}
	}

	void draw( April::SuppressSpeechBubbles::Config& config )
	{
		if ( ImGui::CollapsingHeader( "Speech Bubbles" ) )
		{
			ImGui::Indent();
			draw_save_discard( config );
			ImGui::Checkbox( "Suppress Speech Bubbles", &config.active );
			ImGui::Unindent();
		}
	}

	void draw( April::Gui::ChainedSoulGui::Config& config )
	{
		draw_window_settings( config );
	}

	void draw( April::Gui::DhuumBotGui::Config& config )
	{
		draw_window_settings( config );
	}

	void draw( April::Gui::DhuumInfo::Config& config )
	{
		draw_window_settings( config );
	}

	void draw( April::Gui::Dialogs::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();
			draw_save_discard( config );
			draw_window_settings_collapsing_header( config.window );

			ImGui::PushID( 1 );
			if ( ImGui::CollapsingHeader( "Dialogs" ) )
			{
				ImGui::Indent();
				for (
					auto dialog = config.dialogs.begin();
					dialog != config.dialogs.end();
					/**/ )
				{
					ImGui::PushID( &*dialog );

					char buf[128] = {};
					sprintf_s(
						buf, sizeof( buf ), "%s", dialog->description.c_str() );

					if ( ImGui::InputText(
						"Description",
						buf,
						sizeof( buf ) ) )
					{
						dialog->description = buf;
					}

					ImGui::InputInt(
						"DialogID (Hex)",
						&dialog->dialog,
						1,
						100,
						ImGuiInputTextFlags_CharsHexadecimal );
					if ( ImGui::Button( "Remove" ) )
					{
						dialog = config.dialogs.erase( dialog );
					}
					else ++dialog;

					ImGui::SetCursorPosY( ImGui::GetCursorPos().y + 5 );
					ImGui::PopID();
				}

				ImGui::PushID( "New Dialog" );

				static char description[128] = {};
				static int	id = 0;

				ImGui::Text( "Add new Dialog" );
				ImGui::InputText(
					"Description", description, sizeof( description ) );
				ImGui::InputInt(
					"Dialog (Hex)",
					&id, 1, 100,
					ImGuiInputTextFlags_CharsHexadecimal );

				if ( ImGui::Button( "Apply" ) )
				{
					using Dialog = April::Gui::Dialogs::Config::Dialog;
					config.dialogs.emplace_back( Dialog{ description, id } );
					std::fill_n( description, sizeof( description ), '\0' );
					id = 0;
				}

				ImGui::PopID();

				ImGui::Unindent();
			}
			ImGui::Unindent();
			ImGui::PopID();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::Energybar::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			draw_window_settings_collapsing_header( config.window );

			if ( ImGui::CollapsingHeader( "Style Settings" ) )
			{
				ImGui::Indent();

				ImGui::ColorEdit4(
					"Border",
					&config.border_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Background",
					&config.background_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Energy",
					&config.colors.standard.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::DragFloat2(
					"Left Pips Alignment",
					&config.alignment.left.x, 0.01f, 0, 1, "%.2f" );

				ImGui::DragFloat2(
					"Right Pips Alignment",
					&config.alignment.right.x, 0.01f, 0, 1, "%.2f" );

				ImGui::Unindent();
			}
			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::Healthbar::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			draw_window_settings_collapsing_header( config.window );

			if ( ImGui::CollapsingHeader( "Style Settings" ) )
			{
				ImGui::Indent();

				ImGui::ColorEdit4(
					"Border",
					&config.border_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Background",
					&config.background_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Health",
					&config.colors.standard.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Health (Hexed)",
					&config.colors.degen_hexed.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Health (Deep Wound)",
					&config.colors.deep_wounded.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Health (Bleeding)",
					&config.colors.bleeding.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Health (Poisoned)",
					&config.colors.poisoned.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::DragFloat2(
					"Left Pips Alignment",
					&config.alignment.left.x, 0.01f, 0, 1, "%.2f" );

				ImGui::DragFloat2(
					"Right Pips Alignment",
					&config.alignment.right.x, 0.01f, 0, 1, "%.2f" );

				ImGui::Unindent();
			}
			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::InstanceTimer::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			draw_window_settings_collapsing_header( config.window );

			if ( ImGui::CollapsingHeader( "Style" ) )
			{
				ImGui::ColorEdit4(
					"Label",
					&config.text_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Shadow",
					&config.shadow_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::DragFloat2(
					"Shadow Offset",
					&config.shadow_offset.x, 0.5f, -500, 500, "%.0f" );
			}

			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::Inventory::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			draw_window_settings_collapsing_header( config.window );

			if ( ImGui::CollapsingHeader( "Style" ) )
			{
				ImGui::Indent();
				ImGui::DragFloat2(
					"Slot Size",
					&config.slot_size.w, 0.25f, 0, 0, "%.0f" );

				ImGui::DragFloat2(
					"Spacing",
					&config.item_spacing.x, 0.25f, -10, 10, "%.0f" );

				auto percent = config.button_alpha * 100;
				if (
					ImGui::DragFloat(
						"Surface Alpha", &percent, 1, 0, 100, "%.f%%" ) )
				{
					config.button_alpha = percent / 100;
				}
				ImGui::Checkbox( "Show Border", &config.border );

				ImGui::ColorEdit4(
					"Empty Slot",
					&config.no_item.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Unknown Item",
					&config.unknown_item.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Wrong Instance Type",
					&config.wrong_instance_type.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Need not use",
					&config.need_not_use.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Can not use",
					&config.can_not_use.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Should Use",
					&config.suggest_use.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Special State",
					&config.special_state.r,
					ImGuiColorEditFlags_AlphaPreview );

				static char buf_no_item[2] = { config.label_no_item };
				static char buf_unknown[2] = { config.label_unknown_item };
				static char buf_persistent[2] = { config.label_persistent };
				static char buf_load[2] = { config.label_until_load };
				static char buf_objective[2] = { config.label_until_objective };
				static char buf_inactive[2] = { config.label_inactive };

				if ( ImGui::InputText( "Empty Slot", buf_no_item, 2 ) )
				{
					config.label_no_item = buf_no_item[0];
				}
				if ( ImGui::InputText( "Unknown Item", buf_unknown, 2 ) )
				{
					config.label_unknown_item = buf_unknown[0];
				}
				if ( ImGui::InputText( "Inactive Item", buf_inactive, 2 ) )
				{
					config.label_inactive = buf_inactive[0];
				}
				if ( ImGui::InputText( "Persistent Item", buf_persistent, 2 ) )
				{
					config.label_persistent = buf_persistent[0];
				}
				if ( ImGui::InputText( "Until Load", buf_load, 2 ) )
				{
					config.label_until_load = buf_load[0];
				}
				if ( ImGui::InputText( "Until Objective", buf_objective, 2 ) )
				{
					config.label_until_objective = buf_objective[0];
				}

				ImGui::Unindent();
			}

			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::Settings::Config& config )
	{
		draw_window_settings( config );
	}

	void draw( April::Gui::Skillbar::Config& config )
	{
		ImGui::PushID( &config );
		draw_visibility( config.window );
		ImGui::SameLine();
		if ( ImGui::CollapsingHeader( config.window.name.c_str() ) )
		{
			ImGui::Indent();

			draw_save_discard( config );

			draw_window_settings_collapsing_header( config.window );

			if ( ImGui::CollapsingHeader( "Style" ) )
			{
				static int layout =
					config.skills_per_row == 8 ? 0
					: config.skills_per_row == 4 ? 1
					: config.skills_per_row == 2 ? 2
					: config.skills_per_row == 1 ? 3
					: 0;
				constexpr const char* layouts[4] =
					{ "1x8", "2x4", "4x2", "8x1" };

				if ( ImGui::Combo( "Layout", &layout, layouts, 4 ) )
				{
					if ( layout == 0 )
						config.skills_per_row = 8;
					else if ( layout == 1 )
						config.skills_per_row = 4;
					else if ( layout == 2 )
						config.skills_per_row = 2;
					else if ( layout == 3 )
						config.skills_per_row = 1;
					else
					{
						layout = 0;
						config.skills_per_row = 8;
					}
				}

				ImGui::ColorEdit4(
					"Cooldown",
					&config.text_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Cooldown with HSR",
					&config.hsr_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::ColorEdit4(
					"Border",
					&config.border_color.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::DragFloat2(
					"Spacing", &config.spacing.x, 0.5f, -10, 10, "%.0f" );

				for (
					auto it = config.thresholds.begin();
					it != config.thresholds.end();
					/**/ )
				{
					ImGui::PushID( &*it );

					char buf[16] = {};
					sprintf_s(
						buf, sizeof( buf ),
						"%5.2fs",
						it->time.count() / 1000.f );

					ImGui::ColorEdit4(
						buf,
						&it->color.r,
						ImGuiColorEditFlags_AlphaPreview );

					ImGui::SameLine();

					if ( ImGui::Button( "Remove" ) )
					{
						it = config.thresholds.erase( it );
					}
					else ++it;

					ImGui::PopID();
				}

				ImGui::ColorEdit4(
					"Inactive",
					&config.color_inactive.r,
					ImGuiColorEditFlags_AlphaPreview );

				ImGui::Text( "Add another threshold" );
				static int input_buf = 0;
				static auto color_buf = April::Invisible();

				ImGui::InputInt( "Threshold (ms)", &input_buf );
				ImGui::ColorEdit4(
					"Color",
					&color_buf.r,
					ImGuiColorEditFlags_AlphaPreview );

				if ( ImGui::Button( "Add" ) )
				{
					using Threshold = April::Gui::Skillbar::Config::Threshold;

					config.thresholds.emplace_back(
						Threshold{ milliseconds{ input_buf }, color_buf } );

					std::sort(
						config.thresholds.begin(),
						config.thresholds.end(),
						[]( auto const& lhs, auto const& rhs )
						{
							return lhs.time > rhs.time;
						} );
				}

				ImGui::Checkbox( "Show Uptime", &config.show_uptime );
				if ( config.show_uptime )
				{
					ImGui::DragFloat2(
						"Uptime-Offset",
						&config.uptime_offset.x, 0.2f, -100, 100, "%.0f" );
				}

			}

			ImGui::Unindent();
		}
		ImGui::PopID();
	}

	void draw( April::Gui::TargetInfo::Config& config )
	{
		draw_window_settings( config );
	}

	void draw( April::Gui::UwTimesGui::Config& config )
	{
		draw_window_settings( config );
	}


	template<auto N = 0, typename... Args>
	#pragma warning( suppress: 4100 )
	void draw_settings( std::tuple<Args...>& config )
	{
		if constexpr ( N < sizeof...( Args ) )
		{
			draw( std::get<N>( config ) );
			draw_settings<N + 1>( config );
		}
	}

}


April::Gui::Settings::Settings( ModuleConfigurations& config )
	: configurations{ config }
{
}

void April::Gui::Settings::Display() const
{
	auto& config = std::get<Config>( configurations.gui );

	if ( ImGui::Begin( config.window ) )
	{
		ImGui::PushStyleVar( ImGuiStyleVar_IndentSpacing, 28 );

		if ( ImGui::IsWindowHovered( ImGuiHoveredFlags_ChildWindows ) )
		{
			WndProc::BlockMouseInput();
		}

		ImGui::Text( "Module Settings" );
		draw_settings( configurations.active );
		draw_settings( configurations.passive );

		ImGui::Text( "\nGui Settings" );
		draw_settings( configurations.gui );

		ImGui::Text( "\nGlobal Style Settings" );
		if ( ImGui::CollapsingHeader( "ImGui Style" ) )
		{
			ImGui::ShowStyleEditor();
		}

		ImGui::PopStyleVar();
	}
	ImGui::End();
}

auto April::Gui::Settings::Config::LoadDefault() -> Config
{
	auto const config = Config{
		{ "Settings", true, ImGuiWindowFlags_None }
	};

	return config;
}
