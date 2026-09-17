#include "CSCreature.h"

namespace se::cs {
	const char* Creature::getMovementType() const {
		if (getIsBipedal()) {
			return "Bipedal";
		}
		else if (canWalk()) {
			return "Walk";
		}
		else if (canFly()) {
			return "Fly";
		}
		else if (canSwim()) {
			return "Swim";
		}
		else {
			return "None";
		}
	}

	bool Creature::getIsBipedal() const {
		return (actorFlags & ActorFlagCreature::Biped);
	}

	bool Creature::getUsesWeaponAndShield() const {
		return (actorFlags & ActorFlagCreature::WeaponAndShield);
	}
}
