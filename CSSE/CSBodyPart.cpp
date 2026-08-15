#include "CSBodyPart.h"

namespace se::cs {
	bool BodyPart::getIsFemale() const {
		return (flags & TES3::BodyPartFlag::Female) != 0;
	}

	bool BodyPart::getIsPlayable() const {
		return (flags & TES3::BodyPartFlag::NotPlayable) == 0;
	}

	const char* BodyPart::getPartName() const {
		const auto partNameArray = reinterpret_cast<const char**>(0x6A3D40);
		return partNameArray[part];
	}

	const char* BodyPart::getTypeName() const {
		const auto partTypeNameArray = reinterpret_cast<const char**>(0x694424);
		return partTypeNameArray[partType];
	}

	const char* BodyPart::getRaceName() const {
		if (partType) {
			return "";
		}
		else {
			return Object::getRaceName();
		}
	}
}
