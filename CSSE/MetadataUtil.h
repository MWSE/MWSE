#pragma once

#include "ModMetadata.h"

namespace se::cs::metadata {
	void reloadModMetadata();
	std::shared_ptr<ModMetadata> getMetadataForGameFile(GameFile* gameFile);
	const std::vector<std::shared_ptr<ModMetadata>>& getActiveModMetadata();

	bool isDeprecated(const BaseObject* object);
}
