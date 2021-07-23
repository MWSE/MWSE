#include "NIProperty.h"

#include "MemoryUtil.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "NITexture.h"

#include "LuaUtil.h"

namespace NI {

	//
	// NiProperty
	//

	const auto NI_Property_ctor = reinterpret_cast<Property * (__thiscall*)(Property*)>(0x405990);
	Property::Property() {
		NI_Property_ctor(this);
	}

	const auto NI_Property_dtor = reinterpret_cast<void(__thiscall*)(Property*)>(0x405B40);
	Property::~Property() {
		NI_Property_dtor(this);
	}

	PropertyType Property::getType() const {
		return static_cast<PropertyType>(vTable.asProperty->getType(this));
	}

	void Property::update(float dt) {
		vTable.asProperty->update(this, dt);
	}

	const auto NI_Property_setFlagBitField = reinterpret_cast<void(__thiscall*)(Property*, unsigned short, unsigned short, unsigned int)>(0x408A10);
	void  Property::setFlagBitField(unsigned short value, unsigned short mask, unsigned int index) {
		NI_Property_setFlagBitField(this, value, mask, index);
	}

	//
	// NiAlphaProperty
	//

	AlphaProperty::AlphaProperty() {
		Property::Property();
		vTable.asProperty = (Property_vTable*)0x7465A8;
		setFlag(false, 0);
		setFlagBitField(6, 0xF, 1);
		setFlagBitField(7, 0xF, 5);
		setFlag(false, 9);
		setFlagBitField(0, 0x7, 10);
		alphaTestRef = 0;
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

	void MaterialProperty::setAmbient_lua(sol::object value) {
		ambient = value;
		revisionID++;
	}


	Color MaterialProperty::getDiffuse() {
		return diffuse;
	}

	void MaterialProperty::setDiffuse(Color& value) {
		diffuse = value;
		revisionID++;
	}

	void MaterialProperty::setDiffuse_lua(sol::object value) {
		diffuse = value;
		revisionID++;
	}

	Color MaterialProperty::getSpecular() {
		return specular;
	}

	void MaterialProperty::setSpecular(Color& value) {
		specular = value;
		revisionID++;
	}

	void MaterialProperty::setSpecular_lua(sol::object value) {
		specular = value;
		revisionID++;
	}

	Color MaterialProperty::getEmissive() {
		return emissive;
	}

	void MaterialProperty::setEmissive(Color& value) {
		emissive = value;
		revisionID++;
	}

	void MaterialProperty::setEmissive_lua(sol::object value) {
		emissive = value;
		revisionID++;
	}

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
		return mwse::tes3::_new(size);
	}

	void TexturingProperty::Map::operator delete(void* address) {
		mwse::tes3::_delete(address);
	}

	const auto NI_TexturingProperty_Map_ctor = reinterpret_cast<TexturingProperty::Map * (__thiscall*)(TexturingProperty::Map*)>(0x42DCD0);
	TexturingProperty::Map::Map() {
		NI_TexturingProperty_Map_ctor(this);
	}

	const auto NI_TexturingProperty_Map_ctorWithParams = reinterpret_cast<TexturingProperty::Map * (__thiscall*)(TexturingProperty::Map*, Texture*, unsigned int, TexturingProperty::ClampMode, TexturingProperty::FilterMode)>(0x4CEEC0);
	TexturingProperty::Map::Map(Texture* _texture, ClampMode _clampMode, FilterMode _filterMode, unsigned int _textCoords) {
		NI_TexturingProperty_Map_ctorWithParams(this, _texture, _textCoords, _clampMode, _filterMode);
	}

	TexturingProperty::Map::~Map() {
		texture = nullptr;
		vTable->destructor(this, false);
	}

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

	TexturingProperty::BumpMap::BumpMap() : Map() {
		vTable = (VirtualTable*)0x7507B0;
		lumaScale = 1.0f;
		lumaOffset = 0.0f;
		bumpMat[0][0] = 0.5f;
		bumpMat[0][1] = 0.0f;
		bumpMat[1][0] = 0.0f;
		bumpMat[1][1] = 0.5f;
	}

	TexturingProperty::BumpMap::BumpMap(Texture* _texture, ClampMode _clampMode, FilterMode _filterMode, unsigned int _textCoords) : Map(_texture, _clampMode, _filterMode, _textCoords) {
		vTable = (VirtualTable*)0x7507B0;
		lumaScale = 1.0f;
		lumaOffset = 0.0f;
		bumpMat[0][0] = 0.5f;
		bumpMat[0][1] = 0.0f;
		bumpMat[1][0] = 0.0f;
		bumpMat[1][1] = 0.5f;
	}

	TexturingProperty::Map* TexturingProperty::getBaseMap() {
		return maps[size_t(MapType::BASE)];
	}

	void TexturingProperty::setBaseMap(sol::optional<Map*> map) {
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

	void TexturingProperty::setDarkMap(sol::optional<Map*> map) {
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

	void TexturingProperty::setDetailMap(sol::optional<Map*> map) {
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

	void TexturingProperty::setGlossMap(sol::optional<Map*> map) {
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

	void TexturingProperty::setGlowMap(sol::optional<Map*> map) {
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

	void TexturingProperty::setBumpMap(sol::optional<TexturingProperty::BumpMap*> map) {
		auto currentMap = maps[size_t(MapType::BUMP)];
		if (currentMap) {
			delete currentMap;
			maps.setAtIndex(size_t(MapType::BUMP), nullptr);
		}

		if (map) {
			maps.setAtIndex(size_t(MapType::BUMP), map.value());
		}
	}


	unsigned int TexturingProperty::getDecalCount() const {
		auto count = 0;
		for (unsigned int i = (unsigned int)MapType::DECAL_FIRST; i <= (unsigned int)MapType::DECAL_LAST; i++) {
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
		return getDecalCount() < 7;
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

	sol::optional<std::tuple<TexturingProperty::Map*, unsigned int>> TexturingProperty::addDecalMap_lua(sol::optional<Texture*> texture) {
		auto index = addDecalMap(texture.value_or(nullptr));
		if (index == (size_t)MapType::INVALID) {
			return {};
		}
		return std::make_tuple(maps.at(index), index + 1);
	}

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

	bool TexturingProperty::removeDecal_lua(unsigned int index) {
		return removeDecal(index - 1);
	}

	//
	// VertexColorProperty
	//

	VertexColorProperty::VertexColorProperty() {
		vTable.asProperty = (Property_vTable*)0x7464F8;
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
		Property::Property();
		vTable.asProperty = (Property_vTable*)0x74652C;
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

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Property)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AlphaProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::FogProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::MaterialProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::StencilProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TexturingProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::VertexColorProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ZBufferProperty)
