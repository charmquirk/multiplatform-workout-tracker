#include "ui.h"
void MainUiInitialize() {
    // Register window class (ONCE at startup)
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ClayApp";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    RegisterClassA(&wc);
    
    // Create window (ONCE at startup)
    g_hwnd = CreateWindowA("ClayApp", "Workout Tracker - Clay UI", 
                          WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                          g_screenWidth, g_screenHeight, NULL, NULL, 
                          GetModuleHandle(NULL), NULL);
    
    if (!g_hwnd) {
        printf("Failed to create window\n");
        return;
    }
    
    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);
    
    // Initialize Clay (ONCE at startup)
    Clay_SetMaxElementCount(1000);
    uint64_t totalMemorySize = Clay_MinMemorySize();
    g_clayMemory = malloc(totalMemorySize);
    
    if (!g_clayMemory) {
        printf("Failed to allocate memory for Clay\n");
        return;
    }
    
    g_arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, g_clayMemory);
    Clay_Initialize(g_arena, (Clay_Dimensions){g_screenWidth, g_screenHeight}, 
                   (Clay_ErrorHandler){HandleClayErrors});
    Clay_SetMeasureTextFunction(MeasureText, 0);
    
    printf("Clay UI Initialized\n");
}

void MainUiCleanup() {
    if (g_clayMemory) {
        free(g_clayMemory);
        g_clayMemory = NULL;
    }
}

void MainUiDraw() {
    // Handle Windows messages
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    // Update Clay with current state
    Clay_SetLayoutDimensions((Clay_Dimensions){g_screenWidth, g_screenHeight});
    Clay_SetPointerState((Clay_Vector2){(float)g_mouseX, (float)g_mouseY}, g_mouseDown);
    Clay_UpdateScrollContainers(true, (Clay_Vector2){0, 0}, 0.016f);
    
    // Build UI
    Clay_BeginLayout();
    
    CLAY((Clay_ElementDeclaration){
        .id = CLAY_ID("MainContainer"),
        .layout = {
            .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)},
            .padding = {.left = 16, .right = 16, .top = 16, .bottom = 16},
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = COLOR_LIGHT
    }) {
        // Header
        CLAY((Clay_ElementDeclaration){
            .id = CLAY_ID("Header"),
            .layout = {
                .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(60)}
            },
            .backgroundColor = COLOR_ORANGE
        }) {
            CLAY_TEXT(CLAY_STRING("Workout Tracker"), &headerTextConfig);
        }
        
        // Content
        CLAY((Clay_ElementDeclaration){
            .id = CLAY_ID("Content"),
            .layout = {
                .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0)},
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .childGap = 8
            }
        }) {
            for (int i = 0; i < 5; i++) {
                CLAY((Clay_ElementDeclaration){
                    .id = CLAY_IDI("WorkoutItem", i),
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50)},
                        .padding = {.left = 12, .right = 12, .top = 8, .bottom = 8}
                    },
                    .backgroundColor = COLOR_WHITE,
                    .border = {.width = {.left = 1, .right = 1, .top = 1, .bottom = 1}, .color = COLOR_GRAY}
                }) {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "Workout Exercise %d", i + 1);
                    CLAY_TEXT(CLAY_STRING(buffer), &itemTextConfig);
                }
            }
        }
    }
    
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    
    // Render to screen
    RenderClayCommands(renderCommands);
    
    Sleep(16); // ~60 FPS
}
