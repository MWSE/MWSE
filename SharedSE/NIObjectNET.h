#pragma once

#include "NIDefines.h"

#include "NIObject.h"

#include "NIExtraData.h"
#include "NITimeController.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "TES3Defines.h"
#elif defined(SE_IS_MGE) && SE_IS_MGE == 1
// MGE-XE doesn't reference TES3:: or se::cs:: game classes; getTes3Reference
// is omitted on this target (declaration below also gated).
#else
#include "CSDefines.h"
#endif

namespace NI {
	struct ObjectNET : Object {
		char* name; // 0x8
		Pointer<ExtraData> extraData; // 0xC
		Pointer<TimeController> controllers; // 0x10

		//
		// Other related this-call functions.
		//

		void prependController(TimeController* controller);
		void removeController(TimeController* controller);
		void removeAllControllers();

		const char* getName() const;
		void setName(const char* name);

		void setFlag(bool state, byte index);

		//
		// Custom functions.
		//

		void addExtraData(ExtraData* data);
		void removeExtraData(ExtraData* data);
		void removeAllExtraData();

		Pointer<StringExtraData> getStringDataWithValue(const char* value) const;
		bool hasStringDataWithValue(const char* value) const;

		Pointer<StringExtraData> getStringDataStartingWithValue(const char* value) const;
		bool hasStringDataStartingWithValue(const char* value) const;

		// Per-target return type: TES3::Reference for MWSE, se::cs::Reference
		// for CSSE (legitimate target-specific type, not cosmetic drift).
		// Const-ness is unified (MWSE-canonical). Omitted entirely on MGE,
		// which has no game-class types to return and never calls into
		// the MWSE-only NIObjectNET.cpp implementation.
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		TES3::Reference* getTes3Reference(bool searchParents = false) const;
#elif defined(SE_IS_MGE) && SE_IS_MGE == 1
		// (intentionally omitted)
#else
		se::cs::Reference* getTes3Reference(bool searchParents = false) const;
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		TES3::Reference* getTes3Reference_lua(sol::optional<bool> searchParents = false) const;
#endif

		//
		// Other function addresses.
		//

		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(ObjectNET*, Stream*)>(0x6EA610);
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(const ObjectNET*, Stream*)>(0x6EA710);

	};
	static_assert(sizeof(ObjectNET) == 0x14, "NI::ObjectNET failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ObjectNET)
#endif
