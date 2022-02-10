#pragma once

#include "Event.h"

#include <GLFW/glfw3.h>

namespace Olala {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode)
		{
		}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, bool isRepeated, int modifyKeys)
			: KeyEvent(keycode), m_IsRepeated(isRepeated), m_ModifyKeys(modifyKeys) {}

		inline bool IsRepeated() const { return m_IsRepeated; }
		inline bool IsShiftPressed() const { return m_ModifyKeys & GLFW_MOD_SHIFT; }
		inline bool IsControlPressed() const { return m_ModifyKeys & GLFW_MOD_CONTROL; }
		inline bool IsAltPressed() const { return m_ModifyKeys & GLFW_MOD_ALT; }
		inline bool IsSuperPressed() const { return m_ModifyKeys & GLFW_MOD_SUPER; }
		inline bool IsCapsLockPressed() const { return m_ModifyKeys & GLFW_MOD_CAPS_LOCK; }
		inline bool IsNumLockPressed() const { return m_ModifyKeys & GLFW_MOD_NUM_LOCK; }

		EVENT_CLASS_TYPE(KeyPressed);
	private:
		bool m_IsRepeated;
		int m_ModifyKeys;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped);
	};

}