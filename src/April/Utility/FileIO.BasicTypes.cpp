
#include "April/Utility/FileIO.BasicTypes.h"

#include <exception>
#include <string>

using namespace April::Module;
using Abbreviation = ChatCommands::Abbreviation;


void nlohmann::adl_serializer<Abbreviation>::to_json(
	json& j, Abbreviation const& abbrev )
{
	j[abbr_key] = abbrev.abbreviated();
	j[exp_key] = abbrev.expanded();
}

auto nlohmann::adl_serializer<Abbreviation>::from_json( json const& j )
	-> Abbreviation
{
	auto const abbr = j[abbr_key].get<std::string>();
	auto const exp = j[exp_key].get<std::string>();

	auto const result = make_Abbreviation( abbr, exp );
	if ( result == std::nullopt )
	{
		// cannot gracefully return error value
		throw std::exception{
			"Cannot create Abbrevated from provided input!"
		};
	}

	return *result;
}
