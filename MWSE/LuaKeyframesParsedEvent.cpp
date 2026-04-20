#include "LuaKeyframesParsedEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

namespace mwse::lua::event {
	KeyframesParsedEvent::KeyframesParsedEvent(const char* path, const std::vector<TES3::ParsedTextKeyEntry>& entries) :
		GenericEvent("keyframesParsed"),
		m_Path(path)
	{
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		
		m_Entries = state.create_table();
		for (const auto& entry : entries) {
			auto entry_lua = state.create_table();
			entry_lua["time"] = entry.time;
			entry_lua["key"] = entry.key;
			entry_lua["value"] = entry.value;
			m_Entries.add(entry_lua);
		}
	}

	sol::table KeyframesParsedEvent::createEventTable() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto eventData = state.create_table();

		eventData["path"] = m_Path;
		eventData["textKeys"] = m_Entries;

		return eventData;
	}

	sol::object KeyframesParsedEvent::getEventOptions() {
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();
		auto options = state.create_table();

		options["filter"] = m_Path;

		return options;
	}

	void KeyframesParsedEvent::refillEntryVector(sol::table lEntries, std::vector<TES3::ParsedTextKeyEntry>& cEntries) {
		cEntries.clear();
		cEntries.reserve(lEntries.size());
		for (auto i = 0u; i < lEntries.size(); ++i) {
			sol::table e = lEntries[i + 1];
			sol::optional<float> time = e["time"];
			sol::optional<std::string> key = e["key"];
			sol::optional<std::string> value = e["value"];
			if (!time || !key || !value) {
				// TODO: Error?
				continue;
			}
			cEntries.push_back({ time.value(), key.value(), value.value() });
		}
	}
}
