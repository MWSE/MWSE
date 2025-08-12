#include "TES3CombatSession.h"

#include "LuaManager.h"

#include "LuaDetermineActionEvent.h"
#include "LuaDeterminedActionEvent.h"

namespace TES3 {

	//
	//
	//

	const auto TES3_CombatSessionVanilla_ctor = reinterpret_cast<CombatSessionVanilla*(__thiscall*)(CombatSessionVanilla*, MobileActor*)>(0x537120);
	CombatSessionVanilla* CombatSessionVanilla::ctor(CombatSessionVanilla* self, DWORD _UNUSED_, MobileActor* mobile) {
		return TES3_CombatSessionVanilla_ctor(self, mobile);
	}

	const auto TES3_CombatSessionVanilla_dtor = reinterpret_cast<void(__thiscall*)(CombatSessionVanilla*)>(0x5372F0);
	void CombatSessionVanilla::dtor(CombatSessionVanilla* self) {
		TES3_CombatSessionVanilla_dtor(self);
	}

	//
	//
	//

	const auto TES3_CombatSession_chooseAlchemyWithEffect = reinterpret_cast<float(__thiscall*)(CombatSession*, short)>(0x538330);
	float CombatSession::chooseAlchemyWithEffect(short id) {
		return TES3_CombatSession_chooseAlchemyWithEffect(this, id);
	}

	const auto TES3_CombatSession_changeEquipment = reinterpret_cast<void(__thiscall*)(CombatSession*, EquipmentStack*)>(0x5379E0);
	void CombatSession::changeEquipment(TES3::EquipmentStack* equipmentStack) {
		TES3_CombatSession_changeEquipment(this, equipmentStack);
	}

	const auto TES3_CombatSession_determineNextAction = reinterpret_cast<void (__thiscall*)(CombatSession*)>(0x538F00);
	void CombatSession::determineNextAction() {
		auto& luaManager = mwse::lua::LuaManager::getInstance();
		if (mwse::lua::event::DetermineActionEvent::getEventEnabled()) {
			const auto stateHandle = luaManager.getThreadSafeStateHandle();
			sol::table result = stateHandle.triggerEvent(new mwse::lua::event::DetermineActionEvent(this));
			if (result.valid() && result.get_or("block", false)) {
				if (mwse::lua::event::DeterminedActionEvent::getEventEnabled()) {
					stateHandle.triggerEvent(new mwse::lua::event::DeterminedActionEvent(this));
				}
				return;
			}
		}

		TES3_CombatSession_determineNextAction(this);

		if (mwse::lua::event::DeterminedActionEvent::getEventEnabled()) {
			luaManager.getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::DeterminedActionEvent(this));
		}
	}

	CombatSession::CombatSession(MobileActor* mobile) {
		ctor(this, 0, mobile);
	}

	CombatSession::~CombatSession() {
		dtor(this);
	}

	CombatSession* CombatSession::ctor(CombatSession* self, DWORD _UNUSED_, MobileActor* mobile) {
		CombatSessionVanilla::ctor(self, _UNUSED_, mobile);
		self->luaData = nullptr;
		return self;
	}

	void CombatSession::dtor(CombatSession* self) {
		CombatSessionVanilla::dtor(self);

		if (self->luaData) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			delete self->luaData;
			self->luaData = nullptr;
		}
	}

	void CombatSession::setLuaData(sol::object data) {
		if (data == sol::nil) {
			if (luaData != nullptr) {
				delete luaData;
			}
		}
		else {
			if (luaData == nullptr) {
				luaData = new LuaData();
			}
			luaData->data = data;
		}
	}

	sol::object CombatSession::getLuaData() {
		if (luaData == nullptr) {
			return sol::nil;
		}
		return luaData->data;
	}

	//
	// 
	//

	CombatSession::LuaData::LuaData() {
		data = (sol::object)sol::nil;
	}
}
