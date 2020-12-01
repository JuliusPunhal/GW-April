
#include "April/Config/Gui/HealthEnergy.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::Gui::HealthEnergyConfig;


static_assert( ValidString( health::window_name ) );

static_assert( ValidString( health::font_path ) );
static_assert( IsFontPath( health::font_path ) );


static_assert( ValidString( energy::window_name ) );

static_assert( ValidString( energy::font_path ) );
static_assert( IsFontPath( energy::font_path ) );
