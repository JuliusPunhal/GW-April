
#include "April/Module/GuildEditor.h"

#include "April/Utility/stl.h"

#include <array>
#include <string_view>

using namespace GW::Packet::StoC;


namespace {

	template<auto N>
	void copy_to_array( std::wstring const& wstr, std::array<wchar_t, N>& arr )
	{
		auto const end =
			wstr.size() < arr.size()
			? wstr.end()
			: wstr.begin() + arr.size() - 1; // ensure space for terminator

		auto next = std::copy( wstr.begin(), end, arr.begin() );
		*next = L'\0';
	}

}


April::Module::GuildEditor::GuildEditor( std::shared_ptr<Config const> config )
	: config{ config }
{
}

void April::Module::GuildEditor::Update( MapLoaded const& )
{
	unmodified = std::nullopt;
}

void April::Module::GuildEditor::Update( UpdateGuildInfo& packet )
{
	auto const guild_to_edit = stl::utf8_to_wstr( config->guild_to_edit );
	auto const packet_name = std::wstring_view{ packet.guild_name.data() };
	if ( packet_name != guild_to_edit )
		return;

	unmodified = packet;

	if ( config->active_cape )
		ApplyCapeToPacket( packet );

	if ( config->active_name )
		ApplyNameToPacket( packet );

	if ( config->active_tag )
		ApplyTagToPacket( packet );
}

void April::Module::GuildEditor::ApplyNameToPacket(
	UpdateGuildInfo& packet ) const
{
	if ( config->new_name.empty() )
		return;

	auto const new_name = stl::utf8_to_wstr( config->new_name );
	copy_to_array( new_name, packet.guild_name );
}

void April::Module::GuildEditor::ApplyTagToPacket(
	UpdateGuildInfo& packet ) const
{
	if ( config->new_tag.empty() )
		return;

	auto const new_tag = stl::utf8_to_wstr( config->new_tag );
	copy_to_array( new_tag, packet.guild_tag );
}

void April::Module::GuildEditor::ApplyCapeToPacket(
	UpdateGuildInfo& packet ) const
{
	packet.cape_background_color = config->new_cape.background_color;
	packet.cape_detail_color =     config->new_cape.detail_color;
	packet.cape_emblem_color =     config->new_cape.emblem_color;
	packet.cape_shape =            config->new_cape.shape;
	packet.cape_detail =           config->new_cape.detail;
	packet.cape_emblem =           config->new_cape.emblem;
	packet.cape_trim =             config->new_cape.trim;
}
