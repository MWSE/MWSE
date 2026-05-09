#pragma once

#include "NITimeController.h"

namespace NI {
	struct CopyTransformController_vTable : TimeController_vTable {
		CopyTransformController_vTable();

		static CopyTransformController_vTable Instance;
	};

	struct CopyTransformController : public TimeController {
		//
		// Custom data
		//

		AVObject* copyFrom;

		//
		// Virtual table overrides
		//

		void destructor(bool deleting);
		RTTI* getRunTimeTypeInformation() const;
		Object* createClone() const;
		bool registerStreamables(Stream* stream);
		void linkObject(Stream* stream);
		void loadBinary(Stream* stream);
		void saveBinary(Stream* stream);
		bool isEqual(const CopyTransformController* other) const;
		void update(float fTime);
		bool targetIsRequiredType() const;

		//
		// Custom functions
		//

		CopyTransformController(AVObject* copyFrom = nullptr);
		~CopyTransformController();

		//
		// Runtime type information
		//

		static constexpr auto CopyTransformController_Name = "NiCopyTransformController";
		static RTTI CopyTransformController_RTTI;

		//
		// Stream loading
		//

		static Object* loader(Stream* stream);
	};
}
