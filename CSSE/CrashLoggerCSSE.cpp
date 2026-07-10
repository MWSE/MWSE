#include "CrashLogger.h"

#include "CSBaseObject.h"
#include "CSDataHandler.h"
#include "CSGameFile.h"
#include "CSRecordHandler.h"
#include "DialogRenderWindow.h"
#include "LogUtil.h"

#include "NICamera.h"
#include "NIDX8Renderer.h"
#include "NIObjectNET.h"
#include "NIRTTI.h"

#include "BuildDate.h"

namespace {
	bool IsKnownNiVTable(UINT32 vtable) {
		switch (vtable) {
		case NI::VirtualTableAddress::NiAccumulator:
		case NI::VirtualTableAddress::NiAlphaAccumulator:
		case NI::VirtualTableAddress::NiAlphaController:
		case NI::VirtualTableAddress::NiAlphaProperty:
		case NI::VirtualTableAddress::NiAmbientLight:
		case NI::VirtualTableAddress::NiAutoNormalParticles:
		case NI::VirtualTableAddress::NiAutoNormalParticlesData:
		case NI::VirtualTableAddress::NiAVObject:
		case NI::VirtualTableAddress::NiBillboardNode:
		case NI::VirtualTableAddress::NiBltSource:
		case NI::VirtualTableAddress::NiBSAnimationManager:
		case NI::VirtualTableAddress::NiBSAnimationNode:
		case NI::VirtualTableAddress::BSMirroredNode:
		case NI::VirtualTableAddress::NiBSPArrayController:
		case NI::VirtualTableAddress::NiBSParticleNode:
		case NI::VirtualTableAddress::NiBSPNode:
		case NI::VirtualTableAddress::NiCamera:
		case NI::VirtualTableAddress::NiClusterAccumulator:
		case NI::VirtualTableAddress::NiCollisionSwitch:
		case NI::VirtualTableAddress::NiColorData:
		case NI::VirtualTableAddress::NiDirectionalLight:
		case NI::VirtualTableAddress::NiDitherProperty:
		case NI::VirtualTableAddress::NiDX8Renderer:
		case NI::VirtualTableAddress::NiDynamicEffect:
		case NI::VirtualTableAddress::NiExtraData:
		case NI::VirtualTableAddress::NiFlipController:
		case NI::VirtualTableAddress::NiFloatController:
		case NI::VirtualTableAddress::NiFloatData:
		case NI::VirtualTableAddress::NiFltAnimationNode:
		case NI::VirtualTableAddress::NiFogProperty:
		case NI::VirtualTableAddress::NiGeometry:
		case NI::VirtualTableAddress::NiGeometryData:
		case NI::VirtualTableAddress::NiGeomMorpherController:
		case NI::VirtualTableAddress::NiGravity:
		case NI::VirtualTableAddress::NiKeyframeController:
		case NI::VirtualTableAddress::NiKeyframeData:
		case NI::VirtualTableAddress::NiKeyframeManager:
		case NI::VirtualTableAddress::NiLight:
		case NI::VirtualTableAddress::NiLightColorController:
		case NI::VirtualTableAddress::NiLines:
		case NI::VirtualTableAddress::NiLinesData:
		case NI::VirtualTableAddress::NiLODNode:
		case NI::VirtualTableAddress::NiLookAtController:
		case NI::VirtualTableAddress::NiMaterialColorController:
		case NI::VirtualTableAddress::NiMaterialProperty:
		case NI::VirtualTableAddress::NiMorphData:
		case NI::VirtualTableAddress::NiMorpherController:
		case NI::VirtualTableAddress::NiNode:
		case NI::VirtualTableAddress::NiObject:
		case NI::VirtualTableAddress::NiObjectNET:
		case NI::VirtualTableAddress::NiPalette:
		case NI::VirtualTableAddress::NiParticleBomb:
		case NI::VirtualTableAddress::NiParticleCollider:
		case NI::VirtualTableAddress::NiParticleColorModifier:
		case NI::VirtualTableAddress::NiParticleGrowFade:
		case NI::VirtualTableAddress::NiParticleModifier:
		case NI::VirtualTableAddress::NiParticleRotation:
		case NI::VirtualTableAddress::NiParticles:
		case NI::VirtualTableAddress::NiParticlesData:
		case NI::VirtualTableAddress::NiParticleSystemController:
		case NI::VirtualTableAddress::NiPathController:
		case NI::VirtualTableAddress::NiPixelData:
		case NI::VirtualTableAddress::NiPlanarCollider:
		case NI::VirtualTableAddress::NiPointLight:
		case NI::VirtualTableAddress::NiPosData:
		case NI::VirtualTableAddress::NiProperty:
		case NI::VirtualTableAddress::NiRenderedCubeMap:
		case NI::VirtualTableAddress::NiRenderedTexture:
		case NI::VirtualTableAddress::NiRenderer:
		case NI::VirtualTableAddress::NiRendererSpecificProperty:
		case NI::VirtualTableAddress::NiRollController:
		case NI::VirtualTableAddress::NiRotatingParticles:
		case NI::VirtualTableAddress::NiRotatingParticlesData:
		case NI::VirtualTableAddress::NiScreenPolygon:
		case NI::VirtualTableAddress::NiSequenceStreamHelper:
		case NI::VirtualTableAddress::NiShadeProperty:
		case NI::VirtualTableAddress::NiSkinData:
		case NI::VirtualTableAddress::NiSkinInstance:
		case NI::VirtualTableAddress::NiSkinPartition:
		case NI::VirtualTableAddress::NiSortAdjustNode:
		case NI::VirtualTableAddress::NiSourceTexture:
		case NI::VirtualTableAddress::NiSpecularProperty:
		case NI::VirtualTableAddress::NiSphericalCollider:
		case NI::VirtualTableAddress::NiSpotLight:
		case NI::VirtualTableAddress::NiStencilProperty:
		case NI::VirtualTableAddress::NiStringExtraData:
		case NI::VirtualTableAddress::NiSwitchNode:
		case NI::VirtualTableAddress::NiTextKeyExtraData:
		case NI::VirtualTableAddress::NiTexture:
		case NI::VirtualTableAddress::NiTextureEffect:
		case NI::VirtualTableAddress::NiTexturingProperty:
		case NI::VirtualTableAddress::NiTimeController:
		case NI::VirtualTableAddress::NiTriBasedGeom:
		case NI::VirtualTableAddress::NiTriBasedGeomData:
		case NI::VirtualTableAddress::NiTriShape:
		case NI::VirtualTableAddress::NiTriShapeData:
		case NI::VirtualTableAddress::NiTriShapeDynamicData:
		case NI::VirtualTableAddress::NiTriStrips:
		case NI::VirtualTableAddress::NiTriStripsData:
		case NI::VirtualTableAddress::NiUVController:
		case NI::VirtualTableAddress::NiUVData:
		case NI::VirtualTableAddress::NiVertexColorProperty:
		case NI::VirtualTableAddress::NiVertWeightsExtraData:
		case NI::VirtualTableAddress::NiVisController:
		case NI::VirtualTableAddress::NiVisData:
		case NI::VirtualTableAddress::NiWireframeProperty:
		case NI::VirtualTableAddress::NiZBufferProperty:
			return true;
		default:
			return false;
		}
	}

	const char* GetObjectTypeName(se::cs::ObjectType::ObjectType type) {
		using namespace se::cs::ObjectType;
		switch (type) {
		case Activator: return "Activator";
		case Alchemy: return "Alchemy";
		case Ammo: return "Ammo";
		case AnimationGroup: return "AnimationGroup";
		case Apparatus: return "Apparatus";
		case Armor: return "Armor";
		case Birthsign: return "Birthsign";
		case Bodypart: return "Bodypart";
		case Book: return "Book";
		case Cell: return "Cell";
		case Class: return "Class";
		case Clothing: return "Clothing";
		case Container: return "Container";
		case Creature: return "Creature";
		case CreatureClone: return "CreatureClone";
		case Dialogue: return "Dialogue";
		case DialogueInfo: return "DialogueInfo";
		case Door: return "Door";
		case Enchantment: return "Enchantment";
		case Faction: return "Faction";
		case GameSetting: return "GameSetting";
		case Global: return "GlobalVariable";
		case Ingredient: return "Ingredient";
		case Land: return "Land";
		case LandTexture: return "LandTexture";
		case LeveledCreature: return "LeveledCreature";
		case LeveledItem: return "LeveledItem";
		case Light: return "Light";
		case Lockpick: return "Lockpick";
		case MagicEffect: return "MagicEffect";
		case MagicSourceInstance: return "MagicSourceInstance";
		case Misc: return "Miscellaneous";
		case MobileCreature: return "MobileCreature";
		case MobileNPC: return "MobileNPC";
		case MobileObject: return "MobileObject";
		case MobilePlayer: return "MobilePlayer";
		case MobileProjectile: return "MobileProjectile";
		case MobileSpellProjectile: return "MobileSpellProjectile";
		case NPC: return "NPC";
		case NPCClone: return "NPCClone";
		case PathGrid: return "PathGrid";
		case Probe: return "Probe";
		case Quest: return "Quest";
		case Race: return "Race";
		case Reference: return "Reference";
		case Region: return "Region";
		case Repair: return "RepairTool";
		case Script: return "Script";
		case Skill: return "Skill";
		case Sound: return "Sound";
		case SoundGenerator: return "SoundGenerator";
		case Spell: return "Spell";
		case Static: return "Static";
		case TES3: return "TES3";
		case Training: return "Training";
		case Weapon: return "Weapon";
		default: return nullptr;
		}
	}

	bool IsAddressInConstructionSetImage(const void* address, bool requireExecutable) {
		MEMORY_BASIC_INFORMATION memory = {};
		if (!VirtualQuery(address, &memory, sizeof(memory))) return false;
		if (memory.Type != MEM_IMAGE || memory.AllocationBase != GetModuleHandleA(nullptr)) return false;

		if (!requireExecutable) {
			return (memory.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0;
		}
		return (memory.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0;
	}

	struct RawDescription {
		char type[MAX_PATH];
		char id[MAX_PATH];
		char source[MAX_PATH];
	};

	bool DescribeNiObject(void* pointer, RawDescription& result) {
		__try {
			const auto vtable = *static_cast<UINT32*>(pointer);
			if (!IsKnownNiVTable(vtable)) return false;

			const auto object = static_cast<NI::Object*>(pointer);
			const auto rtti = object->getRunTimeTypeInformation();
			if (!rtti || !rtti->name) return false;
			strncpy_s(result.type, rtti->name, _TRUNCATE);

			for (auto current = rtti; current; current = current->baseRTTI) {
				if (current == reinterpret_cast<NI::RTTI*>(NI::RTTIStaticPtr::NiObjectNET)) {
					const auto name = static_cast<NI::ObjectNET*>(object)->getName();
					if (name) strncpy_s(result.id, name, _TRUNCATE);
					break;
				}
			}
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}

	bool DescribeConstructionSetObject(void* pointer, RawDescription& result) {
		__try {
			const auto object = static_cast<se::cs::BaseObject*>(pointer);
			const auto vtable = object->vtbl.baseObject;
			if (!IsAddressInConstructionSetImage(vtable, false) || !IsAddressInConstructionSetImage(vtable->getObjectID, true)) {
				return false;
			}

			const auto type = GetObjectTypeName(object->objectType);
			if (!type) return false;
			strncpy_s(result.type, type, _TRUNCATE);

			const auto id = object->getObjectID();
			if (id) strncpy_s(result.id, id, _TRUNCATE);
			if (object->sourceFile) strncpy_s(result.source, object->sourceFile->fileName, _TRUNCATE);
			return true;
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			return false;
		}
	}
}

namespace CrashLogger::Version {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
#ifdef APPVEYOR_BUILD_NUMBER
			output << fmt::format("Appveyor Build: {}\n", APPVEYOR_BUILD_NUMBER);
#else
			output << "Appveyor Build: <unavailable>\n";
#endif
			output << fmt::format("Build Date: {}\n", CSSE_BUILD_DATE);
		}
		catch (...) {
			output << "Failed to log version.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Mods {
	static std::stringstream output;

	void Process(EXCEPTION_POINTERS* info) {
		try {
			const auto dataHandler = se::cs::DataHandler::get();
			const auto recordHandler = dataHandler ? dataHandler->recordHandler : nullptr;
			if (!recordHandler || recordHandler->activeModCount <= 0) return;

			output << "File | Active\n";
			const auto count = std::min(recordHandler->activeModCount, 256);
			for (auto index = 0; index < count; ++index) {
				const auto file = recordHandler->activeGameFiles[index];
				if (file) {
					output << file->fileName << " | " << (file == recordHandler->activeFile ? "yes" : "no") << '\n';
				}
			}
		}
		catch (...) {
			output << "Failed to process mods.\n";
		}
	}

	std::stringstream& Get() {
		output.flush();
		return output;
	}
}

namespace CrashLogger::Client {
	std::ostream& GetLog() {
		return se::cs::log::stream;
	}

	std::string GetGPU() {
		const auto renderController = se::cs::dialog::render_window::RenderController::get();
		const auto camera = renderController ? renderController->camera : nullptr;
		const auto renderer = camera ? static_cast<NI::DX8Renderer*>(camera->renderer.get()) : nullptr;
		const auto adapter = renderer ? renderer->getCurrentAdapter() : nullptr;
		return adapter ? adapter->identifier.Description : "<unknown>";
	}

	std::string GetKnownClassName(void* object) {
		RawDescription description = {};
		return DescribeNiObject(object, description) ? description.type : "";
	}

	bool DescribeObject(void* object, std::string& labelName, std::string& objectName, std::string& description) {
		RawDescription raw = {};
		if (DescribeNiObject(object, raw)) {
			labelName = "NiRTTI";
			objectName = raw.type;
			if (raw.id[0]) description = fmt::format("Name: \"{}\"", SanitizeString(raw.id));
			return true;
		}

		if (DescribeConstructionSetObject(object, raw)) {
			labelName = "CS Object";
			objectName = raw.type;
			const auto id = raw.id[0] ? SanitizeString(raw.id) : "<unnamed>";
			const auto source = raw.source[0] ? SanitizeString(raw.source) : "<no source file>";
			description = fmt::format("ID: {} (Plugin: \"{}\")", id, source);
			return true;
		}
		return false;
	}

	bool IsValidVTable(UINT32 vtable) {
		return vtable > 0x670000 && vtable < 0x680000;
	}

	void FillLabels() {
	}
}
