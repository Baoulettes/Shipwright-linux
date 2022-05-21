#include "dpad.h"
#include "textures/icon_item_field_static/icon_item_field_static.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/nes_font_static/nes_font_static.h"

/**
 * Sprite reference
 * void *tex, uint16_t width, uint16_t height, f32 scale_w, f32 scale_h, uint8_t im_fmt, uint8_t im_siz;
 */
sprite_t LeftIcon;
sprite_t RightIcon;

sprite_t sprDPad = { gMsgCharABControlPadTex, 8, 16, 2.5f, 1.25f, G_IM_FMT_IA, G_IM_SIZ_8b, 255, false, true }; 
sprite_t sprNavi = { gNaviCUpENGTex, 32, 8, 1.0f, 1.0f, G_IM_FMT_IA, G_IM_SIZ_4b, 50, false, true };

sprite_t sprEquipedEffect_L = { gMagicArrowEquipEffectTex, 32, 32, 0.65f, 0.65f, G_IM_FMT_IA, G_IM_SIZ_8b, 70, false, true };
sprite_t sprEquipedEffect_R = { gMagicArrowEquipEffectTex, 32, 32, 0.65f, 0.65f, G_IM_FMT_IA, G_IM_SIZ_8b, 70, false, true };

sprite_t sprBrokenBGSword = { gBrokenBiggoronSwordIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };
sprite_t sprBGSword = { gBiggoronSwordIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

sprite_t sprHylianShield = { gHylianShieldIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };
sprite_t sprMirrorShield = { gMirrorShieldIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

sprite_t sprGoronTunic = { gGoronTunicIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };
sprite_t sprZoraTunic = { gZoraTunicIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

sprite_t sprHoverBoots = { gHoverBootsIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };
sprite_t sprIronBoots = { gIronBootsIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

sprite_t sprFairyOcarina = { gFairyOcarinaIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };
sprite_t sprOcarinaOfTime = { gOcarinaofTimeIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

sprite_t sprBunnyHood = { gBunnyHoodIconTex, 32, 32, 0.35f, 0.35f, G_IM_FMT_RGBA, G_IM_SIZ_32b, 255, false, true };

s32 PageSwitcherBTN = BTN_L;
s32 PagesSwitchLeft = NA_SE_SY_WIN_SCROLL_LEFT;
s32 PagesSwitchRight = NA_SE_SY_WIN_SCROLL_RIGHT;

u16 CurrentPage = 1;
u16 MaxPagesCount = 3;
s32 Equi_swords = NA_SE_PL_METALEFFECT_ADULT;
s32 Equi_shields = NA_SE_PL_CHANGE_ARMS;
s32 Equi_tunics = NA_SE_PL_CHANGE_ARMS;
s32 Equi_boots = NA_SE_PL_CHANGE_ARMS;
s32 Equi_mask = NA_SE_PL_CHANGE_ARMS;

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
        Audio_PlaySoundGeneral(PagesSwitchLeft, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
    }
    if ((CHECK_BTN_ALL(p1Input->cur.button, PageSwitcherBTN)) &&
        (CHECK_BTN_ALL(p1Input->press.button, BTN_DRIGHT))) {
        if (CurrentPage == MaxPagesCount) {
            CurrentPage = 1;
        } else {
            CurrentPage++;
        }
        Audio_PlaySoundGeneral(PagesSwitchRight, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
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
                //printf("ID:%d\n",player->exchangeItemId);
                
                if (INV_CONTENT(PLAYER_MASK_BUNNY)) {
                    if (CHECK_BTN_ALL(p1Input->press.button, BTN_DLEFT)) {
                        if (player->currentMask == PLAYER_MASK_BUNNY) {
                            player->currentMask = PLAYER_MASK_NONE;
                            Audio_PlaySoundGeneral(Equi_mask, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                        } else {
                            player->currentMask = PLAYER_MASK_BUNNY;
                            Audio_PlaySoundGeneral(Equi_mask, &D_801333D4, 4, &D_801333E0, &D_801333E0, &D_801333E8);
                        }
                    }
                }

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

    // Draw DPad Icon.
    sprite_draw(&sprDPad, TYPE_DPAD, SLOT_DPAD, sprDPad.isgreyscale, sprDPad.alpha);

    // Draw Navi icon
    sprite_draw(&sprNavi, TYPE_NAVI, SLOT_UP, sprNavi.isgreyscale, sprNavi.alpha);

    //Draw Left and Right effect when you actually wear an equipment
    if (sprEquipedEffect_L.isshow) {
        sprite_draw(&sprEquipedEffect_L, TYPE_EFFECT, SLOT_LEFT, sprEquipedEffect_L.isgreyscale, sprEquipedEffect_L.alpha);
    }
    if (sprEquipedEffect_R.isshow) {
        sprite_draw(&sprEquipedEffect_R, TYPE_EFFECT, SLOT_RIGHT, sprEquipedEffect_R.isgreyscale, sprEquipedEffect_R.alpha);
    }


    // Draw Swords Page - BEGIN
    if (CurrentPage == 1){ //Draw Sword pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            //Biggoron sword is kind of special the icon is in conditions.
            RightIcon = sprMirrorShield;
            if (CUR_EQUIP_VALUE(EQUIP_SWORD) == 3) {
                sprEquipedEffect_L.isshow = true;
            } else { 
                sprEquipedEffect_L.isshow = false; 
            }
            if (CUR_EQUIP_VALUE(EQUIP_SHIELD) == 3) {
                sprEquipedEffect_R.isshow = true;
            } else {
                sprEquipedEffect_R.isshow = false; 
            }
            if (gSaveContext.bgsFlag == 1) {
                LeftIcon = sprBGSword;
                LeftIcon.isgreyscale = false;
            } else if (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 0 && gSaveContext.swordHealth > 0 || (CHECK_OWNED_EQUIP(EQUIP_SWORD, 3) == 8)) {
                if (gSaveContext.swordHealth > 0) {
                    LeftIcon = sprBGSword;
                    LeftIcon.isgreyscale = false;
                } else {
                    LeftIcon = sprBrokenBGSword;
                    LeftIcon.isgreyscale = false;
                }
            } else {
                LeftIcon = sprBGSword;
                LeftIcon.isgreyscale = true;
            }
            if (CHECK_OWNED_EQUIP(EQUIP_SHIELD, 2)) {
                RightIcon.isgreyscale = false;
            } else {
                RightIcon.isgreyscale = true;
            }

        } else {
            LeftIcon = sprBunnyHood;
            RightIcon = sprHylianShield;
            if (LINK_AGE_IN_YEARS != YEARS_CHILD) {
                sprEquipedEffect_L.isshow = true;
            } else {
                sprEquipedEffect_L.isshow = false;
            }
            if (CUR_EQUIP_VALUE(EQUIP_SHIELD) == 2) {
                sprEquipedEffect_R.isshow = true;
            } else {
                sprEquipedEffect_R.isshow = false;
            }
            if (INV_CONTENT(PLAYER_MASK_BUNNY)) {
                LeftIcon.isgreyscale = false;
            } else {
                LeftIcon.isgreyscale = true;
            }
            if (CHECK_OWNED_EQUIP(EQUIP_SHIELD, 1)) {
                RightIcon.isgreyscale = false;
            } else {
                RightIcon.isgreyscale = true;
            }
        }
    }
    // Draw Sword Page - END

    // Draw Tunics Page - BEGIN
    if (CurrentPage == 2){ //Draw Tunics pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            LeftIcon = sprGoronTunic;
            RightIcon = sprZoraTunic;
            if (CUR_EQUIP_VALUE(EQUIP_TUNIC) == 2) {
                sprEquipedEffect_L.isshow = true;
                sprEquipedEffect_R.isshow = false;
            } else if (CUR_EQUIP_VALUE(EQUIP_TUNIC) == 3) {
                sprEquipedEffect_L.isshow = false;
                sprEquipedEffect_R.isshow = true;
            } else if (CUR_EQUIP_VALUE(EQUIP_TUNIC) != 3 && CUR_EQUIP_VALUE(EQUIP_TUNIC) != 2){
                sprEquipedEffect_L.isshow = false;
                sprEquipedEffect_R.isshow = false;
            }

            // Draw Goron Tunic
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 1)) {
                LeftIcon.isgreyscale = false;
            } else {
                LeftIcon.isgreyscale = true;
            }
            // Draw Zora Tunic
            if (CHECK_OWNED_EQUIP(EQUIP_TUNIC, 2)) {
                RightIcon.isgreyscale = false;
            } else {
                RightIcon.isgreyscale = true;
            }
        }
    }
    // Draw Tunics Page - END

    // Draw Boots Page - BEGIN
    if (CurrentPage == 3){ //Draw Boots pages
        if (LINK_AGE_IN_YEARS == YEARS_ADULT) {
            LeftIcon = sprIronBoots;
            RightIcon = sprHoverBoots;
            if (CUR_EQUIP_VALUE(EQUIP_BOOTS) == 2) {
                sprEquipedEffect_L.isshow = true;
                sprEquipedEffect_R.isshow = false;
            } else if (CUR_EQUIP_VALUE(EQUIP_BOOTS) == 3) {
                sprEquipedEffect_L.isshow = false;
                sprEquipedEffect_R.isshow = true;
            } else if (CUR_EQUIP_VALUE(EQUIP_BOOTS) != 3 && CUR_EQUIP_VALUE(EQUIP_BOOTS) != 2){
                sprEquipedEffect_L.isshow = false;
                sprEquipedEffect_R.isshow = false;
            }
            // Draw Iron Boots
            if (CHECK_OWNED_EQUIP(EQUIP_BOOTS, 1)) {
                LeftIcon.isgreyscale = false;
            } else {
                LeftIcon.isgreyscale = true;
            }
            // Draw Hover Boots
            if (CHECK_OWNED_EQUIP(EQUIP_BOOTS, 2)) {
                RightIcon.isgreyscale = false;
            } else {
                RightIcon.isgreyscale = true;
            }
        }
    }
    // Draw Boots Page - END

    //Draw Base of Left and right icon n page.
    sprite_draw(&LeftIcon, TYPE_ICON, SLOT_LEFT, LeftIcon.isgreyscale, LeftIcon.alpha);
    sprite_draw(&RightIcon, TYPE_ICON, SLOT_RIGHT, RightIcon.isgreyscale, RightIcon.alpha);

    // Draw Ocarina
    sprite_t CurrentOcarina;
    if (INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY || INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_TIME) {
        CurrentOcarina.isgreyscale = false;
        if (INV_CONTENT(ITEM_OCARINA_FAIRY) == ITEM_OCARINA_FAIRY) {
            CurrentOcarina = sprFairyOcarina;
        } else {
            CurrentOcarina = sprOcarinaOfTime;
        }
        if (!CAN_USE_OCARINA) {
            CurrentOcarina.alpha = 70;
        }
         
    } else if (INV_CONTENT(ITEM_OCARINA_FAIRY) != ITEM_OCARINA_FAIRY || INV_CONTENT(ITEM_OCARINA_FAIRY) != ITEM_OCARINA_TIME) {
        CurrentOcarina.alpha = 50;
        CurrentOcarina.isgreyscale = true;
    }
    sprite_draw(&CurrentOcarina, TYPE_ICON, SLOT_DOWN, CurrentOcarina.isgreyscale, CurrentOcarina.alpha);
    gDPPipeSync(OVERLAY_DISP++);
    CLOSE_DISPS(gGlobalCtx->state.gfxCtx, "dpad.c", 113);
}