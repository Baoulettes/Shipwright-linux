#include "gfx.h"

#include "global.h"
#include "z64.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"

extern GlobalContext* gGlobalCtx;
/**
 * Used for selected item animation
 */
f32 t;
f32 TweenNavi;
u32 cursorColorR;
u32 cursorColorG;
u32 cursorColorB;
u32 cursorColorA;
u32 cursorColorA_Navi;
f32 cursorAnimTween;
u8 cursorAnimState;
f32 cursorAnimTween_Navi;
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
        t = cursorAnimTween;
        if (cursorAnimState == 0) {
            t += 0.05f;
            if (t >= 1.0f) {
                t = 1.0f;
                cursorAnimState = 1;
            }
        } else {
            t -= 0.05f;
            if (t <= 0.0f) {
                t = 0.0f;
                cursorAnimState = 0;
            }
        }
        if (CVar_GetS32("gHudColors", 1) == 0) {
            cursorColorR = ColChanMix(0, 0.0f, t);
            cursorColorG = ColChanMix(80, 80.0f, t);
            cursorColorB = ColChanMix(255, 0.0f, t);
        } else if (CVar_GetS32("gHudColors", 1) == 1) {
            cursorColorR = ColChanMix(0, 0.0f, t);
            cursorColorG = ColChanMix(255, 80.0f, t);
            cursorColorB = ColChanMix(80, 0.0f, t);
        } else if (CVar_GetS32("gHudColors", 1) == 2) {
            cursorColorR = ColChanMix(CVar_GetS32("gCCABtnPrimR", 90), ((CVar_GetS32("gCCABtnPrimR", 90)/255)*100), t);
            cursorColorG = ColChanMix(CVar_GetS32("gCCABtnPrimG", 90), ((CVar_GetS32("gCCABtnPrimG", 90)/255)*100), t);
            cursorColorB = ColChanMix(CVar_GetS32("gCCABtnPrimB", 90), ((CVar_GetS32("gCCABtnPrimB", 90)/255)*100), t);
        }
        cursorColorA = ColChanMix(255, 0.0f, t);
        cursorAnimTween = t;
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, cursorColorR, cursorColorG, cursorColorB, cursorColorA);
    }
    if (sprite->tex == gNaviCUpENGTex) {
        if (interfaceCtx->naviCalling > 0) {
            TweenNavi += 0.05f;
            if (TweenNavi >= 1.0f) {
                TweenNavi = 1.0f;
            }
        } else {
            TweenNavi -= 0.05f;
            if (TweenNavi <= 0.0f) {
                TweenNavi = 0.0f;
            }
        }
        cursorColorA_Navi = ColChanMix(0, 255.0f, TweenNavi);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, cursorColorA_Navi);
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