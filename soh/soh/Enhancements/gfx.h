#ifndef GFX_ENHANCEMENT_H
#define GFX_EHHANCEMENT_H

#define ColChanMix(c1, c2, m) (c1 - (s32)(c2 * m)) & 0xFF
#define BLOCK_GFX_DRAW      (0x00000001 | \
	                         0x00000002 | \
                             0x00000080 | \
                             0x00000400 | \
                             0x10000000 | \
                             0x20000000)

#define CAN_DRAW_GFX        (((GET_PLAYER(gGlobalCtx)->stateFlags1 & BLOCK_GFX_DRAW) == 0) && gGlobalCtx->pauseCtx.state == 0 && \
                            ((uint32_t)gGlobalCtx->state.destroy == gGameStateOverlayTable[3].destroy) && \
                            (gSaveContext.gameMode == 0))

#define DISPLAY_GFX         CAN_DRAW_GFX == true

#include "z64.h"

typedef struct {
    void *tex;
    uint16_t width;
    uint16_t height;
    uint8_t im_fmt;
    uint8_t im_siz;
} sprite_t;

void sprite_load(sprite_t *sprite, bool grayscale, int alpha);
void sprite_draw(sprite_t *sprite, int left, int top, int width, int height);

#endif