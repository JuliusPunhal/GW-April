
#include "April/Module/ChatFilter.h"

using Config = April::ChatFilter::Config;


namespace {

	constexpr bool ShouldIgnoreByAgentThatDropped( wchar_t const* segment )
	{
		if ( segment == nullptr )
			return false; // something went wrong, don't ignore

		if ( segment[0] == 0xBA9 && segment[1] == 0x107 )
			return false;

		return true;
	}

	constexpr bool FullMatch(
		wchar_t const* s, std::initializer_list<wchar_t> const& msg )
	{
		int i = 0;
		for ( wchar_t b : msg )
		{
			if ( s[i++] != b )
				return false;
		}
		return true;
	}

	constexpr bool IsRare( wchar_t const* item_segment )
	{
		if ( item_segment == nullptr ) // something went wrong, don't ignore
			return false;

		// is gold
		if ( item_segment[0] == 0xA40 )
			return true;

		// Ectos
		if ( FullMatch( item_segment, { 0x108, 0x10A, 0x22D9, 0xE7B8, 0xE9DD, 0x2322 } ) )
			return true;

		// Obsidian shard
		if ( FullMatch( item_segment, { 0x108, 0x10A, 0x22EA, 0xFDA9, 0xDE53, 0x2D16 } ) )
			return true;

		return false;
	}

	constexpr auto Get1stSegment( wchar_t const* message ) -> wchar_t const*
	{
		for ( auto i = 0; message[i] != 0; ++i )
		{
			if ( message[i] == 0x10A )
				return message + i + 1;
		}
		return nullptr;
	}

	constexpr auto Get2ndSegment( wchar_t const* message ) -> wchar_t const*
	{
		for ( auto i = 0; message[i] != 0; ++i )
		{
			if ( message[i] == 0x10B )
				return message + i + 1;
		}
		return nullptr;
	}

	constexpr auto GetNumericSegment( wchar_t const* message ) -> GW::AgentID
	{
		for ( auto i = 0; message[i] != 0; ++i )
		{
			if ( (0x100 < message[i] && message[i] < 0x107)
				|| (0x10D < message[i] && message[i] < 0x110) )
			{
				return (message[i + 1] - 0x100);
			}
		}
		return 0;
	}

	bool in_challenge_mission()
	{
		auto const* info = GW::Map::GetCurrentMapInfo();
		return info && info->type == GW::RegionType_Challenge;
	}

	constexpr bool should_ignore( wchar_t const* message, Config const& config )
	{
		if ( message == nullptr ) return false;

		switch ( message[0] )
		{
			case 0x0108: return false;											// player message
			case 0x0314: return config.guild_announcement;						// Guild Announcement by X: X
			case 0x076D: return false;											// whisper received
			case 0x076E: return false;											// whisper sent
			case 0x0777: return false;											// I'm level x and y% ...	(author is not part of the message)
			case 0x0778: return false;											// I'm following x		(author is not part of the message)
			case 0x077B: return false;											// I'm talking to x		(author is not part of the message)
			case 0x077C: return false;											// I'm wielding x			(author is not part of the message)
			case 0x077D: return false;											// I'm wielding x and y	(author is not part of the message)
			case 0x0781: return false;											// I'm targeting x		(author is not part of the message)
			case 0x0783: return false;											// I'm targeting myself!	(author is not part of the message)
			case 0x0791: return config.emotes;									// emote agree
			case 0x0792: return config.emotes;									// emote attention
			case 0x0793: return config.emotes;									// emote beckon
			case 0x0794: return config.emotes;									// emote beg
			case 0x0795: return config.emotes;									// emote boo
			case 0x07BE: return config.emotes;									// emote yawn
			case 0x07BF: return config.emotes;									// emote yes
			case 0x07C8: return config.quest_reward;							// Quest Reward Accepted: <quest name>
			case 0x07C9: return config.quest_updated;							// Quest Updated: <quest name>
			case 0x07CB: return config.gain_exp;								// You gain (message[5] - 100) experience
			case 0x07CC:
			{
				if ( FullMatch( &message[1], { 0x962D, 0xFEB5, 0x1D08, 0x10A, 0xAC2, 0x101, 0x164, 0x1 } ) )
					return config.lunars;										// you receive 100 gold
				break;
			}
			case 0x07CD: return false;											// You receive <quantity> <item name>
			case 0x07DF: return config.ally_pickup_common;						// party shares gold ?
			case 0x07E0: return config.ally_pickup_common;						// party shares gold
			case 0x07ED: return config.chest_reveal;							// opening the chest reveals x, which your party reserves for y
			case 0x07F0:														// monster/player x drops item y (no assignment)
			{
				auto const agent_who_dropped = Get1stSegment( message );
				if ( !ShouldIgnoreByAgentThatDropped( agent_who_dropped ) )
					return false;

				auto const item_that_dropped = Get2ndSegment( message );
				if ( IsRare( item_that_dropped ) )
					return config.self_drop_rare;

				return config.self_drop_common;
			}
			case 0x07F1:
			{
				// monster x drops item y, your party assigns to player z
				// 0x7F1 0x9A9D 0xE943 0xB33 0x10A <monster> 0x1 0x10B <rarity> 0x10A <item> 0x1 0x1 0x10F <assignee: playernumber + 0x100>
				// <monster> is wchar_t id of several wchars
				// <rarity> is 0x108 for common, 0xA40 gold, 0xA42 purple, 0xA43 green

				auto const owner = GetNumericSegment( message );
				auto const player = GW::Agents::GetCharacter();
				auto const mine = (player && player->player_number == owner);
				auto const rare = IsRare( Get2ndSegment( message ) );

				if ( mine && rare ) return config.self_drop_rare;
				if ( mine && not rare ) return config.self_drop_common;
				if ( not mine && rare ) return config.ally_drop_rare;
				if ( not mine && not rare ) return config.ally_drop_common;
				return false;
			}
			case 0x07F2: return false;											// you drop item x
			case 0x07F6:														// player x picks up item y (note: item can be unassigned gold)
			{
				if ( IsRare( Get2ndSegment( message ) ) )
					return config.ally_pickup_rare;
				else
					return config.ally_pickup_common;
			}
			case 0x07FC: return config.self_pickup;								// you pick up item y (note: item can be unassigned gold)
			case 0x0807: return config.player_joined;							// player joined the game
			case 0x0816: return config.self_gain_skillpoint;					// you gain a skill point
			case 0x0817: return config.ally_gain_skillpoint;					// player x gained a skill point
			case 0x0846: return config.screenshot_taken;						// 'Screenshot saved as <path>'.
			case 0x087B: return config.noone_hears_you;							// 'no one hears you.' (outpost)
			case 0x087C: return config.noone_hears_you;							// 'no one hears you... ' (explorable)
			case 0x087D: return config.away;									// 'Player <name> might not reply...' (Away)
			case 0x087F: return config.dnd;										// 'Failed to send whisper to player <name>...' (Do not disturb)
			case 0x0880: return false;											// 'Player name <name> is invalid.'.
			case 0x0881: return false;											// 'Player <name> is not online.' (Offline)
			case 0x088E: return config.invalid_target;							// Invalid attack target.
			case 0x089B: return config.item_cannot_be_used;						// Item cannot be used in towns or outposts.
			case 0x089C: return config.opening_chest_messages;					// Chest is being used.
			case 0x089D: return config.opening_chest_messages;					// The chest is empty.
			case 0x089E: return config.opening_chest_messages;					// The chest is locked. You must have the correct key or a lockpick.
			case 0x08A0: return config.opening_chest_messages;					// Already used that chest
			case 0x08A5: return config.invalid_target;							// Target is immune to bleeding (no flesh.)
			case 0x08A6: return config.invalid_target;							// Target is immune to disease (no flesh.)
			case 0x08A7: return config.invalid_target;							// Target is immune to poison (no flesh.)
			case 0x08A8: return config.not_enough_energy;						// Not enough adrenaline
			case 0x08A9: return config.not_enough_energy;						// Not enough energy.
			case 0x08AA: return config.inventory_is_full;						// Inventory is full.
			case 0x08AB: return config.invalid_target;							// Your view of the target is obstructed.
			case 0x08C1: return config.invalid_target;							// That skill requires a different weapon type.
			case 0x08C2: return config.invalid_target;							// Invalid spell target.
			case 0x08C3: return config.invalid_target;							// Target is out of range.
			case 0x0AD7: return config.salvage_results;							// You salvaged <number> <item name(s)> from the <item name>
			case 0x2AFC: return false;											// <agent name> hands you <quantity> <item name>
			case 0x4C32: return config.item_cannot_be_used;						// Item can only be used in towns or outposts.
			case 0x52C3:
			{
				// 0x52C3 0xDE9C 0xCD2F 0x78E4 0x101 0x100 - Hold-out bonus: +(message[5] - 0x100) points
				if ( FullMatch( &message[1], { 0xDE9C, 0xCD2F, 0x78E4, 0x101 } ) )
					return config.challenge_mission_messages;
				break;
			}
			case 0x6C9C:
			{
				// 0x6C9C 0x866F 0xB8D2 0x5A20 0x101 0x100 - You gain (message[5] - 0x100) Kurzick faction
				if ( not FullMatch( &message[1], { 0x866F, 0xB8D2, 0x5A20, 0x101 }))
					break;

				#pragma warning( suppress: 6286 )
				return config.faction_gain || config.challenge_mission_messages && in_challenge_mission();
			}
			case 0x6D4D: // 0x6D4D 0xDD4E 0xB502 0x71CE 0x101 0x4E8 - You gain (message[5] - 0x100) Luxon faction
			{
				if ( not FullMatch( &message[1], { 0xDD4E, 0xB502, 0x71CE, 0x101 } ) )
					break;

				#pragma warning( suppress: 6286 )
				return config.faction_gain || config.challenge_mission_messages && in_challenge_mission();
			}
			case 0x7BF4: return config.you_have_been_playing_for;				// You have been playing for x time.
			case 0x7BF5: return config.you_have_been_playing_for;				// You have been playinf for x time. Please take a break.
			case 0x8101:
				switch ( message[1] )
				{
					case 0x03E3:												// Spell failed. Spirits are not affected by this spell.
						return config.invalid_target;
					case 0x1867:												// stay where you are, nine rings is about to begin
					case 0x1868:												// teilah takes 10 festival tickets
					case 0x1869:												// big winner! 55 tickets
					case 0x186A:												// you win 40 tickets
					case 0x186B:												// you win 25 festival tickets
					case 0x186C:												// you win 15 festival tickets
					case 0x186D:												// did not win 9rings
					case 0x1526:												// The rings of fortune did not favor you this time. Stay in the area to try again.
					case 0x1529:												// Pan takes 2 festival tickets
					case 0x152A:												// stay right were you are! rings of fortune is about to begin!
					case 0x152B:												// you win 12 festival tickets
					case 0x152C:												// You win 3 festival tickets
					case 0x39CD:												// you have a special item available: <special item reward>
						return config.ninerings;
					case 0x679C:
						return false;											// You cannot use a <profession> tome because you are not a <profession> (Elite == message[5] == 0x6725)
					case 0x72EB:
						return config.opening_chest_messages;					// The chest is locked. You must use a lockpick to open it.
					case 0x7B91:												// x minutes of favor of the gods remaining. Note: full message is 0x8101 0x7B91 0xC686 0xE490 0x6922 0x101 0x100+value
					case 0x7B92:												// x more achievements must be performed to earn the favor of the gods. // 0x8101 0x7B92 0x8B0A 0x8DB5 0x5135 0x101 0x100+value
						return config.favor_of_gods;
					case 0x7C3E:												// This item cannot be used here.
						return config.item_cannot_be_used;
				}
				if ( FullMatch( &message[1], { 0x6649, 0xA2F9, 0xBBFA, 0x3C27 } )		// you will celebrate a festive new year (rocket or popper)
					|| FullMatch( &message[1], { 0x664B, 0xDBAB, 0x9F4C, 0x6742 } )		// something special is in your future! (lucky aura)
					|| FullMatch( &message[1], { 0x6648, 0xB765, 0xBC0D, 0x1F73 } )		// you will have a prosperous new year! (gain 100 gold)
					|| FullMatch( &message[1], { 0x664C, 0xD634, 0x91F8, 0x76EF } )		// your new year will be a blessed one (lunar blessing)
					|| FullMatch( &message[1], { 0x664A, 0xEFB8, 0xDE25, 0x0363 } ) )	// You will find bad luck in this new year... or bad luck will find you
				{
					return config.lunars;
				}
				return false;
			case 0x8102:
				switch ( message[1] )
				{
					// 0x0EFE is a player message
					case 0x1443: return config.player_achieved_title;			// Player has achieved the title...
					case 0x4650: return config.skill_to_pvp;					// skill has been updated for pvp
					case 0x4651: return config.skill_to_pvp;					// a hero skill has been updated for pvp
					case 0x223F: return false;									// "x minutes of favor of the gods remaining" as a result of /favor command
					case 0x223B: return config.hall_of_heros;					// a party won hall of heroes
					case 0x23E4: return config.favor_of_gods;					// 0xF8AA 0x95CD 0x2766 // the world no longer has the favor of the gods
					case 0x23E5: return true;
					case 0x23E6: return true;
					case 0x29F1: return config.item_cannot_be_used;				// Cannot use this item when no party members are dead.
					case 0x2E35: return config.player_achieved_title;			// Player has achieved the title...
					case 0x2E36: return config.player_achieved_title;			// Player has achieved the title...
					case 0x3772: return false;									// I'm under the effect of x
					case 0x3DCA: return config.item_cannot_be_used;				// This item can only be used in a guild hall
					case 0x4684: return config.summon_in_instance;				// There is already an ally from a summoning stone present in this instance.
					case 0x4685: return config.item_cannot_be_used;				// You have already used a summoning stone within the last 10 minutes.
				}
				return false;
			case 0x8103:
				switch ( message[1] )
				{
					case 0x9CD: return config.item_cannot_be_used;				// You must wait before using another tonic.
				}
			case 0xAD2: return config.item_already_identified;					// That item is already identified
			case 0xADD: return config.item_cannot_be_used;						// That item has no uses remaining
		}

		return false;
	}

}


April::ChatFilter::ChatFilter( Config const& config )
	: config{ config }
{
}

void April::ChatFilter::OnMessage( GW::HookStatus* status ) const
{
	auto& buf = GW::GameContext::instance()->world->message_buff;
	if ( buf.valid() && should_ignore( buf.begin(), config ) )
	{
		status->blocked = true;
		buf.clear();
	}
}

void April::ChatFilter::OnMessage(
	GW::HookStatus* status, wchar_t const* msg ) const
{
	if ( should_ignore( msg, config ) )
	{
		status->blocked = true;
		auto& buf = GW::GameContext::instance()->world->message_buff;
		if ( buf.valid() )
			buf.clear();
	}
}

auto April::ChatFilter::Config::LoadDefault() -> Config
{
	return Config{
		/*bool self_drop_rare  =*/ true,
		/*bool self_drop_common  =*/ true,
		/*bool self_pickup  =*/ true,

		/*bool ally_drop_common  =*/ true,
		/*bool ally_drop_rare  =*/ true,
		/*bool ally_pickup_common  =*/ true,
		/*bool ally_pickup_rare  =*/ true,

		/*bool away  =*/ true,
		/*bool dnd  =*/ false,
		/*bool noone_hears_you  =*/ true,

		/*bool player_joined  =*/ true,
		/*bool player_achieved_title  =*/ true,
		/*bool you_have_been_playing_for  =*/ true,
		/*bool favor_of_gods  =*/ true,
		/*bool screenshot_taken  =*/ true,

		/*bool gain_exp  =*/ true,
		/*bool self_gain_skillpoint  =*/ true,
		/*bool ally_gain_skillpoint  =*/ true,
		/*bool quest_updated  =*/ false,
		/*bool quest_reward  =*/ false,

		/*bool invalid_target  =*/ false,
		/*bool not_enough_energy  =*/ false,

		/*bool item_cannot_be_used  =*/ false,
		/*bool summon_in_instance  =*/ true,
		/*bool inventory_is_full  =*/ false,
		/*bool item_already_identified  =*/ true,
		/*bool salvage_results  =*/ true,

		/*bool challenge_mission_messages  =*/ true,
		/*bool faction_gain  =*/ true,
		/*bool skill_to_pvp  =*/ true,
		/*bool hall_of_heros  =*/ true,

		/*bool emotes  =*/ false,
		/*bool guild_announcement  =*/ false,

		/*bool chest_reveal  =*/ true,
		/*bool opening_chest_messages  =*/ true,

		/*bool lunars  =*/ true,
		/*bool ninerings  =*/ true,
	};
}
