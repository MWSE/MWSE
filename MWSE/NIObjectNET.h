#pragma once

#include "NIDefines.h"

#include "NIObject.h"

#include "NIExtraData.h"
#include "NITimeController.h"

#include "TES3Defines.h"

namespace NI {
	struct ObjectNET : Object {
		char * name; // 0x8
		Pointer<ExtraData> extraData; // 0xC
		Pointer<TimeController> controllers; // 0x10

		//
		// Other related this-call functions.
		//

		void prependController(TimeController * controller);
		void removeController(TimeController * controller);
		void removeAllControllers();

		const char* getName() const;
		void setName(const char* name);

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

		TES3::Reference* getTes3Reference(bool searchParents = false) const;
		TES3::Reference* getTes3Reference_lua(sol::optional<bool> searchParents = false) const;

		//
		// Other function addresses.
		//

		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(ObjectNET*, Stream*)>(0x6EA610);
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(const ObjectNET*, Stream*)>(0x6EA710);

	};
	static_assert(sizeof(ObjectNET) == 0x14, "NI::ObjectNET failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ObjectNET)
