
#include "April/Config/Gui/UwTimesGui.config.hpp"

#include "April/Utility/ConfigAssert.h"

using namespace April::Config;
using namespace April::Gui::UwTimesGuiConfig;


static_assert( ValidString( window_name ) );

static_assert( ValidStrings( quest_names ) );
static_assert( quest_names.size() == 11 );

static_assert( ValidStrings( column_names ) );
static_assert( column_names.size() == 4 );

static_assert( ValidString( default_instance_time ) );
static_assert( ValidString( default_quest_time ) );
