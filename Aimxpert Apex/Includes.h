//Basic includes
#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>
#include <tlhelp32.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <math.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <winioctl.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <WinInet.h>
#include <Mmsystem.h>
#include <mciapi.h>
#include <shobjidl_core.h>
#include <direct.h>
#include <urlmon.h>
#include <random>
#include <lmcons.h>
#include <d3d9.h>
#include <xstring>
#include <xmmintrin.h>
#include <immintrin.h>
#include <future>
#include <bcrypt.h>
#include <string>
#include <WinInet.h>
#include <vector>
#include <array>
#include <corecrt_math.h>
#include <TlHelp32.h>
#include <wtypes.h>
#include <conio.h>
#include <shlobj.h>
#include <shlwapi.h> 
#include <shlobj.h>
#include <set>
#include <algorithm> 
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "urlmon.lib")

//Encryption
#include "Encryption/XorCrypt.h"
#include "Encryption/XorStr.h"
#include "Encryption/SpoofCall.h"
#include "Encryption/Li.h"

//Render
#include "ImGui\Snow.hpp"
#include "ImGui\imgui_impl_dx9.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui_internal.h"
#include "ImGui\Fonts.h"

//Screen Width & Height
float Width = GetSystemMetrics(SM_CXSCREEN);
float Height = GetSystemMetrics(SM_CYSCREEN);

//Generate Randomized strings
std::string RandomString(int length) {
	const std::string characters = XorCrypt("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	std::string randomString;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < length; ++i) {
		int randomIndex = std::rand() % characters.length();
		randomString += characters[randomIndex];
	}
	return randomString;
}
