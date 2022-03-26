
#include "GWCA/GWCA.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#pragma warning( push, 0 )
#include "GWCA/Managers/AgentMgr.h"
#include "GWCA/Managers/CameraMgr.h"
#include "GWCA/Managers/ChatMgr.h"
#include "GWCA/Managers/CtoSMgr.h"
#include "GWCA/Managers/EffectMgr.h"
#include "GWCA/Managers/FriendListMgr.h"
#include "GWCA/Managers/GameThreadMgr.h"
#include "GWCA/Managers/GuildMgr.h"
#include "GWCA/Managers/ItemMgr.h"
#include "GWCA/Managers/MapMgr.h"
#include "GWCA/Managers/MemoryMgr.h"
#include "GWCA/Managers/MerchantMgr.h"
#include "GWCA/Managers/PartyMgr.h"
#include "GWCA/Managers/PlayerMgr.h"
#include "GWCA/Managers/RenderMgr.h"
#include "GWCA/Managers/SkillbarMgr.h"
#include "GWCA/Managers/StoCMgr.h"
#include "GWCA/Managers/TradeMgr.h"
#include "GWCA/Managers/UIMgr.h"
#pragma warning(pop)


void GW::SetRenderCallback( std::function<void( IDirect3DDevice9* )> fn )
{
	GW::Render::SetRenderCallback( fn );
}

void GW::SetResetCallback( std::function<void( IDirect3DDevice9* )> fn )
{
	GW::Render::SetResetCallback( fn );
}
