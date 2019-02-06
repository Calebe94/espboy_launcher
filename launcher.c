/** @file launcher.c
 * 
 * @brief This module has the purpose to create the ESPBoy's game launcher. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group. All rights reserved.
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <lvgl/lvgl.h>

#include "icons/game_consoles_icons.h"
#include "icons/espboy_wallpaper.h"
#include "icons/espboy_wallpaper_big.h"
#include "icons/home_screen_wallpaper.h"

#include "launcher_config.h"

#include "app_object.h"

#if TEST_MODE_VOLUME
#include <time.h>
#include <stdlib.h>
#endif

#include "launcher.h"
#include "volume.h"

/**********************
 *   GLOBAL VARIABLES
 **********************/

const char TAB_NAMES[][6] = {
    {"HOME"},
#if NES_EMULATOR
    {"NES"},
#endif
#if SMS_EMULATOR
    {"SMS"},
#endif
#if GB_EMULATOR
    {"GB"},
#endif
#if GG_EMULATOR
    {"GG"}
#endif
};

// OBJECTS
lv_obj_t * active_tab;      // TODO: change this variable name to 'active_tab; 
lv_obj_t * header_battery_icon;     // TODO: change this variable name to 'header_battery_icon'
lv_obj_t * header_volume_icon;      // TODO: change this variable name to 'header_volume_icon'
lv_obj_t * header_wifi_icon;        // TODO: change this variable name to 'header_wifi_icon'
lv_obj_t * header_storage_icon;     // TODO: change this variable name to 'header_storage_icon'

// LISTS
lv_obj_t *home_tab_list = NULL; // TODO: change this variable name to 'home_tab_list'
#if NES_EMULATOR
lv_obj_t *nes_tab_list = NULL; // TODO: change this variable name to 'nes_tab_list'
#endif
#if SMS_EMULATOR
lv_obj_t *sms_tab_listst = NULL; // TODO: change this variable name to 'sms_tab_list'
#endif
#if GB_EMULATOR
lv_obj_t *gb_tab_list = NULL; // TODO: change this variable name to 'gb_tab_list'
#endif
#if GG_EMULATOR
lv_obj_t *gg_tab_list = NULL; // TODO: change this variable name to 'gg_tab_list'
#endif

// TABS
lv_obj_t * home_tab;
#if NES_EMULATOR
lv_obj_t * nes_tab;
#endif
#if SMS_EMULATOR
lv_obj_t * sms_tab;
#endif
#if GB_EMULATOR
lv_obj_t * gb_tab;
#endif
#if GG_EMULATOR
lv_obj_t * gg_tab;
#endif
/**********************
 *  STATIC VARIABLES
 **********************/
static lv_indev_t   * indev;
static lv_group_t   * group;
static lv_obj_t     * header;
static lv_obj_t     * footer;

// STYLES
static lv_style_t style_tv_btn_bg;
static lv_style_t style_tv_btn_rel;
static lv_style_t style_tv_btn_pr;
static lv_style_t style_background;
static lv_style_t style_is_connected;

// THEME
static lv_theme_t * th; // TODO: change this variable name to 'theme'

// TABVIEW
static lv_obj_t * tv;   // TODO: change this variable name to 'tabview'

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void create_header();

static void theme_init();

static void style_init();

static void create_tabview();

static void create_tabs();

static void set_backgrounds();

static void set_styles();

static void create_lists();

static void add_home_itens();

static void add_nes_itens();

static void add_sms_itens();

static void add_gb_itens();

static void add_gg_itens();

static lv_res_t list_release_action(lv_obj_t * list_btn);

static lv_res_t tab_load_action(lv_obj_t * tabview, uint16_t act_id);

static char *  update_battery(uint16_t battery);

static void update_wifi(bool connected);

static void update_storage(bool connected);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*!
 * @brief Creates and initialize the ESPBoy Launcher
 *
 * @param this function has no parameters
 *
 * @return this function does not return nothing
 */
void launcher_init(void * this)
{
    #if TEST_MODE_VOLUME
        srand(time(NULL));   // Initialization, should only be called once.
    #endif

    indev = ((app_t*)this)->indev;

    /* Initialization of the ESPBoy Theme (Alien Theme)*/
    theme_init();
    /* Initialization of launcher objects style */
    style_init();
    // Create "group" object
    group = lv_group_create();
    // Create the launcher header
    create_header();
    // Create the launcher tabview
    create_tabview();
    // Create the launcher tabs
    create_tabs();
    // Set "wallpapers" to tabs
    set_backgrounds();
    // Set style to components
    set_styles();
    // Create lists to tabs
    create_lists();
    // /* -| ADD a action function to tabview |- */
    lv_tabview_set_tab_load_action(tv, tab_load_action);
    // /* -| ADD itens to group |- */
    lv_group_add_obj(group, tv);
    lv_group_add_obj(group, home_tab_list);
    lv_group_focus_obj(home_tab_list);
    // /* -| SET group to indev |- */
    lv_indev_set_group(indev, group);
}

/*!
 * @brief Update the ESPBoy Launcher info
 *
 * @param this function has no parameters
 *
 * @return this function does not return nothing
 */

void launcher_update(void * this)
{
    #if TEST_MODE_VOLUME
        uint8_t show_volume = rand() % 255;
        if( show_volume > 128 )
        {
            uint8_t aux_volume = rand() % 100;
            show_volume_control(th->bg, aux_volume);
            char * volume_icon = return_volume_symbol(aux_volume);
            if(volume_icon != NULL)
            {
                lv_label_set_text(header_volume_icon, volume_icon);  // Set the Volume symbol
            }
        }
    #endif

    #if TEST_MODE_BATTERY
        int aux_battery = rand() % ((4200 + 1 - 2500) + 2500);
        char * battery_icon = update_battery(aux_battery);
        if(battery_icon != NULL)
        {
            lv_label_set_text(header_battery_icon, battery_icon); // Set the Battery symbol 
        }
    #endif

    #if TEST_MODE_WIFI 
        update_wifi(false); 
    #endif

    #if TEST_MODE_STORAGE
        update_storage(false); 
    #endif
}

void launcher_deinit(void * this)
{

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*!
 * @brief This function is used to create ESPBoy's header.
 */
static void create_header()
{
    header = lv_label_create(lv_scr_act(), NULL); // Create the header label
    lv_label_set_text(header, "ESPBoy");  // Set the text 
    lv_obj_align(header, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 5); // Set its position

    /****************
    * ADD A BATTERY ICON
    ****************/
    header_battery_icon = lv_label_create(lv_scr_act(), NULL); // Create the Battery label
    lv_label_set_text(header_battery_icon, SYMBOL_BATTERY_3); // Set the Battery symbol 
    lv_obj_align(header_battery_icon, header, LV_ALIGN_IN_TOP_RIGHT, 120, 0); // Set its position

    /****************
    * ADD A VOLUME ICON
    ****************/
    header_volume_icon = lv_label_create(lv_scr_act(), NULL); // Create the Audio Volume label
    lv_label_set_text(header_volume_icon, SYMBOL_VOLUME_MAX);  // Set the Volume symbol
    lv_obj_align(header_volume_icon, header, LV_ALIGN_IN_TOP_RIGHT, 90, 0); // Set its position

    /****************
    * ADD A WIFI ICON
    ****************/
    header_wifi_icon = lv_label_create(lv_scr_act(), NULL); // Create the Audio WiFi label
    lv_label_set_text(header_wifi_icon, SYMBOL_WIFI); // Set the WiFi icon (I've choose this symbol to show when there is no connection)
    lv_obj_align(header_wifi_icon, header, LV_ALIGN_IN_TOP_RIGHT, 65, 0); // Set its position 

    /****************************
    * ADD A SDCARD(STORAGE) ICON
    ****************************/
    header_storage_icon = lv_label_create(lv_scr_act(), NULL); // Create the Audio WiFi label
    // lv_label_set_style(header_storage_icon, &style);
    lv_label_set_text(header_storage_icon, SYMBOL_DRIVE); // Set the WiFi icon (I've choose this symbol to show when there is no connection)
    lv_obj_align(header_storage_icon, header, LV_ALIGN_IN_TOP_RIGHT, 40, 0); // Set its position 

    /****************
    * ADD A ACTUAL TAB LABEL
    ****************/
    active_tab = lv_label_create(lv_scr_act(), NULL); // Create the Actual tab Label 
    lv_label_set_text(active_tab, TAB_NAMES[0]); // Set the tab name 
    lv_obj_align(active_tab, header, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(header)-LV_HOR_RES)/2)+10, 0); // Set its position
}

/*!
 * @brief This function is used to initialize ESPBoy's theme.
 */
static void theme_init()
{
    th = lv_theme_alien_init(LV_VER_RES, NULL);
    lv_theme_set_current(th);
    lv_cont_set_style(lv_scr_act(), th->bg);
}

/*!
 * @brief This function is used to initialize ESPBoy's components style.
 */
static void style_init()
{
   // ******** BACKGROUND STYLE ********
    lv_style_copy(&style_background, &lv_style_transp);
    style_background.image.opa = 100;   

    lv_style_copy(&style_tv_btn_bg, &lv_style_scr);
    style_tv_btn_bg.body.main_color = th->bg->body.main_color;
    style_tv_btn_bg.body.grad_color = th->bg->body.grad_color;
    style_tv_btn_bg.body.padding.ver = 0;

    lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);
    style_tv_btn_rel.body.empty = 1;
    style_tv_btn_rel.body.border.width = 0;

    lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
    style_tv_btn_pr.body.radius = 0;
    style_tv_btn_pr.body.opa = LV_OPA_50;
    style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.border.width = 0;
    style_tv_btn_pr.text.color = LV_COLOR_GRAY;

    lv_style_copy(&style_is_connected, &lv_style_plain);
}

/*!
 * @brief This function is used to create the main tabview.
 */
static void create_tabview()
{
    tv = lv_tabview_create(lv_scr_act(), NULL); 
    lv_obj_align(tv, header, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(header)-LV_HOR_RES)/2), 25);    
    lv_obj_set_size(tv, LV_HOR_RES, LV_VER_RES-(lv_obj_get_height(header)+lv_obj_get_height(header)));   
}

/*!
 * @brief This function is used to create the tabview tabs.
 */
static void create_tabs()
{
    home_tab = lv_tabview_add_tab(tv, SYMBOL_HOME);
    #if NES_EMULATOR
    nes_tab = lv_tabview_add_tab(tv, "NES");
    #endif
    #if SMS_EMULATOR
    sms_tab = lv_tabview_add_tab(tv, "SMS");
    #endif
    #if GB_EMULATOR
    gb_tab = lv_tabview_add_tab(tv, "GB");
    #endif
    #if GG_EMULATOR
    gg_tab = lv_tabview_add_tab(tv, "GG");
    #endif
}

/*!
 * @brief This function is used to set the tabs background.
 */
static void set_backgrounds()
{
    // ******** HOME BACKGROUND ********
    lv_obj_t *home_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(home_backgroud, &style_background);
    lv_img_set_src(home_backgroud, &espboy_wallpaper);
    lv_obj_set_width(home_backgroud, espboy_wallpaper.header.w*3);
    lv_obj_set_height(home_backgroud, espboy_wallpaper.header.h*2);
    lv_obj_set_parent(home_backgroud, home_tab);
    lv_obj_set_pos(home_backgroud, (espboy_wallpaper.header.w/5), 0);

    // ******** NES BACKGROUND ********

    #if NES_EMULATOR
    lv_obj_t *nes_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(nes_backgroud, &style_background);
    lv_img_set_src(nes_backgroud, &nes_icon_large);
    lv_obj_set_width(nes_backgroud, nes_icon_large.header.w);
    lv_obj_set_height(nes_backgroud, nes_icon_large.header.h);
    lv_obj_set_parent(nes_backgroud, nes_tab);
    lv_obj_set_pos(nes_backgroud, (LV_HOR_RES/2)-(nes_icon_large.header.w/2), 0);
    #endif

    // ******** MASTERSYSTEM BACKGROUND ********

    #if SMS_EMULATOR
    lv_obj_t *sms_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(sms_backgroud, &style_background);
    lv_img_set_src(sms_backgroud, &mastersystem_icon_large);
    lv_obj_set_width(sms_backgroud, mastersystem_icon_large.header.w);
    lv_obj_set_height(sms_backgroud, mastersystem_icon_large.header.h);
    lv_obj_set_parent(sms_backgroud, sms_tab);
    lv_obj_set_pos(sms_backgroud, (LV_HOR_RES/2)-(mastersystem_icon_large.header.w/2), 0);
    #endif

    // ******** GAMEBOY BACKGROUND ********

    #if GB_EMULATOR
    lv_obj_t *gb_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gb_backgroud, &style_background);
    lv_img_set_src(gb_backgroud, &gameboy_icon_large);
    lv_obj_set_width(gb_backgroud, gameboy_icon_large.header.w);
    lv_obj_set_height(gb_backgroud, gameboy_icon_large.header.h);
    lv_obj_set_parent(gb_backgroud, gb_tab);
    lv_obj_set_pos(gb_backgroud, (LV_HOR_RES/2)-(gameboy_icon_large.header.w/2), 0);
    #endif

    // ******** GAMEGEAR BACKGROUND ********
    #if GG_EMULATOR
    lv_obj_t *gg_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gg_backgroud, &style_background);
    lv_img_set_src(gg_backgroud, &gamegear_icon_large);
    lv_obj_set_width(gg_backgroud, gamegear_icon_large.header.w);
    lv_obj_set_height(gg_backgroud, gamegear_icon_large.header.h);
    lv_obj_set_parent(gg_backgroud, gg_tab);
    lv_obj_set_pos(gg_backgroud, (LV_HOR_RES/2)-(gamegear_icon_large.header.w/2), 0);
    #endif
}

/*!
 * @brief This function is used to set the style to launcher components.
 */
static void set_styles()
{
    // ======= SET STYLE TO TABS ======= 
    lv_cont_set_style(home_tab, th->bg);
    #if NES_EMULATOR
    lv_cont_set_style(nes_tab, th->bg);
    #endif

    #if SMS_EMULATOR
    lv_cont_set_style(sms_tab, th->bg);
    #endif

    #if GB_EMULATOR
    lv_cont_set_style(gb_tab, th->bg);
    #endif

    #if GG_EMULATOR
    lv_cont_set_style(gg_tab, th->bg);
    #endif

    // ======= SET STYLE TO TABVIEW ======= 
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_INDIC, &lv_style_plain);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_PR, &style_tv_btn_pr);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_PR, &style_tv_btn_pr);   
}

/*!
 * @brief This function is used to create tab lists.
 */
static void create_lists()
{
    /* Crate the home_tab_list: HOME */
    home_tab_list = lv_list_create(home_tab, NULL);
    lv_obj_set_size(home_tab_list, LV_HOR_RES, lv_obj_get_height(home_tab));
    lv_list_set_style(home_tab_list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(home_tab_list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(home_tab_list, home_tab, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(home_tab)-LV_HOR_RES)/2), 0);
    
    /* -| Crate the nes_tab_list: NES |- */
    #if NES_EMULATOR
    nes_tab_list = lv_list_create(nes_tab, NULL);
    lv_obj_set_size(nes_tab_list, LV_HOR_RES, lv_obj_get_height(nes_tab));
    lv_list_set_style(nes_tab_list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(nes_tab_list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(nes_tab_list, nes_tab, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(home_tab)-LV_HOR_RES)/2), 0);
    #endif

    /* -| Crate the sms_tab_listst: MASTERSYSTEM |- */
    #if SMS_EMULATOR
    sms_tab_listst = lv_list_create(sms_tab, NULL);
    lv_obj_set_size(sms_tab_listst, LV_HOR_RES, lv_obj_get_height(sms_tab));
    lv_list_set_style(sms_tab_listst, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(sms_tab_listst, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(sms_tab_listst, sms_tab, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(home_tab)-LV_HOR_RES)/2), 0);
    #endif

    /*Crate the gb_tab_list: GAMEBOY */
    #if GB_EMULATOR
    gb_tab_list = lv_list_create(gb_tab, NULL);
    lv_obj_set_size(gb_tab_list, LV_HOR_RES, lv_obj_get_height(gb_tab));
    lv_list_set_style(gb_tab_list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(gb_tab_list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(gb_tab_list, gb_tab, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(home_tab)-LV_HOR_RES)/2), 0);
    #endif

    /* -| Crate the gg_tab_list: GAMEGEAR |- */
    #if GG_EMULATOR
    gg_tab_list = lv_list_create(gg_tab, NULL);
    lv_obj_set_size(gg_tab_list, LV_HOR_RES, lv_obj_get_height(gg_tab));
    lv_list_set_style(gg_tab_list, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(gg_tab_list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(gg_tab_list, gg_tab, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(home_tab)-LV_HOR_RES)/2), 0);
    #endif

    #if TEST_MODE
    #if TEST_MODE_LIST
        add_home_itens();
        #if NES_EMULATOR
        add_nes_itens();
        #endif

        #if SMS_EMULATOR
        add_gb_itens();
        #endif

        #if GB_EMULATOR
        add_gg_itens();
        #endif

        #if GG_EMULATOR
        add_sms_itens();
        #endif
    #endif
    #endif
}

/*!
 * @brief This function is used when to perform some action when an item in tabview is clicked.
 *
 * @param[in] tabview   Is the pointer to the tabview object.
 * @param[in] act_id    Is the number id of the item clicked.
 *
 * @return LV_RES_OK if the object is valid.
 */

// TODO: Check this act_id, when I add another emulators i've to check who is gonna enter to group
static lv_res_t tab_load_action(lv_obj_t * tabview, uint16_t act_id)
{
    lv_label_set_text(active_tab, TAB_NAMES[act_id]); // Set active tab name in header
    switch(act_id)
    {
        case 0: 
            lv_group_add_obj(group, home_tab_list); // Add active tab contant to group
            /* Remove other tabs from group */
            #if NES_EMULATOR
            lv_group_remove_obj(nes_tab_list);
            #endif

            #if SMS_EMULATOR
            lv_group_remove_obj(sms_tab_listst);
            #endif

            #if GB_EMULATOR
            lv_group_remove_obj(gb_tab_list);
            #endif

            #if GG_EMULATOR
            lv_group_remove_obj(gg_tab_list);
            #endif
            break;
        case 1: 
            #if NES_EMULATOR
            lv_group_add_obj(group, nes_tab_list); // Add active tab contant to group
            #endif
            /* Remove other tabs from group */
            lv_group_remove_obj(home_tab_list);

            #if SMS_EMULATOR
            lv_group_remove_obj(sms_tab_listst);
            #endif

            #if GB_EMULATOR
            lv_group_remove_obj(gb_tab_list);
            #endif

            #if GG_EMULATOR
            lv_group_remove_obj(gg_tab_list);
            #endif
            break;
        case 2: 
            #if SMS_EMULATOR
            lv_group_add_obj(group, sms_tab_listst); // Add active tab contant to group
            #endif
            
            #if NES_EMULATOR
            lv_group_remove_obj(nes_tab_list);
            #endif

            #if GB_EMULATOR
            lv_group_remove_obj(gb_tab_list);
            #endif

            #if GG_EMULATOR
            lv_group_remove_obj(gg_tab_list);
            #endif
            
            /* Remove other tabs from group */
            lv_group_remove_obj(home_tab_list);
            break;
        case 3: 
            #if GB_EMULATOR
            lv_group_add_obj(group, gb_tab_list); // Add active tab contant to group
            #endif

            #if NES_EMULATOR
            lv_group_remove_obj(nes_tab_list);
            #endif

            #if SMS_EMULATOR
            lv_group_remove_obj(sms_tab_listst);
            #endif

            #if GG_EMULATOR
            lv_group_remove_obj(gg_tab_list);
            #endif
            /* Remove other tabs from group */
            lv_group_remove_obj(home_tab_list);
            break;
        case 4: 
            #if GG_EMULATOR
            lv_group_add_obj(group, gg_tab_list);
            #endif

            #if NES_EMULATOR
            lv_group_remove_obj(nes_tab_list);
            #endif

            #if SMS_EMULATOR
            lv_group_remove_obj(sms_tab_listst);
            #endif

            #if GB_EMULATOR
            lv_group_remove_obj(gb_tab_list);
            #endif

            lv_group_remove_obj(home_tab_list);

            break;
    }
    /*  focus on the tab content */
    // lv_tabview_set_tab_act(tv, act_id, false);
    lv_group_focus_next(group);

    return LV_RES_OK;
}

static lv_res_t mbox_apply_action(lv_obj_t * btns, const char * txt)
{
    // printf("Mbox button: %s\n", txt);
    #if TEST_MODE
    #if TEST_MODE_MB
        /*
        * Check https://github.com/littlevgl/lvgl/issues/383#issuecomment-416462809 to see how to close MessageBox
        */
        lv_obj_t * mbox = lv_mbox_get_from_btn(btns);
        lv_obj_t * gray_bg = lv_obj_get_parent(mbox);

        lv_obj_del(mbox);
        lv_group_focus_freeze(group, false);
        lv_group_focus_next(group);
    #endif
    #endif
    return LV_RES_OK; /*Return OK if the message box is not deleted*/
}

/*!
 * @brief This function is used when to perform some action when an item in tabview is clicked.
 *
 * @param[in] list_btn   Is the pointer to the list item object.
 *
 * @return LV_RES_OK if the object is valid.
 */
static lv_res_t list_release_action(lv_obj_t * list_btn)
{
    // printf("List element click:%s\n", lv_list_get_btn_text(list_btn));
    #if TEST_MODE
    #if TEST_MODE_MB
        lv_obj_t * mbox1 = lv_mbox_create(lv_scr_act(), NULL);
        lv_mbox_set_text(mbox1, lv_list_get_btn_text(list_btn));                    /*Set the text*/

        /*Add two buttons*/
        static const char * btns[] ={"\221Close", ""}; /*Button description. '\221' lv_btnm like control char*/
        lv_mbox_add_btns(mbox1, btns, mbox_apply_action);
        lv_obj_set_width(mbox1, 200);
        lv_obj_align(mbox1, NULL, LV_ALIGN_IN_TOP_MID, 10, 70); /*Align to the corner*/
        lv_group_add_obj(group, mbox1);
        lv_group_focus_next(group);
        lv_group_focus_freeze(group,true);
    #endif
    #endif

    return LV_RES_OK; /*Return OK because the list is not deleted*/
}

/*!
 * @brief This function is used to add itens to home list
 */
static void add_home_itens()
{
    /* -| ADD Itens to home_tab_list: HOME |- */
    lv_list_add(home_tab_list, SYMBOL_FILE, "Recently Played", list_release_action);
    lv_list_add(home_tab_list, SYMBOL_SETTINGS, "Settings", list_release_action);
}

/*!
 * @brief This function is used to add itens to nes list
 */
static void add_nes_itens()
{
    /* -| ADD Itens to nes_tab_list: NES |- */
    #if NES_EMULATOR
    lv_list_add(nes_tab_list, NULL, "Test 1", list_release_action);
    lv_list_add(nes_tab_list, NULL, "Test 2", list_release_action);
    lv_list_add(nes_tab_list, NULL, "Test 3", list_release_action);
    lv_list_add(nes_tab_list, NULL, "Test 4", list_release_action);
    #endif
}

/*!
 * @brief This function is used to add itens to sms list
 */
static void add_sms_itens()
{
    /* -| ADD Itens to sms_tab_listst: MASTERSYSTEM |- */
    #if SMS_EMULATOR
    lv_list_add(sms_tab_listst, NULL, "Test 1", list_release_action);
    lv_list_add(sms_tab_listst, NULL, "Test 2", list_release_action);
    lv_list_add(sms_tab_listst, NULL, "Test 3", list_release_action);
    lv_list_add(sms_tab_listst, NULL, "Test 4", list_release_action);
    #endif
}

/*!
 * @brief This function is used to add itens to gb list
 */
static void add_gb_itens()
{
    /* -| ADD Itens to gb_tab_list: GAMEBOY |- */
    #if GB_EMULATOR
    lv_list_add(gb_tab_list, NULL, "Test 1", list_release_action);
    lv_list_add(gb_tab_list, NULL, "Test 2", list_release_action);
    lv_list_add(gb_tab_list, NULL, "Test 3", list_release_action);
    lv_list_add(gb_tab_list, NULL, "Test 4", list_release_action); 
    #endif
}

/*!
 * @brief This function is used to add itens to gg list
 */
static void add_gg_itens()
{
    /* -| ADD Itens to gg_tab_list: GAMEGEAR |- */
    #if GG_EMULATOR
    lv_list_add(gg_tab_list, NULL, "Test 1", list_release_action);
    lv_list_add(gg_tab_list, NULL, "Test 2", list_release_action);
    lv_list_add(gg_tab_list, NULL, "Test 3", list_release_action);
    lv_list_add(gg_tab_list, NULL, "Test 4", list_release_action);    
    #endif
}

/*!
 * @brief This function is used when to perform some action when an item in tabview is clicked.
 *
 * @param[in] tabview   Is the pointer to the tabview object.
 * @param[in] act_id    Is the number id of the item clicked.
 *
 * @return LV_RES_OK if the object is valid.
 */
static char *  update_battery(uint16_t battery)
{
    if( (battery > 2500) && (battery < 2600) )
    {
        return SYMBOL_BATTERY_EMPTY;
    }
    else if( (battery > 2600) && (battery < 3000) )
    {
        return SYMBOL_BATTERY_1;
    }
    else if( (battery > 3000) && (battery < 3400) )
    {
        return SYMBOL_BATTERY_2;
    }
    else if( (battery > 3400) && (battery < 3800) )
    {
        return SYMBOL_BATTERY_3;
    }
    else if( battery > 3800 )
    {
        return SYMBOL_BATTERY_FULL;
    }
    else
    {
        return NULL;
    }
}

static void update_wifi(bool connected)
{
    if(connected)
    {
        style_is_connected.text.color = LV_COLOR_WHITE;
    }
    else
    {
        style_is_connected.text.color = LV_COLOR_RED;
    }
    lv_label_set_style(header_wifi_icon, &style_is_connected);
}

static void update_storage(bool connected)
{
    if(connected)
    {
        style_is_connected.text.color = LV_COLOR_WHITE;
    }
    else
    {
        style_is_connected.text.color = LV_COLOR_RED;
    }
    lv_label_set_style(header_storage_icon, &style_is_connected);
}
/*** end of file ***/
