
#include "April/Utility/Exit.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>


April::Exit::Exit( std::shared_ptr<Config> config ) : config{ config }
{
}

void April::Exit::Update()
{
	// GetAsyncKeyState() has to be called every frame
	if ( GetAsyncKeyState( VK_END ) && config->allow_vk_end )
		exit = true;
}
