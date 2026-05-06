#include "NIObjectNET.h"

#include "NIAVObject.h"
#include "NINode.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Reference.h"
#endif

#include "ExceptionUtil.h"

#include <cstring>

namespace NI {
	void ObjectNET::prependController(TimeController* controller) {
#if defined(SE_NI_OBJECTNET_FNADDR_PREPENDCONTROLLER) && SE_NI_OBJECTNET_FNADDR_PREPENDCONTROLLER > 0
		const auto NI_ObjectNET_prependController = reinterpret_cast<void(__thiscall*)(const ObjectNET*, TimeController*)>(SE_NI_OBJECTNET_FNADDR_PREPENDCONTROLLER);
		NI_ObjectNET_prependController(this, controller);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::removeController(TimeController* controller) {
#if defined(SE_NI_OBJECTNET_FNADDR_REMOVECONTROLLER) && SE_NI_OBJECTNET_FNADDR_REMOVECONTROLLER > 0
		const auto NI_ObjectNET_removeController = reinterpret_cast<void(__thiscall*)(const ObjectNET*, TimeController*)>(SE_NI_OBJECTNET_FNADDR_REMOVECONTROLLER);
		NI_ObjectNET_removeController(this, controller);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::removeAllControllers() {
#if defined(SE_NI_OBJECTNET_FNADDR_REMOVEALLCONTROLLERS) && SE_NI_OBJECTNET_FNADDR_REMOVEALLCONTROLLERS > 0
		const auto NI_ObjectNET_removeAllControllers = reinterpret_cast<void(__thiscall*)(const ObjectNET*)>(SE_NI_OBJECTNET_FNADDR_REMOVEALLCONTROLLERS);
		NI_ObjectNET_removeAllControllers(this);
#else
		throw not_implemented_exception();
#endif
	}

	const char* ObjectNET::getName() const {
		return this->name;
	}

	void ObjectNET::setName(const char* name) {
#if defined(SE_NI_OBJECTNET_FNADDR_SETNAME) && SE_NI_OBJECTNET_FNADDR_SETNAME > 0
		const auto NI_ObjectNET_setName = reinterpret_cast<void(__thiscall*)(const ObjectNET*, const char*)>(SE_NI_OBJECTNET_FNADDR_SETNAME);
		NI_ObjectNET_setName(this, name);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::setFlag(bool state, byte index) {
#if defined(SE_NI_OBJECTNET_FNADDR_SETFLAG) && SE_NI_OBJECTNET_FNADDR_SETFLAG > 0
		const auto NI_ObjectNET_setFlag = reinterpret_cast<void(__thiscall*)(ObjectNET*, bool, byte)>(SE_NI_OBJECTNET_FNADDR_SETFLAG);
		NI_ObjectNET_setFlag(this, state, index);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::addExtraData(ExtraData* data) {
#if defined(SE_NI_OBJECTNET_FNADDR_ADDEXTRADATA) && SE_NI_OBJECTNET_FNADDR_ADDEXTRADATA > 0
		const auto NI_ObjectNET_addExtraData = reinterpret_cast<void(__thiscall*)(ObjectNET*, ExtraData*)>(SE_NI_OBJECTNET_FNADDR_ADDEXTRADATA);
		NI_ObjectNET_addExtraData(this, data);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::removeExtraData(ExtraData* data) {
#if defined(SE_NI_OBJECTNET_FNADDR_REMOVEEXTRADATA) && SE_NI_OBJECTNET_FNADDR_REMOVEEXTRADATA > 0
		const auto NI_ObjectNET_removeExtraData = reinterpret_cast<void(__thiscall*)(ObjectNET*, ExtraData*)>(SE_NI_OBJECTNET_FNADDR_REMOVEEXTRADATA);
		NI_ObjectNET_removeExtraData(this, data);
#else
		throw not_implemented_exception();
#endif
	}

	void ObjectNET::removeAllExtraData() {
#if defined(SE_NI_OBJECTNET_FNADDR_REMOVEALLEXTRADATA) && SE_NI_OBJECTNET_FNADDR_REMOVEALLEXTRADATA > 0
		const auto NI_ObjectNET_removeAllExtraData = reinterpret_cast<void(__thiscall*)(ObjectNET*)>(SE_NI_OBJECTNET_FNADDR_REMOVEALLEXTRADATA);
		NI_ObjectNET_removeAllExtraData(this);
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<StringExtraData> ObjectNET::getStringDataWithValue(const char* value) const {
		if (!value) {
			return nullptr;
		}

		ExtraData* extra = extraData;
		while (extra) {
			if (extra->isInstanceOfType(NI::RTTIStaticPtr::NiStringExtraData) && static_cast<NI::StringExtraData*>(extra)->string) {
				if (_stricmp(value, static_cast<NI::StringExtraData*>(extra)->string) == 0) {
					return reinterpret_cast<NI::StringExtraData*>(extra);
				}
			}
			extra = extra->next;
		}
		return nullptr;
	}

	bool ObjectNET::hasStringDataWithValue(const char* value) const {
		return getStringDataWithValue(value) != nullptr;
	}

	Pointer<StringExtraData> ObjectNET::getStringDataStartingWithValue(const char* value) const {
		if (!value) {
			return nullptr;
		}

		size_t maxCount = strlen(value);
		ExtraData* extra = extraData;
		while (extra) {
			if (extra->isInstanceOfType(NI::RTTIStaticPtr::NiStringExtraData) && static_cast<NI::StringExtraData*>(extra)->string) {
				if (_strnicmp(value, static_cast<NI::StringExtraData*>(extra)->string, maxCount) == 0) {
					return reinterpret_cast<NI::StringExtraData*>(extra);
				}
			}
			extra = extra->next;
		}
		return nullptr;
	}

	bool ObjectNET::hasStringDataStartingWithValue(const char* value) const {
		return getStringDataStartingWithValue(value) != nullptr;
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	TES3::Reference* ObjectNET::getTes3Reference(bool searchParents) const {
		for (ExtraData* ed = extraData; ed; ed = ed->next) {
			if (ed->isOfType(RTTIStaticPtr::TES3ObjectExtraData)) {
				return static_cast<Tes3ExtraData*>(ed)->reference;
			}
		}

		if (searchParents && isInstanceOfType(RTTIStaticPtr::NiAVObject) && static_cast<const AVObject*>(this)->parentNode) {
			return static_cast<const AVObject*>(this)->parentNode->getTes3Reference(true);
		}

		return nullptr;
	}
#else
	se::cs::Reference* ObjectNET::getTes3Reference(bool searchParents) const {
		for (ExtraData* ed = extraData; ed; ed = ed->next) {
			if (ed->isOfType(RTTIStaticPtr::TES3ObjectExtraData)) {
				return static_cast<Tes3ExtraData*>(ed)->reference;
			}
		}

		if (searchParents && isInstanceOfType(RTTIStaticPtr::NiAVObject) && static_cast<const AVObject*>(this)->parentNode) {
			return static_cast<const AVObject*>(this)->parentNode->getTes3Reference(true);
		}

		return nullptr;
	}
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	TES3::Reference* ObjectNET::getTes3Reference_lua(sol::optional<bool> searchParents) const {
		return getTes3Reference(searchParents.value_or(false));
	}
#endif
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ObjectNET)
#endif
