#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "ArrayUtil.h"

#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Race.h"
#include "TES3Skill.h"

namespace mwse {
	class xGetRace : InstructionInterface_t {
	public:
		xGetRace();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		std::map<long, long> arrayMap;
	};

	static xGetRace xGetRaceInstance;

	xGetRace::xGetRace() : InstructionInterface_t(OpCode::xGetRace) {}

	float xGetRace::execute(VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get the base record.
		TES3::NPCInstance* object = reinterpret_cast<TES3::NPCInstance*>(reference->baseObject);
		if (object == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetRace: No record found for reference." << std::endl;
			}
			return 0.0f;
		}
		else if (object->objectType != TES3::ObjectType::NPC) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetRace: Called on a non-NPC reference." << std::endl;
			}
			return 0.0f;
		}

		// Get the NPC's race.
		TES3::Race* race = object->getRace();
		auto& stack = Stack::getInstance();

		// Get argument: return variable type.
		short returnTypeParam = stack.popShort();

		// Simple case. Just push the race name.
		if (returnTypeParam == 0) {
			stack.pushString(race->name);
			return 0.0f;
		}

		// Complex case, push array(s).
		else if (returnTypeParam == 1) {
			// Get the array index to use from storage, or create it.
			long mainArrayId = 0;
			long skillArrayId = 0;
			long attributeArrayId = 0;
			auto foundArrayId = arrayMap.find((long)race);
			if (foundArrayId != arrayMap.end()) {
				// Array found. Clear the arrays for reuse.
				mainArrayId = foundArrayId->second;
				Arrays::getInstance().clear("xGetRace", mainArrayId);
				skillArrayId = mainArrayId + 1;
				Arrays::getInstance().clear("xGetRace", skillArrayId);
				attributeArrayId = mainArrayId + 1;
				Arrays::getInstance().clear("xGetRace", attributeArrayId);
			}
			else {
				// Arrays not found. Create them.
				mainArrayId = Arrays::getInstance().create("xGetRace");
				skillArrayId = Arrays::getInstance().create("xGetRace");
				attributeArrayId = Arrays::getInstance().create("xGetRace");
				arrayMap[(long)race] = mainArrayId;
			}

			if (mainArrayId != 0) {
				// Create array for skills.
				if (skillArrayId != 0) {
					ContainedArray_t& skillArray = Arrays::getInstance().get(skillArrayId);
					skillArray.push_back(7);
					for (size_t i = 0; i < 7; ++i) {
						if (race->skillBonuses[i].skill != TES3::SkillID::Invalid) {
							skillArray.push_back(race->skillBonuses[i].skill);
							skillArray.push_back(race->skillBonuses[i].bonus);
						}
						else {
							skillArray[0] = i;
						}
					}
				}

				// Create array for attributes.
				if (attributeArrayId != 0) {
					ContainedArray_t& attributeArray = Arrays::getInstance().get(attributeArrayId);
					for (size_t i = 0; i < 8; ++i) {
						attributeArray.push_back(race->baseAttributes[i].male);
						attributeArray.push_back(race->baseAttributes[i].female);
					}
				}

				// Push the above arrays and other values to the result array.
				ContainedArray_t& returnArray = Arrays::getInstance().get(mainArrayId);
				returnArray.push_back(se::string::store::getOrCreate(race->id));
				returnArray.push_back(se::string::store::getOrCreate(race->name));
				returnArray.push_back(skillArrayId);
				returnArray.push_back(attributeArrayId);
				returnArray.push_back(std::bit_cast<long>(race->height.male));
				returnArray.push_back(std::bit_cast<long>(race->height.female));
				returnArray.push_back(std::bit_cast<long>(race->weight.male));
				returnArray.push_back(std::bit_cast<long>(race->weight.female));
				returnArray.push_back(race->flags & 1);
				returnArray.push_back(race->flags >> 1);

				// Push array result
				stack.pushLong(mainArrayId);
				return 0.0f;
			}
		}

		// Invalid return type, or something wrong happened.
		stack.pushLong(false);
		return 0.0f;
	}
}
