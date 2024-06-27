#include "LuaKeyDownEvent.h"

namespace mwse::lua::event {
	KeyDownEvent::KeyDownEvent(int keyCode, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
		KeyEvent(keyCode, true, controlDown, shiftDown, altDown, superDown)
	{
		m_EventName = "keyDown";
	}
}
