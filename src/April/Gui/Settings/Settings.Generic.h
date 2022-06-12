#pragma once

#include "April/Utility/SettingsWidgets.h"

#include "ImGui/ImGui.hpp"

#include <memory>
#include <type_traits>


namespace April::Gui::detail {

	template<typename T, typename = void>
	struct has_window : std::false_type {};

	template<typename T>
	struct has_window<T, std::void_t<decltype( std::declval<T>().window )>>
		: std::true_type {};

	template<typename T>
	constexpr bool has_window_v = has_window<T>::value;

	template<typename T>
	constexpr bool has_window_only_v =
		has_window_v<T> && sizeof( T ) == sizeof( April::Window );


	template<typename T, typename = void>
	struct has_auto_window : std::false_type {};

	template<typename T>
	struct has_auto_window<
		T,
		std::void_t<
			std::void_t<decltype( std::declval<T>().window )>,
			std::void_t<decltype( std::declval<T>().auto_set_visibility )>>>
		: std::true_type {};

	template<typename T>
	constexpr bool has_auto_window_v = has_auto_window<T>::value;

	struct AutoWindow {
		April::Window window;
		bool          auto_set_visibility;
	};

	template<typename T>
	constexpr bool has_auto_window_only_v =
		has_auto_window_v<T> && sizeof( T ) == sizeof( AutoWindow );


	template<typename T, typename = void>
	struct has_active : std::false_type {};

	template<typename T>
	struct has_active<T, std::void_t<decltype( std::declval<T>().active )>>
		: std::true_type {};

	template<typename T>
	constexpr bool has_active_v = has_active<T>::value;

	template<typename T>
	constexpr bool has_active_only_v =
		has_active_v<T> && sizeof( T ) == sizeof( bool );

}

namespace April::Gui {

	template<typename T>
	class Settings_Generic {
	public:
		Settings_Generic( const char* description, std::shared_ptr<T> config )
			: description{ description }, config{ config }
		{
		}

		void Draw()
		{
			ImGui::TextWrapped( description );

			ImGui::SeparatorSpaced();

			using namespace detail;
			if constexpr ( has_window_only_v<T> )
			{
				DrawWindowSettings( config->window );
			}
			else if constexpr ( has_auto_window_only_v<T> )
			{
				DrawWindowSettings( config->window );

				ImGui::Checkbox(
					"Automatically show and hide",
					&config->auto_set_visibility );
			}
			else if constexpr ( has_active_only_v<T> )
			{
				DrawActivateButton( config->active );
			}
		}


	private:
		const char*        description;
		std::shared_ptr<T> config;
	};

}
