

typedef int (WINAPI* DKEY_DDW)(DWORD);
static DKEY_DDW KeyState;
bool StoreAsyncKeyState()
{
	HMODULE API = GetModuleHandle(XorCryptW(L"win32u.dll"));
	if (API != NULL)
	{
		KeyState = (DKEY_DDW)GetProcAddress(API, XorCrypt("NtUserGetAsyncKeyState"));
		if (KeyState != NULL)
			return true;
		else
			RtlZeroMemory(&KeyState, sizeof(KeyState));
		return false;
	}
}

typedef void (WINAPI* STORE_EVENT)(DWORD, DWORD, DWORD, DWORD, ULONG_PTR);
static STORE_EVENT MouseEvent;
bool StoreMouseEvent()
{
	HMODULE API = GetModuleHandle(XorCryptW(L"user32.dll"));
	if (API != NULL)
	{
		MouseEvent = (STORE_EVENT)GetProcAddress(API, XorCrypt("mouse_event"));
		if (MouseEvent != NULL)
			return true;
		else
			RtlZeroMemory(&MouseEvent, sizeof(MouseEvent));
		return false;
	}

	return false;
}

typedef void (WINAPI* STORE_INPUTEVENT)(UINT, LPINPUT, int);
static STORE_INPUTEVENT MouseInputEvent;
bool StoreMouseInput()
{
	HMODULE API = GetModuleHandle(XorCryptW(L"user32.dll"));
	if (API != NULL)
	{
		MouseInputEvent = (STORE_INPUTEVENT)GetProcAddress(API, XorCrypt("SendInput"));
		if (MouseInputEvent != NULL)
			return true;
		else
			RtlZeroMemory(&MouseInputEvent, sizeof(MouseInputEvent));
		return false;
	}

	return false;
}


class InputManager {
public:
    static int getKeyCode(InputKeyType key) {
        switch (key) {
        case InputKeyType::KEYBOARD_1:
            return 0x31;
        case InputKeyType::KEYBOARD_2:
            return 0x32;
        case InputKeyType::KEYBOARD_3:
            return 0x33;
        case InputKeyType::KEYBOARD_4:
            return 0x34;
        case InputKeyType::KEYBOARD_5:
            return 0x35;
        case InputKeyType::KEYBOARD_6:
            return 0x36;
        case InputKeyType::KEYBOARD_7:
            return 0x37;
        case InputKeyType::KEYBOARD_8:
            return 0x38;
        case InputKeyType::KEYBOARD_9:
            return 0x39;
        case InputKeyType::KEYBOARD_0:
            return 0x30;
        case InputKeyType::KEYBOARD_A:
            return 0x41;
        case InputKeyType::KEYBOARD_B:
            return 0x42;
        case InputKeyType::KEYBOARD_C:
            return 0x43;
        case InputKeyType::KEYBOARD_D:
            return 0x44;
        case InputKeyType::KEYBOARD_E:
            return 0x45;
        case InputKeyType::KEYBOARD_F:
            return 0x46;
        case InputKeyType::KEYBOARD_G:
            return 0x47;
        case InputKeyType::KEYBOARD_H:
            return 0x48;
        case InputKeyType::KEYBOARD_I:
            return 0x49;
        case InputKeyType::KEYBOARD_J:
            return 0x4A;
        case InputKeyType::KEYBOARD_K:
            return 0x4B;
        case InputKeyType::KEYBOARD_L:
            return 0x4C;
        case InputKeyType::KEYBOARD_M:
            return 0x4D;
        case InputKeyType::KEYBOARD_N:
            return 0x4E;
        case InputKeyType::KEYBOARD_O:
            return 0x4F;
        case InputKeyType::KEYBOARD_P:
            return 0x50;
        case InputKeyType::KEYBOARD_Q:
            return 0x51;
        case InputKeyType::KEYBOARD_R:
            return 0x52;
        case InputKeyType::KEYBOARD_S:
            return 0x53;
        case InputKeyType::KEYBOARD_T:
            return 0x54;
        case InputKeyType::KEYBOARD_U:
            return 0x55;
        case InputKeyType::KEYBOARD_V:
            return 0x56;
        case InputKeyType::KEYBOARD_W:
            return 0x57;
        case InputKeyType::KEYBOARD_X:
            return 0x58;
        case InputKeyType::KEYBOARD_Y:
            return 0x59;
        case InputKeyType::KEYBOARD_Z:
            return 0x5A;
        case InputKeyType::KEYBOARD_F1:
            return VK_F1;
        case InputKeyType::KEYBOARD_F2:
            return VK_F2;
        case InputKeyType::KEYBOARD_F3:
            return VK_F3;
        case InputKeyType::KEYBOARD_F4:
            return VK_F4;
        case InputKeyType::KEYBOARD_F5:
            return VK_F5;
        case InputKeyType::KEYBOARD_F6:
            return VK_F6;
        case InputKeyType::KEYBOARD_F7:
            return VK_F7;
        case InputKeyType::KEYBOARD_F8:
            return VK_F8;
        case InputKeyType::KEYBOARD_F9:
            return VK_F9;
        case InputKeyType::KEYBOARD_F10:
            return VK_F10;
        case InputKeyType::KEYBOARD_F11:
            return VK_F11;
        case InputKeyType::KEYBOARD_F12:
            return VK_F12;
        case InputKeyType::KEYBOARD_CAPS_LOCK:
            return VK_CAPITAL;
        case InputKeyType::KEYBOARD_LEFT_SHIFT:
            return VK_LSHIFT;
        case InputKeyType::KEYBOARD_LEFT_CTRL:
            return VK_LCONTROL;
        case InputKeyType::KEYBOARD_LEFT_ALT:
            return VK_LMENU;
        case InputKeyType::KEYBOARD_INSERT:
            return VK_INSERT;
        case InputKeyType::KEYBOARD_DELETE:
            return VK_DELETE;
        case InputKeyType::KEYBOARD_SPACE:
            return VK_SPACE;
        case InputKeyType::MOUSE_Left:
            return VK_LBUTTON;
        case InputKeyType::MOUSE_Right:
            return VK_RBUTTON;
        case InputKeyType::MOUSE_Middle:
            return VK_MBUTTON;
        case InputKeyType::MOUSE_X1:
            return VK_XBUTTON1;
        case InputKeyType::MOUSE_X2:
            return VK_XBUTTON2;
        default:
            return 0;
        }
    }


    static bool isKeyDownOrPress(InputKeyType key) {
        return GetAsyncKeyState(getKeyCode(key)) & 0x8000;
    }

    static bool isKeyDown(InputKeyType key) {
        bool isDown = GetAsyncKeyState(getKeyCode(key)) & 0x8000;
        return isDown;
    }

    static bool isKeyPress(InputKeyType key) {
        static std::unordered_map<int, bool> keyState; 
        bool isDown = GetAsyncKeyState(getKeyCode(key)) & 0x8000;
        bool wasDown = keyState[getKeyCode(key)];
        keyState[getKeyCode(key)] = isDown;
        return isDown && !wasDown;
    }

    static bool isKeyUp(InputKeyType key) {
        return !(GetAsyncKeyState(getKeyCode(key)) & 0x8000);
    }
};
