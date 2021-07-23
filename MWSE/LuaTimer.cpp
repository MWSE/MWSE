#include "LuaTimer.h"

#include "LuaManager.h"

#include "LuaUtil.h"

#include "Log.h"

namespace mwse {
	namespace lua {
		//
		// TimeComparer
		//

		bool TimerComparer::operator()(const std::shared_ptr<Timer>& first, double second) {
			return first->timing < second;
		}

		bool TimerComparer::operator()(const std::shared_ptr<Timer>& first, const std::shared_ptr<Timer>& second) {
			return first->timing < second->timing;
		}

		// Single instance to the comparator used for std::upper_bound.
		TimerComparer comparer;

		//
		// TimerController
		//

		TimerController::TimerController(double initialClock) :
			m_Clock(initialClock)
		{

		}

		void TimerController::setClock(double clock) {
			m_Clock = clock;
			update();
		}

		void TimerController::incrementClock(double delta) {
			m_Clock += delta;
			update();
		}

		double TimerController::getClock() const {
			return m_Clock;
		}

		std::shared_ptr<Timer> TimerController::createTimer(double duration, sol::object callback, int iterations, bool persist) {
			// Validate parameters.
			if (callback == sol::nil) {
				throw std::invalid_argument("Could not create timer: Callback function is nil.");
			}
			else if (callback.get_type() != sol::type::function && callback.get_type() != sol::type::string) {
				throw std::invalid_argument("Could not create timer: Callback must be a function or string.");
			}
			else if (duration <= 0.0) {
				throw std::invalid_argument("Could not create timer: Duration must be >= 0.");
			}
			else if (iterations < 0) {
				iterations = 0;
			}

			// Only named timers can persist.
			if (persist && callback.get_type() != sol::type::string) {
				persist = false;
			}

			// Setup the timer structure.
			auto timer = std::make_shared<Timer>();
			timer->controller = weak_from_this();
			timer->duration = duration;
			timer->timing = m_Clock + duration;
			timer->iterations = iterations;
			timer->iterationCount = iterations;
			timer->callback = callback;
			timer->isPersistent = persist;

			// Find the position in the list to add this timer, and add it.
			insertActiveTimer(timer);

			return timer;
		}

		bool TimerController::pauseTimer(std::shared_ptr<Timer> timer) {
			// Validate timer.
			if (timer->state != TimerState::Active) {
				return false;
			}

			// Remove from the active timer list.
			auto result = std::find(m_ActiveTimers.begin(), m_ActiveTimers.end(), timer);
			if (result == m_ActiveTimers.end()) {
				return false;
			}
			m_ActiveTimers.erase(result);

			// And add it to the paused list.
			m_PausedTimers.insert(timer);

			// Update its state.
			timer->state = TimerState::Paused;
			timer->timing = timer->timing - m_Clock;

			return true;
		}

		bool TimerController::resumeTimer(std::shared_ptr<Timer> timer) {
			// Validate timer.
			if (timer->state != TimerState::Paused) {
				return false;
			}

			// Remove from the paused timer list.
			m_PausedTimers.erase(timer);

			// Add to the active list.
			insertActiveTimer(timer);

			timer->state = TimerState::Active;
			timer->timing = timer->timing + m_Clock;

			return true;
		}

		bool TimerController::resetTimer(std::shared_ptr<Timer> timer) {
			// Add to active list
			if (timer->state != TimerState::Active) {
				if (timer->state == TimerState::Paused) {
					m_PausedTimers.erase(timer);
				}
				insertActiveTimer(timer);
			}

			// Reset to initial state.
			timer->state = TimerState::Active;
			timer->timing = m_Clock + timer->duration;
			timer->iterations = timer->iterationCount;

			// Move it to the right place in the list.
			repositionTimer(timer);

			return true;
		}

		bool TimerController::cancelTimer(std::shared_ptr<Timer> timer) {
			TimerState previousState = timer->state;
			timer->state = TimerState::Expired;

			// Remove from the active list.
			if (previousState == TimerState::Active) {
				auto position = std::find(m_ActiveTimers.begin(), m_ActiveTimers.end(), timer);
				if (position == m_ActiveTimers.end()) {
					return false;
				}
				m_ActiveTimers.erase(position);
				return true;
			}

			// Remove from the paused timer list.
			else if (previousState == TimerState::Paused) {
				return m_PausedTimers.erase(timer) == 1;
			}

			return false;
		}

		void TimerController::clearTimers() {
			// Mark all timers as expired.
			for (auto itt = m_ActiveTimers.begin(); itt != m_ActiveTimers.end(); itt++) {
				(*itt)->state = TimerState::Expired;
			}

			// Free the timers from internal storage.
			m_ActiveTimers.clear();
			m_PausedTimers.clear();
		}

		void TimerController::update() {
			// Keep looking at the front timer until it hasn't expired.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			std::shared_ptr<Timer> timer = nullptr;
			while (!m_ActiveTimers.empty() && (timer = m_ActiveTimers.front()) && timer->timing <= m_Clock) {
				// Build data to send to the callback.
				sol::state& state = stateHandle.state;
				sol::table data = state.create_table();

				data["timer"] = timer;

				// Is this a named callback?
				sol::protected_function callback;
				if (timer->callback.get_type() == sol::type::string) {
					auto& callbackName = timer->callback.as<std::string&>();
					callback = getNamedTimer(callbackName);
					if (callback == sol::nil) {
						log::getLog() << "Lua error encountered in timer callback: Named callback \"" << callbackName << "\" is not registered." << std::endl;
						logStackTrace();
						cancelTimer(timer);
						continue;
					}
				}
				else if (timer->callback.get_type() == sol::type::function) {
					callback = timer->callback;
				}

				// Invoke the callback.
				sol::protected_function_result result = callback(data);
				if (!result.valid()) {
					sol::error error = result;
					log::getLog() << "Lua error encountered in timer callback:" << std::endl << error.what() << std::endl;

					// Cancel the timer.
					cancelTimer(timer);
					continue;
				}

				// Decrement iterations if the timer uses them.
				if (timer->iterations > 0) {
					timer->iterations--;

					// If we just hit 0 left, cancel the timer.
					if (timer->iterations == 0) {
						cancelTimer(timer);
						continue;
					}
				}

				// Update timer and reposition it in the vector.
				timer->timing += timer->duration;
				repositionTimer(timer);
			}
		}

		std::vector<std::shared_ptr<Timer>>::iterator TimerController::insertActiveTimer(std::shared_ptr<Timer> timer) {
			auto position = std::upper_bound(m_ActiveTimers.begin(), m_ActiveTimers.end(), timer, comparer);
			return m_ActiveTimers.insert(position, timer);
		}

		void TimerController::repositionTimer(std::shared_ptr<Timer> timer) {
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();

			// Remove from current position.
			auto position = std::find(m_ActiveTimers.begin(), m_ActiveTimers.end(), timer);
			if (position == m_ActiveTimers.end()) {
				return;
			}
			m_ActiveTimers.erase(position);

			// Then insert it back in.
			insertActiveTimer(timer);
		}

		//
		// Timer
		//

		bool Timer::pause() {
			auto sharedController = controller.lock();
			if (sharedController) {
				return sharedController->pauseTimer(shared_from_this());
			}
			return false;
		}

		bool Timer::resume() {
			auto sharedController = controller.lock();
			if (sharedController) {
				return sharedController->resumeTimer(shared_from_this());
			}
			return false;
		}

		bool Timer::reset() {
			auto sharedController = controller.lock();
			if (sharedController) {
				return sharedController->resetTimer(shared_from_this());
			}
			return false;
		}

		bool Timer::cancel() {
			auto sharedController = controller.lock();
			if (sharedController) {
				return sharedController->cancelTimer(shared_from_this());
			}
			return false;
		}

		std::optional<double> Timer::getTimeLeft() const {
			if (state == TimerState::Active) {
				auto sharedController = controller.lock();
				if (sharedController) {
					return timing - sharedController->getClock();
				}
			}
			else if (state == TimerState::Paused) {
				return timing;
			}
			return std::optional<double>();
		}

		sol::table Timer::toTable(sol::this_state ts) const {
			sol::state_view sv = ts;
			sol::table t = sv.create_table();
			t["c"] = LuaManager::getInstance().getTimerControllerType(controller.lock());
			t["s"] = state;
			t["d"] = duration;
			t["t"] = timing;
			t["i"] = iterations;
			t["x"] = callback;
			t["p"] = isPersistent;
			return t;
		}

		std::shared_ptr<Timer> Timer::createFromTable(sol::table t) {
			auto timer = std::make_shared<Timer>();

			timer->controller = LuaManager::getInstance().getTimerController(t["c"]);
			timer->state = t["s"];
			timer->duration = t["d"];
			timer->timing = t["t"];
			timer->iterations = t["i"];
			timer->callback = t["x"];
			timer->isPersistent = t["p"];

			auto controller = timer->controller.lock();

			if (timer->state == TimerState::Active) {
				controller->insertActiveTimer(timer);
			}
			else if (timer->state == TimerState::Paused) {
				controller->m_PausedTimers.insert(timer);
			}

			return timer;
		}

		//
		// Legacy functions, to help people migrate their code to the new method of performing timers.
		//

		// Create and return a timer given a controller and a table of parameters.
		std::shared_ptr<Timer> startTimer(TimerController* controller, sol::table params) {
			// Get the timer variables.
			double duration = getOptionalParam<double>(params, "duration", 0.0);
			int iterations = getOptionalParam<int>(params, "iterations", 1);
			bool persists = getOptionalParam<bool>(params, "persist", true);

			// Allow infinite repeat.
			if (iterations <= 0) {
				iterations = 0;
			}

			// Create the timer.
			return controller->createTimer(duration, params["callback"], iterations, persists);
		}

		// Create a timer, as above, but get the controller from params.type.
		std::shared_ptr<Timer> startTimerAmbiguous(sol::table params) {
			// Get the timer controller we care about.
			unsigned int type = getOptionalParam<unsigned int>(params, "type", unsigned int(TimerType::SimulationTime));
			std::shared_ptr<TimerController> controller = LuaManager::getInstance().getTimerController(static_cast<TimerType>(type));
			if (controller == nullptr) {
				return nullptr;
			}

			// Create the timer.
			return startTimer(controller.get(), params);
		}

		// Directly create a real-time timer.
		std::shared_ptr<Timer> startTimerLegacyRealWithIterations(double duration, sol::object callback, int iterations) {
			std::shared_ptr<TimerController> controller = LuaManager::getInstance().getTimerController(TimerType::RealTime);
			if (controller == nullptr) {
				return nullptr;
			}

			return controller->createTimer(duration, callback, iterations);
		}
		std::shared_ptr<Timer> startTimerLegacyReal(double duration, sol::object callback) {
			return startTimerLegacyRealWithIterations(duration, callback, 1);
		}

		// Directly create a simulation-time timer.
		std::shared_ptr<Timer> startTimerLegacySimulationWithIterations(double duration, sol::object callback, int iterations) {
			std::shared_ptr<TimerController> controller = LuaManager::getInstance().getTimerController(TimerType::SimulationTime);
			if (controller == nullptr) {
				return nullptr;
			}

			return controller->createTimer(duration, callback, iterations);
		}
		std::shared_ptr<Timer> startTimerLegacySimulation(double duration, sol::object callback) {
			return startTimerLegacySimulationWithIterations(duration, callback, 1);
		}

		// Function to pause a given timer.
		bool legacyTimerPause(std::shared_ptr<Timer> timer) {
			return timer->controller.lock()->pauseTimer(timer);
		}

		// Function to resume a given timer.
		bool legacyTimerResume(std::shared_ptr<Timer> timer) {
			return timer->controller.lock()->resumeTimer(timer);
		}

		// Function to reset a given timer.
		bool legacyTimerReset(std::shared_ptr<Timer> timer) {
			return timer->controller.lock()->resetTimer(timer);
		}

		// Function to cancel a given timer.
		bool legacyTimerCancel(std::shared_ptr<Timer> timer) {
			return timer->controller.lock()->cancelTimer(timer);
		}

		// Create a timer that will complete in the next cycle.
		std::shared_ptr<Timer> legacyTimerDelayOneFrame(sol::object callback) {
			std::shared_ptr<TimerController> controller = LuaManager::getInstance().getTimerController(TimerType::RealTime);
			if (controller == nullptr) {
				return nullptr;
			}
			return controller->createTimer(0.0000001, callback, 1);
		}

		// Create a timer that will complete in the next cycle, defaulting to simulation time.
		std::shared_ptr<Timer> legacyTimerDelayOneFrameSpecified(sol::object callback, sol::optional<int> type) {
			std::shared_ptr<TimerController> controller = LuaManager::getInstance().getTimerController(static_cast<TimerType>(type.value_or((int)TimerType::SimulationTime)));
			if (controller == nullptr) {
				return nullptr;
			}
			return controller->createTimer(0.0000001, callback, 1);
		}

		std::unordered_map<std::string, sol::protected_function> namedTimerMap;

		sol::protected_function getNamedTimer(std::string& name) {
			auto itt = namedTimerMap.find(name);
			if (itt != namedTimerMap.end()) {
				return itt->second;
			}
			return sol::nil;
		}

		void setNamedTimer(std::string& name, sol::protected_function fn) {
			namedTimerMap[name] = fn;
		}

		std::shared_ptr<TimerController> createController(sol::optional<double> startTime) {
			return std::make_shared<TimerController>(startTime.value_or(0.0));
		}

		//
		// Lua binding for new data types and functions.
		//

		void bindLuaTimer() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Bind TimerController.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<TimerController>("mwseTimerController");
				usertypeDefinition["new"] = createController;

				// Basic property binding.
				usertypeDefinition["clock"] = sol::property(&TimerController::getClock, &TimerController::setClock);

				// Allow creating timers.
				usertypeDefinition["create"] = startTimer;
			}

			// Bind Timer.
			{
				// Start our usertype. We must finish this with state.set_usertype.
				auto usertypeDefinition = state.new_usertype<Timer>("mwseTimer");
				usertypeDefinition["new"] = sol::no_constructor;

				// Basic property binding.
				usertypeDefinition["duration"] = sol::readonly_property(&Timer::duration);
				usertypeDefinition["iterations"] = sol::readonly_property(&Timer::iterations);
				usertypeDefinition["state"] = sol::readonly_property(&Timer::state);
				usertypeDefinition["timing"] = sol::readonly_property(&Timer::timing);
				usertypeDefinition["callback"] = sol::readonly_property(&Timer::callback);
				usertypeDefinition["timeLeft"] = sol::readonly_property(&Timer::getTimeLeft);

				// Legacy value binding.
				usertypeDefinition["t"] = &Timer::duration;
				usertypeDefinition["c"] = &Timer::callback;
				usertypeDefinition["i"] = &Timer::iterations;
				usertypeDefinition["f"] = &Timer::timing;

				// Basic function binding.
				usertypeDefinition["pause"] = &Timer::pause;
				usertypeDefinition["resume"] = &Timer::resume;
				usertypeDefinition["reset"] = &Timer::reset;
				usertypeDefinition["cancel"] = &Timer::cancel;
			}

			// Create our timer library.
			state["timer"] = state.create_table();

			// Expose timer types.
			state["timer"]["real"] = TimerType::RealTime;
			state["timer"]["simulate"] = TimerType::SimulationTime;
			state["timer"]["game"] = TimerType::GameTime;

			// Expose timer states.
			state["timer"]["active"] = TimerState::Active;
			state["timer"]["paused"] = TimerState::Paused;
			state["timer"]["expired"] = TimerState::Expired;

			// Bind the legacy and new start functions.
			state["timer"]["start"] = sol::overload(&startTimerAmbiguous, &startTimerLegacySimulation, &startTimerLegacySimulationWithIterations);

			// Legacy support for frame timers.
			state["timer"]["frame"] = state.create_table();
			state["timer"]["frame"]["start"] = sol::overload(&startTimerLegacyReal, &startTimerLegacyRealWithIterations);

			// Allow registering named timers.
			state["timer"]["register"] = setNamedTimer;

			// Legacy support for functions.
			state["timer"]["pause"] = &legacyTimerPause;
			state["timer"]["frame"]["pause"] = &legacyTimerPause;
			state["timer"]["resume"] = &legacyTimerResume;
			state["timer"]["frame"]["resume"] = &legacyTimerResume;
			state["timer"]["reset"] = &legacyTimerReset;
			state["timer"]["frame"]["reset"] = &legacyTimerReset;
			state["timer"]["cancel"] = &legacyTimerCancel;
			state["timer"]["frame"]["cancel"] = &legacyTimerCancel;
			state["timer"]["delayOneFrame"] = &legacyTimerDelayOneFrameSpecified;
			state["timer"]["frame"]["delayOneFrame"] = &legacyTimerDelayOneFrame;

			// Let new TimerControllers get made.
			state["timer"]["createController"] = createController;
		}
	}
}
