#include "gfx.h"

#include "global.h"
#include "z64.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"

extern GlobalContext* gGlobalCtx;
/**
 * Used for selected item animation
 */
f32 Cursor_Alpha[] = {0.2f,0.8f};
f32 Cursor_CurA;
f32 CursorFlashSpeed = 1.0f;
/**
 * Simple wrapper to load a texture to be drawn.
 */

void sprite_load(sprite_t* sprite, bool grayscale, int alpha) {
    InterfaceContext* interfaceCtx = &gGlobalCtx->interfaceCtx;
    OPEN_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 12);
    if(grayscale) {
        gsDPSetGrayscaleColor(OVERLAY_DISP++, 109, 109, 109, alpha);
        gsSPGrayscale(OVERLAY_DISP++, true);
    } else {
        gsSPGrayscale(OVERLAY_DISP++, false);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, alpha);
    }
    if (sprite->tex == gMagicArrowEquipEffectTex) {
        u32 sIconPrimColors[1][3] = {
            {CVar_GetS32("gCCABtnPrimR", 0),CVar_GetS32("gCCABtnPrimG", 150),CVar_GetS32("gCCABtnPrimB", 0)},
        };

        if (DISPLAY_GFX || CVar_GetS32("gDPadShortcuts", 0) != 0) {
            if (Cursor_CurA <= Cursor_Alpha[0]) {
                Math_SmoothStepToF(&Cursor_CurA, Cursor_Alpha[1], 1.0f, 0.01f, 0.0f);
            } else if (Cursor_CurA >= Cursor_Alpha[1]) {
                Math_SmoothStepToF(&Cursor_CurA, Cursor_Alpha[0], 1.0f, 0.01f, 0.0f);
            }
        } else {
            Cursor_CurA = 0;
        }

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, sIconPrimColors[0][0], sIconPrimColors[0][1], sIconPrimColors[0][2], Cursor_CurA);
    }
    if (sprite->tex == gNaviCUpENGTex) {
        s16 sNaviIconFlashInitTimer = 10;
        s16 sNaviIconFlashTimer = 10;
        u32 sNaviIconPrimColors[2] = {
            255,
            50,
        };
        s16 sNaviIconFlashColorIdx = 0;
        s16 sNaviIconPrimA = 0;
        s16 primA;
        if (interfaceCtx->naviCalling >= 0) {
            primA = VREG(61);
        } else if (interfaceCtx->naviCalling <= 0) {
            primA = 50;
        } 
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, primA);
    }
    if (sprite->im_siz == G_IM_SIZ_16b) {
        gDPLoadTextureBlock(
            OVERLAY_DISP++,
            sprite->tex,
            sprite->im_fmt,
            G_IM_SIZ_16b, // @TEMP until I figure out how to use sprite->im_siz
            sprite->width, sprite->height,
            0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
            G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
    } else if (sprite->im_siz == G_IM_SIZ_8b) {
        gDPLoadTextureBlock(
            OVERLAY_DISP++,
            sprite->tex,
            sprite->im_fmt,
            G_IM_SIZ_8b,
            sprite->width, sprite->height,
            0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 
            G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
        gDPSetCombineLERP(OVERLAY_DISP++, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE,  0, PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, TEXEL0, 0, PRIMITIVE, 0);
    } else if (sprite->im_siz == G_IM_SIZ_4b) {
        gDPLoadTextureBlock_4b(
            OVERLAY_DISP++, 
            sprite->tex, 
            G_IM_FMT_IA, 
            32, 
            8, 
            0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | 
            G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
    } else {
        gDPLoadTextureBlock(
            OVERLAY_DISP++,
            sprite->tex,
            sprite->im_fmt,
            G_IM_SIZ_32b, // @TEMP until I figure out how to use sprite->im_siz
            sprite->width, sprite->height,
            0,
            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
            G_TX_NOMASK, G_TX_NOMASK,
            G_TX_NOLOD, G_TX_NOLOD
        );
    }

    CLOSE_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 40);
}

/**
 * Simple wrapper to draw a sprite/texture on the screen.
 * sprite_load needs to be ran right before this.
 */
void sprite_draw(sprite_t* sprite, int left, int top, int width, int height) {
    int width_factor = (1 << 10) * sprite->width / width;
    int height_factor = (1 << 10) * sprite->height / height;

    OPEN_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 51);
    left = left + CVar_GetS32("gHUDMargin_R", 0);
    s16 PosX = OTRGetRectDimensionFromRightEdge(left);
    s16 PosY = top;
    if (sprite->im_siz == G_IM_SIZ_4b) {
        gSPWideTextureRectangle(
            OVERLAY_DISP++, 
            PosX << 2, 
            PosY << 2, 
            (PosX + width) << 2, 
            (PosY + height) << 2,
           G_TX_RENDERTILE, 
           0, 0, 
           1 << 10, 
           1 << 10
        );
    } else {
        gSPWideTextureRectangle(
            OVERLAY_DISP++,
            PosX << 2, 
            PosY << 2,
            (PosX + width) << 2, 
            (PosY + height) << 2,
            G_TX_RENDERTILE,
            0, 0,
            width_factor, height_factor
        );
    }
    gsSPGrayscale(OVERLAY_DISP++, false);
    CLOSE_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 62);
}