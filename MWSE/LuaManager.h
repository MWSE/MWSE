#pragma once

#include <unordered_map>
#include <queue>

#include <mutex>

#include "TES3Util.h"

#include "sol.hpp"

#include "LuaBaseEvent.h"

namespace mwse {
	namespace lua {
		typedef std::unordered_map<unsigned long, sol::object> UserdataMap;

		class TimerController;

		enum class TimerType {
			RealTime,
			SimulationTime,
			GameTime
		};

		class LuaManager {
		public:
			// Returns an instance to the singleton.
			static LuaManager& getInstance() {
				return singleton;
			};

			// Returns a reference to the sol2 lua state.
			sol::state& getState();

			sol::table createTable();

			// Uses the MemoryUtil library to create the necessary injections into Morrowind.
			void hook();

			// Performs cleanup to safely detach the DLL.
			void cleanup();

			// Set context for lua scripts.
			TES3::Script* getCurrentScript();
			void setCurrentScript(TES3::Script*);
			TES3::Reference* getCurrentReference();
			void setCurrentReference(TES3::Reference*);

			// Event management.
			sol::object triggerEvent(event::BaseEvent*);
			void triggerBackgroundThreadEvents();

			// Handle our button pressed callbacks. There can only be one at a time.
			void setButtonPressedCallback(sol::optional<sol::protected_function>);
			void triggerButtonPressed();

			// Guarded access to the userdata cache.
			sol::object getCachedUserdata(TES3::BaseObject*);
			sol::object getCachedUserdata(TES3::MobileObject*);
			void insertUserdataIntoCache(TES3::BaseObject*, sol::object);
			void insertUserdataIntoCache(TES3::MobileObject*, sol::object);
			void removeUserdataFromCache(TES3::BaseObject*);
			void removeUserdataFromCache(TES3::MobileObject*);

			// Helper function to execute main.lua scripts recursively in a directory.
			void executeMainModScripts(const char* path, const char* filename = "main.lua");

			// Management functions for timers.
			void updateTimers(float deltaTime, double simulationTimestamp, bool simulating);
			void clearTimers();
			std::shared_ptr<TimerController> getTimerController(TimerType type);

		private:
			LuaManager();

			//
			void bindData();

			// 
			static LuaManager singleton;

			// 
			sol::state luaState;

			//
			TES3::Script* currentScript = NULL;
			TES3::Reference* currentReference = NULL;

			//
			std::mutex backgroundThreadEventsMutex;
			std::queue<event::BaseEvent*> backgroundThreadEvents;

			// Storage for our current button pressed callback.
			sol::protected_function buttonPressedCallback = sol::nil;

			// 
			std::mutex userdataMapMutex;
			UserdataMap userdataCache;

			// Timers.
			std::shared_ptr<TimerController> gameTimers;
			std::shared_ptr<TimerController> simulateTimers;
			std::shared_ptr<TimerController> realTimers;
		};
	}
}
