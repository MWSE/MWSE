#pragma once

#include "NIDefines.h"

#include "NIObject.h"

#include "NIExtraData.h"
#include "NITimeController.h"

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

		GameReferenceType* getTes3Reference(bool searchParents = false) const;

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		TES3::Reference* getTes3Reference_lua(sol::optional<bool> searchParents = false) const;
#endif

		//
		// Other function addresses.
		//

#if defined(SE_NI_OBJECTNET_FNADDR_LOADBINARY) && SE_NI_OBJECTNET_FNADDR_LOADBINARY == 1
		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(ObjectNET*, Stream*)>(SE_NI_OBJECTNET_FNADDR_LOADBINARY);
#endif
#if defined(SE_NI_OBJECTNET_FNADDR_SAVEBINARY) && SE_NI_OBJECTNET_FNADDR_SAVEBINARY == 1
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(const ObjectNET*, Stream*)>(SE_NI_OBJECTNET_FNADDR_SAVEBINARY);
#endif

	};
	static_assert(sizeof(ObjectNET) == 0x14, "NI::ObjectNET failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ObjectNET)
#endif
