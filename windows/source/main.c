#include "ui.h"

int main() {
    MainUiInitialize();
    
    while (1) {
        MainUiDraw();
    }
    
    MainUiCleanup();
    return 0;
}
