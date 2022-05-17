#include "debugger.h"
#include "debugSaveEditor.h"
#include "debugMapSelect.h"
#include "colViewer.h"

extern "C" {
void Debug_Init(void) {
    InitSaveEditor();
    InitColViewer();
    InitMapSelect();
}

void Debug_Draw(void) {
    DrawColViewer();
}
}
