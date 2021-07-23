#include "TES3Statistic.h"

#define TES3_Statistic_getBase 0x401030
#define TES3_Statistic_getCurrent 0x40D3B0
#define TES3_Statistic_getNormalized 0x4012B0
#define TES3_Statistic_modBaseWithCaps 0x401100
#define TES3_Statistic_modCurrentWithCaps 0x401150
#define TES3_Statistic_setBase 0x401240
#define TES3_Statistic_setBaseAndCurrent 0x401290
#define TES3_Statistic_setBaseToCurrent 0x4012A0
#define TES3_Statistic_setCurrentCapped 0x401250

namespace TES3 {
	//
	// Virtual table function wrappers.
	//

	float Statistic::getCurrent() {
		return vTable->getCurrentValue(this);
	}

	//
	// Other related this-call functions.
	//

	float Statistic::getBase() {
		return reinterpret_cast<float(__thiscall *)(Statistic*)>(TES3_Statistic_getBase)(this);
	}

	float Statistic::getCurrentRaw() {
		return reinterpret_cast<float(__thiscall *)(Statistic*)>(TES3_Statistic_getCurrent)(this);
	}

	float Statistic::getNormalized() {
		return reinterpret_cast<float(__thiscall *)(Statistic*)>(TES3_Statistic_getNormalized)(this);
	}

	void Statistic::modBaseCapped(float delta, bool capAt0, bool capAt100) {
		reinterpret_cast<float(__thiscall *)(Statistic*, float, bool, bool)>(TES3_Statistic_modBaseWithCaps)(this, delta, capAt0, capAt100);
	}

	void Statistic::modCurrentCapped(float delta, bool capAt0, bool capAtBase, bool capAt100) {
		reinterpret_cast<float(__thiscall *)(Statistic*, float, bool, bool, bool)>(TES3_Statistic_modCurrentWithCaps)(this, delta, capAt0, capAtBase, capAt100);
	}

	void Statistic::setBase(float value) {
		reinterpret_cast<float(__thiscall *)(Statistic*, float)>(TES3_Statistic_setBase)(this, value);
	}

	void Statistic::setBaseAndCurrent(float value) {
		reinterpret_cast<float(__thiscall *)(Statistic*, float)>(TES3_Statistic_setBaseAndCurrent)(this, value);
	}

	void Statistic::setBaseToCurrent() {
		reinterpret_cast<float(__thiscall *)(Statistic*)>(TES3_Statistic_setBaseToCurrent)(this);
	}

	void Statistic::setCurrentCapped(float value, bool applyCaps) {
		reinterpret_cast<float(__thiscall *)(Statistic*, float, bool)>(TES3_Statistic_setCurrentCapped)(this, value, applyCaps);
	}

	void Statistic::setCurrent_lua(float value) {
		setCurrentCapped(value, false);
	}

	const auto TES3_SkillStatistic_modSkillCapped = reinterpret_cast<void(__thiscall*)(SkillStatistic*, float, bool, bool)>(0x401060);
	void SkillStatistic::modSkillCapped(float delta, bool capAt0, bool capAt100) {
		TES3_SkillStatistic_modSkillCapped(this, delta, capAt0, capAt100);
	}
}