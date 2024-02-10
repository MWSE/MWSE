#include "CSContainer.h"

namespace se::cs {
	bool Container::getIsOrganic() const {
		return (actorFlags & ActorFlagContainer::Organic) != 0;
	}
}
