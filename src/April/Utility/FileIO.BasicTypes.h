#pragma once

#include "April/Utility/FontAtlas.h"

#include "ImGui/ImGui.hpp"

#include "nlohmann/json.hpp"

#include <optional>


namespace nlohmann {

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
