#include "NativeUIWindowBase.h"
#include <sgecore/BitUtil.h>

namespace SimpleGameEngine {

	void NativeUIWindowBase::onUINativeMouseEvent(UIMouseEvent& ev) {
		using Button = UIMouseEventButton;
		using Type   = UIMouseEventType;

		switch (ev.type) {
			case Type::Down: { BitUtil::set(  _pressedMouseButtons, ev.button); } break;
			case Type::Up:   { BitUtil::unset(_pressedMouseButtons, ev.button); } break;
		}

		ev.pressedButtons = _pressedMouseButtons;

		ev.deltaPos = ev.pos - _mousePos;
		_mousePos = ev.pos;

		onUIMouseEvent(ev);
	}

}