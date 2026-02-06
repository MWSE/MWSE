#include "NIProperty.h"

#include "BitUtil.h"
#include "MemoryUtil.h"

#include "NIBinaryStream.h"
#include "NIStream.h"
#include "NITexture.h"

#include "LuaUtil.h"
#include "StringUtil.h"

namespace NI {

	//
	// NiProperty
	//

	const auto NI_Property_ctor = reinterpret_cast<Property * (__thiscall*)(Property*)>(0x405990);
	Property::Property() {
		flags = 0;
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

	const auto NI_Property_copyMembers = reinterpret_cast<void(__thiscall*)(const Property*, Property*)>(0x6E95F0);
	void Property::copyMembers(Property* to) const {
		NI_Property_copyMembers(this, to);
	}

	bool Property::getFlag(unsigned char index) const {
		return BIT_TEST(flags, index);
	}

	const auto NI_Property_setFlag = reinterpret_cast<void(__thiscall*)(Property*, bool, unsigned char)>(0x405960);
	void Property::setFlag(bool state, unsigned char index) {
		NI_Property_setFlag(this, state, index);
	}

	const auto NI_Property_setFlagBitField = reinterpret_cast<void(__thiscall*)(Property*, unsigned short, unsigned short, unsigned int)>(0x408A10);
	void Property::setFlagBitField(unsigned short value, unsigned short mask, unsigned int index) {
		NI_Property_setFlagBitField(this, value, mask, index);
	}

	//
	// NiAlphaProperty
	//

	AlphaProperty::AlphaProperty() {
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
	// NiStencilProperty
	//


	StencilProperty::StencilProperty() {
		vTable.asProperty = (Property_vTable*)0x746A5C;
		enabled = false;
		testFunc = TEST_GREATER;
		reference = 0;
		mask = UINT_MAX;
		failAction = ACTION_KEEP;
		zFailAction = ACTION_KEEP;
		passAction = ACTION_INCREMENT;
		drawMode = DRAW_CCW_OR_BOTH;
	}

	StencilProperty::~StencilProperty() {

	}

	Pointer<StencilProperty> StencilProperty::create() {
		return new StencilProperty();
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
		vTable->destructor(this, false);
	}

	void TexturingProperty::Map::copyMembers(Map* to) const {
		to->texture = texture;
		to->clampMode = clampMode;
		to->filterMode = filterMode;
		to->texCoordSet = texCoordSet;
	}

	Pointer<Texture> TexturingProperty::Map::getTexture_lua() const {
		return texture;
	}

	void TexturingProperty::Map::setTexture_lua(Texture* t) {
		texture = t;
	}

	bool TexturingProperty::Map::isBasicMap() const {
		return vTable == (const VirtualTable*)0x7465E8;
	}

	bool TexturingProperty::Map::isBumpMap() const {
		return vTable == (const VirtualTable*)0x7507B0;
	}

	bool TexturingProperty::Map::isExtendedMap() const {
		return vTable == &ExtendedMap::ExtendedVirtualTable;
	}

	float TexturingProperty::Map::getPriority() const {
		if (!isExtendedMap()) {
			return DEFAULT_PRIORITY;
		}

		const auto asExtended = static_cast<const ExtendedMap*>(this);
		return asExtended->priority;
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

	void TexturingProperty::BumpMap::copyMembers(BumpMap* to) const {
		Map::copyMembers(to);
		to->lumaScale = lumaScale;
		to->lumaScale = lumaOffset;
		to->bumpMat[0][0] = bumpMat[0][0];
		to->bumpMat[0][1] = bumpMat[0][1];
		to->bumpMat[1][0] = bumpMat[1][0];
		to->bumpMat[1][1] = bumpMat[1][1];
	}

	TexturingProperty::Map::VirtualTable TexturingProperty::ExtendedMap::ExtendedVirtualTable;

	TexturingProperty::ExtendedMap::ExtendedMap() : Map() {
		vTable = &ExtendedVirtualTable;
	}

	TexturingProperty::ExtendedMap::ExtendedMap(Texture* _texture, ClampMode _clampMode, FilterMode _filterMode, unsigned int _textCoords) : Map(_texture, _clampMode, _filterMode, _textCoords) {
		vTable = &ExtendedVirtualTable;
	}

	void TexturingProperty::ExtendedMap::copyMembers(ExtendedMap* to) const {
		Map::copyMembers(to);
		to->priority = priority;
	}

	void TexturingProperty::copyMembers(TexturingProperty* to) const {
		Property::copyMembers(to);

		to->applyMode = applyMode;
		to->decalCount = decalCount;
		to->maps.growToFit(maps.endIndex);

		for (auto i = 0u; i < maps.endIndex; ++i) {
			const auto map = getMap(i);
			if (map == nullptr) continue;

			if (map->isExtendedMap()) {
				const auto newMap = new ExtendedMap();
				static_cast<const ExtendedMap*>(map)->copyMembers(newMap);
				to->maps.setAtIndex(i, newMap);
			}
			else if (map->isBumpMap()) {
				const auto newMap = new BumpMap();
				static_cast<const BumpMap*>(map)->copyMembers(newMap);
				to->maps.setAtIndex(i, newMap);
			}
			else {
				const auto newMap = new Map();
				map->copyMembers(newMap);
				to->maps.setAtIndex(i, newMap);
			}
		}
	}

	TexturingProperty::Map* TexturingProperty::getMap(unsigned int index) const {
		if (index >= maps.endIndex) {
			return nullptr;
		}
		return maps.at(index);
	}

	TexturingProperty::Map* TexturingProperty::getBaseMap() const {
		return getMap(size_t(MapType::BASE));
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

	TexturingProperty::Map* TexturingProperty::getDarkMap() const {
		return getMap(size_t(MapType::DARK));
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

	TexturingProperty::Map* TexturingProperty::getDetailMap() const {
		return getMap(size_t(MapType::DETAIL));
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

	TexturingProperty::Map* TexturingProperty::getGlossMap() const {
		return getMap(size_t(MapType::GLOSS));
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

	TexturingProperty::Map* TexturingProperty::getGlowMap() const {
		return getMap(size_t(MapType::GLOW));
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

	TexturingProperty::BumpMap* TexturingProperty::getBumpMap() const {
		return static_cast<BumpMap*>(getMap(size_t(MapType::BUMP)));
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

	unsigned int TexturingProperty::getUsedMapCount() const {
		unsigned int count = 0;
		for (auto map : maps) {
			if (map && map->texture) {
				count++;
			}
		}
		return count;
	}

	unsigned int TexturingProperty::getUsedNonDecalMapCount() const {
		unsigned int count = 0;
		for (auto i = 0u; i < (unsigned int)MapType::DECAL_FIRST; ++i) {
			const auto& map = maps.at(i);
			if (map && map->texture) count++;
		}
		return count;
	}

	bool TexturingProperty::canAddMap() const {
		return true;
	}

	void TexturingProperty::removeUnsupportedDecals() {
		const auto usedMaps = getUsedMapCount();
		if (usedMaps < MAX_MAP_COUNT) {
			return;
		}

		auto toRemove = usedMaps - MAX_MAP_COUNT;
		for (auto i = maps.endIndex; toRemove > 0; --i) {
			const auto& map = maps.at(i);
			if (map == nullptr) continue;

			delete map;
			maps.erase(i);
			toRemove--;
		}
	}

	unsigned int TexturingProperty::getDecalCount() const {
		return decalCount;
	}

	bool TexturingProperty::canAddDecalMap() const {
		return true;
	}

	unsigned int TexturingProperty::getDecal(const std::string_view& fileName) const {
		for (auto i = 0u; i < maps.endIndex; ++i) {
			const auto map = maps.at(i);
			if (map == nullptr) continue;

			const auto& texture = map->texture;
			if (texture == nullptr) continue;
			if (!texture->isInstanceOfType(RTTIStaticPtr::NiSourceTexture)) continue;

			const auto texName = static_cast<const SourceTexture*>(texture.get())->fileName;
			if (!texName) continue;
			if (mwse::string::iequal(texName, fileName)) return i;
		}
		return (unsigned int)MapType::INVALID;
	}

	unsigned int TexturingProperty::getDecal(Texture* texture) const {
		for (auto i = 0u; i < maps.endIndex; ++i) {
			const auto map = maps.at(i);
			if (map == nullptr) continue;

			if (map->texture == texture) return i;
		}
		return (unsigned int)MapType::INVALID;
	}

	sol::optional<std::tuple<TexturingProperty::Map*, unsigned int>> TexturingProperty::getDecal_lua(sol::stack_object object) const {
		auto index = (unsigned int)MapType::INVALID;
		if (object.is<Texture*>()) {
			index = getDecal(object.as<Texture*>());
		}
		else if (object.is<std::string>()) {
			index = getDecal(object.as<std::string>());
		}

		if (index == (unsigned int)MapType::INVALID) {
			return {};
		}

		return std::make_tuple(getMap(index), index + 1);
	}

	unsigned int TexturingProperty::addDecalMap(Texture* texture, float priority, bool allowDuplicates) {
		// Abort if we already have this decal.
		if (!allowDuplicates) {
			const auto existingTextureIndex = getDecal(texture);
			if (existingTextureIndex != (unsigned int)MapType::INVALID) {
				return existingTextureIndex;
			}
		}

		unsigned int index = (unsigned int)MapType::DECAL_FIRST + getDecalCount();
		maps.growToFit(index);

		auto map = new ExtendedMap(texture);
		map->priority = priority;
		maps.setAtIndex(index, map);
		decalCount++;

		// Sort and compact based on priority.
		sortDecals();
		compactDecals();

		return getDecal(texture);
	}

	sol::optional<std::tuple<TexturingProperty::Map*, unsigned int>> TexturingProperty::addDecalMap_lua(sol::optional<Texture*> texture, sol::optional<float> priority, sol::optional<bool> allowDuplicates) {
		const auto newIndex = addDecalMap(texture.value_or(nullptr), priority.value_or(Map::DEFAULT_PRIORITY), allowDuplicates.value_or(false));
		if (newIndex == (size_t)MapType::INVALID) {
			return {};
		}

		return std::make_tuple(getMap(newIndex), newIndex);
	}

	bool TexturingProperty::removeDecal(unsigned int index) {
		if (index < (unsigned int)MapType::DECAL_FIRST || index > (unsigned int)MapType::DECAL_LAST) {
			throw std::invalid_argument("Invalid map index provided.");
		}

		if (index >= maps.size()) {
			return false;
		}

		const auto existing = maps.at(index);
		if (existing == nullptr) {
			return false;
		}

		delete existing;
		maps.setAtIndex(index, nullptr);
		decalCount--;

		return true;
	}

	bool TexturingProperty::removeDecal_lua(unsigned int index) {
		if (!removeDecal(index - 1)) {
			return false;
		}

		compactDecals();
		return true;
	}

	void TexturingProperty::removeDecals() {
		for (auto i = (unsigned int)MapType::DECAL_FIRST; i < maps.storageCount; ++i) {
			removeDecal(i);
		}
	}

	void TexturingProperty::sortDecals() {
		std::sort(maps.begin() + (unsigned int)MapType::DECAL_FIRST, maps.end(),
			[](const auto& a, const auto& b) {
				const auto aP = a ? a->getPriority() : std::numeric_limits<float>::lowest();
				const auto bP = b ? b->getPriority() : std::numeric_limits<float>::lowest();
				return aP > bP;
			}
		);
	}

	void TexturingProperty::compactDecals() {
		const auto startIndex = (unsigned int)MapType::DECAL_FIRST;
		for (auto i = startIndex, j = startIndex; i < maps.endIndex; ++i) {
			const auto& iV = maps.at(i);
			auto& jV = maps.at(j);
			if (iV == nullptr) continue;
			if (iV != jV) {
				jV = iV;
			}
			j++;
		}
		recalculateDecalCount();
	}

	void TexturingProperty::recalculateDecalCount() {
		decalCount = 0;
		for (auto i = (unsigned int)MapType::DECAL_FIRST; i < maps.storageCount; ++i) {
			if (maps.at(i) == nullptr) {
				break;
			}
			decalCount++;
		}
	}

	//
	// VertexColorProperty
	//

	VertexColorProperty::VertexColorProperty() {
		vTable.asProperty = (Property_vTable*)0x7464F8;
		source = SOURCE_IGNORE;
		lighting = LIGHTING_E_A_D;
	}

	VertexColorProperty::~VertexColorProperty() {
	}

	Pointer<VertexColorProperty> VertexColorProperty::create() {
		return new VertexColorProperty();
	}

	//
	// WireframeProperty
	//

	bool WireframeProperty::getEnabled() const {
		return BITMASK_TEST(flags, WireframePropertyFlags::Enabled);
	}

	void WireframeProperty::setEnabled(bool state) {
		BITMASK_SET(flags, WireframePropertyFlags::Enabled, state);
	}

	//
	// NiZBufferProperty
	//

	ZBufferProperty::ZBufferProperty() {
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
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::WireframeProperty)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::ZBufferProperty)
