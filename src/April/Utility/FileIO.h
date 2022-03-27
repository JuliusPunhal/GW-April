#pragma once

#include "April/Utility/FileIO.ConfigTypes.h"
#include "April/Utility/FileIO.JsonKey.h"

#include "nlohmann/json.hpp"


namespace April {

	auto load_json_from_file() -> nlohmann::json;
	void save_json_to_file( nlohmann::json const& );


	template<typename T>
	auto from_json( nlohmann::json const& json ) -> std::shared_ptr<T>
	{
		try
		{
			return std::make_shared<T>( json.at( JsonKey<T>::key ).get<T>() );
		}
		catch ( std::exception const& )
		{
			return std::make_shared<T>();
		}
	}

	template<typename T>
	auto to_json( std::shared_ptr<T> config ) -> nlohmann::json
	{
		auto json = nlohmann::json{};

		if ( config )
			json[JsonKey<T>::key] = *config;

		return json;
	}

}
