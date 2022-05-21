#include "z_kaleido_scope.h"
#include "SohHooks.h"
#include "textures/parameter_static/parameter_static.h"

/**
 * Generic variables
 */
s16 Kaleido_CurrentPage = 0; //Page the player is currently viewing.

/**
 * Sound effects
 */
s32 CursorMoveSFX = NA_SE_SY_DECIDE;
s32 CloseMenuSFX = NA_SE_SY_DECIDE;
s32 PagesSwitchLeftSFX = NA_SE_SY_WIN_SCROLL_LEFT;
s32 PagesSwitchRightSFX = NA_SE_SY_WIN_SCROLL_RIGHT;

void KaleidoScope_InitVerts(GlobalContext* globalCtx, GraphicsContext* gfxCtx) {
    PauseContext* pauseCtx = &globalCtx->pauseCtx;
    s16 phi_t1;
    s16 phi_t2;
    s16 phi_t2_2;
    s16 phi_t3;
    s16 phi_t4;
    s16 phi_t5;
    pauseCtx->offsetY = 0;

    if ((pauseCtx->state == 4) || (pauseCtx->state >= 0x12) ||
        ((pauseCtx->state == 7) && ((pauseCtx->unk_1EC == 2) || (pauseCtx->unk_1EC == 5))) ||
        ((pauseCtx->state >= 8) && (pauseCtx->state <= 0xD))) {
        pauseCtx->offsetY = 80;
    }
}
void KaleidoScore_CloseMenu(GlobalContext* globalCtx){
    PauseContext* pauseCtx = &globalCtx->pauseCtx;
    pauseCtx->debugState = 0;
    Audio_PlaySoundGeneral(CloseMenuSFX, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    return;
}
void KaleidoScope_DrawNewMenu(GlobalContext* globalCtx) {
    Kaleido_CurrentPage = CVar_GetS32("gNewKaleidoPage", 0);
    PauseContext* pauseCtx = &globalCtx->pauseCtx;
    Input* input = &globalCtx->state.input[0];
    Gfx* gfx;
    Gfx* gfxRef;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_kaleido_new.c", 402);

    func_800944C4_KAL(globalCtx->state.gfxCtx);

    gDPSetRenderMode(POLY_KAL_DISP++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
    gDPSetCombineMode(POLY_KAL_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, 0, 0, 0, 220);
    gDPFillRectangle(POLY_KAL_DISP++, 24, 12, 298, 228);
    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetCombineLERP(POLY_KAL_DISP++, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0,
                      PRIMITIVE, 0);

    gfxRef = POLY_KAL_DISP;
    gfx = Graph_GfxPlusOne(gfxRef);
    gSPDisplayList(OVERLAY_DISP++, gfx);


    gSPEndDisplayList(gfx++);
    Graph_BranchDlist(gfxRef, gfx);
    POLY_KAL_DISP = gfx;

    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, 255, 0, 0, 255);
    gDPSetEnvColor(POLY_KAL_DISP++, 0, 0, 0, 0);


    gDPPipeSync(POLY_KAL_DISP++);
    gDPSetPrimColor(POLY_KAL_DISP++, 0, 0, 255, 255, 255, 255);


    if (CHECK_BTN_ALL(input->press.button, BTN_START)) { //Un pause game and close menu
        KaleidoScore_CloseMenu(globalCtx);
    }

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_kaleido_new.c", 861);
}
