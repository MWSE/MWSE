#include "TES3CrimeEvent.h"



namespace TES3 {
	CrimeEvent::CrimeEvent() {
		ctor();
	}

	CrimeEvent::CrimeEvent(const CrimeEvent& event) : CrimeEvent() {
		copy(&event);
	}

	CrimeEvent::~CrimeEvent() {
		dtor();
	}

	const auto TES3_CrimeEvent_ctor = reinterpret_cast<CrimeEvent * (__thiscall*)(CrimeEvent *)>(0x51F250);
	CrimeEvent * CrimeEvent::ctor() {
		return TES3_CrimeEvent_ctor(this);
	}

	const auto TES3_CrimeEvent_dtor = reinterpret_cast<CrimeEvent * (__thiscall*)(CrimeEvent *)>(0x51F580);
	void CrimeEvent::dtor() {
		TES3_CrimeEvent_dtor(this);
	}

	void CrimeEvent::operator=(const CrimeEvent& event) {
		copy(&event);
	}

	const auto TES3_CrimeEvent_copy = reinterpret_cast<CrimeEvent * (__thiscall*)(CrimeEvent *, const CrimeEvent *)>(0x51F3A0);
	void CrimeEvent::copy(const CrimeEvent* crimeEvent) {
		TES3_CrimeEvent_copy(this, crimeEvent);
	}

	const char* CrimeEvent::getBountyKey() const {
		return bountyKey.c_str;
	}

	void CrimeEvent::setBountyKey(const char* key) {
		bountyKey = key;
	}
}
