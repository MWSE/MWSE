#include "TES3Book.h"

#include "TES3Util.h"

#include "LuaManager.h"

#include "LuaBookGetTextEvent.h"

#include "MemoryUtil.h"
#include "Log.h"

namespace TES3 {
	const auto TES3_Book_ctor = reinterpret_cast<void(__thiscall*)(Book*)>(0x4A1E90);
	Book::Book() {
		TES3_Book_ctor(this);
	}

	const auto TES3_Book_dtor = reinterpret_cast<void(__thiscall*)(Book*)>(0x4A1F70);
	Book::~Book() {
		TES3_Book_dtor(this);
	}

	const auto TES3_Book_loadBookText = reinterpret_cast<const char*(__thiscall*)(const Book*)>(0x4A2A90);
	const char* Book::getBookText() {
		// Allow the event to override the text.
		if (mwse::lua::event::BookGetTextEvent::getEventEnabled()) {
			const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
			sol::object eventResult = stateHandle.triggerEvent(new mwse::lua::event::BookGetTextEvent(this));
			if (eventResult.valid()) {
				sol::table eventData = eventResult;
				sol::optional<const char*> newText = eventData["text"];
				if (newText) {
					// Create our new buffer.
					auto length = strlen(newText.value());
					auto buffer = reinterpret_cast<char*>(mwse::tes3::_new(length + 1));

					// Delete the previous buffer and replace it with this one.
					if (*BOOK_TEXT_CACHE) {
						mwse::tes3::_delete(*BOOK_TEXT_CACHE);
					}
					*BOOK_TEXT_CACHE = buffer;

					// Copy into the buffer and get out of here.
					buffer[length] = '\0';
					strcpy(buffer, newText.value());
					return buffer;
				}
			}
		}

		return TES3_Book_loadBookText(this);
	}

	void Book::setIconPath(const char* path) {
		if (strnlen_s(path, 32) >= 32) {
			throw std::invalid_argument("Path must not be 32 or more characters.");
		}
		mwse::tes3::setDataString(&icon, path);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_TES3(TES3::Book)
