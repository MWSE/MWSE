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
			IsLoaded = 0x20,
			BehavesAsExterior = 0x80
		};
	}

	struct PackedColor {
		unsigned char r; // 0x0
		unsigned char g; // 0x1
		unsigned char b; // 0x2
		unsigned char a; // 0x3
	};
	static_assert(sizeof(PackedColor) == 0x4, "TES3::PackedColor failed size validation");

	struct MapNote {
		Vector3 position; // 0x0
		char * text; // 0xC
	};
	static_assert(sizeof(MapNote) == 0x10, "TES3::MapNote failed size validation");

	struct PathGrid : BaseObject {
		struct Node {
			int x;
			int y;
			int z;
			IteratedList<Node*>* connectedNodes; // 0xC
			PathGrid* parentGrid; // 0x10
			int index; // 0x14
			NI::Pointer<NI::Node> debugNode; // 0x18
		};

		NI::Pointer<NI::Node> sceneNode; // 0x10
		char unknown_0x14;
		char unknown_0x15;
		char unknown_0x16;
		char unknown_0x17;
		Cell* parentCell; // 0x18
		short granularity; // 0x1C
		short pointCount; // 0x1E
		IteratedList<Node*> nodes; // 0x20
		unsigned int fileOffset; // 0x34
		char unknown_0x38;
	};
	static_assert(sizeof(PathGrid) == 0x3C, "TES3::PathGrid failed size validation");
	static_assert(sizeof(PathGrid::Node) == 0x1C, "TES3::PathGrid::Node failed size validation");

	struct Cell : BaseObject {
		struct MovedRef {
			struct Coordinates {
				int gridX;
				int gridY;
			};
			unsigned char flags;
			Reference * reference;
			unsigned int sourceID;
			union {
				const char* targetCellName;
				Coordinates* targetCellXY;
			} duringLoad;
		};
		struct SourceMod {
			GameFile * sourceFile;
			unsigned int fileOffsetCellRecord;
			unsigned int fileOffsetTempRefs;
		};
		struct MappingVisuals {
			int unknown_0x0;
			int unknown_0x4;
			int unknown_0x8;
			int unknown_0xC;
			int unknown_0x10;
			NI::Pointer<NI::SourceTexture> texture; // 0x14
		};

		char * name; // 0x10
		NI::Node * pickObjectsRoot; // 0x14
		unsigned int cellFlags; // 0x18
		union {
			struct {
				TES3::PackedColor regionMapColor; // 0x0
				void * landscape; // 0x4
				int gridX; // 0x8
				int gridY; // 0xC
			} exterior;
			struct {
				PackedColor ambientColor; // 0x0
				PackedColor sunColor; // 0x4
				PackedColor fogColor; // 0x8
				float fogDensity; // 0xC
			} interior;
		} variantData; // 0x1C
		NI::Node * staticObjectsRoot; // 0x2C
		ReferenceList actors; // 0x30
		ReferenceList persistentRefs; // 0x40
		IteratedList<MovedRef*> * movedReferences; // 0x50
		IteratedList<unsigned int*> * moveRefSourceIDs; // 0x54
		ReferenceList temporaryRefs; // 0x58
		IteratedList<SourceMod*> allSourceMods; // 0x68
		void * fogOfWarData;
		GameFile * lastModifyingFile;
		MappingVisuals * mappingVisuals;
		IteratedList<MapNote*> * mapNotes; // 0x88
		void * pathGrid; // 0x8C
		union {
			float waterLevel;
			Region * region;
		} waterLevelOrRegion; // 0x90

		//
		// Other related this-call functions.
		//

		static Cell* create();

		Reference * getFirstObjectOfType(ObjectType::ObjectType, bool) const;

		bool isInterior() const;
		int getGridX() const;
		void setGridX(int x);
		int getGridY() const;
		void setGridY(int y);

		void setName(const char* name);

		void addMapNote(Vector2* position, float unknown, const char* text);

		void insertReference(Reference* reference);

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

		Region * getRegion() const;

		TES3::PackedColor* getAmbientColor();
		TES3::PackedColor* getFogColor();
		TES3::PackedColor* getSunColor();

		bool getBehavesAsExterior() const;
		void setBehavesAsExterior(bool value);

		bool getHasWater() const;
		void setHasWater(bool value);

		bool getIsInterior() const;
		void setIsInterior(bool value);

		bool getSleepingIsIllegal() const;
		void setSleepingIsIllegal(bool value);

		//
		// Custom functions.
		//

		void setCellActive();
		void setCellInactive();
		bool getCellActive() const;

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
