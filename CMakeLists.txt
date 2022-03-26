
cmake_minimum_required( VERSION 3.19.0 )


project( GW-April )
add_library( GW-April SHARED )

target_include_directories( GW-April PRIVATE src/ )

target_sources( GW-April PRIVATE src/DllMain.cpp )

target_compile_features( GW-April PUBLIC cxx_std_17 )

if ( MSVC )
	target_compile_options(
		GW-April
		PUBLIC /permissive-
		PRIVATE /W4 /MP /utf-8 )
endif()
