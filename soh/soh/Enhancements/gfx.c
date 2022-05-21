#include "gfx.h"

#include "global.h"
#include "z64.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"

extern GlobalContext* gGlobalCtx;
/**
 * Used for selected item animation
 */
const u16 ItemSlotsPos[5][2] = {
    //X,Y
    //Top
    {263, 56},
    //Left
    {261, 67},
    //Right
    {287, 67},
    //Down
    {274, 79},
    //Dpad background
    {270, 64}
};
u16 PosFixEfX = 5;
u16 PosFixEfY = 5;

s32 EquipedEffect(){

    printf("New value should be []\n");
    return 0;
}
/**
 * Simple wrapper to load a texture to be drawn.
 */
void sprite_load(sprite_t* sprite, int type) {
    InterfaceContext* interfaceCtx = &gGlobalCtx->interfaceCtx;
    OPEN_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 12);

    if (type == 1) {        //Navi texture load
        
        if (interfaceCtx->naviCalling >= 1) {
            printf("Nave is calling [%d]\n",interfaceCtx->naviCalling);
            static s16 pointPulseAlpha[2] = {
                { 255 },
                { 0 },
            };
            static s16 pointPulseStage = 1;
            static s16 pointPulseTimer = 5;
            s16 stepA;
            stepA = ABS(sprite->alpha - pointPulseAlpha[pointPulseStage]) / pointPulseTimer;
            if (sprite->alpha >= pointPulseAlpha[pointPulseStage]) {
                sprite->alpha -= stepA;
            } else {
                sprite->alpha += stepA;
            }
            pointPulseTimer--;
            if (pointPulseTimer == 0) {
                sprite->alpha = pointPulseAlpha[pointPulseStage];
                pointPulseStage ^= 1;
                pointPulseTimer = 5;
            }
        } else if (interfaceCtx->naviCalling == 0) {
            sprite->alpha = 70;
        } 
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, sprite->alpha);
        gDPLoadTextureBlock_4b(OVERLAY_DISP++, sprite->tex, G_IM_FMT_IA, 32, 8, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if (type == 2) { //Selected item effect.
        if (DISPLAY_GFX) {
            static s16 pointPulseAlpha[2] = {
                { 180 },
                { 120 },
            };
            static s16 pointPulseStage = 1;
            static s16 pointPulseTimer = 20;
            s16 stepA;
            stepA = ABS(sprite->alpha - pointPulseAlpha[pointPulseStage]) / pointPulseTimer;
            if (sprite->alpha >= pointPulseAlpha[pointPulseStage]) {
                sprite->alpha -= stepA;
            } else {
                sprite->alpha += stepA;
            }
            pointPulseTimer--;
            if (pointPulseTimer == 0) {
                sprite->alpha = pointPulseAlpha[pointPulseStage];
                pointPulseStage ^= 1;
                pointPulseTimer = 20;
            }
        } else {
            sprite->alpha = 70;
        }
        if (CVar_GetS32("gHudColors", 1) == 0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 50, 255, sprite->alpha);
        } else if (CVar_GetS32("gHudColors", 1) == 1) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 0, 255, 50, sprite->alpha);
        } else if (CVar_GetS32("gHudColors", 1) == 2) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, CVar_GetS32("gCCABtnPrimR", 0), CVar_GetS32("gCCABtnPrimG", 255), CVar_GetS32("gCCABtnPrimB", 50), sprite->alpha);
        }
        gDPLoadTextureBlock(OVERLAY_DISP++, sprite->tex, sprite->im_fmt, G_IM_SIZ_8b, sprite->width, sprite->height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if (type == 3) { //DPad Background
        gDPLoadTextureBlock(OVERLAY_DISP++, sprite->tex, sprite->im_fmt, G_IM_SIZ_8b, sprite->width, sprite->height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    } else if (type == 0) { //Anything else.
        gDPLoadTextureBlock(OVERLAY_DISP++, sprite->tex, sprite->im_fmt, G_IM_SIZ_32b, sprite->width, sprite->height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    }
    CLOSE_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 40);
}

/**
 * Simple wrapper to draw a sprite/texture on the screen.
 * type of sprite shader: 
 * 0 items icon 32x32 / 1 : Navi icon / 2 : Selected item effect. / 3DPad texture
 * slot : 0:Top / 1:Left / 2:Right / 3:Down / 4: DPad Background. 
 * grayscale : if true draw in black and white.
 */
void sprite_draw(sprite_t* sprite, int type, int slot, bool grayscale, s16 alpha) {
    InterfaceContext* interfaceCtx = &gGlobalCtx->interfaceCtx;

    sprite_load(sprite, type);

    int height = sprite->height * sprite->scale_h;
    int width = sprite->width * sprite->scale_w;

    int width_factor = (1 << 10) * sprite->width / width;
    int height_factor = (1 << 10) * sprite->height / height;

    OPEN_DISPS(gGlobalCtx->state.gfxCtx, "gfx.c", 51);
    if(grayscale) {
        gsDPSetGrayscaleColor(OVERLAY_DISP++, 109, 109, 109, alpha);
        gsSPGrayscale(OVERLAY_DISP++, true);
    } else {
        gsSPGrayscale(OVERLAY_DISP++, false);
    }
    if (type != 1 && type != 2 ) {
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, alpha);
    }
    s16 PosX = OTRGetRectDimensionFromRightEdge(ItemSlotsPos[slot][0] + CVar_GetS32("gHUDMargin_R", 0));
    s16 PosY = ItemSlotsPos[slot][1];
    if (type == 2) {
        PosX = PosX - PosFixEfX;
        PosY = PosY - PosFixEfY;
    }
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