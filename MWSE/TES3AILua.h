#pragma once

namespace mwse {
	namespace lua {
		template <typename T>
		void setUserdataForTES3AIPackage(sol::usertype<T>& usertypeDefinition) {
			// Basic property binding.
			usertypeDefinition["destinationCell"] = &T::destinationCell;
			usertypeDefinition["distance"] = &T::distance;
			usertypeDefinition["duration"] = &T::duration;
			usertypeDefinition["hourOfDay"] = &T::hourOfDay;
			usertypeDefinition["isDone"] = &T::done;
			usertypeDefinition["isFinalized"] = &T::finalized;
			usertypeDefinition["isMoving"] = &T::moving;
			usertypeDefinition["isReset"] = &T::reset;
			usertypeDefinition["isStarted"] = &T::started;
			usertypeDefinition["mobile"] = &T::owningActor;
			usertypeDefinition["startGameHour"] = &T::startGameHour;
			usertypeDefinition["targetActor"] = sol::property(&T::getTargetActor, &T::setTargetActor);
			usertypeDefinition["targetPosition"] = &T::targetPosition;
			usertypeDefinition["type"] = sol::readonly_property(&T::packageType);
		}

		void bindTES3AI();
	}
}
