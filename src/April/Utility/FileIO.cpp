
#include "April/Utility/FileIO.h"

#include <fstream>


namespace {

	constexpr auto path = "GW-April.json";

}

auto April::load_json_from_file() -> nlohmann::json
{
	if ( auto ifstream = std::ifstream{ path } )
		return nlohmann::json::parse( ifstream );

	return nlohmann::json{};
}

void April::save_json_to_file( nlohmann::json const& json )
{
	if ( auto ofstream = std::ofstream{ path }; ofstream )
		ofstream << json.dump( 4 ) << std::endl;
}
