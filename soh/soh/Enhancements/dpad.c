#include "dpad.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/nes_font_static/nes_font_static.h"

// Sprite/texture references.

sprite_t sprDPad = { gMsgCharABControlPadTex, 8, 16, G_IM_FMT_IA, G_IM_SIZ_8b }; 
sprite_t sprFairyOcarina = { gFairyOcarinaIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprOcarinaOfTime = { gOcarinaofTimeIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprNavi = { gNaviCUpENGTex, 8, 32, G_IM_FMT_IA, G_IM_SIZ_4b };

sprite_t sprEquipedEffect = { gMagicArrowEquipEffectTex, 32, 32, G_IM_FMT_IA, G_IM_SIZ_8b };

sprite_t sprBrokenBGSword = { gBrokenBiggoronSwordIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprBGSword = { gBiggoronSwordIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };

sprite_t sprHylianShield = { gHylianShieldIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprMirrorShield = { gMirrorShieldIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };

sprite_t sprGoronTunic = { gGoronTunicIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprZoraTunic = { gZoraTunicIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };

sprite_t sprHoverBoots = { gHoverBootsIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };
sprite_t sprIronBoots = { gIronBootsIconTex, 32, 32, G_IM_FMT_RGBA, G_IM_SIZ_32b };

s32 PageSwitcherBTN = BTN_L;

u16 CurrentPage = 1; //1=Swords/shield,2=tunics,3=boots
u16 MaxPagesCount = 3;
s32 Equi_swords = NA_SE_PL_METALEFFECT_ADULT;
s32 Equi_shields = NA_SE_PL_CHANGE_ARMS;
s32 Equi_tunics = NA_SE_PL_CHANGE_ARMS;
s32 Equi_boots = NA_SE_PL_CHANGE_ARMS;
u16 PosFixEfX = 5;
u16 PosFixEfY = 4;
u16 ItemSlotsPos[5][2] = {
    //X,Y
    //Dpad background
    {270, 64},
    //Top
    {263, 56},
    //Left
    {258, 64},
    //Right
    {284, 64},
    //Down
    {271, 77}
};
u16 ItemIconSize[4][2] = {
    //Dpad
    {20, 20},
    //Navi
    {32, 8},
    //Item
    {16, 16},
    //Effect
    {24, 24}
};
s16 NaviAlpha = 0;
/**
 * @NOTE Need to use <value> - 1 for CHECK_OWNED_EQUIP vs CUR_EQUIP_VALUE (idk, some bitwise stuff I don't understand)
 * So to check if the player has Iron Boots: CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1)
 * Then, to check if Iron Boots are equipped: CUR_EQUIP_VALUE(EQUIP_BOOTS) == 2
 */

/**
 * Handles the DPad shortcuts, such as toggling iron/hover boots
 * and playing the ocarina.
 */
void handle_dpad() {
    Input* p1Input = &gGlobalCtx->state.input[0];
    Player* player = GET_PLAYER(gGlobalCtx);

    if (!CAN_USE_DPAD || !DISPLAY_DPAD || CVar_GetS32("gDPadShortcuts", 0) == 0)
        return;

    // Don't allow toggling from the pause menu if DPad Support is enabled for pause/file select.
    if (gGlobalCtx->pauseCtx.state != 0 && CVar_GetS32("gDpadPauseName", 0) == 1)
        return;

    if ((CHECK_BTN_ALL(p1Input->cur.button, PageSwitcherBTN)) &&
        (CHECK_BTN_ALL(p1Input->press.button, BTN_DLEFT))) {
        if (CurrentPage==1) { 
            CurrentPage = MaxPagesCount; 
        } else {
            CurrentPage--;
        }
    }
    if ((CHECK_BTN_ALL(p1Input->cur.button, PageSwitcherBTN)) &&
        (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT))) {
        if (CurrentPage == MaxPagesCount) {
            CurrentPage = 1;
        } else {
            CurrentPage++;
        }
    }
    if (!CHECK_BTN_ALL(p1Input->cur.button, PageSwitcherBTN)) {
        if (CurrentPage == 1) {
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DLEFT)) {
                    // Toggle Biggoron or knife
                    if (CUR_EQUIP_VALUE(EQUIP_SWORD) != 2) {
                        Inventory_ChangeEquipment(EQUIP_SWORD, 2);
                        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_MASTER;
                        Audio_PlaySoundGeneral(Equi_swords, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                    } else {
                        if (gSaveContext.bgsFlag == 1) {
                            Inventory_ChangeEquipment(EQUIP_SWORD, PLAYER_SWORD_BGS);
                            gSaveContext.equips.buttonItems[0] = ITEM_SWORD_BGS;
                            Audio_PlaySoundGeneral(Equi_swords, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                        } else if (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 0 && gSaveContext.swordHealth > 0) {
                            Inventory_ChangeEquipment(EQUIP_SWORD, 3);
                            gSaveContext.equips.buttonItems[0] = ITEM_SWORD_BGS;
                            Audio_PlaySoundGeneral(Equi_swords, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                        } else if (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 8) {
                            Inventory_ChangeEquipment(EQUIP_SWORD, 3);
                            gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KNIFE;
                            Audio_PlaySoundGeneral(Equi_swords, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                        }
                    }
                }
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT)) {
                    // Toggle Mirror Shield
                    if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT) && 
                        CHECK_OWNED_EQUIP(EQUIP_SHIELD, 2)) {
                        Inventory_ChangeEquipment(EQUIP_SHIELD, CUR_EQUIP_VALUE(EQUIP_SHIELD) != 3 ? 3 : 2);
                        Player_SetEquipmentData(&gGlobalCtx, player);
                        Audio_PlaySoundGeneral(Equi_shields, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                    }
                }
            } else {
                //Equip mask there on left


                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT)) {
                    // Toggle Hylian shield
                    if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT) && CHECK_OWNED_EQUIP(EQUIP_SHIELD, 1)) {
                        if (!CHECK_OWNED_EQUIP(EQUIP_SHIELD, 1)) {
                            //Check if play has burned Deku shield or not.
                            Inventory_ChangeEquipment(EQUIP_SHIELD, CUR_EQUIP_VALUE(EQUIP_SHIELD) != 2 ? 2 : 0);
                        } else {
                            Inventory_ChangeEquipment(EQUIP_SHIELD, CUR_EQUIP_VALUE(EQUIP_SHIELD) != 2 ? 2 : 1);
                        }
                        Player_SetEquipmentData(&gGlobalCtx, player);
                        Audio_PlaySoundGeneral(Equi_shields, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                    }
                }
            }
        } else if (CurrentPage == 2) {
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                // Toggle Goron Tunic.
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DLEFT) && 
                    CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                    Inventory_ChangeEquipment(EQUIP_TUNIC, CUR_EQUIP_VALUE(EQUIP_TUNIC) != 2 ? 2 : 1);
                    Player_SetEquipmentData(&gGlobalCtx, player);
                    Audio_PlaySoundGeneral(Equi_tunics, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                }
                // Toggle Zora Tunic.
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT) && 
                    CHECK_OWNED_EQUIP(EQUIP_TUNIC, 2)) {
                    Inventory_ChangeEquipment(EQUIP_TUNIC, CUR_EQUIP_VALUE(EQUIP_TUNIC) != 3 ? 3 : 1);
                    Player_SetEquipmentData(&gGlobalCtx, player);
                    Audio_PlaySoundGeneral(Equi_tunics, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                }
            }
        } else if (CurrentPage == 3) {
            if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
                // Toggle Iron Boots.
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DLEFT) && 
                    CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1)) {
                    Inventory_ChangeEquipment(EQUIP_BOOTS, CUR_EQUIP_VALUE(EQUIP_BOOTS) != 2 ? 2 : 1);
                    Player_SetEquipmentData(&gGlobalCtx, player);
                    Audio_PlaySoundGeneral(Equi_boots, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                }
                // Toggle Hover Boots.
                if (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT) && 
                    CHECK_OWNED_EQUIP(EQUIP_BOOTS, 2)) {
                    Inventory_ChangeEquipment(EQUIP_BOOTS, CUR_EQUIP_VALUE(EQUIP_BOOTS) != 3 ? 3 : 1);
                    Player_SetEquipmentData(&gGlobalCtx, player);
                    Audio_PlaySoundGeneral(Equi_boots, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                }
            }
        }
    }
    // Ocarina shortcut.
    if (CHECK_BTN_ALL(p1Input->press.button, BTN_DDOWN) && CAN_USE_OCARINA) {
        // Bad/undocumented function name :((((
        // @TODO Maybe extract this to a nicely named utility?
        func_80835F44(&gGlobalCtx, player, INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY ? ITEM_OCARINA_FAIRY : ITEM_OCARINA_TIME);
    }
}

/**
 * Draws the DPad UI.
 */
void draw_dpad() {
    if (!DISPLAY_DPAD || CVar_GetS32("gDPadShortcuts", 0) == 0)
        return;

    Player* player = GET_PLAYER(gGlobalCtx);

    OPEN_DISPS(gGlobalCtx->state.gfxCtx, "dpad.c", 60);
    gDPPipeSync(OVERLAY_DISP++);
    gDPSetCombineMode(OVERLAY_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    uint16_t alpha = gGlobalCtx->interfaceCtx.minimapAlpha;

    if (alpha == 170) {
        alpha = 255;
    }

    // Draw DPad Icon.
    sprite_load(&sprDPad, false, alpha);
    sprite_draw(&sprDPad, ItemSlotsPos[0][0], ItemSlotsPos[0][1], ItemIconSize[0][0], ItemIconSize[0][1]);

    if (alpha == 255 && !CAN_USE_DPAD) {
        alpha = 70;
    }
    // Draw Navi icon
    sprite_load(&sprNavi, false, NaviAlpha);
    sprite_draw(&sprNavi, ItemSlotsPos[1][0], ItemSlotsPos[1][1], ItemIconSize[1][0], ItemIconSize[1][1]);

    // Draw Swords Page - BEGIN
    if (CurrentPage == 1){ //Draw Sword pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            if (CUR_EQUIP_VALUE(EQUIP_SWORD) == 3) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[2][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            } 
            if (CUR_EQUIP_VALUE(EQUIP_SHIELD) == 3) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[3][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            }
            if (gSaveContext.bgsFlag == 1) {
                sprite_load(&sprBGSword, false, alpha);
                sprite_draw(&sprBGSword, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else if (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 0 && gSaveContext.swordHealth > 0 || (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 8)) {
                if (gSaveContext.swordHealth > 0) {
                    sprite_load(&sprBGSword, false,  alpha);
                    sprite_draw(&sprBGSword, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
                } else {
                    sprite_load(&sprBrokenBGSword, false, alpha);
                    sprite_draw(&sprBrokenBGSword, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
                }
            } else {
                sprite_load(&sprBGSword, true, alpha);
                sprite_draw(&sprBGSword, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
            if (CHECK_OWNED_EQUIP(EQUIP_SHIELD, 2)) {
                sprite_load(&sprMirrorShield, false, alpha);
                sprite_draw(&sprMirrorShield, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprMirrorShield, true, alpha);
                sprite_draw(&sprMirrorShield, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }

        } else {
            //Link is child what to add in first page? Mask ?
            if (CUR_EQUIP_VALUE(EQUIP_BOOTS) == 10) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[2][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            } else if (CUR_EQUIP_VALUE(EQUIP_SHIELD) == 2) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[3][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            }
            if (CHECK_OWNED_EQUIP(EQUIP_SHIELD, 1)) {
                sprite_load(&sprHylianShield, false, alpha);
                sprite_draw(&sprHylianShield, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprHylianShield, true, alpha);
                sprite_draw(&sprHylianShield, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
        }
    }
    // Draw Sword Page - END
    // Draw Tunics Page - BEGIN
    if (CurrentPage == 2){ //Draw Tunics pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            if (CUR_EQUIP_VALUE(EQUIP_TUNIC) == 2) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[2][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            } else if (CUR_EQUIP_VALUE(EQUIP_TUNIC) == 3) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[3][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            }
            // Draw Goron Tunic
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                sprite_load(&sprGoronTunic, false, alpha);
                sprite_draw(&sprGoronTunic, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprGoronTunic, true, alpha);
                sprite_draw(&sprGoronTunic, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
            // Draw Zora Tunic
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 2)) {
                sprite_load(&sprZoraTunic, false, alpha);
                sprite_draw(&sprZoraTunic, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprZoraTunic, true, alpha);
                sprite_draw(&sprZoraTunic, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
        }
    }
    // Draw Tunics Page - END
    // Draw Boots Page - BEGIN
    if (CurrentPage == 3){ //Draw Boots pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            if (CUR_EQUIP_VALUE(EQUIP_BOOTS) == 2) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[2][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            } else if (CUR_EQUIP_VALUE(EQUIP_BOOTS) == 3) {
                sprite_load(&sprEquipedEffect, false, alpha);
                sprite_draw(&sprEquipedEffect, ItemSlotsPos[3][0]-PosFixEfX, ItemSlotsPos[2][1]-PosFixEfY, ItemIconSize[3][0], ItemIconSize[3][1]);
            }
            // Draw Iron Boots
            if (CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1)) {
                sprite_load(&sprIronBoots, false, alpha);
                sprite_draw(&sprIronBoots, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprIronBoots, true, alpha);
                sprite_draw(&sprIronBoots, ItemSlotsPos[2][0], ItemSlotsPos[2][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
            // Draw Hover Boots
            if (CHECK_OWNED_EQUIP(EQUIP_BOOTS, 2)) {
                sprite_load(&sprHoverBoots, false, alpha);
                sprite_draw(&sprHoverBoots, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            } else {
                sprite_load(&sprHoverBoots, true, alpha);
                sprite_draw(&sprHoverBoots, ItemSlotsPos[3][0], ItemSlotsPos[3][1], ItemIconSize[2][0], ItemIconSize[2][1]);
            }
        }
    }
    // Draw Boots Page - END

    // Draw Ocarina
    if (INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY || INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_TIME) {
        if (alpha == 0xFF && !CAN_USE_OCARINA) {
            alpha = 0x46;
        }
        if (INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY) {
            sprite_load(&sprFairyOcarina, false, alpha);
            sprite_draw(&sprFairyOcarina, ItemSlotsPos[4][0], ItemSlotsPos[4][1], 16, 16);
        } else {
            sprite_load(&sprOcarinaOfTime, false, alpha);
            sprite_draw(&sprOcarinaOfTime, ItemSlotsPos[4][0], ItemSlotsPos[4][1], 16, 16);
        }
    } else if (INV_CONTENT(ITEM_OCARINA_FAIRY) != ITEM_OCARINA_FAIRY || INV_CONTENT(ITEM_OCARINA_FAIRY) != ITEM_OCARINA_TIME) {
        sprite_load(&sprFairyOcarina, true, alpha);
        sprite_draw(&sprFairyOcarina, ItemSlotsPos[4][0], ItemSlotsPos[4][1], 16, 16);
    }

    gDPPipeSync(OVERLAY_DISP++);

    CLOSE_DISPS(gGlobalCtx->state.gfxCtx, "dpad.c", 113);
}