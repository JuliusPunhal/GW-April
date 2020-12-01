
#include "April/Config/Gui/Inventory.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::Gui::InventoryConfig;


static_assert( ValidString( window_name ) );

static_assert( ValidString( font_path ) );
static_assert( IsFontPath( font_path ) );
