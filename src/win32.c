/**
 * This is the win32 main file for the gtms testing program.
 * The opening of windows and the main loop is represented here.
 * 
 */
#ifdef _WIN32

static struct FM_File 
win32_open_file(String8 path, u32 access, u32 mode, u32 options, u32 flags) {
	struct FM_File file = {0};
	file.handle = CreateFileA((char*)path.str, access, mode, 0, options, flags, 0);
	file.path = path;
	FILETIME ft = {0};  
	BOOL success = GetFileTime(file.handle, NULL, NULL, &ft);   
	file.info.mod_time =  ft.dwHighDateTime;
	return (file);
}

static void 
win32_close_file(struct FM_File file) {
	CloseHandle(file.handle);
}


LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wParams, LPARAM lParams) {
	LRESULT result; 
	switch(msg) {
	case WM_DESTROY: {
		PostQuitMessage(0);
	}break;

	default: 
		result = DefWindowProc(handle, msg, wParams, lParams); 
	}

	return(result); 
} 


// The string that appears in the application's title bar.
static TCHAR szTitle[] = ("Title of window");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;


int WINAPI 
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
	GLOBAL_BASE_ALLOCATOR = mm_create_malloc_base_memory();
	const char* szWindowClass = "Vulkan window";

	WNDCLASSEX wcex = {0};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WinProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = szWindowClass;
	wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW,
							   szWindowClass,
							   szTitle,
							   WS_OVERLAPPEDWINDOW,
							   CW_USEDEFAULT, CW_USEDEFAULT,
							   800, 600,
							   NULL,
							   NULL,
							   hInstance,
							   NULL);

	if (!hWnd){
		MessageBox(0, "Call to CreateWindow failed!", "Title",0);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	/* Vulkan initialization */
	VulkanInitData vk_init_data = {0};
	VulkanData vk_data = {0};
	vk_init_data.hwnd = hWnd;
	vk_init_data.hinstance = hInstance;
	vk_init_data.argv = &pCmdLine;
	lhvk_init_vulkan(vk_init_data, &vk_data);
	// End of vulkan init
	MSG msg;
	// MAIN LOOP
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if(msg.message == WM_QUIT) {
			break;
		} 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
#endif
