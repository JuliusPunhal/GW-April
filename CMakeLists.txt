
cmake_minimum_required( VERSION 3.19.0 )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )


project( GW-April )
add_library( GW-April SHARED )

target_include_directories( GW-April PRIVATE src/ )

target_sources(
	GW-April
	PRIVATE
		src/DllMain.cpp
		src/GWCA/GWCA.hpp
		src/GWCA/GWCA.cpp )

if ( MSVC )
	target_compile_options(
		GW-April
		PUBLIC /permissive-
		PRIVATE /W4 /MP /utf-8 )
endif()

add_subdirectory( "external/GWCA" )
target_link_libraries( GW-April PUBLIC gwca )
