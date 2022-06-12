
#include "April/Utility/SettingsWidgets.h"

#include <string>

using namespace April;


bool April::DrawActivateButton(
	bool& active,
	const char* when_active,
	const char* when_inactive )
{
	auto const x = ImGui::GetContentRegionMax().x;
	auto const button_size = ImVec2{
		x * 0.8f, ImGui::GetFrameHeight() * 1.5f
	};

	ImGui::MoveCursorPosX( x * 0.1f );

	auto const label = active ? when_active : when_inactive;
	auto const clicked = ImGui::Button( label, button_size );
	if ( clicked )
		active = not active;

	return clicked;
}


namespace {

	auto font_name( FontInfo const& info ) -> std::string
	{
		return info.name + " (" + std::to_string( info.size ) + " px)";
	}

	auto font_name( std::optional<FontInfo> const& info ) -> std::string
	{
		return info ? font_name( *info ) : "Default Font";
	}

	void draw_loaded( Font& current, FontAtlas const& fonts )
	{
		for ( auto const& loaded : fonts.GetLoadedFonts() )
		{
			ImGui::PushID( loaded.name.c_str() );

			auto const selected = (loaded == current);
			if ( ImGui::Selectable( font_name( loaded ).c_str(), selected ) )
			{
				current = loaded;
			}
			ImGui::PopID();
		}
		ImGui::Separator();
		if ( ImGui::Selectable( "Default Font", current == std::nullopt ) )
		{
			current = std::nullopt;
		}
	}

	void draw_load_new( Font& current, FontAtlas const& fonts )
	{
		static char new_font_name[128] = {};
		static auto new_font_size = 14;
		static auto status = std::string{};

		ImGui::InputText( "Name", new_font_name, sizeof( new_font_name ) );
		ImGui::SameLine();
		ImGui::HelpMarker(
			"If only <filename.ttf> is provided, April will check the current "
			"GW-directory, C:/Users/<User>/AppData/Roaming/GW-April/Fonts/, "
			"and C:/Windows/Fonts/ for the file, in that order.\n"
			"Alternatively, provide the full path to the font you want to "
			"load." );

		if ( ImGui::InputInt( "Size", &new_font_size, 1, 10 ) )
		{
			new_font_size = std::max( 1, new_font_size );
		}

		auto const size = ImVec2{ ImGui::GetContentRegionAvail().x, 0 };
		if ( ImGui::Button( "Load font & apply", size ) )
		{
			auto const new_font = FontInfo{
				std::string{ new_font_name },
				new_font_size
			};

			if ( fonts.CanLoad( new_font ) )
			{
				current = new_font;
				status = "";
			}
			else status = "Cannot load font!";
		}

		if ( not status.empty() )
		{
			ImGui::PushStyleColor( ImGuiCol_Button, Invisible() );
			ImGui::PushStyleColor( ImGuiCol_ButtonActive, Invisible() );
			ImGui::PushStyleColor( ImGuiCol_ButtonHovered, Invisible() );
			if ( ImGui::SmallButton( status ) )
			{
				status = "";
			}
			ImGui::PopStyleColor( 3 );
		}
	}

}


void April::DrawFontSelector(
	const char* label, Font& current, FontAtlas const& fonts )
{
	ImGui::PushID( &current );
	if ( ImGui::BeginCombo( label, font_name( current ).c_str() ) )
	{
		draw_loaded( current, fonts );
		if ( ImGui::BeginMenu( "Load new Font" ) )
		{
			draw_load_new( current, fonts );
			ImGui::EndMenu();
		}

		ImGui::EndCombo();
	}
	ImGui::PopID();
}


namespace {

	namespace WindowFlags {

		constexpr auto regular = ImGuiWindowFlags_None;
		constexpr auto minimal = ImGuiWindowFlags_NoDecoration;
		constexpr auto overlay =
			ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoScrollWithMouse
			| ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoFocusOnAppearing
			| ImGuiWindowFlags_NoBringToFrontOnFocus;

		struct DrawInfo {
			const char*      title;
			const char*      description;
			ImGuiWindowFlags flags;
		};

		constexpr DrawInfo draw_info[] = {
			{
				"Automatic Resize",
				"Automatically resizes the window to fit its content without "
				"the need for scrolling. This is mainly meant to be used with "
				"windows that have variable content (e.g. Target Info/Chained "
				"Soul Info); however, some widgets try to fill their window "
				"which may cause issues when this is enabled.",
				ImGuiWindowFlags_AlwaysAutoResize
			},
			{
				"Disable Movement",
				"Disables the ability to move windows by click-and-drag. "
				"Adjusting the position via the Settings window is not "
				"affected by this and will remain possible.",
				ImGuiWindowFlags_NoMove
			},
			{
				"Disable Resizing",
				"Disables the ability to resize windows by click-and-drag on "
				"the corner and edges. Also hides the resize-indicator in the "
				"bottom-right of the window. Adjusting the size via the "
				"Settings window is not affected by this and will remain "
				"possible.",
				ImGuiWindowFlags_NoResize
			},
			{
				"Hide Titlebar",
				"Hides the window-titlebar. Will therefore silently disable "
				"the ability to collapse the window.",
				ImGuiWindowFlags_NoTitleBar
			},
			{
				"Disable Collapsing",
				"Disables the ability to collapse windows by clicking the "
				"icon in the title-bar or by double-clicking the title-bar.",
				ImGuiWindowFlags_NoCollapse
			},
			{
				"Hide Background",
				"When enabled, the window-background will not be shown so "
				"that only widgets are visible. This is mainly meant to be "
				"used for overlays. Be aware that this will not disable move-"
				"by-mouse and resize-by-mouse; also, the resize-indicator in "
				"the bottom-right will still be visible.",
				ImGuiWindowFlags_NoBackground
			},
			{
				"Disable Scrolling",
				"Disables the ability to scroll within windows via mouse-"
				"wheel. Scrolling via scrollbar -if enabled- is not affected "
				"by this and will remain possible.",
				ImGuiWindowFlags_NoScrollWithMouse
			},
			{
				"Disable Scrollbar",
				"Disables the vertical scrollbar when the window-contents do "
				"not fit within the given size. Scrolling via mouse-wheel is "
				"not affected by this and will remain possible.",
				ImGuiWindowFlags_NoScrollbar
			},
			{
				"Enable H-Scrollbar",
				"When enabled, the horizontal scrollbar will be shown when "
				"the window-contents do not fit within the given size.",
				ImGuiWindowFlags_HorizontalScrollbar
			},
			{
				"Disable Focus when appearing",
				"When a window goes from a hidden state to visual state, this "
				"setting will determine if a window will be focussed. This is "
				"mainly meant to be used for overlays that are automatically "
				"shown to avoid losing focus from other windows for keyboard-"
				"inputs.",
				ImGuiWindowFlags_NoFocusOnAppearing
			},
			{
				"Disable bring to front on focus",
				"When a window is focussed, either by clicking on it or <see "
				"above>, this setting will determine if the window will be "
				"moved to the very front. This is mainly meant to be used for "
				"overlays that should never be on top of regular windows.",
				ImGuiWindowFlags_NoBringToFrontOnFocus
			},
			{
				"Disable Mouse Inputs",
				"Disables all mouse-inputs to the window, which will disable "
				"Buttons and other widgets but also the ability to move or "
				"resize the window. This is mainly useful for static overlays "
				"that are not supposed to be interacted with.",
				ImGuiWindowFlags_NoMouseInputs
			},
			{
				"Disable Keyboard Inputs",
				"Disables all keyboard-inputs to the window, which will "
				"disable Text- and Number-inputs. This is mainly useful for "
				"static overlays that are not supposed to be interacted with.",
				ImGuiWindowFlags_NoNavInputs
			}
		};

	}


	void draw_name( Window& window )
	{
		static auto new_name = std::string{};

		ImGui::InputTextWithHint(
			"##Window", window.name.c_str(), &new_name );

		ImGui::SameLine();
		if ( ImGui::Button( "Rename" ) && new_name.size() > 0 )
		{
			auto const* wnd = ImGui::FindWindowByName( window.name.c_str() );
			if ( wnd != nullptr )
			{
				auto const flags =
					window.flags
					| ImGuiWindowFlags_NoFocusOnAppearing
					| ImGuiWindowFlags_NoBringToFrontOnFocus;

				ImGui::SetNextWindowPos( wnd->Pos );
				ImGui::SetNextWindowSize( wnd->Size );
				ImGui::Begin( new_name.c_str(), &window.visible, flags );
				{
					// Prime ImGui so the new (renamed) window will have the
					// same position and size.
				}
				ImGui::End();
			}

			window.name = new_name;
			new_name = "";
		}

		ImGui::SameLine();
		ImGui::HelpMarker(
			"This will mainly change the displayed name in the Titlebar of "
			"the window. Be aware this may affect your Chat Commands!" );
	}


	constexpr auto regular_str = "Regular Window";
	constexpr auto minimal_str = "Minimal Window";
	constexpr auto overlay_str = "Overlay";


	auto flags_to_string( ImGuiWindowFlags const flags ) -> const char*
	{
		if ( flags == WindowFlags::regular ) return regular_str;
		if ( flags == WindowFlags::minimal ) return minimal_str;
		if ( flags == WindowFlags::overlay ) return overlay_str;

		return "Custom";
	}

	void impl_draw_flags( WindowFlags::DrawInfo const& info, Window& window )
	{
		auto const is_set = (window.flags & info.flags) == info.flags;

		auto checked = is_set;
		if ( ImGui::Checkbox( info.title, &checked ) )
		{
			is_set
				? window.flags &= ~info.flags
				: window.flags |= info.flags;
		}

		ImGui::SameLine();
		ImGui::HelpMarker( info.description );
	};

	void draw_flags( Window& window )
	{
		using namespace ::WindowFlags;

		if ( ImGui::BeginCombo( "Style", flags_to_string( window.flags ) ) )
		{
			if ( ImGui::Selectable( regular_str, window.flags == regular ) )
				window.flags = regular;

			if ( ImGui::Selectable( minimal_str, window.flags == minimal ) )
				window.flags = minimal;

			if ( ImGui::Selectable( overlay_str, window.flags == overlay ) )
				window.flags = overlay;

			if ( ImGui::BeginMenu( "Custom" ) )
			{
				for ( auto const& x : draw_info )
					impl_draw_flags( x, window );

				ImGui::EndMenu();
			}

			ImGui::EndCombo();
		}
	}

	void draw_position( Window& window )
	{
		if ( auto* wnd = ImGui::FindWindowByName( window.name.c_str() ) )
		{
			ImGui::DragFloat2( "Position", &wnd->Pos.x, 1, 0, 0, "%.0f" );
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(
				"Cannot find Position-Info for Window %s!",
				window.name.c_str() );
			ImGui::SameLine();
			ImGui::HelpMarker(
				"This may happen when the window has not yet been drawn.\n"
				"Toggle the window-visibility to resolve this." );
		}
	}

	void draw_size( Window& window )
	{
		if ( auto* wnd = ImGui::FindWindowByName( window.name.c_str() ) )
		{
			ImGui::DragFloat2( "Size", &wnd->SizeFull.x, 1, 0, 0, "%.0f" );
			if ( (window.flags & ImGuiWindowFlags_AlwaysAutoResize) != 0 )
			{
				ImGui::SameLine();
				ImGui::HelpMarker(
					"Disable Style -> Custom -> Automatic Resize to manually "
					"define size" );
			}
		}
		else
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text(
				"Cannot find Size-Info for Window %s!",
				window.name.c_str() );
			ImGui::SameLine();
			ImGui::HelpMarker(
				"This may happen when the window has not yet been drawn.\n"
				"Toggle the window-visibility to resolve this." );
		}
	}

	void draw_visibility( Window& window )
	{
		auto const str = window.visible ? "Hide Window" : "Show Window";
		if ( ImGui::Button( str, { ImGui::CalcItemWidth(), 0 } ) )
			window.visible = not window.visible;
	}

}


void April::DrawWindowSettings( Window& window )
{
	ImGui::PushID( &window );
	{
		draw_name( window );
		draw_flags( window );
		draw_position( window );
		draw_size( window );
		draw_visibility( window );
	}
	ImGui::PopID();
}
