#pragma once

#include "April/Gui/Skillbar.h"
#include "April/Utility/FontAtlas.h"
#include "April/Utility/sorted_vector.h"

#include "GWCA/GWCA.hpp"

#include "ImGui/ImGui.hpp"

#include "nlohmann/json.hpp"

#include <optional>


namespace nlohmann {

	template<typename T, typename Comp>
	struct adl_serializer<April::sorted_vector<T, Comp>> {
		using sort_vec = April::sorted_vector<T, Comp>;

		static void to_json( json& j, sort_vec const& s_vec )
		{
			using underlying = typename sort_vec::underlying_t;
			j = underlying{ s_vec.begin(), s_vec.end() };
		}

		static void from_json( json const& j, sort_vec& s_vec )
		{
			s_vec = j.get<sort_vec::underlying_t>();
		}
	};

	template<>
	struct adl_serializer<GW::ms32> {
		static void to_json( json& j, GW::ms32 const ms )
		{
			j = ms.count();
		}

		static void from_json( json const& j, GW::ms32& ms )
		{
			ms = GW::ms32{ j.get<int>() };
		}
	};

	template<typename T>
	struct adl_serializer<std::optional<T>> {
		static void to_json( json& j, std::optional<T> const& opt )
		{
			if ( opt.has_value() )
			{
				j = *opt;
			}
			else j = nullptr;
		}

		static void from_json( json const& j, std::optional<T>& opt )
		{
			if ( j.is_null() )
			{
				opt = std::nullopt;
			}
			else opt = j.get<T>();
		}
	};

}

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ImVec2, x, y )
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( ImVec4, x, y, z, w )

namespace ImGui {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( Window, name, visible, flags )

}

namespace April {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE( FontInfo, name, size )

}

namespace April::Gui {

	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
		Skillbar::Config::Threshold, uptime, color )

}
