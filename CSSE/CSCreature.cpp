#include "CSCreature.h"

namespace se::cs {
	const char* Creature::getMovementType() const {
		if (getIsBipedal()) {
			return "Bipedal";
		}
		else if (hasMovementWalk()) {
			return "Walk";
		}
		else if (hasMovementFly()) {
			return "Fly";
		}
		else if (hasMovementSwim()) {
			return "Swim";
		}
		else {
			return "None";
		}
	}

	bool Creature::getIsBipedal() const {
		return (actorFlags & TES3::ActorFlagCreature::Biped);
	}

	bool Creature::getUsesWeaponAndShield() const {
		return (actorFlags & TES3::ActorFlagCreature::WeaponAndShield);
	}
}
