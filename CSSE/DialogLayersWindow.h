#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "CSCell.h"
#include "CSRecordHandler.h"
#include "CSDataHandler.h"
#include "NIIteratedList.h"
#include "CSReference.h"
#include "NITriShape.h"
#include "NIProperty.h"

constexpr size_t HIDDEN_LAYER_ID = 0;
constexpr auto MAX_LAYERS = 32;
constexpr auto LAYER_CONFIG_PATH = "csse_layers.toml";

namespace se::cs::dialog::layer_window {

	struct NodeColorData {
		NI::Pointer<NI::MaterialProperty> originalMaterial = nullptr;
		NI::Pointer<NI::AlphaProperty> originalAlphaProperty = nullptr;
		NI::Pointer<NI::VertexColorProperty> originalVColorProperty = nullptr;
	};

	struct LayerData {
		std::unordered_map<Cell*, std::unordered_set<Reference*>> perCellReferences;
		std::unordered_map<NI::TriShape*, NodeColorData*> nodeMaterialData;
		std::unordered_set<Reference*> deletedReferences; // For Undo support

		size_t id = 0;
		std::string* layerName = nullptr;
		bool isOverlayActive = false;
		bool isLayerHidden = false;

		NI::Color layerOverlayColor = NI::Color{ 0.0f, 1.0f, 0.4f }; //rbg for some reason
		NI::Pointer<NI::MaterialProperty> layerOverlayMaterial = nullptr;
		NI::Pointer<NI::AlphaProperty> layerAlphaProperty = nullptr;
		NI::Pointer<NI::VertexColorProperty> layerVertexColorProperty = nullptr;

		~LayerData() noexcept;

		NI::Color getLayerColor() const;
		void setLayerColor(const NI::Color& c);

		NI::Pointer<NI::MaterialProperty> getLayerOverlayMaterial(NI::Pointer<NI::MaterialProperty> material);
		NI::Pointer<NI::AlphaProperty> getLayerAlphaProperty();
		NI::Pointer<NI::VertexColorProperty> getLayerVertexColorProperty();

		NodeColorData* getNodeColorData(NI::TriShape* node);
		void removeNodeColorData(NI::TriShape* node);

		bool removeObject(Reference* objRef, bool undoable = false);
		void addObject(Reference* objRef);
		void updateObject(Reference* objRef, bool forceRestore = false);
		void moveObjectToLayer(Reference* objRef, LayerData* currentLayerInput = nullptr);
		
		void moveSelectionToLayer();
		void selectObjects();
		void refreshObjects();
		void clearLayer();

		size_t get_counts();
	};

	void createLayersWindow();
	void toggleLayersWindow(bool show);
	void loadOrCreateLayers();

	void forceToggleLayersWindow(HMENU viewMenu);
	void refreshLayersMenuItem(HMENU viewMenu);
	bool IsRenderWindowVisible(HMENU viewMenu);

	void toggleLayerVisuals(size_t layerIndex, bool overlay);
	void updateLayerWindowUI();
	void updateStatusLabel();

	std::unordered_map<size_t, LayerData*> GetLayers();
	LayerData* getLayerById(size_t id);
	LayerData* getLayerByObject(Reference* obj);

	inline HWND hLayersWnd = NULL;
	inline std::vector<LayerData*> g_Layers; // Array for the layer window UI, TODO: maybe enforce uniqueness?
	inline std::unordered_map<size_t, LayerData*> g_LayersIdMap; // maps layer IDs to their data
	inline std::unordered_map<Reference*, LayerData*> g_ObjectLayerMap; // maps objects to their layers (for faster lookup)

	void saveLayersToToml();
	void loadLayersFromToml();

	void installPatches();
}