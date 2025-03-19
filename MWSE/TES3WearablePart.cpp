#include "TES3WearablePart.h"

namespace TES3 {
	WearablePart::WearablePart() {
		bodypartID = 255;
		male = nullptr;
		female = nullptr;
	}

	BodyPart* WearablePart::getPart(bool isFemale) const {
		if (!isValid()) {
			return nullptr;
		}

		const auto PART_INVALID = reinterpret_cast<BodyPart*>(-1);
		if (isFemale && female && female != PART_INVALID) {
			return female;
		}
		else {
			return male;
		}
	}

	bool WearablePart::isValid() const {
		return bodypartID != 255;
	}
}