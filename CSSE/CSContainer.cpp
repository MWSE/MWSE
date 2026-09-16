#include "CSContainer.h"

namespace se::cs {
	bool Container::getIsOrganic() const {
		return (actorFlags & TES3::ActorFlagContainer::Organic) != 0;
	}
}
