#include "TES3Cell.h"

#include "MemoryUtil.h"

#include "LuaManager.h"

#include "LuaCellActivatedEvent.h"
#include "LuaCellDeactivatedEvent.h"

#include "TES3DataHandler.h"
#include "TES3GameSetting.h"
#include "TES3Land.h"
#include "TES3Region.h"

namespace TES3 {
	//
	// PathGrid
	//

	const auto TES3_PathGrid_Show = reinterpret_cast<void(__thiscall*)(PathGrid*)>(0x4F4560);
	void PathGrid::show() {
		TES3_PathGrid_Show(this);
	}

	const auto TES3_PathGrid_Hide = reinterpret_cast<void(__thiscall*)(PathGrid*)>(0x4F48D0);
	void PathGrid::hide() {
		TES3_PathGrid_Hide(this);
	}

	//
	// Cell
	//

	const auto TES3_Cell_constructor = reinterpret_cast<Cell*(__thiscall *)(Cell*)>(0x4DB500);
	Cell * Cell::create() {
		return TES3_Cell_constructor(mwse::tes3::_new<Cell>());
	}

	const auto TES3_Cell_getFirstObjectOfType = reinterpret_cast<Reference*(__thiscall *)(const Cell*, int, unsigned char)>(0x4DBEF0);
	Reference * Cell::getFirstObjectOfType(ObjectType::ObjectType type, bool skipDeleted) const {
		return TES3_Cell_getFirstObjectOfType(this, type, skipDeleted);
	}

	const auto TES3_Cell_getExteriorGridX = reinterpret_cast<int(__thiscall *)(const Cell*)>(0x4DB9D0);
	int Cell::getGridX() const {
		return TES3_Cell_getExteriorGridX(this);
	}

	const auto TES3_Cell_setGridX = reinterpret_cast<void(__thiscall *)(Cell*, int)>(0x4DB9E0);
	void Cell::setGridX(int x) {
		TES3_Cell_setGridX(this, x);
	}

	const auto TES3_Cell_getExteriorGridY = reinterpret_cast<int(__thiscall *)(const Cell*)>(0x4DB9F0);
	int Cell::getGridY() const {
		return TES3_Cell_getExteriorGridY(this);
	}

	const auto TES3_Cell_setGridY = reinterpret_cast<void(__thiscall *)(Cell*, int)>(0x4DBA00);
	void Cell::setGridY(int y) {
		TES3_Cell_setGridY(this, y);
	}

	const auto TES3_Cell_setName = reinterpret_cast<void(__thiscall *)(Cell*, const char*)>(0x4E25E0);
	void Cell::setName(const char* name) {
		TES3_Cell_setName(this, name);
	}

	const auto TES3_Cell_addMapNote = reinterpret_cast<void(__thiscall*)(Cell*, Vector2*, float, const char*)>(0x4E3730);
	void Cell::addMapNote(Vector2* position, float unknown, const char* text) {
		TES3_Cell_addMapNote(this, position, unknown, text);
	}

	const auto TES3_Cell_addReference = reinterpret_cast<void(__thiscall*)(Cell*, Reference*)>(0x4DC0A0);
	void Cell::addReference(Reference* reference) {
		TES3_Cell_addReference(this, reference);
	}

	const auto TES3_Cell_insertReference = reinterpret_cast<void(__thiscall *)(Cell*, Reference*)>(0x4DC030);
	void Cell::insertReference(Reference* reference) {
		TES3_Cell_insertReference(this, reference);

		// Fire off activation event.
		if (getCellActive()) {
			reference->setReferenceActive();
		}
	}

	const auto TES3_Cell_getOrCreateActivatorsNode = reinterpret_cast<NI::Node*(__thiscall*)(Cell*)>(0x4E2680);
	NI::Node* Cell::getOrCreateActivatorsNode() {
		return TES3_Cell_getOrCreateActivatorsNode(this);
	}

	const char* Cell::getName() const {
		return name;
	}

	bool Cell::getCellFlag(unsigned int flag) const {
		return (cellFlags & flag);
	}

	void Cell::setCellFlag(unsigned int flag, bool value) {
		if (value) {
			cellFlags |= flag;
		}
		else {
			cellFlags &= ~flag;
		}
	}

	std::optional<float> Cell::getFogDensity() const {
		if (getIsInterior()) {
			return variantData.interior.fogDensity;
		}
		return {};
	}

	void Cell::setFogDensity(float value) {
		if (getIsInterior()) {
			variantData.interior.fogDensity = value;
		}
	}

	std::optional<float> Cell::getWaterLevel() const {
		if (getIsOrBehavesAsExterior()) {
			return 0.0;
		}
		else if (getHasWater()) {
			return waterLevelOrRegion.waterLevel;
		}
		return {};
	}

	void Cell::setWaterLevel(float value) {
		if (getIsOrBehavesAsExterior()) {
			return;
		}
		else {
			if (getHasWater()) {
				waterLevelOrRegion.waterLevel = value;
			}
		}
	}

	Land* Cell::getLandscape() const {
		if (!getIsInterior()) {
			return variantData.exterior.landscape;
		}
		else {
			return nullptr;
		}
	}

	Region* Cell::getRegion() const {
		if (getIsOrBehavesAsExterior()) {
			return waterLevelOrRegion.region;
		}
		else {
			return nullptr;
		}
	}

	NI::PackedColor* Cell::getAmbientColor() {
		if (getIsInterior()) {
			return &variantData.interior.ambientColor;
		}
		return nullptr;
	}

	NI::PackedColor* Cell::getFogColor() {
		if (getIsInterior()) {
			return &variantData.interior.fogColor;
		}
		return nullptr;
	}

	NI::PackedColor* Cell::getSunColor() {
		if (getIsInterior()) {
			return &variantData.interior.sunColor;
		}
		return nullptr;
	}

	bool Cell::getBehavesAsExterior() const {
		return getCellFlag(TES3::CellFlag::BehavesAsExterior);
	}

	void Cell::setBehavesAsExterior(bool value) {
		setCellFlag(TES3::CellFlag::BehavesAsExterior, value);
	}

	bool Cell::getHasWater() const {
		return getCellFlag(TES3::CellFlag::HasWater);
	}

	void Cell::setHasWater(bool value) {
		setCellFlag(TES3::CellFlag::HasWater, value);
	}

	bool Cell::getIsInterior() const {
		return getCellFlag(TES3::CellFlag::Interior);
	}

	void Cell::setIsInterior(bool value) {
		setCellFlag(TES3::CellFlag::Interior, value);
	}

	bool Cell::getSleepingIsIllegal() const {
		return getCellFlag(TES3::CellFlag::SleepIsIllegal);
	}

	void Cell::setSleepingIsIllegal(bool value) {
		setCellFlag(TES3::CellFlag::SleepIsIllegal, value);
	}

	bool Cell::getIsMarkerDrawn() const {
		return getCellFlag(TES3::CellFlag::MarkerDrawn);
	}

	bool Cell::getIsOrBehavesAsExterior() const {
		return !getIsInterior() || getBehavesAsExterior();
	}

	static std::unordered_set<const TES3::Cell*> activeCells;

	void Cell::setCellActive() {
		// Skip if the cell is already active.
		if (activeCells.find(this) != activeCells.end()) {
			return;
		}

		// Fire off reference active events.
		for (auto ref : actors) ref->setReferenceActive();
		for (auto ref : persistentRefs) ref->setReferenceActive();
		for (auto ref : temporaryRefs) ref->setReferenceActive();

		// Add to active cells set.
		activeCells.insert(this);

		// Fire off cell activated event.
		if (mwse::lua::event::CellActivatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::CellActivatedEvent(this));
		}
	}

	void Cell::setCellInactive() {
		if (activeCells.find(this) == activeCells.end()) {
			return;
		}

		// Fire off reference inactive events.
		for (auto ref : actors) ref->setReferenceInactive();
		for (auto ref : persistentRefs) ref->setReferenceInactive();
		for (auto ref : temporaryRefs) ref->setReferenceInactive();

		// Add to active cells set.
		activeCells.erase(this);

		// Fire off cell deactivated event.
		if (mwse::lua::event::CellDeactivatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::CellDeactivatedEvent(this));
		}
	}

	bool Cell::getCellActive() const {
		return activeCells.find(this) != activeCells.end();
	}

	bool Cell::getHasCellMarker() const {
		// Interiors never have a marker.
		if (getIsInterior()) {
			return false;
		}

		// If we don't have a name, we don't have a marker.
		auto name = getName();
		if (name == nullptr || name[0] == '\0') {
			return false;
		}

		return true;
	}

	const char* Cell::getDisplayName() const {
		// Try the cell name first.
		auto name = getName();
		if (name) {
			return name;
		}

		// Fall back to region name.
		auto region = getRegion();
		if (region) {
			return region->getName();
		}

		// Fallback to GMST.
		return TES3::DataHandler::get()->nonDynamicData->GMSTs[TES3::GMST::sDefaultCellname]->value.asString;
	}

	std::string Cell::getEditorName() const {
		std::stringstream ss;

		ss << getDisplayName();
		if (!getIsInterior()) {
			ss << " (" << getGridX() << ", " << getGridY() << ")";
		}

		return std::move(ss.str());
	}

	bool Cell::isPointInCell(float x, float y) const {
		if (getIsInterior()) {
			return true;
		}

		return toGridCoord(x) == variantData.exterior.gridX && toGridCoord(y) == variantData.exterior.gridY;
	}

	int Cell::toGridCoord(float x) {
		return int(x) >> 13;
	}

	sol::object PathGrid::Node::getConnectedNodes_lua(sol::this_state ts) const {
		sol::state_view state = ts;
		sol::table conn = state.create_table();

		// Resolve extra indirections to make a flat node table.
		if (this->connectedNodes) {
			size_t n = 1;
			for (auto nodePtr : *this->connectedNodes) {
				if (nodePtr) {
					conn[n++] = *nodePtr;
				}
			}
		}
		return conn;
	}

	Vector3 PathGrid::Node::getPosition() const {
		// Convert local position to world position.
		const auto cell = parentGrid->parentCell;
		const int cellX = 8192 * cell->getGridX(), cellY = 8192 * cell->getGridY();

		return { float(cellX + relativeX), float(cellY + relativeY), float(relativeZ) };
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Cell)
