#include "LuaKeyUpEvent.h"

namespace mwse::lua::event {
	KeyUpEvent::KeyUpEvent(int keyCode, bool controlDown, bool shiftDown, bool altDown, bool superDown) :
		KeyEvent(keyCode, false, controlDown, shiftDown, altDown, superDown)
	{
		m_EventName = "keyUp";
	}
}
