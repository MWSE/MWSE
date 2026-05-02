#include "TES3Game.h"

#include "LuaActivationTargetChangedEvent.h"

#include "LuaManager.h"

namespace TES3 {
	void Game::setGamma(float value) {
		vTable->SetGamma(this, value);
	}

	const auto TES3_Game_initialize = reinterpret_cast<bool(__thiscall *)(Game*)>(0x417880);
	bool Game::initialize() {
		return TES3_Game_initialize(this);
	}

	const auto TES3_Game_clearTarget = reinterpret_cast<bool(__thiscall *)(Game*)>(0x41CD00);
	void Game::clearTarget() {
		TES3_Game_clearTarget(this);
	}

	const auto TES3_Game_savePlayerOptions = reinterpret_cast<void(__thiscall*)(Game*)>(0x4293A0);
	void Game::savePlayerOptions() {
		TES3_Game_savePlayerOptions(this);
	}

	void Game::setPlayerTarget(Reference* reference) {
		if (reference != playerTarget) {
			const auto previous = playerTarget;
			playerTarget = reference;
			if (mwse::lua::event::ActivationTargetChangedEvent::getEventEnabled()) {
				const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
				stateHandle.triggerEvent(new mwse::lua::event::ActivationTargetChangedEvent(reference));
			}
		}
	}

	Game* Game::get() {
		return *reinterpret_cast<TES3::Game**>(0x7C6CDC);
	}
}
