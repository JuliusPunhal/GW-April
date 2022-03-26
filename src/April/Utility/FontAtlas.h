#pragma once

#include "ImGui/ImGui.hpp"

#include <optional>
#include <string>
#include <vector>


namespace April {

	struct FontInfo {
		std::string name;
		int         size;
	};
	bool operator==( FontInfo const&, FontInfo const& );

	using Font = std::optional<FontInfo>;


	class FontAtlas {
	public:
		FontAtlas() = default;

		auto Get( Font const& ) const -> ImFont*;

		// If provided, check full path, else check (1) active directory,
		// (2) AppData/Roaming/GW-April/, and (3) Windows/Fonts/
		bool LoadRequestedFonts();


	private:
		std::vector<FontInfo> mutable requested_fonts{};
		std::vector<FontInfo> unable_to_load{};
	};

}
