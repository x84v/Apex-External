#include "main.h"

void Render() {

	SPOOF_FUNC;
	if (KeyState(VK_INSERT) & 1) {
		Settings::ShowMenu = !Settings::ShowMenu;
	}

	if (KeyState(VK_END) & 1) {
		LI_FN(exit)(2);
	}

	Spoof(ImGui_ImplDX9_NewFrame)();
	Spoof(ImGui_ImplWin32_NewFrame)();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = Settings::ShowMenu;

	if (Settings::ShowMenu == true) {
		Menu();

		SetWindowLongW(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	}
	else {
		//ScreenTools();
		ESP->RenderDrawings(AimAssist, Myself);
		MapRadar->RenderDrawings(Myself);
		AimAssist->Update();
		ESP->Initialize();

		//ImGui::End();

		SetWindowLongW(Overlay.Hwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	}

	ImGui::EndFrame();
	DirectX9.pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	if (DirectX9.pDevice->BeginScene() >= 0) {
		ImGui::Render();
		Spoof(ImGui_ImplDX9_RenderDrawData)(ImGui::GetDrawData());
		DirectX9.pDevice->EndScene();
	}

	HRESULT result = DirectX9.pDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST && DirectX9.pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		Spoof(ImGui_ImplDX9_InvalidateDeviceObjects)();
		DirectX9.pDevice->Reset(&DirectX9.pParameters);
		Spoof(ImGui_ImplDX9_CreateDeviceObjects)();
	}
}

void MainLoop() {
	static RECT OldRect;
	ZeroMemory(&DirectX9.Message, sizeof(MSG));

	ImGuiIO& io = ImGui::GetIO();
	static const ImWchar IconRange[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig FontConfig2;
	ImFontConfig IconConfig;
	ImFontConfig BigFontFT;
	IconConfig.MergeMode = true;
	IconConfig.PixelSnapH = true;
	FontSecondaryB = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 20, &FontConfig2);
	FontSecondaryA = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 17, &FontConfig2);
	FontSecondaryC = io.Fonts->AddFontFromMemoryTTF((void*)SecondaryFont, sizeof(SecondaryFont), 15.5, &FontConfig2);
	ThickFontA = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 24, &FontConfig2);
	ThickFontB = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 18, &FontConfig2);
	ThickFontC = io.Fonts->AddFontFromMemoryTTF((void*)ThickFont, sizeof(ThickFont), 16, &FontConfig2);
	FontMain = io.Fonts->AddFontFromMemoryTTF((void*)MainFont, sizeof(MainFont), 18, &IconConfig, IconRange);
	if (FontMain == NULL || FontSecondaryA == NULL)
	{
		MessageBoxA(0, XorCrypt("Failed to Load Fonts!"), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}


	Style();
	while (DirectX9.Message.message != WM_QUIT) {

		if (PeekMessage(&DirectX9.Message, Overlay.Hwnd, 0, 0, PM_REMOVE)) {
			Spoof(TranslateMessage)(&DirectX9.Message);
			DispatchMessage(&DirectX9.Message);
		}

		HWND TempHwnd = Spoof(GetWindow)(Process.Hwnd, GW_HWNDPREV);
		Spoof(SetWindowPos)(Overlay.Hwnd, TempHwnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		RECT TempRect;
		POINT TempPoint;
		ZeroMemory(&TempRect, sizeof(RECT));
		ZeroMemory(&TempPoint, sizeof(POINT));

		Spoof(GetClientRect)(Process.Hwnd, &TempRect);
		Spoof(ClientToScreen)(Process.Hwnd, &TempPoint);
		TempRect.left = TempPoint.x;
		TempRect.top = TempPoint.y;
		ImGuiIO& io = ImGui::GetIO();
		if (TempRect.left != OldRect.left || TempRect.right != OldRect.right || TempRect.top != OldRect.top || TempRect.bottom != OldRect.bottom) {
			OldRect = TempRect;
			Width = TempRect.right;
			Height = TempRect.bottom;
			DirectX9.pParameters.BackBufferWidth = Width;
			DirectX9.pParameters.BackBufferHeight = Height;
			Spoof(SetWindowPos)(Overlay.Hwnd, (HWND)0, TempPoint.x, TempPoint.y, Width, Height, SWP_NOREDRAW);
			DirectX9.pDevice->Reset(&DirectX9.pParameters);
		}

		Render();
	}

	Spoof(ImGui_ImplDX9_Shutdown)();
	Spoof(ImGui_ImplWin32_Shutdown)();
	ImGui::DestroyContext();
	if (DirectX9.pDevice != NULL) {
		DirectX9.pDevice->EndScene();
		DirectX9.pDevice->Release();
	}
	if (DirectX9.IDirect3D9 != NULL) {
		DirectX9.IDirect3D9->Release();
	}
	Spoof(DestroyWindow)(Overlay.Hwnd);
	UnregisterClass(Overlay.WindowClass.lpszClassName, Overlay.WindowClass.hInstance);
}

int main()
{

	if (!aimxpert::find_driver()) {
		std::cerr << "Driver Failed" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		return 1;
	}

	if (!StoreMouseInput()) {
		MessageBoxA(0, XorCrypt("MouseInput Pointer Failed."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	if (!StoreMouseEvent()) {
		MessageBoxA(0, XorCrypt("MouseEvent Pointer Failed."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	if (!StoreAsyncKeyState()) {
		MessageBoxA(0, XorCrypt("GetAsyncKeyState Pointer Failed."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	aimxpert::process_id = aimxpert::find_process(xorstr_(L"r5apex_dx12.exe"));
	while (aimxpert::process_id == NULL) {
		Sleep(2000);
		aimxpert::process_id = aimxpert::find_process(xorstr_(L"r5apex_dx12.exe"));
		printf("Waiting for Apex!\n");
	}

	aimxpert::fetch_cr3();
	OFF_REGION = aimxpert::find_image();
	if (OFF_REGION == NULL) {
		std::string baseAddressNotFoundMessage = std::string(xorstr_("Base Address Not Found !\n"));
		Sleep(2000);
		return 1;
	}

	Overlay.Name = RandomString(10).c_str();
	if (Overlay.Name == NULL) {
		MessageBoxA(0, XorCrypt("Failed to create Overlay."), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}

	SetupOverlay();
	DirectXInit();

	_beginthread(LocalPlayerThreadRun, 0, NULL);
	_beginthread(GameDataThread, 0, NULL);
	LI_FN(Sleep)(120);

	try {
		srand(static_cast<unsigned>(time(0)));
		HumanPlayers->reserve(70);
		for (int i = 0; i < 70; i++) {
			HumanPlayers->push_back(new Player(i, Myself));
		}

		Dummies->reserve(10000);
		for (int i = 0; i < 10000; i++) {
			Dummies->push_back(new Player(i, Myself));
		}

		MainLoop();
	}
	catch (...) {
		MessageBoxA(0, XorCrypt("!Failed!"), XorCrypt("Error"), MB_ICONERROR);
		LI_FN(Sleep)(3000);
		LI_FN(exit)(2);
	}
}
