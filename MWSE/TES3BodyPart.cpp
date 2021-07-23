#include "TES3BodyPart.h"

namespace TES3 {
	bool BodyPart::getFlag(BodyPartFlag::Flag flag) {
		return (flags & flag) != 0;
	}

	void BodyPart::setFlag(BodyPartFlag::Flag flag, bool set) {
		if (set) {
			flags |= flag;
		}
		else {
			flags &= ~flag;
		}
	}

	bool BodyPart::getIsFemale() {
		return getFlag(TES3::BodyPartFlag::Female);
	}

	void BodyPart::setIsFemale(bool value) {
		setFlag(TES3::BodyPartFlag::Female, value);
	}

	bool BodyPart::getIsPlayable() {
		return getFlag(TES3::BodyPartFlag::Playable);
	}

	void BodyPart::setIsPlayable(bool value) {
		setFlag(TES3::BodyPartFlag::Playable, value);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::BodyPart)
