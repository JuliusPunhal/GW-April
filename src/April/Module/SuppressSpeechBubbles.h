#pragma once

#include "GWCA/GWCA.hpp"

#include <memory>


namespace April::Module {

	class SuppressSpeechBubbles {
	public:
		struct Config {
			bool active = true;
		};


		SuppressSpeechBubbles( std::shared_ptr<Config const> );

		void Update(
			GW::HookStatus&, GW::Packet::StoC::SpeechBubble const& ) const;

	private:
		std::shared_ptr<Config const> config;
	};

}
