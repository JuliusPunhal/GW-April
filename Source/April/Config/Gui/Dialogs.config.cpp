
#include "April/Config/Gui/Dialogs.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::Gui::DialogsConfig;


namespace {

	constexpr bool ValidDescriptions()
	{
		for ( auto const& dialog : dialogs )
		{
			if ( not ValidString( dialog.description ) ) return false;
		}
		return true;
	}

}


static_assert( ValidString( window_name ) );

static_assert( ValidDescriptions() );
