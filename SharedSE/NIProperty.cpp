#include "NIProperty.h"

#include "MemoryUtil.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "NITexture.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "LuaUtil.h"
#endif

#include "ExceptionUtil.h"

namespace NI {

	//
	// NiProperty
	//

	Property::Property() {
#if defined(SE_NI_PROPERTY_FNADDR_CTOR) && SE_NI_PROPERTY_FNADDR_CTOR > 0
		const auto NI_Property_ctor = reinterpret_cast<Property * (__thiscall*)(Property*)>(SE_NI_PROPERTY_FNADDR_CTOR);
		NI_Property_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	Property::~Property() {
#if defined(SE_NI_PROPERTY_FNADDR_DTOR) && SE_NI_PROPERTY_FNADDR_DTOR > 0
		const auto NI_Property_dtor = reinterpret_cast<void(__thiscall*)(Property*)>(SE_NI_PROPERTY_FNADDR_DTOR);
		NI_Property_dtor(this);
#endif
	}

	PropertyType Property::getType() const {
		return static_cast<PropertyType>(vTable.asProperty->getType(this));
	}

	void Property::update(float dt) {
		vTable.asProperty->update(this, dt);
	}

	void Property::setFlagBitField(unsigned short value, unsigned short mask, unsigned int index) {
#if defined(SE_NI_PROPERTY_FNADDR_SETFLAGBITFIELD) && SE_NI_PROPERTY_FNADDR_SETFLAGBITFIELD > 0
		const auto NI_Property_setFlagBitField = reinterpret_cast<void(__thiscall*)(Property*, unsigned short, unsigned short, unsigned int)>(SE_NI_PROPERTY_FNADDR_SETFLAGBITFIELD);
		NI_Property_setFlagBitField(this, value, mask, index);
#else
		throw not_implemented_exception();
#endif
	}

	//
	// NiAlphaProperty
	//

	AlphaProperty::AlphaProperty() {
		vTable.asProperty = (Property_vTable*)SE_NI_ALPHAPROPERTY_VTBL;
		/*
		// Disabled, cause setFlagBitField causes crashing, will set flags directly instead.
		setFlag(false, 0);
		setFlagBitField(6, 0xF, 1);
		setFlagBitField(7, 0xF, 5);
		setFlag(false, 9);
		setFlagBitField(0, 0x7, 10);
		*/
		alphaTestRef = 255; // Default to 0% alpha threshold
		flags = 4844; // Default flags for alpha property from NifSkope
	}

	AlphaProperty::~AlphaProperty() {

	}

	Pointer<AlphaProperty> AlphaProperty::create() {
		return new AlphaProperty();
	}

	//
	// NiFogProperty
	//

	std::reference_wrapper<unsigned char[4]> FogProperty::getColor() {
		return std::ref(color);
	}

	//
	// NiMaterialProperty
	//

	Color MaterialProperty::getAmbient() {
		return ambient;
	}

	void MaterialProperty::setAmbient(Color& value) {
		ambient = value;
		revisionID++;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void MaterialProperty::setAmbient_lua(sol::object value) {
		ambient = value;
		revisionID++;
	}
#endif

	Color MaterialProperty::getDiffuse() {
		return diffuse;
	}

	void MaterialProperty::setDiffuse(Color& value) {
		diffuse = value;
		revisionID++;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void MaterialProperty::setDiffuse_lua(sol::object value) {
		diffuse = value;
		revisionID++;
	}
#endif

	Color MaterialProperty::getSpecular() {
		return specular;
	}

	void MaterialProperty::setSpecular(Color& value) {
		specular = value;
		revisionID++;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void MaterialProperty::setSpecular_lua(sol::object value) {
		specular = value;
		revisionID++;
	}
#endif

	Color MaterialProperty::getEmissive() {
		return emissive;
	}

	void MaterialProperty::setEmissive(Color& value) {
		emissive = value;
		revisionID++;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void MaterialProperty::setEmissive_lua(sol::object value) {
		emissive = value;
		revisionID++;
	}
#endif

	float MaterialProperty::getShininess() {
		return shininess;
	}

	void MaterialProperty::setShininess(float value) {
		shininess = value;
		revisionID++;
	}

	float MaterialProperty::getAlpha() {
		return alpha;
	}

	void MaterialProperty::setAlpha(float value) {
		alpha = value;
		revisionID++;
	}

	void MaterialProperty::incrementRevisionId() {
		revisionID++;
	}

	//
	// NiTexturingProperty
	//

	void* TexturingProperty::Map::operator new(size_t size) {
		return se::memory::_new(size);
	}

	void TexturingProperty::Map::operator delete(void* address) {
		se::memory::_delete(address);
	}

	TexturingProperty::Map::Map() {
#if defined(SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTOR) && SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTOR > 0
		const auto NI_TexturingProperty_Map_ctor = reinterpret_cast<TexturingProperty::Map * (__thiscall*)(TexturingProperty::Map*)>(SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTOR);
		NI_TexturingProperty_Map_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	TexturingProperty::Map::Map(Texture* _texture, ClampMode _clampMode, FilterMode _filterMode, unsigned int _textCoords) {
#if defined(SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTORWITHPARAMS) && SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTORWITHPARAMS > 0
		const auto NI_TexturingProperty_Map_ctorWithParams = reinterpret_cast<TexturingProperty::Map * (__thiscall*)(TexturingProperty::Map*, Texture*, unsigned int, TexturingProperty::ClampMode, TexturingProperty::FilterMode)>(SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTORWITHPARAMS);
		NI_TexturingProperty_Map_ctorWithParams(this, _texture, _textCoords, _clampMode, _filterMode);
#else
		throw not_implemented_exception();
#endif
	}

	TexturingProperty::Map::~Map() {
		texture = nullptr;
		vTable->destructor(this, false);
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	Pointer<Texture> TexturingProperty::Map::getTexture_lua() const {
		return texture;
	}

	void TexturingProperty::Map::setTexture_lua(Texture* t) {
		texture = t;
	}

	TexturingProperty::Map* TexturingProperty::Map::create(sol::optional<sol::table> params) {
		auto texture = mwse::lua::getOptionalParam<Texture*>(params, "texture", nullptr);
		auto clampMode = mwse::lua::getOptionalParam(params, "clampMode", ClampMode::WRAP_S_WRAP_T);
		auto filterMode = mwse::lua::getOptionalParam(params, "filterMode", FilterMode::TRILERP);
		auto textCoords = mwse::lua::getOptionalParam(params, "textCoords", 0u);

		if (mwse::lua::getOptionalParam(params, "isBumpMap", false)) {
			return new BumpMap(texture, clampMode, filterMode, textCoords);
		}
		else {
			return new Map(texture, clampMode, filterMode, textCoords);
		}
	}
#endif

	TexturingProperty::BumpMap::BumpMap() : Map() {
		vTable = (VirtualTable*)SE_NI_TEXTURINGPROPERTY_BUMPMAP_VTBL;
		lumaScale = 1.0f;
		lumaOffset = 0.0f;
		bumpMat[0][0] = 0.5f;
		bumpMat[0][1] = 0.0f;
		bumpMat[1][0] = 0.0f;
		bumpMat[1][1] = 0.5f;
	}

	TexturingProperty::BumpMap::BumpMap(Texture* _texture, ClampMode _clampMode, FilterMode _filterMode, unsigned int _textCoords) : Map(_texture, _clampMode, _filterMode, _textCoords) {
		vTable = (VirtualTable*)SE_NI_TEXTURINGPROPERTY_BUMPMAP_VTBL;
		lumaScale = 1.0f;
		lumaOffset = 0.0f;
		bumpMat[0][0] = 0.5f;
		bumpMat[0][1] = 0.0f;
		bumpMat[1][0] = 0.0f;
		bumpMat[1][1] = 0.5f;
	}

	TexturingProperty::TexturingProperty() {
#if defined(SE_NI_TEXTURINGPROPERTY_FNADDR_CTOR) && SE_NI_TEXTURINGPROPERTY_FNADDR_CTOR > 0
		// Cleanup crust from automatic construction.
		se::memory::_delete(maps.storage);
		memset(this, 0, sizeof(TexturingProperty));

		const auto NI_TexturingProperty_ctor = reinterpret_cast<TexturingProperty*(__thiscall*)(TexturingProperty*)>(SE_NI_TEXTURINGPROPERTY_FNADDR_CTOR);
		NI_TexturingProperty_ctor(this);
#else
		throw not_implemented_exception();
#endif
	}

	TexturingProperty::~TexturingProperty() {
#if defined(SE_NI_TEXTURINGPROPERTY_FNADDR_DTOR) && SE_NI_TEXTURINGPROPERTY_FNADDR_DTOR > 0
		const auto NI_TexturingProperty_dtor = reinterpret_cast<TexturingProperty * (__thiscall*)(TexturingProperty*)>(SE_NI_TEXTURINGPROPERTY_FNADDR_DTOR);
		NI_TexturingProperty_dtor(this);
#else
		throw not_implemented_exception();
#endif
	}

	TexturingProperty::Map* TexturingProperty::getBaseMap() {
		return maps[size_t(MapType::BASE)];
	}

	void TexturingProperty::setBaseMap(std::optional<Map*> map) {
		auto currentMap = maps[size_t(MapType::BASE)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::BASE), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::BASE), map.value());
		}
	}

	TexturingProperty::Map* TexturingProperty::getDarkMap() {
		return maps[size_t(MapType::DARK)];
	}

	void TexturingProperty::setDarkMap(std::optional<Map*> map) {
		auto currentMap = maps[size_t(MapType::DARK)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::DARK), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::DARK), map.value());
		}
	}

	TexturingProperty::Map* TexturingProperty::getDetailMap() {
		return maps[size_t(MapType::DETAIL)];
	}

	void TexturingProperty::setDetailMap(std::optional<Map*> map) {
		auto currentMap = maps[size_t(MapType::DETAIL)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::DETAIL), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::DETAIL), map.value());
		}
	}

	TexturingProperty::Map* TexturingProperty::getGlossMap() {
		return maps[size_t(MapType::GLOSS)];
	}

	void TexturingProperty::setGlossMap(std::optional<Map*> map) {
		auto currentMap = maps[size_t(MapType::GLOSS)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::GLOSS), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::GLOSS), map.value());
		}
	}

	TexturingProperty::Map* TexturingProperty::getGlowMap() {
		return maps[size_t(MapType::GLOW)];
	}

	void TexturingProperty::setGlowMap(std::optional<Map*> map) {
		auto currentMap = maps[size_t(MapType::GLOW)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::GLOW), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::GLOW), map.value());
		}
	}

	TexturingProperty::BumpMap* TexturingProperty::getBumpMap() {
		return static_cast<BumpMap*>(maps[size_t(MapType::BUMP)]);
	}

	void TexturingProperty::setBumpMap(std::optional<TexturingProperty::BumpMap*> map) {
		auto currentMap = maps[size_t(MapType::BUMP)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::BUMP), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::BUMP), map.value());
		}
	}

	unsigned int TexturingProperty::getUsedMapCount() const {
		unsigned int count = 0;
		for (auto map : maps) {
			if (map && map->texture) {
				count++;
			}
		}
		return count;
	}

	bool TexturingProperty::canAddMap() const {
		return getUsedMapCount() < MAX_MAP_COUNT;
	}

	unsigned int TexturingProperty::getDecalCount() const {
		auto count = 0;
		for (auto i = (unsigned int)MapType::DECAL_FIRST; i <= (unsigned int)MapType::DECAL_LAST; ++i) {
			if (i >= maps.size()) {
				break;
			}

			if (maps[i] != nullptr) {
				count++;
			}
		}
		return count;
	}

	bool TexturingProperty::canAddDecalMap() const {
		if (!canAddMap()) {
			return false;
		}
		return getDecalCount() < MAX_DECAL_COUNT;
	}

	unsigned int TexturingProperty::addDecalMap(Texture* texture) {
		unsigned int index = (unsigned int)MapType::DECAL_FIRST;
		while (index < maps.size() && maps[index] != nullptr) {
			++index;
		}

		if (index > (size_t)MapType::DECAL_LAST) {
			return (size_t)MapType::INVALID;
		}

		maps.growToFit(index);

		auto map = new Map(texture);
		maps.setAtIndex(index, map);

		return index;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	sol::optional<std::tuple<TexturingProperty::Map*, unsigned int>> TexturingProperty::addDecalMap_lua(sol::optional<Texture*> texture) {
		auto index = addDecalMap(texture.value_or(nullptr));
		if (index == (size_t)MapType::INVALID) {
			return {};
		}
		return std::make_tuple(maps.at(index), index + 1);
	}
#endif

	bool TexturingProperty::removeDecal(unsigned int index) {
		if (index < (unsigned int)MapType::DECAL_FIRST || index >(unsigned int)MapType::DECAL_LAST) {
			throw std::invalid_argument("Invalid map index provided.");
		}

		if (index >= maps.size()) {
			return false;
		}

		if (maps[index] == nullptr) {
			return false;
		}

		delete maps[index];
		maps.setAtIndex(index, nullptr);
		return true;
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	bool TexturingProperty::removeDecal_lua(unsigned int index) {
		return removeDecal(index - 1);
	}
#endif

	//
	// VertexColorProperty
	//

	VertexColorProperty::VertexColorProperty() {
		vTable.asProperty = (Property_vTable*)SE_NI_VERTEXCOLORPROPERTY_VTBL;
		source = 0;
		lighting = 1;
	}

	VertexColorProperty::~VertexColorProperty() {
	}

	Pointer<VertexColorProperty> VertexColorProperty::create() {
		return new VertexColorProperty();
	}

	//
	// NiZBufferProperty
	//

	ZBufferProperty::ZBufferProperty() {
		vTable.asProperty = (Property_vTable*)SE_NI_ZBUFFERPROPERTY_VTBL;
		setFlag(false, 0);
		setFlag(false, 1);
		testFunction = TestFunction::LESS_EQUAL;
	}

	ZBufferProperty::~ZBufferProperty() {

	}

	Pointer<ZBufferProperty> ZBufferProperty::create() {
		return new ZBufferProperty();
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Property)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AlphaProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::FogProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::MaterialProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::StencilProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TexturingProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::VertexColorProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ZBufferProperty)
#endif
