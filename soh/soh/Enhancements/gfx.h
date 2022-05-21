#ifndef GFX_ENHANCEMENT_H
#define GFX_EHHANCEMENT_H

#define SLOT_UP 0
#define SLOT_LEFT 1
#define SLOT_RIGHT 2
#define SLOT_DOWN 3
#define SLOT_DPAD 4
#define TYPE_ICON 0
#define TYPE_NAVI 1
#define TYPE_EFFECT 2
#define TYPE_DPAD 3

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
                            //((z64_event_state_1 & 0x20) == 0)) // @NOTE Figure out what this relates to? Pretty sure it's a flag for scene transition.
                                                                 // Watched RAM address 0x800EF1B0 in BizHawk and it only ever flipped to 0x20 during
                                                                 // scene transitions from my testing. BLOCK_DPAD should take care of this and this check
                                                                 // may just be redundant in the randomizer repo.

#define DISPLAY_GFX         CAN_DRAW_GFX == true

#include "z64.h"

typedef struct {
    void *tex;
    uint16_t width;
    uint16_t height;
    f32 scale_w;
    f32 scale_h;
    uint8_t im_fmt;
    uint8_t im_siz;
    s16 alpha;
    bool isgreyscale;
    bool isshow;
} sprite_t;

void sprite_draw(sprite_t* sprite, int type, int slot, bool grayscale, s16 alpha);
void sprite_load(sprite_t* sprite, int type);
#endif