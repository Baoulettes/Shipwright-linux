#include "debugMapSelect.h"
#include "../../util.h"
#include "../libultraship/SohImGuiImpl.h"
#include "ImGuiHelpers.h"
#include "../translations/debugs_translations.h"
 
#include <array>
#include <bit>
#include <map>
#include <string>

extern "C" {
#include <z64.h>
#include "variables.h"
#include "functions.h"
#include "macros.h"
 
extern GlobalContext* gGlobalCtx;
extern TitleContext* TitleCtxx;
extern SelectContext* Selectxx;
extern GameState* thisx;
}

int SelectedLanguage = 0;
int Stage = 0;
s32 CutsceneIndex[11] = {
    0xFFF0,0xFFF1,0xFFF2,
    0xFFF3,0xFFF4,0xFFF5,
    0xFFF6,0xFFF7,0xFFF8,
    0xFFF9,0xFFFA
};
int Age = 0;
 
char* dbg_select_texts[10];
char* dbg_select_loadings[12];
char* dbg_select_levels[126];
 
extern char* dbg_select_texts_JPN[];
extern char* dbg_select_loadings_JPN[];
extern char* dbg_select_levels_JPN[];
 
extern char* dbg_select_texts_ENG[];
extern char* dbg_select_loadings_ENG[];
extern char* dbg_select_levels_ENG[];
 
extern char* dbg_select_texts_GER[];
extern char* dbg_select_loadings_GER[];
extern char* dbg_select_levels_GER[];
 
extern char* dbg_select_texts_FRA[];
extern char* dbg_select_loadings_FRA[];
extern char* dbg_select_levels_FRA[];
 
s32 sScenes_Entrance[] = {
    0x00CD,0x00DB,0x00E4,0x00EA,0x00EE,0x00FC,0x0102,0x0108,0x010E,0x0117,0x011E,
    0x0123,0x0129,0x0130,0x0138,0x013D,0x0147,0x014D,0x0157,0x0053,0x006B,0x003B,
    0x007A,0x031C,0x004B,0x002D,0x0315,0x036D,0x0371,0x043F,0x0400,0x045F,0x0507,
    0x004F,0x05D0,0x007E,0x0072,0x0063,0x0550,0x0033,0x00B1,0x00AD,0x0171,0x00BB,
    0x02FD,0x043B,0x00C9,0x009C,0x0433,0x0437,0x02F9,0x030D,0x0398,0x039C,0x0043,
    0x03A0,0x00B7,0x00C1,0x037C,0x0380,0x0384,0x0388,0x0390,0x0530,0x0008,0x0000,
    0x040F,0x0004,0x040B,0x0028,0x0301,0x0169,0x000C,0x0098,0x0037,0x0413,0x0165,
    0x0305,0x0010,0x0417,0x0082,0x008D,0x05EC,0x041B,0x041F,0x0088,0x044F,0x0467,
    0x0517,0x0179,0x01B5,0x03DC,0x03E4,0x056C,0x0486,0x048E,0x0496,0x049E,0x04AE,
    0x0570,0x003F,0x0598,0x059C,0x05A0,0x05A4,0x05A8,0x05AC,0x05B0,0x05B4,0x05B8,
    0x05BC,0x05C0,0x05C4,0x05FC,0x00A0,0x0520,0x0018,0x0094,0x0024,0x001C,0x0020,
    0x0047,0x02EA,0x00B6,0x0076,0x0000
};
void LoadLanguages(){
    int i;
    if (SelectedLanguage == 0) {
        for (s16 i = 0; i <= 9; i++)    { dbg_select_texts[i]    = dbg_select_texts_ENG[i]; }
        for (s16 i = 0; i <= 11; i++) { dbg_select_loadings[i] = dbg_select_loadings_ENG[i]; }
        for (s16 i = 0; i <= 125; i++)   { dbg_select_levels[i]   = dbg_select_levels_ENG[i]; }
    } else if (SelectedLanguage == 1) {
        for (s16 i = 0; i <= 9; i++)    { dbg_select_texts[i]    = dbg_select_texts_GER[i]; }
        for (s16 i = 0; i <= 11; i++) { dbg_select_loadings[i] = dbg_select_loadings_GER[i]; }
        for (s16 i = 0; i <= 125; i++)   { dbg_select_levels[i]   = dbg_select_levels_GER[i]; }
    } else if (SelectedLanguage == 2 ) {
        for (s16 i = 0; i <= 9; i++)    { dbg_select_texts[i]    = dbg_select_texts_FRA[i]; }
        for (s16 i = 0; i <= 11; i++) { dbg_select_loadings[i] = dbg_select_loadings_FRA[i]; }
        for (s16 i = 0; i <= 125; i++)   { dbg_select_levels[i]   = dbg_select_levels_FRA[i]; }
    } else if (SelectedLanguage == 3 ) {
        for (s16 i = 0; i >= 9; i++)    { dbg_select_texts[i]=dbg_select_texts_JPN[i]; }
        for (s16 i = 0; i >= 11; i++) { dbg_select_loadings[i]=dbg_select_loadings_JPN[i]; }
        for (s16 i = 0; i >= 125; i++)   { dbg_select_levels[i]=dbg_select_levels_JPN[i]; }
    } else {
        //Mod disabled fall back to English as we had before file edit.
        for (i = 0; i >= 9; i++)    { dbg_select_texts[i]    = dbg_select_texts_ENG[i]; }
        for (i = 0; i >= 11; i++) { dbg_select_loadings[i] = dbg_select_loadings_ENG[i]; }
        for (i = 0; i >= 125; i++)   { dbg_select_levels[i]   = dbg_select_levels_ENG[i]; }
    };
}
void Select_LoadTitle(GameState* thisx) {
    TitleContext* TitleCtx = (TitleContext*)thisx;
    printf("Select_LoadTitle Sram_InitSram\n");
    if(!TitleCtx->state.running){
        Sram_InitSram(&TitleCtx->state, &TitleCtx->sramCtx);
    }
    //Sram_InitSram(&TitleCtx->state, &TitleCtx->sramCtx);
    printf("Select_LoadTitle fileNum\n");
    //gSaveContext.fileNum = 0xFF;
    printf("Select_LoadTitle Sram_OpenSave\n");
    //Sram_OpenSave(&TitleCtx->sramCtx);
    //gSaveContext.gameMode = 0;
    TitleCtx->state.running = false;
    SET_NEXT_GAMESTATE(&TitleCtx->state, Select_Init, SelectContext);
        gSaveContext.respawn[0].entranceIndex = -1;
        gSaveContext.respawnFlag = 0;
        //gSaveContext.seqId = (u8)0xFFFF;
        gSaveContext.natureAmbienceId = 0xFF;
        gSaveContext.showTitleCard = true;
        gSaveContext.dogParams = 0;
        gSaveContext.timer1State = 0;
        gSaveContext.timer2State = 0;
        gSaveContext.eventInf[0] = 0;
        gSaveContext.eventInf[1] = 0;
        gSaveContext.eventInf[2] = 0;
        gSaveContext.eventInf[3] = 0;
        gSaveContext.unk_13EE = 0x32;
        gSaveContext.nayrusLoveTimer = 0;
        gSaveContext.healthAccumulator = 0;
        gSaveContext.unk_13F0 = 0;
        gSaveContext.unk_13F2 = 0;
        gSaveContext.forcedSeqId = 0;
        gSaveContext.skyboxTime = 0;
        gSaveContext.nextTransition = 0xFF;
        //gSaveContext.nextCutsceneIndex = 0xFFEF;
        gSaveContext.cutsceneTrigger = 0;
        gSaveContext.chamberCutsceneNum = 0;
        gSaveContext.nextDayTime = 0xFFFF;
        gSaveContext.unk_13C3 = 0;
        gSaveContext.buttonStatus[0] = gSaveContext.buttonStatus[1] = gSaveContext.buttonStatus[2] = gSaveContext.buttonStatus[3] = gSaveContext.buttonStatus[4] = BTN_ENABLED;
        gSaveContext.unk_13E7 = gSaveContext.unk_13E8 = gSaveContext.unk_13EA = gSaveContext.unk_13EC = gSaveContext.unk_13F4 = 0;
        gSaveContext.unk_13F6 = gSaveContext.magic;
        gSaveContext.magic = 0;
        gSaveContext.magicLevel = gSaveContext.magic;
        gSaveContext.naviTimer = 0;
    printf("Select_LoadTitle should be done\n");
}
void Select_LoadGame(GameState* thisx, s32 entranceIndex) {
    SelectContext* SelectCtx = (SelectContext*)thisx;
    printf("Select_LoadGame \n");
    if (gSaveContext.fileNum == 0xFF) {
        printf("Select_LoadGame - No saved game loaded \n");
        Sram_InitDebugSave();
        gSaveContext.unk_13F6 = gSaveContext.magic;
        gSaveContext.magic = 0;
        gSaveContext.unk_13F4 = 0;
        gSaveContext.magicLevel = gSaveContext.magic;
    }
    gSaveContext.buttonStatus[0] = gSaveContext.buttonStatus[1] = gSaveContext.buttonStatus[2] = gSaveContext.buttonStatus[3] = gSaveContext.buttonStatus[4] = BTN_ENABLED;
    gSaveContext.unk_13E7 = gSaveContext.unk_13E8 = gSaveContext.unk_13EA = gSaveContext.unk_13EC = 0;
    Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | 0x100000FF);
    gSaveContext.entranceIndex = entranceIndex;
 
    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = -1;
    gSaveContext.seqId = (u8)0xFFFF;
    gSaveContext.natureAmbienceId = 0xFF;
    gSaveContext.showTitleCard = true;
    gWeatherMode = 0;
    SelectCtx->state.running = false;
    printf("Select_LoadGame - Gameplay_Init \n");
    SET_NEXT_GAMESTATE(&SelectCtx->state, Gameplay_Init, GlobalContext);
    printf("Select_LoadGame should be done\n");
}
void DrawMapSelect(bool& open, GameState* thisx) {
    if (!open) {
        return;
    }
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(dbg_select_texts[0], &open, ImGuiWindowFlags_NoFocusOnAppearing)) {
        ImGui::End();
        return;
    }
    ImVec2 ScrollListSize = ImGui::GetContentRegionAvail();
    ImGui::BeginListBox("Maps list",ImVec2(ScrollListSize.x,(ScrollListSize.y)-62));
    for (int i = 0; i <= 125 ; i++) {
        ImVec2 ButtonSize = ImGui::GetContentRegionAvail();
        if (ImGui::Button(dbg_select_levels[i],ImVec2(ButtonSize.x,18))) {
            gSaveContext.linkAge = Age;
            gSaveContext.cutsceneIndex = CutsceneIndex[Stage];
            if(i == 125){
                //Select_LoadTitle(thisx);
            } else {
                gSaveContext.entranceIndex = 0x0000;
                //Select_LoadGame(thisx, 0x0000);
            }
        };
    }
    ImGui::EndListBox();
 
    ImGui::Text("Stage:%d",Stage);
    if (ImGui::SliderInt("##", &Stage, 0, 11, "", ImGuiSliderFlags_AlwaysClamp)) {}
 
    ImGui::Text("Age:");
    ImGui::SameLine(0,0);
    if (ImGui::RadioButton(" Child", Age==0)) { Age = 0; }
    ImGui::SameLine(0,0);
    if (ImGui::RadioButton(" Adult", Age==1)) { Age = 1; }
    ImGui::End();
}
void InitMapSelect() {
    SohImGui::AddWindow("Developer Tools", "Zelda Map Select", DrawMapSelect);
    LoadLanguages();
}
