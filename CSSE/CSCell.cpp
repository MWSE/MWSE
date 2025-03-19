#include "CSCell.h"

#include "CSDataHandler.h"
#include "CSGameSetting.h"
#include "CSRecordHandler.h"
#include "CSRegion.h"

namespace se::cs {
	bool Cell::getIsInterior() const {
		return cellFlags & CellFlag::Interior;
	}

	bool Cell::getBehavesAsExterior() const {
		return cellFlags & CellFlag::BehavesAsExterior;
	}

	bool Cell::getIsOrBehavesAsExterior() const {
		return !getIsInterior() || getBehavesAsExterior();
	}

	int Cell::getGridX() const {
		if (getIsInterior()) {
			return 0;
		}
		return gridX;
	}

	int Cell::getGridY() const {
		if (getIsInterior()) {
			return 0;
		}
		return gridY;
	}

	Land* Cell::getLand() const {
		if (getIsInterior()) {
			return nullptr;
		}
		return landscape;
	}

	Region* Cell::getRegion() const {
		if (getIsOrBehavesAsExterior()) {
			return region;
		}
		else {
			return nullptr;
		}
	}

	const char* Cell::getDisplayName() const {
		// Try the cell name first.
		auto name = this->name;
		if (name) {
			return name;
		}

		// Fall back to region name.
		auto region = getRegion();
		if (region) {
			return region->name;
		}

		// Fallback to GMST.
		return DataHandler::get()->recordHandler->gameSettingsHandler->gameSettings[GMST::sDefaultCellname]->value.asString;
	}

	std::string Cell::getEditorId() const {
		std::stringstream ss;

		ss << getDisplayName();
		if (!getIsInterior()) {
			ss << " (" << getGridX() << ", " << getGridY() << ")";
		}

		return std::move(ss.str());
	}
}
