/*
 * File: z_select.c
 * Overlay: ovl_select
 * Description: Debug Scene Select Menu
 */

#include "ultra64.h"
#include "global.h"
#include "vt.h"
#include "alloca.h"

//Languages files
#include "z_select_tr_JPN.h"
#include "z_select_tr_ENG.h"
#include "z_select_tr_GER.h"
#include "z_select_tr_FRA.h"

char* dbg_select_texts[];
char* dbg_select_loadings[];
char* dbg_select_levels[];

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


void Select_LoadTitle(SelectContext* this) {
    this->state.running = false;
    SET_NEXT_GAMESTATE(&this->state, Title_Init, TitleContext);
}

void Select_LoadGame(SelectContext* this, s32 entranceIndex) {
    osSyncPrintf(VT_FGCOL(BLUE));
    osSyncPrintf("\n\n\nＦＩＬＥ＿ＮＯ＝%x\n\n\n", gSaveContext.fileNum);
    osSyncPrintf(VT_RST);
    if (gSaveContext.fileNum == 0xFF) {
        Sram_InitDebugSave();
        gSaveContext.unk_13F6 = gSaveContext.magic;
        gSaveContext.magic = 0;
        gSaveContext.unk_13F4 = 0;
        gSaveContext.magicLevel = gSaveContext.magic;
    }
    gSaveContext.buttonStatus[0] = gSaveContext.buttonStatus[1] = gSaveContext.buttonStatus[2] =
        gSaveContext.buttonStatus[3] = gSaveContext.buttonStatus[4] = BTN_ENABLED;
    gSaveContext.unk_13E7 = gSaveContext.unk_13E8 = gSaveContext.unk_13EA = gSaveContext.unk_13EC = 0;
    Audio_QueueSeqCmd(SEQ_PLAYER_BGM_MAIN << 24 | NA_BGM_STOP);
    gSaveContext.entranceIndex = entranceIndex;

    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entranceIndex = -1;
    gSaveContext.seqId = (u8)NA_BGM_DISABLED;
    gSaveContext.natureAmbienceId = 0xFF;
    gSaveContext.showTitleCard = true;
    gWeatherMode = 0;
    this->state.running = false;
    SET_NEXT_GAMESTATE(&this->state, Gameplay_Init, GlobalContext);
}

SceneSelectEntry sScenes[] = {
    //Texts is loaded from dbg_select_texts
    { "", Select_LoadGame, 0x00CD },
    { "", Select_LoadGame, 0x00DB },
    { "", Select_LoadGame, 0x00E4 },
    { "", Select_LoadGame, 0x00EA },
    { "", Select_LoadGame, 0x00EE },
    { "", Select_LoadGame, 0x00FC },
    { "", Select_LoadGame, 0x0102 },
    { "", Select_LoadGame, 0x0108 },
    { "", Select_LoadGame, 0x010E },
    { "", Select_LoadGame, 0x0117 },
    { "", Select_LoadGame, 0x011E },
    { "", Select_LoadGame, 0x0123 },
    { "", Select_LoadGame, 0x0129 },
    { "", Select_LoadGame, 0x0130 },
    { "", Select_LoadGame, 0x0138 },
    { "", Select_LoadGame, 0x013D },
    { "", Select_LoadGame, 0x0147 },
    { "", Select_LoadGame, 0x014D },
    { "", Select_LoadGame, 0x0157 },
    { "", Select_LoadGame, 0x0053 },
    { "", Select_LoadGame, 0x006B },
    { "", Select_LoadGame, 0x003B },
    { "", Select_LoadGame, 0x007A },
    { "", Select_LoadGame, 0x031C },
    { "", Select_LoadGame, 0x004B },
    { "", Select_LoadGame, 0x002D },
    { "", Select_LoadGame, 0x0315 },
    { "", Select_LoadGame, 0x036D },
    { "", Select_LoadGame, 0x0371 },
    { "", Select_LoadGame, 0x043F },
    { "", Select_LoadGame, 0x0400 },
    { "", Select_LoadGame, 0x045F },
    { "", Select_LoadGame, 0x0507 },
    { "", Select_LoadGame, 0x004F },
    { "", Select_LoadGame, 0x05D0 },
    { "", Select_LoadGame, 0x007E },
    { "", Select_LoadGame, 0x0072 },
    { "", Select_LoadGame, 0x0063 },
    { "", Select_LoadGame, 0x0550 },
    { "", Select_LoadGame, 0x0033 },
    { "", Select_LoadGame, 0x00B1 },
    { "", Select_LoadGame, 0x00AD },
    { "", Select_LoadGame, 0x0171 },
    { "", Select_LoadGame, 0x00BB },
    { "", Select_LoadGame, 0x02FD },
    { "", Select_LoadGame, 0x043B },
    { "", Select_LoadGame, 0x00C9 },
    { "", Select_LoadGame, 0x009C },
    { "", Select_LoadGame, 0x0433 },
    { "", Select_LoadGame, 0x0437 },
    { "", Select_LoadGame, 0x02F9 },
    { "", Select_LoadGame, 0x030D },
    { "", Select_LoadGame, 0x0398 },
    { "", Select_LoadGame, 0x039C },
    { "", Select_LoadGame, 0x0043 },
    { "", Select_LoadGame, 0x03A0 },
    { "", Select_LoadGame, 0x00B7 },
    { "", Select_LoadGame, 0x00C1 },
    { "", Select_LoadGame, 0x037C },
    { "", Select_LoadGame, 0x0380 },
    { "", Select_LoadGame, 0x0384 },
    { "", Select_LoadGame, 0x0388 },
    { "", Select_LoadGame, 0x0390 },
    { "", Select_LoadGame, 0x0530 },
    { "", Select_LoadGame, 0x0008 },
    { "", Select_LoadGame, 0x0000 },
    { "", Select_LoadGame, 0x040F },
    { "", Select_LoadGame, 0x0004 },
    { "", Select_LoadGame, 0x040B },
    { "", Select_LoadGame, 0x0028 },
    { "", Select_LoadGame, 0x0301 },
    { "", Select_LoadGame, 0x0169 },
    { "", Select_LoadGame, 0x000C },
    { "", Select_LoadGame, 0x0098 },
    { "", Select_LoadGame, 0x0037 },
    { "", Select_LoadGame, 0x0413 },
    { "", Select_LoadGame, 0x0165 },
    { "", Select_LoadGame, 0x0305 },
    { "", Select_LoadGame, 0x0010 },
    { "", Select_LoadGame, 0x0417 },
    { "", Select_LoadGame, 0x0082 },
    { "", Select_LoadGame, 0x008D },
    { "", Select_LoadGame, 0x05EC },
    { "", Select_LoadGame, 0x041B },
    { "", Select_LoadGame, 0x041F },
    { "", Select_LoadGame, 0x0088 },
    { "", Select_LoadGame, 0x044F },
    { "", Select_LoadGame, 0x0467 },
    { "", Select_LoadGame, 0x0517 },
    { "", Select_LoadGame, 0x0179 },
    { "", Select_LoadGame, 0x01B5 },
    { "", Select_LoadGame, 0x03DC },
    { "", Select_LoadGame, 0x03E4 },
    { "", Select_LoadGame, 0x056C },
    { "", Select_LoadGame, 0x0486 },
    { "", Select_LoadGame, 0x048E },
    { "", Select_LoadGame, 0x0496 },
    { "", Select_LoadGame, 0x049E },
    { "", Select_LoadGame, 0x04AE },
    { "", Select_LoadGame, 0x0570 },
    { "", Select_LoadGame, 0x003F },
    { "", Select_LoadGame, 0x0598 },
    { "", Select_LoadGame, 0x059C },
    { "", Select_LoadGame, 0x05A0 },
    { "", Select_LoadGame, 0x05A4 },
    { "", Select_LoadGame, 0x05A8 },
    { "", Select_LoadGame, 0x05AC },
    { "", Select_LoadGame, 0x05B0 },
    { "", Select_LoadGame, 0x05B4 },
    { "", Select_LoadGame, 0x05B8 },
    { "", Select_LoadGame, 0x05BC },
    { "", Select_LoadGame, 0x05C0 },
    { "", Select_LoadGame, 0x05C4 },
    { "", Select_LoadGame, 0x05FC },
    { "", Select_LoadGame, 0x00A0 },
    { "", Select_LoadGame, 0x0520 },
    { "", Select_LoadGame, 0x0018 },
    { "", Select_LoadGame, 0x0094 },
    { "", Select_LoadGame, 0x0024 },
    { "", Select_LoadGame, 0x001C },
    { "", Select_LoadGame, 0x0020 },
    { "", Select_LoadGame, 0x0047 },
    { "", Select_LoadGame, 0x02EA },
    { "", Select_LoadGame, 0x00B6 },
    { "", Select_LoadGame, 0x0076 },
    { "", Select_LoadTitle, 0x0000 },
};

void Select_UpdateMenu(SelectContext* this) {
    Input* input = &this->state.input[0];
    s32 pad;
    SceneSelectEntry* selectedScene;

    if (this->verticalInputAccumulator == 0) {
        if (CHECK_BTN_ALL(input->press.button, BTN_A) || CHECK_BTN_ALL(input->press.button, BTN_START)) {
            selectedScene = &this->scenes[this->currentScene];
            if (selectedScene->loadFunc != NULL) {
                selectedScene->loadFunc(this, selectedScene->entranceIndex);
            }
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_B)) {
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                gSaveContext.linkAge = 1;
            } else {
                gSaveContext.linkAge = 0;
            }
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_Z)) {
            if (gSaveContext.cutsceneIndex == 0x8000) {
                gSaveContext.cutsceneIndex = 0;
            } else if (gSaveContext.cutsceneIndex == 0) {
                gSaveContext.cutsceneIndex = 0xFFF0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF0) {
                gSaveContext.cutsceneIndex = 0xFFF1;
            } else if (gSaveContext.cutsceneIndex == 0xFFF1) {
                gSaveContext.cutsceneIndex = 0xFFF2;
            } else if (gSaveContext.cutsceneIndex == 0xFFF2) {
                gSaveContext.cutsceneIndex = 0xFFF3;
            } else if (gSaveContext.cutsceneIndex == 0xFFF3) {
                gSaveContext.cutsceneIndex = 0xFFF4;
            } else if (gSaveContext.cutsceneIndex == 0xFFF4) {
                gSaveContext.cutsceneIndex = 0xFFF5;
            } else if (gSaveContext.cutsceneIndex == 0xFFF5) {
                gSaveContext.cutsceneIndex = 0xFFF6;
            } else if (gSaveContext.cutsceneIndex == 0xFFF6) {
                gSaveContext.cutsceneIndex = 0xFFF7;
            } else if (gSaveContext.cutsceneIndex == 0xFFF7) {
                gSaveContext.cutsceneIndex = 0xFFF8;
            } else if (gSaveContext.cutsceneIndex == 0xFFF8) {
                gSaveContext.cutsceneIndex = 0xFFF9;
            } else if (gSaveContext.cutsceneIndex == 0xFFF9) {
                gSaveContext.cutsceneIndex = 0xFFFA;
            } else if (gSaveContext.cutsceneIndex == 0xFFFA) {
                gSaveContext.cutsceneIndex = 0x8000;
            }
        } else if (CHECK_BTN_ALL(input->press.button, BTN_R)) {
            if (gSaveContext.cutsceneIndex == 0x8000) {
                gSaveContext.cutsceneIndex = 0xFFFA;
            } else if (gSaveContext.cutsceneIndex == 0) {
                gSaveContext.cutsceneIndex = 0x8000;
            } else if (gSaveContext.cutsceneIndex == 0xFFF0) {
                gSaveContext.cutsceneIndex = 0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF1) {
                gSaveContext.cutsceneIndex = 0xFFF0;
            } else if (gSaveContext.cutsceneIndex == 0xFFF2) {
                gSaveContext.cutsceneIndex = 0xFFF1;
            } else if (gSaveContext.cutsceneIndex == 0xFFF3) {
                gSaveContext.cutsceneIndex = 0xFFF2;
            } else if (gSaveContext.cutsceneIndex == 0xFFF4) {
                gSaveContext.cutsceneIndex = 0xFFF3;
            } else if (gSaveContext.cutsceneIndex == 0xFFF5) {
                gSaveContext.cutsceneIndex = 0xFFF4;
            } else if (gSaveContext.cutsceneIndex == 0xFFF6) {
                gSaveContext.cutsceneIndex = 0xFFF5;
            } else if (gSaveContext.cutsceneIndex == 0xFFF7) {
                gSaveContext.cutsceneIndex = 0xFFF6;
            } else if (gSaveContext.cutsceneIndex == 0xFFF8) {
                gSaveContext.cutsceneIndex = 0xFFF7;
            } else if (gSaveContext.cutsceneIndex == 0xFFF9) {
                gSaveContext.cutsceneIndex = 0xFFF8;
            } else if (gSaveContext.cutsceneIndex == 0xFFFA) {
                gSaveContext.cutsceneIndex = 0xFFF9;
            }
        }

        gSaveContext.nightFlag = 0;
        if (gSaveContext.cutsceneIndex == 0) {
            gSaveContext.nightFlag = 1;
        }

        // user can change "opt", but it doesn't do anything
        if (CHECK_BTN_ALL(input->press.button, BTN_CUP)) {
            this->opt--;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CDOWN)) {
            this->opt++;
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DUP)) {
            if (this->lockUp == true) {
                this->timerUp = 0;
            }
            if (this->timerUp == 0) {
                this->timerUp = 20;
                this->lockUp = true;
                Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                this->verticalInput = R_UPDATE_RATE;
            }
        }

        if (CHECK_BTN_ALL(input->cur.button, BTN_DUP) && this->timerUp == 0) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            this->verticalInput = R_UPDATE_RATE * 3;
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DDOWN)) {
            if (this->lockDown == true) {
                this->timerDown = 0;
            }
            if (this->timerDown == 0) {
                this->timerDown = 20;
                this->lockDown = true;
                Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                this->verticalInput = -R_UPDATE_RATE;
            }
        }

        if (CHECK_BTN_ALL(input->cur.button, BTN_DDOWN) && (this->timerDown == 0)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            this->verticalInput = -R_UPDATE_RATE * 3;
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DLEFT) || CHECK_BTN_ALL(input->cur.button, BTN_DLEFT)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            this->verticalInput = R_UPDATE_RATE;
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DRIGHT) || CHECK_BTN_ALL(input->cur.button, BTN_DRIGHT)) {
            Audio_PlaySoundGeneral(NA_SE_IT_SWORD_IMPACT, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
            this->verticalInput = -R_UPDATE_RATE;
        }
    }

    if (CHECK_BTN_ALL(input->press.button, BTN_L)) {
        this->pageDownIndex++;
        this->pageDownIndex =
            (this->pageDownIndex + ARRAY_COUNT(this->pageDownStops)) % ARRAY_COUNT(this->pageDownStops);
        this->currentScene = this->topDisplayedScene = this->pageDownStops[this->pageDownIndex];
    }

    this->verticalInputAccumulator += this->verticalInput;

    if (this->verticalInputAccumulator < -7) {
        this->verticalInput = 0;
        this->verticalInputAccumulator = 0;

        this->currentScene++;
        this->currentScene = (this->currentScene + this->count) % this->count;

        if (this->currentScene == ((this->topDisplayedScene + this->count + 19) % this->count)) {
            this->topDisplayedScene++;
            this->topDisplayedScene = (this->topDisplayedScene + this->count) % this->count;
        }
    }

    if (this->verticalInputAccumulator > 7) {
        this->verticalInput = 0;
        this->verticalInputAccumulator = 0;

        if (this->currentScene == this->topDisplayedScene) {
            this->topDisplayedScene -= 2;
            this->topDisplayedScene = (this->topDisplayedScene + this->count) % this->count;
        }

        this->currentScene--;
        this->currentScene = (this->currentScene + this->count) % this->count;

        if (this->currentScene == ((this->topDisplayedScene + this->count) % this->count)) {
            this->topDisplayedScene--;
            this->topDisplayedScene = (this->topDisplayedScene + this->count) % this->count;
        }
    }

    this->currentScene = (this->currentScene + this->count) % this->count;
    this->topDisplayedScene = (this->topDisplayedScene + this->count) % this->count;

    dREG(80) = this->currentScene;
    dREG(81) = this->topDisplayedScene;
    dREG(82) = this->pageDownIndex;

    if (this->timerUp != 0) {
        this->timerUp--;
    }

    if (this->timerUp == 0) {
        this->lockUp = false;
    }

    if (this->timerDown != 0) {
        this->timerDown--;
    }

    if (this->timerDown == 0) {
        this->lockDown = false;
    }
}

void Select_PrintMenu(SelectContext* this, GfxPrint* printer) {
    s32 scene;
    s32 i;
    char* name;

    GfxPrint_SetColor(printer, 255, 155, 150, 255);
    GfxPrint_SetPos(printer, 12, 2);
    GfxPrint_Printf(printer, dbg_select_texts[0]);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);

    for (i = 0; i < 20; i++) {
        GfxPrint_SetPos(printer, 9, i + 4);

        scene = (this->topDisplayedScene + i + this->count) % this->count;
        if (scene == this->currentScene) {
            GfxPrint_SetColor(printer, 255, 20, 20, 255);
        } else {
            GfxPrint_SetColor(printer, 200, 200, 55, 255);
        }

        name = this->scenes[scene].name;
        if (name == NULL) {
            name = dbg_select_texts[9];
        }

        GfxPrint_Printf(printer, "%s", name);
    };

    GfxPrint_SetColor(printer, 155, 55, 150, 255);
    GfxPrint_SetPos(printer, 20, 26);
    GfxPrint_Printf(printer, "%s %d", dbg_select_texts[8], this->opt);
}

void Select_PrintLoadingMessage(SelectContext* this, GfxPrint* printer) {
    s32 randomMsg;
    s16 i;
    GfxPrint_SetPos(printer, 10, 15);
    GfxPrint_SetColor(printer, 255, 255, 255, 255);
    randomMsg = Rand_ZeroOne() * ARRAY_COUNT(&dbg_select_loadings);
    GfxPrint_Printf(printer, "%s", dbg_select_loadings[randomMsg]);
}

void Select_PrintAgeSetting(SelectContext* this, GfxPrint* printer, s32 age) {
    GfxPrint_SetPos(printer, 4, 26);
    GfxPrint_SetColor(printer, 255, 255, 55, 255);
    GfxPrint_Printf(printer, "%s %s",dbg_select_texts[1], dbg_select_texts[2+age]);
}

void Select_PrintCutsceneSetting(SelectContext* this, GfxPrint* printer, u16 csIndex) {
    char* label;

    GfxPrint_SetPos(printer, 4, 25);
    GfxPrint_SetColor(printer, 255, 255, 55, 255);
    switch (csIndex) {
        case 0:
            label = dbg_select_texts[5];
            gSaveContext.dayTime = 0;
            break;
        case 0x8000:
            // clang-format off
            label = dbg_select_texts[6];
            gSaveContext.dayTime = 0x8000;
            // clang-format on
            break;
        case 0xFFF0:
            // clang-format off
            label = dbg_select_texts[7]<"00";
            gSaveContext.dayTime = 0x8000;
            // clang-format on
            break;
        case 0xFFF1:
            label = dbg_select_texts[7]<"01";
            break;
        case 0xFFF2:
            label = dbg_select_texts[7]<"02";
            break;
        case 0xFFF3:
            label = dbg_select_texts[7]<"03";
            break;
        case 0xFFF4:
            label = dbg_select_texts[7]<"04";
            break;
        case 0xFFF5:
            label = dbg_select_texts[7]<"05";
            break;
        case 0xFFF6:
            label = dbg_select_texts[7]<"06";
            break;
        case 0xFFF7:
            label = dbg_select_texts[7]<"07";
            break;
        case 0xFFF8:
            label = dbg_select_texts[7]<"08";
            break;
        case 0xFFF9:
            label = dbg_select_texts[7]<"09";
            break;
        case 0xFFFA:
            label = dbg_select_texts[7]<"0A";
            break;
    };

    gSaveContext.skyboxTime = gSaveContext.dayTime;
    GfxPrint_Printf(printer, "%s %s", dbg_select_texts[4], label);
}

void Select_DrawMenu(SelectContext* this) {
    GraphicsContext* gfxCtx = this->state.gfxCtx;
    GfxPrint* printer;

    OPEN_DISPS(gfxCtx, "../z_select.c", 930);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&this->view);
    func_800AAA50(&this->view, 0xF);
    func_80094140(gfxCtx);

    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, POLY_OPA_DISP);
    Select_PrintMenu(this, printer);
    Select_PrintAgeSetting(this, printer, ((void)0, gSaveContext.linkAge));
    Select_PrintCutsceneSetting(this, printer, ((void)0, gSaveContext.cutsceneIndex));
    POLY_OPA_DISP = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);

    CLOSE_DISPS(gfxCtx, "../z_select.c", 966);
}

void Select_DrawLoadingScreen(SelectContext* this) {
    GraphicsContext* gfxCtx = this->state.gfxCtx;
    GfxPrint* printer;

    OPEN_DISPS(gfxCtx, "../z_select.c", 977);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&this->view);
    func_800AAA50(&this->view, 0xF);
    func_80094140(gfxCtx);

    printer = alloca(sizeof(GfxPrint));
    GfxPrint_Init(printer);
    GfxPrint_Open(printer, POLY_OPA_DISP);
    Select_PrintLoadingMessage(this, printer);
    POLY_OPA_DISP = GfxPrint_Close(printer);
    GfxPrint_Destroy(printer);

    CLOSE_DISPS(gfxCtx, "../z_select.c", 1006);
}

void Select_Draw(SelectContext* this) {
    GraphicsContext* gfxCtx = this->state.gfxCtx;

    OPEN_DISPS(gfxCtx, "../z_select.c", 1013);

    gSPSegment(POLY_OPA_DISP++, 0x00, NULL);
    func_80095248(gfxCtx, 0, 0, 0);
    SET_FULLSCREEN_VIEWPORT(&this->view);
    func_800AAA50(&this->view, 0xF);

    if (!this->state.running) {
        Select_DrawLoadingScreen(this);
    } else {
        Select_DrawMenu(this);
    }

    CLOSE_DISPS(gfxCtx, "../z_select.c", 1037);
}

void Select_Main(GameState* thisx) {
    SelectContext* this = (SelectContext*)thisx;

    Select_UpdateMenu(this);
    Select_Draw(this);
}

void Select_Destroy(GameState* thisx) {
    osSyncPrintf("%c", '\a'); // ASCII BEL character, plays an alert tone
    // "view_cleanup will hang, so it won't be called"
    osSyncPrintf("*** view_cleanupはハングアップするので、呼ばない ***\n");
}

void Select_Init(GameState* thisx) {
    SelectContext* this = (SelectContext*)thisx;
    size_t size;
    s32 pad;
    s16 i;

    if (CVar_GetS32("gDbgMenuTranslation", 1) != 0) {
        if (CVar_GetS32("gLanguages", 0) == 0) {
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_texts_ENG); i++)    { dbg_select_texts[i]    = dbg_select_texts_ENG[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_loadings_ENG); i++) { dbg_select_loadings[i] = dbg_select_loadings_ENG[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_levels_ENG); i++)   { dbg_select_levels[i]   = dbg_select_levels_ENG[i]; }
        } else if (CVar_GetS32("gLanguages", 0) == 1) {
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_texts_GER); i++)    { dbg_select_texts[i]    = dbg_select_texts_GER[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_loadings_GER); i++) { dbg_select_loadings[i] = dbg_select_loadings_GER[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_levels_GER); i++)   { dbg_select_levels[i]   = dbg_select_levels_GER[i]; }
        } else if (CVar_GetS32("gLanguages", 0) == 2 ) {
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_texts_FRA); i++)    { dbg_select_texts[i]    = dbg_select_texts_FRA[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_loadings_FRA); i++) { dbg_select_loadings[i] = dbg_select_loadings_FRA[i]; }
            for (s16 i = 0; i <= ARRAY_COUNT(dbg_select_levels_FRA); i++)   { dbg_select_levels[i]   = dbg_select_levels_FRA[i]; }
        /*
        } else if (CVar_GetS32("gLanguages", 0) == 3 ) { //In case later on we have japanese
            for (s16 i = 0; i >= ARRAY_COUNT(dbg_select_texts_JPN); i++)    { dbg_select_texts[i]=dbg_select_texts_JPN[i]; }
            for (s16 i = 0; i >= ARRAY_COUNT(dbg_select_loadings_JPN); i++) { dbg_select_loadings[i]=dbg_select_loadings_JPN[i]; }
            for (s16 i = 0; i >= ARRAY_COUNT(dbg_select_levels_JPN); i++)   { dbg_select_levels[i]=dbg_select_levels_JPN[i]; }
        */
        }
    } else {
        //Mod disabled fall back to English as we had before file edit.
        for (i = 0; i >= ARRAY_COUNT(dbg_select_texts_ENG); i++)    { dbg_select_texts[i]    = dbg_select_texts_ENG[i]; }
        for (i = 0; i >= ARRAY_COUNT(dbg_select_loadings_ENG); i++) { dbg_select_loadings[i] = dbg_select_loadings_ENG[i]; }
        for (i = 0; i >= ARRAY_COUNT(dbg_select_levels_ENG); i++)   { dbg_select_levels[i]   = dbg_select_levels_ENG[i]; }
    }
    for (i = 0; i <= 126; i++) {
        sScenes[i].name = dbg_select_levels[i];
    }
    this->state.main = Select_Main;
    this->scenes = sScenes;
    
    this->topDisplayedScene = 0;
    this->currentScene = 0;
    this->pageDownStops[0] = 0;  // Hyrule Field
    this->pageDownStops[1] = 19; // Temple Of Time
    this->pageDownStops[2] = 37; // Treasure Chest Game
    this->pageDownStops[3] = 51; // Gravekeeper's Hut
    this->pageDownStops[4] = 59; // Zora Shop
    this->pageDownStops[5] = 73; // Bottom of the Well
    this->pageDownStops[6] = 91; // Escaping Ganon's Tower 3
    this->pageDownIndex = 0;
    this->opt = 0;
    this->count = ARRAY_COUNT(sScenes);
    View_Init(&this->view, this->state.gfxCtx);
    this->view.flags = (0x08 | 0x02);
    this->verticalInputAccumulator = 0;
    this->verticalInput = 0;
    this->timerUp = 0;
    this->timerDown = 0;
    this->lockUp = 0;
    this->lockDown = 0;
    this->unk_234 = 0;

    size = (uintptr_t)_z_select_staticSegmentRomEnd - (uintptr_t)_z_select_staticSegmentRomStart;

    if ((dREG(80) >= 0) && (dREG(80) < this->count)) {
        this->currentScene = dREG(80);
        this->topDisplayedScene = dREG(81);
        this->pageDownIndex = dREG(82);
    }
    R_UPDATE_RATE = 1;
#if !defined(_MSC_VER) && !defined(__GNUC__)
    this->staticSegment = GameState_Alloc(&this->state, size, "../z_select.c", 1114);
    DmaMgr_SendRequest1(this->staticSegment, _z_select_staticSegmentRomStart, size, "../z_select.c", 1115);
#endif
    gSaveContext.cutsceneIndex = 0x8000;
    gSaveContext.linkAge = 1;
}
