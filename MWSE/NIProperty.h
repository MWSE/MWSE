#pragma once

#include "NIObjectNET.h"

#include "NIColor.h"
#include "NILinkedList.h"

namespace NI {
	enum class PropertyType : int {
		Alpha = 0x0,
		Fog = 0x1,
		Material = 0x2,
		Stencil = 0x3,
		Texturing = 0x4,
		VertexColor = 0x5,
		Wireframe = 0x6,
		ZBuffer = 0x7,
		Dither = 0x8,
		Specular = 0x9,
		Shade = 0xA,
		RendererSpecific = 0xB,

		FirstPropertyType = Alpha,
		LastPropertyType = RendererSpecific,
	};

	struct Property_vTable : Object_vTable {
		PropertyType (__thiscall * getType)(const Property*); // 0x2C
		void (__thiscall * update)(Property*, float); // 0x30
	};
	static_assert(sizeof(Property_vTable) == 0x34, "NI::Property's vtable failed size validation");

	struct Property : ObjectNET {
		unsigned short flags;

		Property();
		~Property();

		//
		// vTable wrappers.
		//

		PropertyType getType() const;
		void update(float dt);

		//
		// Other function addresses.
		//

		void setFlagBitField(unsigned short value, unsigned short mask, unsigned int index);

		static constexpr auto _loadBinary = reinterpret_cast<void(__thiscall*)(Property*, Stream*)>(0x6E9610);
		static constexpr auto _saveBinary = reinterpret_cast<void(__thiscall*)(const Property*, Stream*)>(0x6E9660);

	};
	static_assert(sizeof(Property) == 0x18, "NI::Property failed size validation");

	struct AlphaProperty : Property {
		unsigned char alphaTestRef;

		AlphaProperty();
		~AlphaProperty();

		static Pointer<AlphaProperty> create();
	};
	static_assert(sizeof(AlphaProperty) == 0x1C, "NI::AlphaProperty failed size validation");

	struct FogProperty : Property {
		float density;
		unsigned char color[4];

		//
		// Custom functions.
		//

		std::reference_wrapper<unsigned char[4]> getColor();

	};
	static_assert(sizeof(FogProperty) == 0x20, "NI::FogProperty failed size validation");

	struct MaterialProperty : Property {
		int index;
		Color ambient;
		Color diffuse;
		Color specular;
		Color emissive;
		float shininess;
		float alpha;
		unsigned int revisionID;

		//
		// Custom functions.
		//

		Color getAmbient();
		void setAmbient(Color& value);
		void setAmbient_lua(sol::object value);

		Color getDiffuse();
		void setDiffuse(Color& value);
		void setDiffuse_lua(sol::object value);

		Color getSpecular();
		void setSpecular(Color& value);
		void setSpecular_lua(sol::object value);

		Color getEmissive();
		void setEmissive(Color& value);
		void setEmissive_lua(sol::object value);

		float getShininess();
		void setShininess(float value);

		float getAlpha();
		void setAlpha(float value);

		void incrementRevisionId();

	};
	static_assert(sizeof(MaterialProperty) == 0x58, "NI::MaterialProperty failed size validation");

	struct StencilProperty : Property {
		bool enabled;
		int testFunc;
		unsigned int reference;
		unsigned int mask;
		int failAction;
		int zFailAction;
		int passAction;
		int drawMode;
	};
	static_assert(sizeof(StencilProperty) == 0x38, "NI::StencilProperty failed size validation");

	struct TexturingProperty : Property {
		enum struct ClampMode {
			CLAMP_S_CLAMP_T,
			CLAMP_S_WRAP_T,
			WRAP_S_CLAMP_T,
			WRAP_S_WRAP_T
		};
		enum struct FilterMode {
			NEAREST,
			BILERP,
			TRILERP,
			NEAREST_MIPNEAREST,
			NEAREST_MIPLERP,
			BILERP_MIPNEAREST
		};
		enum struct ApplyMode {
			REPLACE,
			DECAL,
			MODULATE
		};
		enum struct MapType : unsigned int {
			BASE,
			DARK,
			DETAIL,
			GLOSS,
			GLOW,
			BUMP,
			DECAL_1,
			DECAL_2,
			DECAL_3,
			DECAL_4,
			DECAL_5,
			DECAL_6,
			DECAL_7,

			DECAL_FIRST = DECAL_1,
			DECAL_LAST = DECAL_7,

			INVALID = UINT32_MAX,
		};

		struct Map {
			struct VirtualTable {
				void(__thiscall* destructor)(Map*, bool); // 0x0
				void(__thiscall* loadBinary)(Map*, Stream*); // 0x4
				void(__thiscall* saveBinary)(Map*, Stream*); // 0x8
			};

			VirtualTable* vTable; // 0x0
			Pointer<Texture> texture; // 0x4
			ClampMode clampMode; // 0x8
			FilterMode filterMode; // 0xC
			unsigned int texCoordSet; // 0x10

			static void* operator new(size_t size);
			static void operator delete(void* block);

			Map();
			Map(Texture* texture, ClampMode clampMode = ClampMode::WRAP_S_WRAP_T, FilterMode filterMode = FilterMode::TRILERP, unsigned int textureCoords = 0);
			~Map();

			Pointer<Texture> getTexture_lua() const;
			void setTexture_lua(Texture* texture);

			static Map* create(sol::optional<sol::table> params);

		};
		struct BumpMap : Map {
			float lumaScale;
			float lumaOffset;
			float bumpMat[2][2];

			BumpMap();
			BumpMap(Texture* texture, ClampMode clampMode = ClampMode::WRAP_S_WRAP_T, FilterMode filterMode = FilterMode::BILERP, unsigned int textureCoords = 0);
		};

		ApplyMode applyMode; // 0x18
		TArray<Map*> maps; // 0x1C
		int unknown_34; // 0x34

		Map* getBaseMap();
		void setBaseMap(sol::optional<Map*> map);
		Map* getDarkMap();
		void setDarkMap(sol::optional<Map*> map);
		Map* getDetailMap();
		void setDetailMap(sol::optional<Map*> map);
		Map* getGlossMap();
		void setGlossMap(sol::optional<Map*> map);
		Map* getGlowMap();
		void setGlowMap(sol::optional<Map*> map);
		BumpMap* getBumpMap();
		void setBumpMap(sol::optional<BumpMap*> map);

		unsigned int getDecalCount() const;
		bool canAddDecalMap() const;

		unsigned int addDecalMap(Texture* texture);
		sol::optional<std::tuple<Map*, unsigned int>> addDecalMap_lua(sol::optional<Texture*> texture);

		bool removeDecal(unsigned int index);
		bool removeDecal_lua(unsigned int index);

	};
	static_assert(sizeof(TexturingProperty) == 0x38, "NI::TexturingProperty failed size validation");
	static_assert(sizeof(TexturingProperty::Map) == 0x14, "NI::TexturingProperty::Map failed size validation");
	static_assert(sizeof(TexturingProperty::BumpMap) == 0x2C, "NI::TexturingProperty::BumpMap failed size validation");

	struct VertexColorProperty : Property {
		int source; // 0x18
		int lighting; // 0x1C

		VertexColorProperty();
		~VertexColorProperty();

		static Pointer<VertexColorProperty> create();
	};
	static_assert(sizeof(VertexColorProperty) == 0x20, "NI::VertexColorProperty failed size validation");

	struct ZBufferProperty : Property {
		enum struct TestFunction : unsigned int {
			ALWAYS,
			LESS,
			EQUAL,
			LESS_EQUAL,
			GREATER,
			NOT_EQUAL,
			GREATER_EQUAL,
			NEVER,
		};

		TestFunction testFunction; // 0x18

		ZBufferProperty();
		~ZBufferProperty();

		static Pointer<ZBufferProperty> create();
	};
	static_assert(sizeof(ZBufferProperty) == 0x1C, "NI::ZBufferProperty failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::Property)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AlphaProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::FogProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::MaterialProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::StencilProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::TexturingProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::VertexColorProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::ZBufferProperty)
