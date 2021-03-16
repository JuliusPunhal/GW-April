
#include "April/Framework/Instance.h"
#include "April/Framework/Instance.OnCommand.hpp"

#include "April/Utility/FileIO.h"

#include "Dependencies/ImGui.hpp"

#include <filesystem>

namespace fs = std::filesystem;


namespace {

	void default_style( ImGuiStyle& style )
	{
		style.Alpha                     = 1.0f;

		style.WindowPadding             = ImVec2{ 8, 8 };
		style.WindowRounding            = 3.f;
		style.WindowBorderSize          = 1.0f;
		style.WindowMinSize             = ImVec2{ 32, 32 };
		style.WindowTitleAlign          = ImVec2{ 0.0f, 0.5f };
		style.WindowMenuButtonPosition  = ImGuiDir_Left;

		style.ChildRounding             = 3.f;
		style.ChildBorderSize           = 1.0f;

		style.PopupRounding             = 3.f;
		style.PopupBorderSize           = 1.0f;

		style.FramePadding              = ImVec2{ 4, 3 };
		style.FrameRounding             = 2.f;
		style.FrameBorderSize           = 0.0f;

		style.ItemSpacing               = ImVec2{ 8, 4 };
		style.ItemInnerSpacing          = ImVec2{ 4, 4 };

		style.TouchExtraPadding         = ImVec2{ 0, 0 };

		style.IndentSpacing             = 26.0f;
		style.ColumnsMinSpacing         = 6.0f;

		style.ScrollbarSize             = 14.0f;
		style.ScrollbarRounding         = 2.f;

		style.GrabMinSize               = 10.0f;
		style.GrabRounding              = 2.f;

		style.LogSliderDeadzone         = 4.0f;

		style.TabRounding               = 2.f;
		style.TabBorderSize             = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;

		style.ColorButtonPosition       = ImGuiDir_Right;

		style.ButtonTextAlign           = ImVec2{ 0.5f, 0.5f };
		style.SelectableTextAlign       = ImVec2{ 0.0f, 0.0f };

		style.DisplayWindowPadding      = ImVec2{ 19, 19 };
		style.DisplaySafeAreaPadding    = ImVec2{ 3, 3 };

		style.MouseCursorScale          = 1.0f;

		style.AntiAliasedLines          = true;
		style.AntiAliasedLinesUseTex    = true;
		style.AntiAliasedFill           = true;

		style.CurveTessellationTol      = 1.25f;
		style.CircleSegmentMaxError     = 1.60f;


		using namespace April;
		auto const neutral_darker = RGBA{ 15/255.f, 15/255.f, 15/255.f, 0.9f };
		auto const neutral_dark = RGBA{ 76/255.f, 76/255.f, 76/255.f, 1 };
		auto const neutral_light = RGBA{ 127/255.f, 127/255.f, 127/255.f, 1 };
		auto const neutral_lighter = RGBA{ 186/255.f, 186/255.f, 186/255.f, 1 };

		auto const accent_dark = RGBA{ 23/255.f, 105/255.f, 6/255.f, 1 };
		auto const accent_light = RGBA{ 31/255.f, 148/255.f, 29/255.f, 1 };
		auto const accent_lighter = RGBA{ 21/255.f, 187/255.f, 37/255.f, 1 };

		auto& col = style.Colors;
		col[ImGuiCol_Text]                  = White();
		col[ImGuiCol_TextDisabled]          = { 0.5f, 0.5f, 0.5f, 1 };
		col[ImGuiCol_TextSelectedBg]        = accent_dark;
		col[ImGuiCol_DragDropTarget]        = accent_lighter;

		col[ImGuiCol_WindowBg]              = neutral_darker;
		col[ImGuiCol_ChildBg]               = Invisible();
		col[ImGuiCol_MenuBarBg]             = neutral_darker;
		col[ImGuiCol_PopupBg]               = neutral_darker;

		col[ImGuiCol_Border]                = neutral_light;
		col[ImGuiCol_BorderShadow]          = Invisible();

		col[ImGuiCol_FrameBg]               = neutral_dark;
		col[ImGuiCol_FrameBgHovered]        = neutral_light;
		col[ImGuiCol_FrameBgActive]         = neutral_lighter;

		col[ImGuiCol_TitleBg]               = neutral_dark;
		col[ImGuiCol_TitleBgActive]         = accent_dark;
		col[ImGuiCol_TitleBgCollapsed]      = neutral_darker;

		col[ImGuiCol_ScrollbarBg]           = neutral_darker;
		col[ImGuiCol_ScrollbarGrab]         = neutral_dark;
		col[ImGuiCol_ScrollbarGrabHovered]  = neutral_light;
		col[ImGuiCol_ScrollbarGrabActive]   = neutral_lighter;

		col[ImGuiCol_CheckMark]             = accent_lighter;

		col[ImGuiCol_SliderGrab]            = accent_light;
		col[ImGuiCol_SliderGrabActive]      = accent_lighter;

		col[ImGuiCol_Button]                = accent_dark;
		col[ImGuiCol_ButtonHovered]         = accent_light;
		col[ImGuiCol_ButtonActive]          = accent_lighter;

		col[ImGuiCol_Header]                = accent_dark;
		col[ImGuiCol_HeaderHovered]         = accent_light;
		col[ImGuiCol_HeaderActive]          = accent_lighter;

		col[ImGuiCol_Separator]             = neutral_light;
		col[ImGuiCol_SeparatorHovered]      = neutral_lighter;
		col[ImGuiCol_SeparatorActive]       = neutral_lighter;

		col[ImGuiCol_ResizeGrip]            = neutral_dark;
		col[ImGuiCol_ResizeGripHovered]     = neutral_light;
		col[ImGuiCol_ResizeGripActive]      = neutral_lighter;

		col[ImGuiCol_Tab]                   = neutral_dark;
		col[ImGuiCol_TabHovered]            = neutral_light;
		col[ImGuiCol_TabActive]             = accent_dark;
		col[ImGuiCol_TabUnfocused]          = neutral_dark;
		col[ImGuiCol_TabUnfocusedActive]    = accent_dark;

		col[ImGuiCol_PlotLines]             = accent_light;
		col[ImGuiCol_PlotLinesHovered]      = accent_lighter;
		col[ImGuiCol_PlotHistogram]         = accent_light;
		col[ImGuiCol_PlotHistogramHovered]  = accent_lighter;

		//col[ImGuiCol_NavHighlight]          = { 0.26f, 0.59f, 0.98f, 1.00f };
		//col[ImGuiCol_NavWindowingHighlight] = { 1.00f, 1.00f, 1.00f, 0.70f };
		//col[ImGuiCol_NavWindowingDimBg]     = { 0.80f, 0.80f, 0.80f, 0.20f };
		//col[ImGuiCol_ModalWindowDimBg]      = { 0.80f, 0.80f, 0.80f, 0.35f };
	}

	template<typename T>
	auto load_config()
	{
		auto const config = April::IO::from_file<T>( T::path );
		if ( config == std::nullopt )
			return T::LoadDefault();

		return *config;
	}

	void RegisterCallbacks( std::shared_ptr<April::Instance> inst )
	{
		auto* entry = reinterpret_cast<GW::HookEntry*>( inst.get() );

		using namespace April;
		using namespace GW::Packet::StoC;

		// Callbacks will only be cleaned up during GWCA shutdown.
		GW::StoC::RegisterPacketCallback<AgentAdd>(
			entry,
			[inst]( auto* status, auto* packet )
			{
				auto& agent_filter = std::get<AgentFilter>( inst->modules );
				auto& config = std::get<AgentFilter::Config>( inst->config );

				if ( agent_filter.should_suppress( *packet, config ) )
				{
					status->blocked = true;
					agent_filter.register_suppressed( *packet );
				}
			} );

		GW::StoC::RegisterPacketCallback<AgentName>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<UwTimer>( inst->modules ).Update( *packet );
			} );

		GW::StoC::RegisterPacketCallback<AgentRemove>(
			entry,
			[inst]( auto* status, auto* packet )
			{
				auto& agent_filter = std::get<AgentFilter>( inst->modules );

				if ( agent_filter.should_suppress( *packet ) )
				{
					status->blocked = true;
				}
			} );

		GW::StoC::RegisterPacketCallback<AgentUpdateAllegiance>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<UwTimer>( inst->modules ).Update( *packet );
				CallCommand(
					std::get<WindowMgr>( inst->modules ).Update( *packet ),
					*inst );
			} );

		GW::StoC::RegisterPacketCallback<ItemGeneral>(
			entry,
			[inst] ( auto*, auto* packet )
			{
				std::get<ShowKitUses>( inst->modules ).UpdateKitUses(
					*packet, std::get<ShowKitUses::Config>( inst->config ) );
			} );

		GW::StoC::RegisterPacketCallback<ItemGeneral_ReuseID>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<AgentFilter>( inst->modules ).DeleteOwner( *packet );
				std::get<ShowKitUses>( inst->modules ).UpdateKitUses(
					*packet, std::get<ShowKitUses::Config>( inst->config ) );
			} );

		GW::StoC::RegisterPacketCallback<MapLoaded>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<Gui::Skillbar>( inst->modules ).UpdateHSR( *packet );
				std::get<AgentFilter>( inst->modules ).Reset();
				std::get<ConsumablesMgr>( inst->modules ).Update( *packet );
				std::get<DhuumsJudgement>( inst->modules ).Update( *packet );
				std::get<UwTimer>( inst->modules ).Reset();
				CallCommand(
					std::get<WindowMgr>( inst->modules ).Update( *packet ),
					*inst );
			} );

		GW::StoC::RegisterPacketCallback<MessageGlobal>(
			entry,
			[inst] ( auto* status, auto* )
			{
				auto const& chat_filter = std::get<ChatFilter>( inst->modules );
				auto const& cfg = std::get<ChatFilter::Config>( inst->config );

				auto& buf = GW::GameContext::instance()->world->message_buff;
				if ( buf.valid()
					&& chat_filter.should_suppress( buf.begin(), cfg ) )
				{
					status->blocked = true;
					buf.clear();
				}
			} );

		GW::StoC::RegisterPacketCallback<MessageLocal>(
			entry,
			[inst] ( auto* status, auto* )
			{
				auto const& chat_filter = std::get<ChatFilter>( inst->modules );
				auto const& cfg = std::get<ChatFilter::Config>( inst->config );

				auto& buf = GW::GameContext::instance()->world->message_buff;
				if ( buf.valid()
					&& chat_filter.should_suppress( buf.begin(), cfg ) )
				{
					status->blocked = true;
					buf.clear();
				}
			} );

		GW::StoC::RegisterPacketCallback<MessageServer>(
			entry,
			[inst] ( auto* status, auto* )
			{
				auto const& chat_filter = std::get<ChatFilter>( inst->modules );
				auto const& cfg = std::get<ChatFilter::Config>( inst->config );

				auto& buf = GW::GameContext::instance()->world->message_buff;
				if ( buf.valid()
					&& chat_filter.should_suppress( buf.begin(), cfg ) )
				{
					status->blocked = true;
					buf.clear();
				}
			} );

		GW::StoC::RegisterPacketCallback<ObjectiveAdd>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<UwTimer>( inst->modules ).Update( *packet );
			} );

		GW::StoC::RegisterPacketCallback<ObjectiveDone>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<ConsumablesMgr>( inst->modules ).Update( *packet );
				std::get<DhuumBot>( inst->modules ).Update( *packet );
				std::get<UwTimer>( inst->modules ).Update( *packet );
				CallCommand(
					std::get<WindowMgr>( inst->modules ).Update( *packet ),
					*inst );
			} );

		GW::StoC::RegisterPacketCallback<ObjectiveUpdateName>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<UwTimer>( inst->modules ).Update( *packet );
			} );

		GW::StoC::RegisterPacketCallback<PartyDefeated>(
			entry,
			[inst]( auto*, auto* )
			{
				CallCommand(
					std::get<ReturnToOutpost>( inst->modules ).OnDefeated(
						std::get<ReturnToOutpost::Config>( inst->config ) ),
					*inst );
			} );

		GW::StoC::RegisterPacketCallback<RemoveEffect>(
			entry,
			[inst]( auto*, auto* packet )
			{
				CallCommand(
					std::get<NotifyEffectLoss>( inst->modules ).OnEffectLoss(
						*packet,
						std::get<NotifyEffectLoss::Config>( inst->config ) ),
					*inst );
			} );

		GW::StoC::RegisterPacketCallback<SkillRecharge>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<Gui::Skillbar>( inst->modules ).UpdateHSR( *packet );
			} );

		GW::StoC::RegisterPacketCallback<SkillRecharged>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<Gui::Skillbar>( inst->modules ).UpdateHSR( *packet );
			} );

		GW::StoC::RegisterPacketCallback<SpeechBubble>(
			entry,
			[inst]( auto* status, auto* )
			{
				auto const& bubbles =
					std::get<SuppressSpeechBubbles>( inst->modules );
				auto const& config =
					std::get<SuppressSpeechBubbles::Config>( inst->config );

				if ( bubbles.should_suppress( config ) )
					status->blocked = true;
			} );

		GW::StoC::RegisterPacketCallback<UpdateItemOwner>(
			entry,
			[inst]( auto*, auto* packet )
			{
				std::get<AgentFilter>( inst->modules ).UpdateOwner( *packet );
			} );

		GW::Chat::RegisterLocalMessageCallback(
			entry,
			[inst]( GW::HookStatus* status, int, wchar_t const* msg )
			{
				auto const& chat_filter = std::get<ChatFilter>( inst->modules );
				auto const& cfg = std::get<ChatFilter::Config>( inst->config );

				if ( chat_filter.should_suppress( msg, cfg ) )
				{
					status->blocked = true;
				}
			} );

		GW::Chat::RegisterSendChatCallback(
			entry,
			[inst](
				GW::HookStatus* status, GW::Chat::Channel channel, wchar_t* msg )
			{
				auto& chat_commands = std::get<ChatCommands>( inst->modules );
				auto& config = std::get<ChatCommands::Config>( inst->config );

				auto cmds = chat_commands.ParseMessage( channel, msg, config );
				CallCommand( cmds, *inst );

				if ( cmds.size() > 0 )
					status->blocked = true;
			} );
	}

}


auto April::make_instance( IDirect3DDevice9* device )
	-> std::shared_ptr<Instance>
{
	ImGui::CreateContext();
	ImGui_ImplWin32_Init( GW::MemoryMgr::GetGWWindowHandle() );
	ImGui_ImplDX9_Init( device );

	if ( not fs::directory_entry( "April" ).exists() )
	{
		fs::create_directory( "April" );
	}

	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\Consola.ttf", 14.f );
	io.IniFilename = "April\\imgui.ini";
	default_style( ImGui::GetStyle() );

	auto config = Configuration{
		load_config<ConsumablesMgr::Config>(),
		load_config<AgentFilter::Config>(),
		load_config<ChatCommands::Config>(),
		load_config<ChatFilter::Config>(),
		load_config<NotifyEffectLoss::Config>(),
		load_config<ReturnToOutpost::Config>(),
		load_config<ShowKitUses::Config>(),
		load_config<SuppressSpeechBubbles::Config>(),
		load_config<Gui::ChainedSoulGui::Config>(),
		load_config<Gui::DhuumBotGui::Config>(),
		load_config<Gui::DhuumInfo::Config>(),
		load_config<Gui::Dialogs::Config>(),
		load_config<Gui::Energybar::Config>(),
		load_config<Gui::Healthbar::Config>(),
		load_config<Gui::InstanceTimer::Config>(),
		load_config<Gui::Inventory::Config>(),
		load_config<Gui::Settings::Config>(),
		load_config<Gui::Skillbar::Config>(),
		load_config<Gui::TargetInfo::Config>(),
		load_config<Gui::UwTimesGui::Config>()
	};

	auto modules = Modules{
		ConsumablesMgr{},
		ChainedSoul{},
		DhuumBot{},
		DhuumsJudgement{},
		UwTimer{},
		WindowMgr{},
		AgentFilter{},
		ChatCommands{},
		ChatFilter{},
		CursorFix{},
		NotifyEffectLoss{},
		ReturnToOutpost{},
		ShowKitUses{},
		SuppressSpeechBubbles{},
		Gui::ChainedSoulGui{},
		Gui::Energybar{
			std::get<Gui::Energybar::Config>( config )
		},
		Gui::DhuumBotGui{},
		Gui::DhuumInfo{},
		Gui::Dialogs{},
		Gui::Healthbar{
			std::get<Gui::Healthbar::Config>( config )
		},
		Gui::InstanceTimer{
			std::get<Gui::InstanceTimer::Config>( config )
		},
		Gui::Inventory{
			std::get<Gui::Inventory::Config>( config )
		},
		Gui::Settings{},
		Gui::Skillbar{
			std::get<Gui::Skillbar::Config>( config )
		},
		Gui::TargetInfo{},
		Gui::UwTimesGui{}
	};

	auto instance =
		std::make_shared<Instance>(
			Instance{ std::move( modules ), std::move( config ) } );

	RegisterCallbacks( instance );

	return instance;
}

void April::Update( Instance& instance )
{
	if (
		auto const cons =
			std::get<ConsumablesMgr>( instance.modules ).should_use_item(
				std::get<ConsumablesMgr::Config>( instance.config ) );
		cons.has_value() )
	{
		CallCommand( UseConsumable{ cons->consumable, cons->item }, instance );
	}

	std::get<ChainedSoul>( instance.modules ).Update();
	if (
		auto const skill =
			std::get<DhuumBot>( instance.modules ).should_use_skill();
		skill.has_value() )
	{
		CallCommand( UseSkill{ skill->slot, skill->target }, instance );
	}

	std::get<DhuumsJudgement>( instance.modules ).Update();
	std::get<UwTimer>( instance.modules ).Update();
	CallCommand( std::get<WindowMgr>( instance.modules ).Update(), instance );
}

void April::Display( Instance& instance )
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	CallCommand(
		std::get<Gui::ChainedSoulGui>( instance.modules ).Display(
			std::get<ChainedSoul>( instance.modules ).get(),
			std::get<Gui::ChainedSoulGui::Config>( instance.config ) ),
		instance );

	std::get<Gui::Energybar>( instance.modules ).Display(
		std::get<Gui::Energybar::Config>( instance.config ) );

	std::get<Gui::DhuumBotGui>( instance.modules ).Display(
		std::get<DhuumBot>( instance.modules ),
		std::get<Gui::DhuumBotGui::Config>( instance.config ) );

	CallCommand(
		std::get<Gui::DhuumInfo>( instance.modules ).Display(
			std::get<DhuumsJudgement>( instance.modules ),
			std::get<Gui::DhuumInfo::Config>( instance.config ) ),
		instance );

	CallCommand(
		std::get<Gui::Dialogs>( instance.modules ).Display(
			std::get<Gui::Dialogs::Config>( instance.config ) ),
		instance );

	std::get<Gui::Healthbar>( instance.modules ).Display(
		std::get<Gui::Healthbar::Config>( instance.config ) );

	CallCommand(
		std::get<Gui::InstanceTimer>( instance.modules ).Display(
			std::get<Gui::InstanceTimer::Config>( instance.config ) ),
		instance );

	CallCommand(
		std::get<Gui::Inventory>( instance.modules ).Display(
			std::get<ConsumablesMgr>( instance.modules ),
			std::get<Gui::Inventory::Config>( instance.config ) ),
		instance );

	CallCommand(
		std::get<Gui::Settings>( instance.modules ).Display(
			std::forward_as_tuple(
				std::get<ConsumablesMgr::Config>( instance.config ),
				std::get<AgentFilter::Config>( instance.config ),
				std::get<ChatCommands::Config>( instance.config ),
				std::get<ChatFilter::Config>( instance.config ),
				std::get<NotifyEffectLoss::Config>( instance.config ),
				std::get<ReturnToOutpost::Config>( instance.config ),
				std::get<ShowKitUses::Config>( instance.config ),
				std::get<SuppressSpeechBubbles::Config>( instance.config ),
				std::get<Gui::ChainedSoulGui::Config>( instance.config ),
				std::get<Gui::DhuumBotGui::Config>( instance.config ),
				std::get<Gui::DhuumInfo::Config>( instance.config ),
				std::get<Gui::Dialogs::Config>( instance.config ),
				std::get<Gui::Energybar::Config>( instance.config ),
				std::get<Gui::Healthbar::Config>( instance.config ),
				std::get<Gui::InstanceTimer::Config>( instance.config ),
				std::get<Gui::Inventory::Config>( instance.config ),
				std::get<Gui::Settings::Config>( instance.config ),
				std::get<Gui::Skillbar::Config>( instance.config ),
				std::get<Gui::TargetInfo::Config>( instance.config ),
				std::get<Gui::UwTimesGui::Config>( instance.config ) ) ),
		instance );

	std::get<Gui::Skillbar>( instance.modules ).Display(
		std::get<Gui::Skillbar::Config>( instance.config ) );

	std::get<Gui::TargetInfo>( instance.modules ).Display(
		std::get<Gui::TargetInfo::Config>( instance.config ) );

	CallCommand(
		std::get<Gui::UwTimesGui>( instance.modules ).Display(
			std::get<UwTimer>( instance.modules ),
			std::get<Gui::UwTimesGui::Config>( instance.config ) ),
		instance );

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
}

void April::CallCommand( Command const& cmd, Instance& instance )
{
	std::visit( OnCommand{ instance }, cmd );
}

void April::CallCommand(
	std::vector<Command> const& commands, Instance& instance )
{
	for ( auto const& command : commands )
	{
		CallCommand( command, instance );
	}
}

void April::Shutdown( Instance& instance )
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CallCommand( ShowSuppresedAgents{}, instance );
}

void April::OnDeviceReset( IDirect3DDevice9* )
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}
