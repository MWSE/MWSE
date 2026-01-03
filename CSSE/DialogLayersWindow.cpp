#include "DialogLayersWindow.h"
#include "DialogRenderWindow.h"
#include "RenderWindowSelectionData.h"
#include "WindowMain.h"

#define IDC_LAYERS_LIST 2001
#define IDC_BTN_SAVE 2002
#define IDC_BTN_ADD 2003
#define IDC_BTN_DEL 2004
#define IDC_BTN_HELP 2005
#define IDC_STATUS_LABEL 2006

#define IDM_LAYER_MOVE 3001
#define IDM_LAYER_CLEAR 3002
#define IDM_LAYER_RENAME 3003
#define IDM_LAYER_DELETE 3004
#define IDM_LAYER_SELECT 3005

namespace se::cs::dialog::layer_window {

	namespace rw = se::cs::dialog::render_window;

	static HWND hListView = NULL;
	static HWND hStatus = NULL;
	static HWND hTooltip = NULL;

	bool isLayerWndForceHidden = false;
	static COLORREF g_CustomColors[16] = { 0 }; // color picker history

	LayerData::~LayerData() noexcept {
		clearLayer();

		for (auto& kv : nodeMaterialData) {
			NodeColorData* p = kv.second;
			if (p) {
				delete p;
			}
		}
		nodeMaterialData.clear();

		if (layerName) {
			delete layerName;
			layerName = nullptr;
		}

		perCellReferences.clear();

		layerOverlayMaterial = nullptr;
		layerAlphaProperty = nullptr;
		layerVertexColorProperty = nullptr;
	}

	NI::Color LayerData::getLayerColor() const { return layerOverlayColor; }

	void LayerData::setLayerColor(const NI::Color& c) {
		layerOverlayColor = c;
		if (layerOverlayMaterial) {
			layerOverlayMaterial->setEmissive(layerOverlayColor);
			layerOverlayMaterial->setDiffuse(layerOverlayColor);
			layerOverlayMaterial->setAmbient(layerOverlayColor);
		}
	}

	NI::Pointer<NI::MaterialProperty> LayerData::getLayerOverlayMaterial(NI::Pointer<NI::MaterialProperty> material) {
		if (!layerOverlayMaterial && material) {
			layerOverlayMaterial = static_cast<NI::MaterialProperty*>(material->createClone()); // Cloning instead of creating a new one, cause constructor causes crashing
			layerOverlayMaterial->setEmissive(layerOverlayColor);
			layerOverlayMaterial->setDiffuse(layerOverlayColor);
			layerOverlayMaterial->setAmbient(layerOverlayColor);
			layerOverlayMaterial->setAlpha(1.0f);
		}
		return layerOverlayMaterial;
	}

	NI::Pointer<NI::AlphaProperty> LayerData::getLayerAlphaProperty() {
		if (!layerAlphaProperty) {
			layerAlphaProperty = new NI::AlphaProperty();
			layerAlphaProperty->flags = 3821;
			layerAlphaProperty->alphaTestRef = 255;
		}
		return layerAlphaProperty;
	}

	NI::Pointer<NI::VertexColorProperty> LayerData::getLayerVertexColorProperty() {
		if (!layerVertexColorProperty) {
			layerVertexColorProperty = new NI::VertexColorProperty();
		}
		return layerVertexColorProperty;
	}

	NodeColorData* LayerData::getNodeColorData(NI::TriShape* node) {
		auto it = nodeMaterialData.find(node);
		if (it != nodeMaterialData.end()) {
			return it->second;
		}
		else {
			auto newData = new NodeColorData();
			nodeMaterialData[node] = newData;
			return newData;
		}
	}

	// TODO: Stale references might accumulate if we reload the cell
	// and CS actually creates new TriShape instances,
	// however they will never be accessed, so no crashing should occur
	void LayerData::removeNodeColorData(NI::TriShape* node) {
		auto it = nodeMaterialData.find(node);
		if (it != nodeMaterialData.end()) {
			delete it->second;
			nodeMaterialData.erase(it);
		}
	}

	// Removes the object and returns true if it was on this layer
	// Set undoable to true if we want to keep the object in the global obj<->layer map for Undo support
	bool LayerData::removeObject(Reference* objRef, bool undoable) {
		auto objCell = objRef ? objRef->getCell() : nullptr;
		if (!objCell) return false;

		bool isRemoved = false;
		auto objIt = perCellReferences.find(objCell);
		if (objIt != perCellReferences.end()) {
			auto cellRefs = &objIt->second;
			if (cellRefs->find(objRef) != cellRefs->end()) {
				updateObject(objRef, true);
				isRemoved = true;
				cellRefs->erase(objRef);
				if (cellRefs->empty()) {
					perCellReferences.erase(objCell);
				}
				if (!undoable)
					g_ObjectLayerMap.erase(objRef);
			}
		}
		return isRemoved;
	}

	void LayerData::addObject(Reference* objRef) {
		auto objCell = objRef ? objRef->getCell() : nullptr;
		if (!objCell) return;
		perCellReferences[objCell].insert(objRef);
		g_ObjectLayerMap[objRef] = this;
		updateObject(objRef);
	}

	void LayerData::updateObject(Reference* objRef, bool forceRestore) {
		auto objCell = objRef ? objRef->getCell() : nullptr;
		if (!objCell) return;

		if (perCellReferences.find(objCell) == perCellReferences.end()) return;

		auto& cellRefs = perCellReferences[objCell];
		if (cellRefs.find(objRef) == cellRefs.end()) return;

		auto node = objRef ? objRef->sceneNode : nullptr;
		if (!node) return;

		for (auto& child : node->children) {
			if (child && child->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {

				auto triShape = static_cast<NI::TriShape*>(child.get());

				auto currMaterialProp = triShape->getMaterialProperty();

				if (currMaterialProp) {

					auto nodeColorData = getNodeColorData(triShape);

					auto layerColor = getLayerColor();
					auto layerMaterial = getLayerOverlayMaterial(currMaterialProp);
					auto layerVertexProp = getLayerVertexColorProperty();
					auto layerAlphaProp = getLayerAlphaProperty();

					auto currVertexProp = triShape->getVertexColorProperty();
					auto currAlphaProperty = triShape->getAlphaProperty();

					if (isOverlayActive && !forceRestore) {
						// Apply overlay material
						if (!nodeColorData->originalMaterial) {
							nodeColorData->originalMaterial = currMaterialProp;
						}
						triShape->setMaterialProperty(layerMaterial);

						// Apply vertex color prop
						if (!nodeColorData->originalVColorProperty) {
							nodeColorData->originalVColorProperty = currVertexProp;
						}
						triShape->setVertexColorProperty(layerVertexProp);
					}
					else {
						// Restore original material
						auto& originalMaterial = nodeColorData->originalMaterial;
						if (originalMaterial) {
							triShape->setMaterialProperty(originalMaterial);
						}
						else if (layerMaterial == currMaterialProp) {
							triShape->detachPropertyByType(NI::PropertyType::Material);
						}

						// Restore original vertex color prop
						auto& originalVertexProp = nodeColorData->originalVColorProperty;
						if (originalVertexProp) {
							triShape->setVertexColorProperty(originalVertexProp);
						}
						else if (layerVertexProp == currVertexProp) {
							triShape->detachPropertyByType(NI::PropertyType::VertexColor);
						}
					}

					if (isOverlayActive && isLayerHidden && !forceRestore) {
						// Apply transparency 
						if (!nodeColorData->originalAlphaProperty) {
							nodeColorData->originalAlphaProperty = currAlphaProperty;
						}

						triShape->setAlphaProperty(layerAlphaProp);

						layerMaterial->setAlpha(0.5f);
					}
					else {
						// Restore original alpha prop
						auto& originalAlphaProp = nodeColorData->originalAlphaProperty;

						layerMaterial->setAlpha(1.0f);
						if (originalAlphaProp) {
							triShape->setAlphaProperty(originalAlphaProp);
						}
						else if (layerAlphaProp == currAlphaProperty) {
							triShape->detachPropertyByType(NI::PropertyType::Alpha);
						}
					}

					triShape->updateProperties();

					if (forceRestore) {
						removeNodeColorData(triShape);
					}
				}
			}
		}

		// Cull object if layer is hidden and overlay is not active
		bool isObjectCulled = isLayerHidden && !isOverlayActive && !forceRestore;
		node->setAppCulled(isObjectCulled);
		
		// Updates all children as well
		node->update();
	}

	static bool isCellLoaded(Cell* cell) {
		auto dataHandler = DataHandler::get();
		auto cellGridX = dataHandler ? dataHandler->currentGridX : 0;
		auto cellGridY = dataHandler ? dataHandler->currentGridY : 0;
		auto interiorCell = dataHandler ? dataHandler->currentInteriorCell : nullptr;
		
		if (!cell) {
			return false;
		}

		// If interior cell, check if it's the current one
		if (cell->getIsInterior()) {
			if (cell == interiorCell) {
				return true;
			}
			return false;
		}

		// Check if the cells coords are within -2 to +2 of the current grid
		int gridX = cell->getGridX();
		int gridY = cell->getGridY();
		if (gridX >= cellGridX - 2 && gridX <= cellGridX + 2 &&
			gridY >= cellGridY - 2 && gridY <= cellGridY + 2) {
			return true;
		}

		return false;
	}

	// TODO: Refresh only active cell objects
	void LayerData::refreshObjects() {
		auto selectionData = rw::SelectionData::get();
		for (auto& [cell, cell_refs] : perCellReferences) {
			if (!isCellLoaded(cell)) {
				continue;
			}
			for (auto objRef : cell_refs) {
				updateObject(objRef);

				// Deselect object if layer is hidden
				if (isLayerHidden && objRef->hasActiveSelectionWidget()) {
					selectionData->removeReference(objRef, true);
				}
			}
		}
		selectionData->recalculateBound();
		rw::renderNextFrame();
	}

	void LayerData::moveObjectToLayer(Reference* objRef, LayerData* currentLayerInput) {
		if (!objRef) return;

		auto currentLayer = currentLayerInput ? currentLayerInput : getLayerByObject(objRef);

		if (currentLayer) {
			currentLayer->removeObject(objRef);
		}

		addObject(objRef);
	}

	void LayerData::moveSelectionToLayer() {
		auto selectionData = rw::SelectionData::get();

		std::vector<Reference*> selectedRefs; // copy to avoid modification during iteration
		for (auto target = selectionData->firstTarget; target; target = target->next) {
			if (target->reference) {
				selectedRefs.push_back(target->reference);
			}
		}

		for (auto objRef : selectedRefs) {
			if (!objRef) continue;
			moveObjectToLayer(objRef);

			if (isLayerHidden && objRef->hasActiveSelectionWidget()) {
				selectionData->removeReference(objRef, true);
			}
		}

		selectionData->recalculateBound();
		updateStatusLabel();
		rw::renderNextFrame();
	}

	void LayerData::clearLayer() {
		// copy to avoid problems with modification during iteration
		auto c_perCellReferences = this->perCellReferences;
		for (auto& cell_data : c_perCellReferences) {
			for (auto objRef : cell_data.second) {
				this->removeObject(objRef);
			}
		}

		deletedReferences.clear();
		updateStatusLabel();
		rw::renderNextFrame();
	}

	void LayerData::selectObjects() {
		if (isLayerHidden) {
			return;
		}
		auto selectionData = rw::SelectionData::get();
		selectionData->clear();
		for (auto& [cell, cell_refs] : perCellReferences) {
			if (!isCellLoaded(cell)) {
				continue;
			}
			for (auto objRef : cell_refs) {
				selectionData->addReference(objRef, true);
			}
		}
		selectionData->recalculateBound();
		rw::renderNextFrame();
	}

	size_t LayerData::get_counts() {
		size_t total = 0;
		for (auto& cell_data : perCellReferences) {
			total += cell_data.second.size();
		}
		return total;
	}

	size_t getNextLayerId() {
		size_t nextId = 0;
		while (g_LayersIdMap.find(nextId) != g_LayersIdMap.end()) {
			++nextId;
		}

		return nextId >= MAX_LAYERS ? MAX_LAYERS + 1 : nextId;
	}

	void saveLayersToToml() {
		toml::value root = toml::table{};

		for (const auto& [layer_id, layer] : g_LayersIdMap) {
			if (!layer || !layer->layerName) continue;

			toml::value layerTable = toml::table{};

			// Basic properties
			layerTable["layer_id"] = layer->id;
			layerTable["hidden"] = layer->isLayerHidden;
			layerTable["overlay"] = layer->isOverlayActive;

			layerTable["layer_rgb"] = toml::array{
				layer->layerOverlayColor.r,
				layer->layerOverlayColor.g,
				layer->layerOverlayColor.b
			};

			// Cell references
			toml::value cellRefTable = toml::table{};

			for (const auto& [cell, refs] : layer->perCellReferences) {
				if (!cell) continue;

				std::string cellId = cell->getEditorId();

				// Collect object IDs for this cell
				toml::array objIds;
				for (const auto* ref : refs) {
					if (ref) {
						objIds.push_back(ref->getUniqueID());
					}
				}

				if (!objIds.empty()) {
					cellRefTable[cellId] = objIds;
				}
			}

			layerTable["cellRefs"] = cellRefTable;

			root[*layer->layerName] = layerTable;
		}

		std::ofstream out(LAYER_CONFIG_PATH);
		if (out.is_open()) {
			out << root;
			out.close();
		}
	}

	void loadLayersFromToml() {
		if (!std::filesystem::exists(LAYER_CONFIG_PATH)) return;

		toml::value data;
		try {
			data = toml::parse(LAYER_CONFIG_PATH);
		}
		catch (std::exception& e) {
			// TODO: Handle parse error
			return;
		}

		// Clean up old state?
		// clearAllLayers(); 

		// This allows fast lookup per object when iterating the world database
		// Map<CellIDString, Map<ObjIDString, LayerData*>>
		std::unordered_map<std::string, std::unordered_map<std::string, LayerData*>> restorationMap;

		const auto& layerTable = data.as_table();

		// Parse TOML and create layers
		for (const auto& [layerNameStr, layerVal] : layerTable) {
			LayerData* newLayer = new LayerData();

			// Restore basic properties
			newLayer->layerName = new std::string(layerNameStr);
			newLayer->id = toml::find_or<size_t>(layerVal, "layer_id", getNextLayerId());
			newLayer->isLayerHidden = toml::find_or<bool>(layerVal, "hidden", false);
			newLayer->isOverlayActive = toml::find_or<bool>(layerVal, "overlay", false);

			auto rgb = toml::find<std::vector<float>>(layerVal, "layer_rgb");
			if (rgb.size() >= 3) {
				newLayer->setLayerColor(NI::Color{ rgb[0], rgb[1], rgb[2] });
			}

			// Register this layer
			g_Layers.push_back(newLayer);
			g_LayersIdMap[newLayer->id] = newLayer;

			// Build the restoration Map
			if (layerVal.contains("cellRefs")) {
				const auto& cells = toml::find(layerVal, "cellRefs").as_table();

				for (const auto& [cellId, objIdArray] : cells) {
					const auto& ids = objIdArray.as_array();
					for (const auto& idVal : ids) {
						std::string uniqueId = idVal.as_string();
						restorationMap[cellId][uniqueId] = newLayer;
					}
				}
			}
		}

		// Sort g_Layers by their IDs (ascending) for consistent ordering in the UI
		std::sort(g_Layers.begin(), g_Layers.end(), [](LayerData* a, LayerData* b) {
			return a->id < b->id;
			});

		auto recordHandler = DataHandler::get()->recordHandler;
		auto cellList = recordHandler->cells;

		// Populate layer data with actual object references
		for (auto cellIt = cellList->begin(); cellIt != cellList->end(); ++cellIt) {
			Cell* cell = *cellIt;
			if (!cell) continue;

			std::string currentCellId = cell->getEditorId(); // will fail to be restored if cell was renamed

			auto mapIt = restorationMap.find(currentCellId);
			if (mapIt == restorationMap.end()) {
				continue;
			}

			auto& objIDToLayerMap = mapIt->second;

			// Iterate objects in the cell
			auto refList = &cell->cellObjRefs;
			auto npcList = &cell->cellNpcRefs;
			for (auto refListPtr : { &cell->cellObjRefs, &cell->cellNpcRefs }) {
				for (auto refIt = refListPtr->begin(); refIt != refListPtr->end(); ++refIt) {
					Reference* ref = *refIt;
					if (!ref) continue;

					auto refId = ref->getUniqueID(); // will fail to be restored if object's position or its object ID changed

					auto targetLayerIt = objIDToLayerMap.find(refId);
					if (targetLayerIt != objIDToLayerMap.end()) {
						LayerData* targetLayer = targetLayerIt->second;
						targetLayer->addObject(ref);
					}
				}
			}
		}

		// Refresh all layers to apply their properties
		for (const auto& [id, layer] : g_LayersIdMap) {
			if (layer) {
				layer->refreshObjects();
			}
		}
	}

	void loadOrCreateLayers() {

		// Clear any existing layers
		for (auto& layer : g_Layers) {
			delete layer;
		}
		g_Layers.clear();
		g_LayersIdMap.clear();
		g_ObjectLayerMap.clear();

		if (std::filesystem::exists(LAYER_CONFIG_PATH)) {
			loadLayersFromToml();
		}

		// if layer id 0 is missing, recreate default layer
		if (g_LayersIdMap.find(0) == g_LayersIdMap.end()) {
			auto hiddenLayer = new LayerData();
			hiddenLayer->id = HIDDEN_LAYER_ID;
			hiddenLayer->layerName = new std::string("Hidden");
			hiddenLayer->isLayerHidden = true;
			hiddenLayer->isOverlayActive = false;
			g_Layers.push_back(hiddenLayer);
			g_LayersIdMap[hiddenLayer->id] = hiddenLayer;
		}

		if (hListView) {
			ListView_DeleteAllItems(hListView);

			for (int i = 0; i < (int)g_Layers.size(); ++i) {
				LVITEMA lvI = { 0 };
				lvI.mask = LVIF_TEXT;
				lvI.iItem = i;
				lvI.pszText = LPSTR_TEXTCALLBACK;
				ListView_InsertItem(hListView, &lvI);
			}

			updateLayerWindowUI();
		}
	}

	void toggleLayerVisuals(size_t layerIndex, bool overlay) {
		if (layerIndex >= g_Layers.size()) return;

		LayerData* layer = g_Layers[layerIndex];
		if (!layer) return;

		if (overlay)
			layer->isOverlayActive = !layer->isOverlayActive;
		else
			layer->isLayerHidden = !layer->isLayerHidden;

		layer->refreshObjects();

		if (hListView) {
			ListView_RedrawItems(hListView, layerIndex, layerIndex);
		}
	}

	// NI::Color uses rbg order, while COLORREF uses rgb order.
	COLORREF NIColorToRef(const NI::Color& c) {
		return RGB(
			(int)(c.r * 255.0f),
			(int)(c.b * 255.0f),
			(int)(c.g * 255.0f)
		);
	}

	NI::Color RefToNIColor(COLORREF c) {
		return NI::Color{
			GetRValue(c) / 255.0f,
			GetBValue(c) / 255.0f,
			GetGValue(c) / 255.0f
		};
	}

	// Hacky way to check if the render window is visible by reading the View menu's Render Window item.
	// We can use IsIconic(hRender), but animations window hides the render window without triggering that, so this is more reliable.
	bool IsRenderWindowVisible(HMENU viewMenu) {
		if (viewMenu) {
			const int rendererPos = 2;
			const UINT state = GetMenuState(viewMenu, rendererPos, MF_BYPOSITION);
			if (state != 0xFFFFFFFF && (state & MF_CHECKED)) {
				return TRUE;
			}
		}
		return FALSE;
	}

	std::unordered_map<size_t, LayerData*> GetLayers() { return g_LayersIdMap; }

	LayerData* getLayerById(size_t id) {
		if (id >= MAX_LAYERS || id < 0) return nullptr;
		auto it = g_LayersIdMap.find(id);
		if (it != g_LayersIdMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	LayerData* getLayerByObject(Reference* obj)
	{
		if (!obj) return nullptr;
		auto it = g_ObjectLayerMap.find(obj);
		if (it != g_ObjectLayerMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void toggleLayersWindow(bool show) {
		if (hLayersWnd) {
			ShowWindow(hLayersWnd, show ? SW_RESTORE : SW_MINIMIZE);
		}
		else {
			createLayersWindow();
		}
	}

	// If render window became minimized, minimize our layers window too.
	// TODO BUG: won't trigger if the render window is brought back up via closing the animation window with a close button.
	void forceToggleLayersWindow(HMENU viewMenu) {
		if (hLayersWnd) {
			if (IsRenderWindowVisible(viewMenu))
			{
				if (isLayerWndForceHidden) {
					toggleLayersWindow(true);
					isLayerWndForceHidden = false;
				}
			}
			else {
				isLayerWndForceHidden = !IsIconic(hLayersWnd);
				toggleLayersWindow(false);
			}
		}
	}

	void refreshLayersMenuItem(HMENU viewMenu) {
		if (viewMenu) {

			UINT check = (hLayersWnd != NULL && !IsIconic(hLayersWnd)) ? MF_CHECKED : MF_UNCHECKED;
			CheckMenuItem(viewMenu, se::cs::window::main::MENU_ID_VIEW_LAYERS_WINDOW, check);

			UINT enable = IsRenderWindowVisible(viewMenu) ? MF_ENABLED : MF_GRAYED;
			EnableMenuItem(viewMenu, se::cs::window::main::MENU_ID_VIEW_LAYERS_WINDOW, enable);
		}
	}

	void updateStatusLabel() {
		if (!hListView || !hStatus) return;

		int iItem = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
		if (iItem != -1 && iItem < (int)g_Layers.size()) {
			auto layer = g_Layers[iItem];
			std::string statusText = "Objects: " + std::to_string(layer->get_counts());
			SetWindowTextA(hStatus, statusText.c_str());
		}
		else {
			SetWindowTextA(hStatus, "Objects: -");
		}
	}

	void updateLayerWindowUI() {
		if (hListView) {
			InvalidateRect(hListView, NULL, FALSE);
			updateStatusLabel();
		}
	}

	void AddTooltip(HWND hParent, HWND hControl, LPCSTR text) {
		if (!hTooltip) {
			hTooltip = CreateWindowExA(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
				WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hParent, NULL, (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE), NULL);
		}

		TOOLINFOA ti = { 0 };
		ti.cbSize = sizeof(ti);
		ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
		ti.hwnd = hParent;
		ti.uId = (UINT_PTR)hControl;
		ti.lpszText = (LPSTR)text;
		SendMessage(hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);
	}

	LRESULT CALLBACK LayersWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		static HWND hBtnSave, hBtnAdd, hBtnDel, hBtnHelp;
		static bool allowLabelEdit = false; 

		int padding = 4;
		int btnSize = 24;
		int topBarHeight = 24;

		switch (msg) {
		case WM_CREATE: {

			hBtnSave = CreateWindowExA(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
				0, 0, 0, 0, hWnd, (HMENU)IDC_BTN_SAVE, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			hBtnAdd = CreateWindowExA(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
				0, 0, 0, 0, hWnd, (HMENU)IDC_BTN_ADD, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			hBtnDel = CreateWindowExA(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
				0, 0, 0, 0, hWnd, (HMENU)IDC_BTN_DEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			hBtnHelp = CreateWindowExA(0, "BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
				0, 0, 0, 0, hWnd, (HMENU)IDC_BTN_HELP, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			AddTooltip(hWnd, hBtnSave, "Save current layer configuration");
			AddTooltip(hWnd, hBtnAdd, "Create a new layer");
			AddTooltip(hWnd, hBtnDel, "Delete selected layer");
			AddTooltip(hWnd, hBtnHelp, "Open Help and Documentation");

			HMODULE hCurrentModule = NULL;
			GetModuleHandleExA(
				GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCSTR)&LayersWindowProc,
				&hCurrentModule
			);

			HBITMAP hBmp = LoadBitmap(hCurrentModule, MAKEINTRESOURCE(IDB_TOOLBAR_STRIP));

			HIMAGELIST hImageList = ImageList_Create(16, 16, ILC_COLOR24 | ILC_MASK, 3, 0);

			ImageList_AddMasked(hImageList, hBmp, CLR_DEFAULT);
			DeleteObject(hBmp); 

			HICON hIconSave = ImageList_GetIcon(hImageList, 0, ILD_TRANSPARENT);
			HICON hIconAdd = ImageList_GetIcon(hImageList, 1, ILD_TRANSPARENT);
			HICON hIconDel = ImageList_GetIcon(hImageList, 2, ILD_TRANSPARENT);
			HICON hIconHelp = ImageList_GetIcon(hImageList, 3, ILD_TRANSPARENT);

			SendMessage(hBtnSave, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIconSave);
			SendMessage(hBtnAdd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIconAdd);
			SendMessage(hBtnDel, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIconDel);
			SendMessage(hBtnHelp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIconHelp);

			ImageList_Destroy(hImageList);

			hListView = CreateWindowExA(0, WC_LISTVIEW, "",
				WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_SINGLESEL | LVS_SHOWSELALWAYS | LVS_EDITLABELS,
				0, 0, 0, 0,
				hWnd, (HMENU)IDC_LAYERS_LIST, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

			LVCOLUMNA lvc = { 0 };
			lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

			lvc.fmt = LVCFMT_LEFT;
			lvc.cx = 135;
			lvc.pszText = (LPSTR)"Name";
			ListView_InsertColumn(hListView, 0, &lvc);

			lvc.fmt = LVCFMT_CENTER;
			lvc.cx = 40;
			lvc.pszText = (LPSTR)"Color";
			ListView_InsertColumn(hListView, 1, &lvc);

			lvc.cx = 45;
			lvc.pszText = (LPSTR)"Visible";
			ListView_InsertColumn(hListView, 2, &lvc);

			lvc.cx = 50;
			lvc.pszText = (LPSTR)"Overlay";
			ListView_InsertColumn(hListView, 3, &lvc);

			// Insert default layers
			for (int i = 0; i < (int)g_Layers.size(); ++i) {
				LVITEMA lvI = { 0 };
				lvI.mask = LVIF_TEXT;
				lvI.iItem = i;
				lvI.pszText = LPSTR_TEXTCALLBACK;
				ListView_InsertItem(hListView, &lvI);
			}

			hStatus = CreateWindowExA(0, "STATIC", "Objects: 0",
				WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE,
				0, 0, 0, 0, hWnd, (HMENU)IDC_STATUS_LABEL, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

			HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hListView, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hBtnSave, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hBtnAdd, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hBtnDel, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hBtnHelp, WM_SETFONT, (WPARAM)hFont, 0);
			SendMessage(hStatus, WM_SETFONT, (WPARAM)hFont, 0);
			break;
		}

		case WM_SIZE: {
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			int currentY = padding;

			MoveWindow(hBtnSave, padding, currentY, btnSize, topBarHeight, TRUE);
			MoveWindow(hBtnAdd, padding + btnSize + 2, currentY, btnSize, topBarHeight, TRUE);
			MoveWindow(hBtnDel, padding + (btnSize * 2) + 4, currentY, btnSize, topBarHeight, TRUE);
			MoveWindow(hBtnHelp, width - padding - btnSize, currentY, btnSize, topBarHeight, TRUE);

			currentY += topBarHeight + padding;

			int listHeight = height - currentY - topBarHeight;
			if (listHeight < 0) listHeight = 0;

			MoveWindow(hListView, 0, currentY, width, listHeight, TRUE);

			MoveWindow(hStatus, padding, height - topBarHeight, width - padding, topBarHeight, TRUE);

			// Resize "Name" Column to fill width
			RECT rcList;
			GetClientRect(hListView, &rcList); 
			int w1 = ListView_GetColumnWidth(hListView, 1);
			int w2 = ListView_GetColumnWidth(hListView, 2);
			int w3 = ListView_GetColumnWidth(hListView, 3);

			int newNameWidth = rcList.right - (w1 + w2 + w3);

			if (newNameWidth < 50) newNameWidth = 50;

			ListView_SetColumnWidth(hListView, 0, newNameWidth);
			break;
		}

		case WM_GETMINMAXINFO: {
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
			// restrict window size to prevent UI overlap
			lpMMI->ptMinTrackSize.x = 4 * btnSize + 7 * padding;
			lpMMI->ptMinTrackSize.y = ((int)g_Layers.size() + 4) * topBarHeight;
			return 0;
		}

		case WM_DRAWITEM: {
			LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
			if (pDIS->CtlID == IDC_LAYERS_LIST) {
				if (pDIS->itemID >= (int)g_Layers.size()) break;

				LayerData* layer = g_Layers[pDIS->itemID];

				// Background
				if (pDIS->itemState & ODS_SELECTED) {
					FillRect(pDIS->hDC, &pDIS->rcItem, GetSysColorBrush(COLOR_HIGHLIGHT));
					SetTextColor(pDIS->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
				}
				else {
					FillRect(pDIS->hDC, &pDIS->rcItem, GetSysColorBrush(COLOR_WINDOW));
					SetTextColor(pDIS->hDC, GetSysColor(COLOR_WINDOWTEXT));
				}
				SetBkMode(pDIS->hDC, TRANSPARENT);

				RECT rcLabel, rcColor, rcVis, rcOver;
				ListView_GetSubItemRect(hListView, pDIS->itemID, 0, LVIR_BOUNDS, &rcLabel);
				ListView_GetSubItemRect(hListView, pDIS->itemID, 1, LVIR_BOUNDS, &rcColor);
				ListView_GetSubItemRect(hListView, pDIS->itemID, 2, LVIR_BOUNDS, &rcVis);
				ListView_GetSubItemRect(hListView, pDIS->itemID, 3, LVIR_BOUNDS, &rcOver);

				// Name
				rcLabel.left += 4;
				const char* name = layer->layerName ? layer->layerName->c_str() : "(unnamed)";
				std::string label = std::to_string(static_cast<int>(pDIS->itemID) + 1) + ": " + name;
				DrawTextA(pDIS->hDC, label.c_str(), -1, &rcLabel, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

				// Color swatch
				InflateRect(&rcColor, -4, -4);
				HBRUSH hBrush = CreateSolidBrush(NIColorToRef(layer->getLayerColor()));
				FillRect(pDIS->hDC, &rcColor, hBrush);
				FrameRect(pDIS->hDC, &rcColor, (HBRUSH)GetStockObject(BLACK_BRUSH));
				DeleteObject(hBrush);

				// Visible checkbox
				UINT stateVis = DFCS_BUTTONCHECK | (!layer->isLayerHidden ? DFCS_CHECKED : 0);
				DrawFrameControl(pDIS->hDC, &rcVis, DFC_BUTTON, stateVis);

				// Overlay checkbox
				UINT stateOver = DFCS_BUTTONCHECK | (layer->isOverlayActive ? DFCS_CHECKED : 0);
				DrawFrameControl(pDIS->hDC, &rcOver, DFC_BUTTON, stateOver);

				return TRUE;
			}
			break;
		}

		case WM_COMMAND: {
			int id = LOWORD(wParam);

			if (id == IDC_BTN_SAVE) {
				if (std::filesystem::exists(LAYER_CONFIG_PATH) &&
					MessageBoxA(hWnd, "Overwrite existing layers configuration?", "Save Layers", MB_YESNO | MB_ICONQUESTION) != IDYES) {
						break;
				}
				saveLayersToToml();
			}

			else if (id == IDC_BTN_ADD) {
				auto nextId = getNextLayerId();
				if (g_Layers.size() < MAX_LAYERS && nextId < MAX_LAYERS) {
					LayerData* newLayer = new LayerData();
					newLayer->id = nextId;
					newLayer->layerName = new std::string(("Layer " + std::to_string(nextId)));
					auto randomColor = NI::Color{
						static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
						static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
						static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
					};
					newLayer->setLayerColor(randomColor);
					g_Layers.push_back(newLayer);
					g_LayersIdMap[newLayer->id] = newLayer;

					LVITEMA lvI = { 0 };
					lvI.mask = LVIF_TEXT;
					lvI.iItem = (int)g_Layers.size() - 1;
					lvI.pszText = LPSTR_TEXTCALLBACK;
					ListView_InsertItem(hListView, &lvI);
				}
				else {
					MessageBoxA(hWnd, "Maximum of 32 layers reached.", "Limit Reached", MB_OK | MB_ICONINFORMATION);
				}
			}

			else if (id == IDC_BTN_DEL) {
				int iItem = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
				if (iItem != -1) {
					NMHDR nm = { hListView, IDC_LAYERS_LIST, LVN_KEYDOWN };
					NMLVKEYDOWN nkd = { nm, VK_DELETE, 0 };
					SendMessage(hWnd, WM_NOTIFY, IDC_LAYERS_LIST, (LPARAM)&nkd);
				}
			}

			else if (id == IDC_BTN_HELP) {
				std::string helpMsg =
					"Layer Manager Help:\n\n"
					"UI Actions:\n"
					"- Use the buttons to save, add or delete layers, respectively.\n"
					"- Click color swatch to change layer overlay color.\n"
					"- Toggle visibility and overlay via checkboxes.\n"
					"- Left-click a layer name to select it and view object count in status bar.\n"
					"- Press 'DELETE' key to delete selected layer.\n"
					"- Double-click a layer name to select all objects from this layer that are currently being rendered.\n\n"
					"Right-Click Menu (on a layer entry):\n"
					"- Rename/Delete: Manage the layer entry.\n"
					"- Select objects: Selects all objects assigned to this layer.\n"
					"- Assign selected objects: Assigns currently selected objects to this layer.\n"
					"- Clear layer: Removes all objects from this layer. \n\n"
					"Hotkeys (Render Window):\n"
					"- Ctrl + [0-9]: Toggle visibility of layer N.\n"
					"- Shift + [0-9]: Toggle color overlay of layer N.\n"
					"- Q > L > [0-9]: Move selected objects to layer N via the Quick Menu.\n"
					"(Note: Press two numbers quickly to access layers beyond 9)\n\n"
					"Would you like to open the full documentation online?";

				int result = MessageBoxA(hWnd, helpMsg.c_str(), "Layer Manager Help", MB_YESNO | MB_ICONINFORMATION);

				if (result == IDYES) {
					ShellExecuteA(NULL, "open", "https://mwse.github.io/MWSE/references/general/csse/", NULL, NULL, SW_SHOWNORMAL);
				}
			}

			break;
		}

		case WM_NOTIFY: {
			LPNMHDR lpnm = (LPNMHDR)lParam;

			if (lpnm == NULL)
				break;

			if (lpnm->idFrom == IDC_LAYERS_LIST && (lpnm->code == LVN_BEGINLABELEDITA)) {
				if (!allowLabelEdit) {
					return TRUE;
				}
				allowLabelEdit = false;
			}

			if (lpnm->idFrom == IDC_LAYERS_LIST && lpnm->code == LVN_ITEMCHANGED) {
				LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lParam;
				if ((pnmv->uNewState & LVIS_SELECTED)) {
					updateStatusLabel();
				}
			}

			if (lpnm->idFrom == IDC_LAYERS_LIST && lpnm->code == NM_RCLICK) {
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;

				POINT pt;
				GetCursorPos(&pt);
				POINT ptClient = pt;
				ScreenToClient(hListView, &ptClient);

				LVHITTESTINFO hitInfo = { 0 };
				hitInfo.pt = ptClient;
				ListView_SubItemHitTest(hListView, &hitInfo);

				if (hitInfo.iItem != -1 && hitInfo.iItem < (int)g_Layers.size()) {
					ListView_SetItemState(hListView, hitInfo.iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
					LayerData* layer = g_Layers[hitInfo.iItem];

					HMENU hPopup = CreatePopupMenu();
					AppendMenuA(hPopup, MF_STRING, IDM_LAYER_RENAME, "Rename");
					AppendMenuA(hPopup, MF_STRING, IDM_LAYER_DELETE, "Delete");
					AppendMenuA(hPopup, MF_SEPARATOR, 0, NULL);
					AppendMenuA(hPopup, MF_STRING, IDM_LAYER_SELECT, "Select objects");
					AppendMenuA(hPopup, MF_STRING, IDM_LAYER_MOVE, "Assign selected objects");
					AppendMenuA(hPopup, MF_STRING, IDM_LAYER_CLEAR, "Clear layer");

					int selected = TrackPopupMenu(hPopup, TPM_RETURNCMD | TPM_NONOTIFY | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
					DestroyMenu(hPopup);

					if (selected == IDM_LAYER_RENAME) {
						allowLabelEdit = true;
						ListView_EditLabel(hListView, hitInfo.iItem);
					}
					else if (selected == IDM_LAYER_DELETE) {
						SendMessage(hWnd, WM_COMMAND, IDC_BTN_DEL, 0);
					}
					else if (selected == IDM_LAYER_SELECT) {
						layer->selectObjects();
					}
					else if (selected == IDM_LAYER_MOVE) {
						layer->moveSelectionToLayer();
					}
					else if (selected == IDM_LAYER_CLEAR) {
						if (MessageBoxA(hWnd, "Are you sure? Objects will be removed from this layer! This cannot be undone!", "Clear Layer", MB_YESNO | MB_ICONWARNING) == IDYES) {
							layer->clearLayer();
						}
					}
				}
			}

			if (lpnm->idFrom == IDC_LAYERS_LIST && lpnm->code == NM_CLICK) {
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;

				if (lpnmitem->iItem != -1 && lpnmitem->iItem < (int)g_Layers.size()) {
					LayerData* layer = g_Layers[lpnmitem->iItem];

					//color picker
					if (lpnmitem->iSubItem == 1) {
						CHOOSECOLORA cc = { 0 };
						cc.lStructSize = sizeof(cc);
						cc.hwndOwner = hWnd;
						cc.lpCustColors = g_CustomColors;
						cc.rgbResult = NIColorToRef(layer->getLayerColor());
						cc.Flags = CC_FULLOPEN | CC_RGBINIT;

						if (ChooseColorA(&cc)) {
							layer->setLayerColor(RefToNIColor(cc.rgbResult));
							InvalidateRect(hListView, NULL, FALSE);
						}
					}

					// visibility toggle
					else if (lpnmitem->iSubItem == 2) {
						layer->isLayerHidden = !layer->isLayerHidden;
						InvalidateRect(hListView, NULL, FALSE);
					}

					// color overlay toggle
					else if (lpnmitem->iSubItem == 3) {
						layer->isOverlayActive = !layer->isOverlayActive;
						InvalidateRect(hListView, NULL, FALSE);
					}

					layer->refreshObjects();
				}
			}

			// double-click to edit layer name
			if (lpnm->idFrom == IDC_LAYERS_LIST && lpnm->code == NM_DBLCLK) {
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
				if (lpnmitem->iItem != -1 && lpnmitem->iItem < (int)g_Layers.size()) {

					LayerData* layer = g_Layers[lpnmitem->iItem];

					layer->selectObjects();
				}
			}

			// layer name edit completed
			if (lpnm->idFrom == IDC_LAYERS_LIST && (lpnm->code == LVN_ENDLABELEDIT)) {
				LPNMLVDISPINFOA pDisp = (LPNMLVDISPINFOA)lParam;
				if (pDisp && pDisp->item.iItem >= 0 && pDisp->item.iItem < (int)g_Layers.size()) {
					if (pDisp->item.pszText) {
						// Update name
						delete g_Layers[pDisp->item.iItem]->layerName;
						g_Layers[pDisp->item.iItem]->layerName = new std::string(pDisp->item.pszText);
						InvalidateRect(hListView, NULL, FALSE);
						return TRUE; 
					}
				}
			}

			if (lpnm->idFrom == IDC_LAYERS_LIST && lpnm->code == LVN_KEYDOWN) {
				LPNMLVKEYDOWN pnkd = (LPNMLVKEYDOWN)lParam;
				if (pnkd->wVKey == VK_DELETE) {
					int iItem = ListView_GetNextItem(hListView, -1, LVNI_SELECTED);
					if (iItem != -1 && iItem < (int)g_Layers.size()) {
						LayerData* l = g_Layers[iItem];

						// Prevent deleting default layers
						if (l->id == 0) {
							MessageBoxA(hWnd, "Cannot delete Hidden layer.", "Error", MB_OK | MB_ICONERROR);
						}
						else {

							std::string msg = "Are you sure you want to delete layer '" + *l->layerName + "'?\nThis operation cannot be undone!";
							if (MessageBoxA(hWnd, msg.c_str(), "Delete Layer", MB_YESNO | MB_ICONWARNING) == IDYES) {


								delete g_Layers[iItem];
								g_Layers.erase(g_Layers.begin() + iItem);
								g_LayersIdMap.erase(l->id);

								ListView_DeleteItem(hListView, iItem);

								SetWindowTextA(hStatus, "Objects: -");
							}
						}
					}
				}
			}
			break;
		}

		case WM_DESTROY:
			if (hTooltip) {
				DestroyWindow(hTooltip);
				hTooltip = NULL;
			}

			if (hBtnSave) {
				HICON prev = (HICON)SendMessage(hBtnSave, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)NULL);
				if (prev) DestroyIcon(prev);
				DestroyWindow(hBtnSave);
				hBtnSave = NULL;
			}
			if (hBtnAdd) {
				HICON prev = (HICON)SendMessage(hBtnAdd, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)NULL);
				if (prev) DestroyIcon(prev);
				DestroyWindow(hBtnAdd);
				hBtnAdd = NULL;
			}
			if (hBtnDel) {
				HICON prev = (HICON)SendMessage(hBtnDel, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)NULL);
				if (prev) DestroyIcon(prev);
				DestroyWindow(hBtnDel);
				hBtnDel = NULL;
			}
			if (hBtnHelp) {
				HICON prev = (HICON)SendMessage(hBtnHelp, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)NULL);
				if (prev) DestroyIcon(prev);
				DestroyWindow(hBtnHelp);
				hBtnHelp = NULL;
			}

			if (hListView) {
				hListView = NULL;
			}
			if (hStatus) {
				hStatus = NULL;
			}

			hLayersWnd = NULL;

			break;

		case WM_CLOSE:
			// just in case
			ShowWindow(hWnd, SW_HIDE);
			return 0;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	void createLayersWindow() {

		if (hLayersWnd) {
			ShowWindow(hLayersWnd, SW_SHOW);
			if (IsIconic(hLayersWnd)) OpenIcon(hLayersWnd);
			SetForegroundWindow(hLayersWnd);
			return;
		}

		WNDCLASSEXA wc = {};
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = LayersWindowProc;
		wc.hInstance = se::cs::window::main::hInstance::get();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wc.lpszClassName = "CSSE_LayersWindow";
		RegisterClassExA(&wc);

		hLayersWnd = CreateWindowExA(
			WS_EX_TOOLWINDOW,
			"CSSE_LayersWindow",
			"Layers",
			WS_VISIBLE | WS_CAPTION | WS_THICKFRAME,
			100, 100, 250, 400,
			se::cs::window::main::ghWnd::get(),
			NULL,
			se::cs::window::main::hInstance::get(),
			NULL
		);
	}

	static void OnCellLoaded() {
		if (g_LayersIdMap.empty()) return;
		else {
			for (const auto& [id, layer] : g_LayersIdMap) {
				if (layer) {
					layer->refreshObjects();
				}
			}
		}
	}

	static void __fastcall Patch_CellLoad_Wrapper(NI::AVObject* self) {
		// Call overwritten code.
		self->updateProperties();

		OnCellLoaded();
	}

	static bool __fastcall Patch_CellGridLoad_Wrapper(DataHandler* self, DWORD _EDX_, NI::Vector3* position) {
		// Call overwritten code.
		const auto TES3_CS_CellGridLoad = reinterpret_cast<bool(__thiscall*)(DataHandler*, NI::Vector3*)>(0x4A0090);
		const auto returnValue = TES3_CS_CellGridLoad(self, position);

		OnCellLoaded();

		return returnValue;
	}

	// bIsDeleting (1 = Delete, 0 = Restore/Undo)
	void HandleObjectDeleteState(Reference* objRef, bool bIsDeleting) {
		if (g_LayersIdMap.empty()) return;

		auto layer = getLayerByObject(objRef);

		if (!layer) return;

		if (bIsDeleting && layer->removeObject(objRef, true)) {
			layer->deletedReferences.insert(objRef);
			updateStatusLabel();
			return;
		}

		if (layer->deletedReferences.erase(objRef)) {
			layer->addObject(objRef);
			updateStatusLabel();
			rw::renderNextFrame(); // TODO: figure out why sometimes randomly the visual state is not being applied 
		}
	}

	void __fastcall Patch_SetDeleted(BaseObject* object, DWORD _EDX_, bool deleted) {
		if (object->objectType == ObjectType::Reference) {
			HandleObjectDeleteState(static_cast<Reference*>(object), deleted);
		}

		// Call overwritten code.
		object->setDeleted(deleted);
	}

	void installPatches() {
		using memory::genCallEnforced;
		using memory::genJumpEnforced;

		// For rendering layer overlays on cell loading
		genCallEnforced(0x45FE47, 0x5DB090, reinterpret_cast<DWORD>(Patch_CellLoad_Wrapper));
		genCallEnforced(0x49F0F9, 0x403A0D, reinterpret_cast<DWORD>(Patch_CellGridLoad_Wrapper));

		// For undo functionality and proper object removal from static maps
		genJumpEnforced(0x404719, 0x547810, reinterpret_cast<DWORD>(Patch_SetDeleted));
	}
}