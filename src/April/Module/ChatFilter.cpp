
#include "April/Module/ChatFilter.h"

#include <string_view>

using Config = April::Module::ChatFilter::Config;
using namespace GW::Packet::StoC;


namespace {

	// Based on https://github.com/HasKha/GwToolboxpp's implementation.

	auto substr_after( std::wstring_view const msg, wchar_t const id )
		-> std::wstring_view
	{
		auto const it = msg.find( id );
		if ( it == msg.npos || it == msg.size() - 1 )
			return L"";

		return msg.substr( it + 1 );
	}

	auto find_owner( std::wstring_view const msg )
	{
		constexpr auto ids =
			L"\x100\x101\x102\x103\x104\x105\x106\x107\x10D\x10E\x10F\x110";

		auto const pos = msg.find_first_of( ids );
		if ( pos == msg.npos || pos == msg.size() - 1 )
			return 0;

		return static_cast<int>( msg[pos + 1] ) - 0x100;
	}

	bool starts_with(
		std::wstring_view const msg, std::wstring_view const rhs )
	{
		return msg.substr( 0, rhs.size() ) == rhs;
	}

	bool is_rare( std::wstring_view const item_segment )
	{
		constexpr auto ecto =      L"\x0108\x010A\x22D9\xE7B8\xE9DD\x2322";
		constexpr auto gold_item = L"\xA40";
		constexpr auto lockpick =  L"\x0108\x010A\x8101\x730E";
		constexpr auto obby =      L"\x0108\x010A\x22EA\xFDA9\xDE53\x2D16";

		return
			starts_with( item_segment, ecto )
			|| starts_with( item_segment, gold_item )
			|| starts_with( item_segment, lockpick )
			|| starts_with( item_segment, obby );
	}

	bool was_dropped_by_ally( std::wstring_view const msg )
	{
		return starts_with( substr_after( msg, L'\x10A' ), L"\xBA9\x107" );
	}

	bool is_nine_rings_message( std::wstring_view const msg )
	{
		constexpr auto ids =
			L"\x1526\x1529\x152A\x152B\x152C\x1867\x1868\x1869\x186A\x186B"
			L"\x186C\x186D\x39CD";

		return
			msg.size() > 0 && msg.front() == L'\x8101'
			&& msg.substr( 1, 1 ).find_first_of( ids ) != msg.npos;
	}

	bool is_lunar_message( std::wstring_view const msg )
	{
		return
			starts_with(
				msg, L"\x7CC\x962D\xFEB5\x1D08\x10A\xAC2\x101\x164\x1" )
			|| starts_with( msg, L"\x8101\x6648\xB765\xBC0D\x1F73" )
			|| starts_with( msg, L"\x8101\x6649\xA2F9\xBBFA\x3C27" )
			|| starts_with( msg, L"\x8101\x664A\xEFB8\xDE25\x0363" )
			|| starts_with( msg, L"\x8101\x664B\xDBAB\x9F4C\x6742" )
			|| starts_with( msg, L"\x8101\x664C\xD634\x91F8\x76EF" );
	}

	bool should_block( std::wstring const msg, Config const& config )
	{
		if ( not config.active )
			return false;

		switch ( msg[0] )
		{
			case 0x07CC:
			{
				if ( is_lunar_message( msg ) )
					return config.lunars;
				break;
			}
			case 0x07DF: return config.ally_pickup_common; // party shares gold
			case 0x07E0: return config.ally_pickup_common; // party shares gold
			case 0x07F0:
			{
				if ( was_dropped_by_ally( msg ) )
					return false;

				return
					is_rare( substr_after( msg, L'\x10B' ) )
						? config.user_drop_rare : config.user_drop_common;
			}
			case 0x07F1:
			{
				auto const* player = GW::GetCharacter();
				auto const owner = find_owner( msg );
				auto const mine = player && player->player_number == owner;
				auto const rare = is_rare( substr_after( msg, L'\x10B' ) );

				if ( mine && rare ) return config.user_drop_rare;
				if ( mine && not rare ) return config.user_drop_common;
				if ( not mine && rare ) return config.ally_drop_rare;
				if ( not mine && not rare ) return config.ally_drop_common;
			}

			case 0x07F6:
				return
					is_rare( substr_after( msg, L'\x10A' ) )
						? config.ally_pickup_rare : config.ally_pickup_common;

			case 0x07FC:
				return
					is_rare( substr_after( msg, L'\x10A' ) )
						? config.user_pickup_rare : config.user_pickup_common;

			case 0x0816: return config.skill_points;
			case 0x0817: return config.skill_points;
			case 0x087B: return config.no_one_hears_you;
			case 0x087C: return config.no_one_hears_you;
			case 0x087D: return config.whisper_target_away;
			case 0x08A5: return config.target_is_immune; // bleeding (no flesh)
			case 0x08A6: return config.target_is_immune; // disease (no flesh)
			case 0x08A7: return config.target_is_immune; // poison (no flesh)
			case 0x7BF4: return config.you_have_been_playing_for;
			case 0x7BF5: return config.you_have_been_playing_for;
			case 0x8101:
			{
				if ( msg[1] == L'\x029A' )
					return config.guest_privileges;

				if ( msg[1] == L'\x7B91' || msg[1] == L'\x7B92' )
					return config.favor;

				if ( is_lunar_message( msg ) )
					return config.lunars;

				if ( is_nine_rings_message( msg ) )
					return config.nine_rings;

				break;
			}
			case 0x8102:
			{
				switch ( msg[1] )
				{
					case 0x1443: return config.player_has_achieved_title;
					case 0x223B: return config.hall_of_heros;
					case 0x23E2: return config.player_has_achieved_title;
					case 0x23E3: return config.favor;
					case 0x23E4: return config.favor;
					case 0x23E5: return config.player_has_achieved_title;
					case 0x23E6: return config.player_has_achieved_title;
					case 0x4650: return config.skill_updated_for_pvp;
					case 0x4651: return config.skill_updated_for_pvp;
				}

				break;
			}
		}

		return false;
	}

	void update( GW::HookStatus& status, Config const& config )
	{
		if ( should_block( GW::GetMessageBuffer().begin(), config ) )
		{
			status.blocked = true;
			GW::GetMessageBuffer().clear();
		}
	}

}


April::Module::ChatFilter::ChatFilter( std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::ChatFilter::Update(
	GW::HookStatus& status, MessageGlobal const& ) const
{
	update( status, *config );
}

void April::Module::ChatFilter::Update(
	GW::HookStatus& status, MessageLocal const& ) const
{
	update( status, *config );
}

void April::Module::ChatFilter::Update(
	GW::HookStatus& status, MessageServer const& ) const
{
	update( status, *config );
}

void April::Module::ChatFilter::Update(
	GW::HookStatus& status, GW::LocalMessageInfo info ) const
{
	if ( should_block( info.msg, *config ) )
		status.blocked = true;
}
