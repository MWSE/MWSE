#include "TES3GlobalVariable.h"

namespace TES3 {
	float GlobalVariable::getValue() const {
		switch (tolower(valueType)) {
		case 's': return short(value);
		case 'l': return int(value);
		}
		return value;
	}

	double GlobalVariable::getValue_lua() const {
		switch (tolower(valueType)) {
		case 's': return short(value);
		case 'l': return int(value);
		}
		return value;
	}

	void GlobalVariable::setValue_lua(double v) {
		switch (tolower(valueType)) {
		case 's':
			value = float(short(v));
			break;
		case 'l':
			value = float(int(v));
			break;
		default:
			value = float(v);
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::GlobalVariable)
