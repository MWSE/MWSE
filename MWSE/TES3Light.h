#pragma once

#include "TES3Defines.h"

#include "TES3Item.h"
#include "TES3IteratedList.h"

namespace TES3 {
	namespace LightFlags {
		typedef unsigned int value_type;

		enum Flag : value_type {
			Dynamic = 0x1,
			CanCarry = 0x2,
			Negative = 0x4,
			Flicker = 0x8,
			Fire = 0x10,
			OffByDefault = 0x20,
			FlickerSlow = 0x40,
			Pulse = 0x80,
			PulseSlow = 0x100
		};
	}

	struct Light : Item {
		void * animationData;
		IteratedList<TES3::BaseObject*> stolenList; // 0x30
		char* name; // 0x48
		Script* script; // 0x4C
		char* model; // 0x50
		char* icon; // 0x54
		float weight; // 0x58
		long value; // 0x5C
		int time; // 0x60
		int radius; // 0x64
		unsigned char color[4]; // 0x68
		unsigned int flags; // 0x6C
		Sound * sound; // 0x70

		static constexpr auto OBJECT_TYPE = ObjectType::Light;

		Light();
		~Light();

		//
		// Custom functions.
		//

		bool getIsDynamic() const;
		void setIsDynamic(bool);

		bool getCanCarry() const;
		void setCanCarry(bool);

		bool getIsNegative() const;
		void setIsNegative(bool);

		bool getFlickers() const;
		void setFlickers(bool);

		bool getIsFire() const;
		void setIsFire(bool);

		bool getIsOffByDefault() const;
		void setIsOffByDefault(bool);

		bool getFlickersSlowly() const;
		void setFlickersSlowly(bool);

		bool getPulses() const;
		void setPulses(bool);

		bool getPulsesSlowly() const;
		void setPulsesSlowly(bool);

		void setIconPath(const char* path);

		std::reference_wrapper<unsigned char[4]> getColor();

		sol::optional<float> getTimeLeft_lua(sol::object object) const;

	};
	static_assert(sizeof(Light) == 0x74, "TES3::Light failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_TES3(TES3::Light)
