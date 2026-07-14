#include "TES3Game.h"

#include "LuaActivationTargetChangedEvent.h"

#include "LuaManager.h"

#include "WindowsUtil.h"

namespace TES3 {
	void Game::setGamma(float value) {
		vTable->SetGamma(this, value);
	}

	const auto TES3_Game_readSettings = reinterpret_cast<bool(__thiscall*)(Game*)>(0x4F54F0);
	bool Game::readSettings() {
		const auto result = TES3_Game_readSettings(this);
		applyApplicationIniOverrides();
		return result;
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

	const auto TES3_Game_renderNextFrame = reinterpret_cast<void(__thiscall*)(Game*, int)>(0x41BE90);
	void Game::renderNextFrame(int renderType) {
		TES3_Game_renderNextFrame(this, renderType);
	}

	void Game::applyApplicationIniOverrides() {
		constexpr auto morrowindIniFile = ".\\Morrowind.ini";

		windowWidth = se::windows::ini::GetInt(morrowindIniFile, "Application", "Screen Width").value_or(windowWidth);
		windowHeight = se::windows::ini::GetInt(morrowindIniFile, "Application", "Screen Height").value_or(windowHeight);
		screenDepth = se::windows::ini::GetInt(morrowindIniFile, "Application", "Screen Depth").value_or(screenDepth);
		backBuffers = se::windows::ini::GetInt(morrowindIniFile, "Application", "Backbuffers").value_or(backBuffers);
		multiSamples = se::windows::ini::GetInt(morrowindIniFile, "Application", "Multisamples").value_or(multiSamples);
		mipmapSkipLevel = se::windows::ini::GetInt(morrowindIniFile, "Application", "Mipmap Skip Level").value_or(mipmapSkipLevel);
		vertexProcessing = se::windows::ini::GetInt(morrowindIniFile, "Application", "Vertex Processing").value_or(vertexProcessing);
		swapEffect = se::windows::ini::GetInt(morrowindIniFile, "Application", "Swap Effect").value_or(swapEffect);
		refreshRate = se::windows::ini::GetInt(morrowindIniFile, "Application", "Refresh Rate").value_or(refreshRate);
		presentationInterval = se::windows::ini::GetInt(morrowindIniFile, "Application", "Presentation Interval").value_or(presentationInterval);
		adapter = se::windows::ini::GetInt(morrowindIniFile, "Application", "Adapter").value_or(adapter);
		gamma = se::windows::ini::GetFloat(morrowindIniFile, "Application", "Gamma").value_or(gamma);
		fullscreen = se::windows::ini::GetBool(morrowindIniFile, "Application", "Fullscreen").value_or(fullscreen);
		pixelShader = se::windows::ini::GetBool(morrowindIniFile, "Application", "Pixelshader").value_or(pixelShader);
		stencil = se::windows::ini::GetBool(morrowindIniFile, "Application", "Stencil").value_or(stencil);
		mipmap = se::windows::ini::GetBool(morrowindIniFile, "Application", "Mipmap").value_or(mipmap);
		hardware = se::windows::ini::GetBool(morrowindIniFile, "Application", "Hardware").value_or(hardware);
		multiPass = se::windows::ini::GetBool(morrowindIniFile, "Application", "Multipass").value_or(multiPass);

		gamma = std::clamp(gamma, 0.0f, 2.0f);
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
