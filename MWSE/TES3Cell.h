#pragma once

#include "TES3Defines.h"

#include "TES3Object.h"
#include "TES3ReferenceList.h"

#include "NISourceTexture.h"

namespace TES3 {
	namespace CellFlag {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Interior = 0x1,
			HasWater = 0x2,
			SleepIsIllegal = 0x4,
			WasLoaded = 0x8,
			TempRefsLoaded = 0x10,
			MarkerDrawn = 0x20,
			BehavesAsExterior = 0x80
		};
	}

	struct MapNote {
		NI::Point3 position; // 0x0
		char * text; // 0xC

		MapNote() = delete;
		~MapNote() = delete;
	};
	static_assert(sizeof(MapNote) == 0x10, "TES3::MapNote failed size validation");

	struct PathGrid : BaseObject {
		struct Node {
			int relativeX;
			int relativeY;
			int relativeZ;
			NI::IteratedList<Node**>* connectedNodes; // 0xC
			PathGrid* parentGrid; // 0x10
			unsigned char connectedCount; // 0x14
			NI::Pointer<NI::Node> debugNode; // 0x18

			Node() = delete;
			~Node() = delete;

			sol::object getConnectedNodes_lua(sol::this_state ts) const;
			NI::Point3 getPosition() const;
		};

		NI::Pointer<NI::Node> sceneNode; // 0x10
		char unknown_0x14;
		char unknown_0x15;
		char unknown_0x16;
		char unknown_0x17;
		Cell* parentCell; // 0x18
		short granularity; // 0x1C
		unsigned short nodeCount; // 0x1E
		NI::IteratedList<Node*> nodes; // 0x20
		unsigned int fileOffset; // 0x34
		bool isLoaded;

		PathGrid() = delete;
		~PathGrid() = delete;

		void show();
		void hide();
	};
	static_assert(sizeof(PathGrid) == 0x3C, "TES3::PathGrid failed size validation");
	static_assert(sizeof(PathGrid::Node) == 0x1C, "TES3::PathGrid::Node failed size validation");

	struct Cell : BaseObject {
		struct MovedRef {
			struct Coordinates {
				int gridX;
				int gridY;

				Coordinates() = delete;
				~Coordinates() = delete;
			};
			unsigned char flags;
			Reference * reference;
			unsigned int sourceID;
			union {
				const char* targetCellName;
				Coordinates* targetCellXY;
			} duringLoad;

			MovedRef() = delete;
			~MovedRef() = delete;
		};
		struct SourceMod {
			GameFile * sourceFile;
			unsigned int fileOffsetCellRecord;
			unsigned int fileOffsetTempRefs;

			SourceMod() = delete;
			~SourceMod() = delete;
		};
		struct MappingVisuals {
			int unknown_0x0;
			int unknown_0x4;
			int unknown_0x8;
			int unknown_0xC;
			int unknown_0x10;
			NI::Pointer<NI::SourceTexture> texture; // 0x14

			MappingVisuals() = delete;
			~MappingVisuals() = delete;
		};

		char * name; // 0x10
		NI::Node * pickObjectsRoot; // 0x14
		unsigned int cellFlags; // 0x18
		union {
			struct {
				NI::PackedColor regionMapColor; // 0x0
				Land * landscape; // 0x4
				int gridX; // 0x8
				int gridY; // 0xC
			} exterior;
			struct {
				NI::PackedColor ambientColor; // 0x0
				NI::PackedColor sunColor; // 0x4
				NI::PackedColor fogColor; // 0x8
				float fogDensity; // 0xC
			} interior;
		} variantData; // 0x1C
		NI::Node * staticObjectsRoot; // 0x2C
		ReferenceList actors; // 0x30
		ReferenceList persistentRefs; // 0x40
		NI::IteratedList<MovedRef*> * movedReferences; // 0x50
		NI::IteratedList<unsigned int*> * moveRefSourceIDs; // 0x54
		ReferenceList temporaryRefs; // 0x58
		NI::IteratedList<SourceMod*> allSourceMods; // 0x68
		void * fogOfWarData;
		GameFile * lastModifyingFile;
		MappingVisuals * mappingVisuals;
		NI::IteratedList<MapNote*> * mapNotes; // 0x88
		PathGrid * pathGrid; // 0x8C
		union {
			float waterLevel;
			Region * region;
		} waterLevelOrRegion; // 0x90

		Cell() = delete;
		~Cell() = delete;

		//
		// Other related this-call functions.
		//

		static Cell* create();

		Reference * getFirstObjectOfType(ObjectType::ObjectType, bool) const;

		int getGridX() const;
		void setGridX(int x);
		int getGridY() const;
		void setGridY(int y);

		void setName(const char* name);

		void addMapNote(NI::Point2* position, float unknown, const char* text);

		void addReference(Reference* reference);
		void insertReference(Reference* reference);

		NI::Node* getOrCreateActivatorsNode();

		//
		// Other getter/setter functions.
		//

		const char* getName() const;

		bool getCellFlag(unsigned int) const;
		void setCellFlag(unsigned int, bool);

		std::optional<float> getFogDensity() const;
		void setFogDensity(float value);

		std::optional<float> getWaterLevel() const;
		void setWaterLevel(float);

		Land* getLandscape() const;
		Region* getRegion() const;

		NI::PackedColor* getAmbientColor();
		NI::PackedColor* getFogColor();
		NI::PackedColor* getSunColor();

		bool getBehavesAsExterior() const;
		void setBehavesAsExterior(bool value);

		bool getHasWater() const;
		void setHasWater(bool value);

		bool getIsInterior() const;
		void setIsInterior(bool value);

		bool getSleepingIsIllegal() const;
		void setSleepingIsIllegal(bool value);

		bool getIsMarkerDrawn() const;

		bool getIsOrBehavesAsExterior() const;

		//
		// Custom functions.
		//

		void setCellActive();
		void setCellInactive();
		bool getCellActive() const;

		bool getHasCellMarker() const;

		const char* getDisplayName() const;
		std::string getEditorName() const;

		//
		// Helper functions.
		//

		bool isPointInCell(float x, float y) const;

		static constexpr int exteriorGridWidth = 8192;

		static int toGridCoord(float x);
	};
	static_assert(sizeof(Cell) == 0x94, "TES3::Cell failed size validation");
	static_assert(sizeof(Cell::MovedRef) == 0x10, "TES3::Cell::MovedRef failed size validation");
	static_assert(sizeof(Cell::SourceMod) == 0xC, "TES3::Cell::SourceMod failed size validation");
	static_assert(sizeof(Cell::MappingVisuals) == 0x18, "TES3::Cell::MappingVisuals failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Cell)
