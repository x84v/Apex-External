#include <Windows.h>
#include <intrin.h>
#include <TlHelp32.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

uint64_t OFF_REGION;
#define RDWCode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x4756, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SHACode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x3626, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define FGACode CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1347, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define CR3Code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1348, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SecurityCode 0x457c1d6

typedef struct _RD {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} RD, * RDW;

typedef struct _SH {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} SH, * SHA;

typedef struct _FA {
	INT32 security;
	ULONGLONG* address;
} FA, * FGA;

typedef struct _MOUSE_MOVE_REQUEST {
	LONG dx;
	LONG dy;
} MOUSE_MOVE_REQUEST, * PMOUSE_MOVE_REQUEST;

typedef struct _MEMORY_OPERATION_DATA {
	uint32_t pid;
	ULONGLONG* cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;

using mouse_invoke = struct _mouse_invoke {
	uint32_t pid;
	USHORT IndicatorFlags;
	LONG MovementX;
	LONG MovementY;
	ULONG PacketsConsumed;
};
using pmouse_invoke = mouse_invoke*;

namespace aimxpert {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		driver_handle = CreateFileW((L"\\\\.\\{rdbss4241}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;
		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		_RD arguments = { 0 };

		arguments.security = SecurityCode;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, RDWCode, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_RD arguments = { 0 };

		arguments.security = SecurityCode;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, RDWCode, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}


	uintptr_t fetch_cr3() {
		uintptr_t cr3 = NULL;
		_MEMORY_OPERATION_DATA arguments = { 0 };

		arguments.pid = process_id;
		arguments.cr3 = (ULONGLONG*)&cr3;

		DeviceIoControl(driver_handle, CR3Code, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return cr3;
	}

	uintptr_t find_image() {
		uintptr_t image_address = { NULL };
		_SH arguments = { NULL };

		arguments.security = SecurityCode;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, SHACode, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	uintptr_t get_guarded_region() {
		uintptr_t guarded_region_address = { NULL };
		_FA arguments = { NULL };

		arguments.security = SecurityCode;
		arguments.address = (ULONGLONG*)&guarded_region_address;

		DeviceIoControl(driver_handle, FGACode, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return guarded_region_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}

	template <typename T>
	inline T read(uint64_t address, std::string stringRead) {
		SPOOF_FUNC;
		T buffer{ };
		read_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}
	inline bool IsValidPointer(uint64_t Pointer) {
		SPOOF_FUNC;
		if (Pointer <= 0x400000 || Pointer == 0xCCCCCCCCCCCCCCCC || reinterpret_cast<void*>(Pointer) == nullptr || Pointer >
			0x7FFFFFFFFFFFFFFF) {
			return false;
		}
		return true;
	}
	inline void read(uint64_t address, PVOID buffer, DWORD size) {
		SPOOF_FUNC;
		read_physical((PVOID)address, buffer, size);
	}

	inline void Read(uint64_t address, PVOID buffer, DWORD size) {
		SPOOF_FUNC;
		read_physical((PVOID)address, buffer, size);
	}

	template <typename T>
	inline T Read(uint64_t address, const char* whatAreYouReading) {
		SPOOF_FUNC;
		T buffer{ };
		read_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	T read(uint64_t address) {
		SPOOF_FUNC;
		T buffer{ };
		read_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	inline T write(uint64_t address, T buffer) {
		SPOOF_FUNC;
		write_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}
	inline std::string readString(uint64_t address, DWORD size) {
		SPOOF_FUNC;
		char* buffer = new char[size];
		read(address, buffer, size);
		auto str = std::string(buffer);
		delete[] buffer;
		return str;
	}
	inline std::string ReadString(uint64_t address, DWORD size) {
		SPOOF_FUNC;
		char buffer[128] = { 0 }; // Fixed-size buffer
		if (size > 128) size = 128; // Ensure size does not exceed the buffer
		read(address, buffer, size); // Read memory
		return std::string(buffer); // Return as std::string
	}
}

#define xorstr_(str) (str)
const wchar_t* decodeString(const char* encoded) {
	SPOOF_FUNC;
	static std::wstring decoded;
	decoded.clear();
	for (size_t i = 0; encoded[i] != '\0'; ++i) {
		decoded.push_back(encoded[i] ^ 0xAA);  // XOR with a random key for example
	}
	return decoded.c_str();
}

