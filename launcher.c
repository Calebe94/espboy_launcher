/** @file launcher.c
 * 
 * @brief This module has the purpose to create the ESPBoy's game launcher. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group. All rights reserved.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "lvgl/lvgl.h"

#include "icons/game_consoles_icons.h"
#include "icons/espboy_wallpaper.h"
#include "icons/espboy_wallpaper_big.h"
#include "icons/home_screen_wallpaper.h"

#include "launcher.h"

/**********************
 *   GLOBAL VARIABLES
 **********************/

const char TAB_NAMES[][6] = {
    {"HOME"},
    {"NES"},
    {"SMS"},
    {"GB"},
    {"GG"}
};

// OBJECTS
lv_obj_t * actual;      // TODO: change this variable name to 'active_tab; 

// LISTS
lv_obj_t *list1 = NULL; // TODO: change this variable name to 'home_tab_list'
lv_obj_t *list2 = NULL; // TODO: change this variable name to 'nes_tab_list'
lv_obj_t *list3 = NULL; // TODO: change this variable name to 'sms_tab_list'
lv_obj_t *list4 = NULL; // TODO: change this variable name to 'gb_tab_list'
lv_obj_t *list5 = NULL; // TODO: change this variable name to 'gg_tab_list'

// TABS
lv_obj_t * tab1;
lv_obj_t * tab2;
lv_obj_t * tab3;
lv_obj_t * tab4;
lv_obj_t * tab5;
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
void launcher_init(lv_indev_t * aux_indev)
{
    indev = aux_indev;
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
    lv_group_add_obj(group, list1);
    lv_group_focus_obj(list1);
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

void launcher_update()
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
    lv_obj_t * battery = lv_label_create(lv_scr_act(), NULL); // Create the Battery label
    lv_label_set_text(battery, SYMBOL_BATTERY_3); // Set the Battery symbol 
    lv_obj_align(battery, header, LV_ALIGN_IN_TOP_RIGHT, (LV_HOR_RES - lv_obj_get_width(header)-30)/2, 0); // Set its position

    /****************
    * ADD A VOLUME ICON
    ****************/
    lv_obj_t * volume = lv_label_create(lv_scr_act(), NULL); // Create the Audio Volume label
    lv_label_set_text(volume, SYMBOL_MUTE);  // Set the Volume symbol
    lv_obj_align(volume, header, LV_ALIGN_IN_TOP_RIGHT, (LV_HOR_RES - lv_obj_get_width(header)-20)/3, 0); // Set its position

    /****************
    * ADD A WIFI ICON
    ****************/
    lv_obj_t * wifi = lv_label_create(lv_scr_act(), NULL); // Create the Audio WiFi label
    lv_label_set_text(wifi, SYMBOL_CLOSE); // Set the WiFi icon (I've choose this symbol to show when there is no connection)
    lv_obj_align(wifi, header, LV_ALIGN_IN_TOP_RIGHT, (LV_HOR_RES - lv_obj_get_width(header)-40)/4, 0); // Set its position 

    /****************
    * ADD A ACTUAL TAB LABEL
    ****************/
    actual = lv_label_create(lv_scr_act(), NULL); // Create the Actual tab Label 
    lv_label_set_text(actual, TAB_NAMES[0]); // Set the tab name 
    lv_obj_align(actual, header, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(header)-LV_HOR_RES)/2)+20, 0); // Set its position
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
    // style_tv_btn_bg.body.border.color = LV_COLOR_GREEN;
    // style_tv_btn_bg.body.border.width = 1;
    // style_tv_btn_bg.body.shadow.color = LV_COLOR_GREEN;
    // style_tv_btn_bg.body.shadow.width = 1;

    style_tv_btn_bg.body.padding.ver = 0;

    lv_style_copy(&style_tv_btn_rel, &lv_style_btn_rel);
    style_tv_btn_rel.body.empty = 1;
    style_tv_btn_rel.body.border.width = 0;
    // style_tv_btn_rel.body.padding.ver = 0;

    lv_style_copy(&style_tv_btn_pr, &lv_style_btn_pr);
    style_tv_btn_pr.body.radius = 0;
    style_tv_btn_pr.body.opa = LV_OPA_50;
    style_tv_btn_pr.body.main_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.grad_color = LV_COLOR_WHITE;
    style_tv_btn_pr.body.border.width = 0;
    style_tv_btn_pr.text.color = LV_COLOR_GRAY;
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
    tab1 = lv_tabview_add_tab(tv, SYMBOL_HOME);
    tab2 = lv_tabview_add_tab(tv, "NES");
    tab3 = lv_tabview_add_tab(tv, "SMS");
    tab4 = lv_tabview_add_tab(tv, "GB");
    tab5 = lv_tabview_add_tab(tv, "GG");
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
    lv_obj_set_parent(home_backgroud, tab1);
    lv_obj_set_pos(home_backgroud, (espboy_wallpaper.header.w/5), 0);

    // ******** NES BACKGROUND ********
    lv_obj_t *nes_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(nes_backgroud, &style_background);
    lv_img_set_src(nes_backgroud, &nes_icon_large);
    lv_obj_set_width(nes_backgroud, nes_icon_large.header.w);
    lv_obj_set_height(nes_backgroud, nes_icon_large.header.h);
    lv_obj_set_parent(nes_backgroud, tab2);
    lv_obj_set_pos(nes_backgroud, (LV_HOR_RES/2)-(nes_icon_large.header.w/2), 0);

    // ******** MASTERSYSTEM BACKGROUND ********
    lv_obj_t *sms_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(sms_backgroud, &style_background);
    lv_img_set_src(sms_backgroud, &mastersystem_icon_large);
    lv_obj_set_width(sms_backgroud, mastersystem_icon_large.header.w);
    lv_obj_set_height(sms_backgroud, mastersystem_icon_large.header.h);
    lv_obj_set_parent(sms_backgroud, tab3);
    lv_obj_set_pos(sms_backgroud, (LV_HOR_RES/2)-(mastersystem_icon_large.header.w/2), 0);

    // ******** GAMEBOY BACKGROUND ********
    lv_obj_t *gb_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gb_backgroud, &style_background);
    lv_img_set_src(gb_backgroud, &gameboy_icon_large);
    lv_obj_set_width(gb_backgroud, gameboy_icon_large.header.w);
    lv_obj_set_height(gb_backgroud, gameboy_icon_large.header.h);
    lv_obj_set_parent(gb_backgroud, tab4);
    lv_obj_set_pos(gb_backgroud, (LV_HOR_RES/2)-(gameboy_icon_large.header.w/2), 0);

    // ******** GAMEGEAR BACKGROUND ********
    lv_obj_t *gg_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gg_backgroud, &style_background);
    lv_img_set_src(gg_backgroud, &gamegear_icon_large);
    lv_obj_set_width(gg_backgroud, gamegear_icon_large.header.w);
    lv_obj_set_height(gg_backgroud, gamegear_icon_large.header.h);
    lv_obj_set_parent(gg_backgroud, tab5);
    lv_obj_set_pos(gg_backgroud, (LV_HOR_RES/2)-(gamegear_icon_large.header.w/2), 0);
}

/*!
 * @brief This function is used to set the style to launcher components.
 */
static void set_styles()
{
    // ======= SET STYLE TO TABS ======= 
    lv_cont_set_style(tab1, th->bg);
    lv_cont_set_style(tab2, th->bg);
    lv_cont_set_style(tab3, th->bg);
    lv_cont_set_style(tab4, th->bg);
    lv_cont_set_style(tab5, th->bg);

    // ======= SET STYLE TO TABVIEW ======= 
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_bg);
    // lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_rel);
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
    /* Crate the list1: HOME */
    list1 = lv_list_create(tab1, NULL);
    lv_obj_set_size(list1, LV_HOR_RES, lv_obj_get_height(tab1));
    lv_list_set_style(list1, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list1, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list1, tab1, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);
    
    /* -| Crate the list2: NES |- */
    list2 = lv_list_create(tab2, NULL);
    lv_obj_set_size(list2, LV_HOR_RES, lv_obj_get_height(tab2));
    lv_list_set_style(list2, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list2, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list2, tab2, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| Crate the list3: MASTERSYSTEM |- */
    list3 = lv_list_create(tab3, NULL);
    lv_obj_set_size(list3, LV_HOR_RES, lv_obj_get_height(tab3));
    lv_list_set_style(list3, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list3, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list3, tab3, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /*Crate the list4: GAMEBOY */
    list4 = lv_list_create(tab4, NULL);
    lv_obj_set_size(list4, LV_HOR_RES, lv_obj_get_height(tab4));
    lv_list_set_style(list4, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list4, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list4, tab4, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| Crate the list5: GAMEGEAR |- */
    list5 = lv_list_create(tab5, NULL);
    lv_obj_set_size(list5, LV_HOR_RES, lv_obj_get_height(tab5));
    lv_list_set_style(list5, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list5, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list5, tab5, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    #if ADD_TEST_LIST
        add_home_itens();
        add_nes_itens();
        add_gb_itens();
        add_gg_itens();
        add_sms_itens();
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
static lv_res_t tab_load_action(lv_obj_t * tabview, uint16_t act_id)
{
    lv_label_set_text(actual, TAB_NAMES[act_id]); // Set active tab name in header
    switch(act_id)
    {
        case 0: 
            lv_group_add_obj(group, list1); // Add active tab contant to group
            /* Remove other tabs from group */
            lv_group_remove_obj(list2);
            lv_group_remove_obj(list3);
            lv_group_remove_obj(list4);
            lv_group_remove_obj(list5);
            break;
        case 1: 
            lv_group_add_obj(group, list2); // Add active tab contant to group
            /* Remove other tabs from group */
            lv_group_remove_obj(list1);
            lv_group_remove_obj(list3);
            lv_group_remove_obj(list4);
            lv_group_remove_obj(list5);
            break;
        case 2: 
            lv_group_add_obj(group, list3); // Add active tab contant to group
            /* Remove other tabs from group */
            lv_group_remove_obj(list1);
            lv_group_remove_obj(list2);
            lv_group_remove_obj(list4);
            lv_group_remove_obj(list5);
            break;
        case 3: 
            lv_group_add_obj(group, list4); // Add active tab contant to group
            /* Remove other tabs from group */
            lv_group_remove_obj(list1);
            lv_group_remove_obj(list2);
            lv_group_remove_obj(list3);
            lv_group_remove_obj(list5);
            break;
        case 4: 
            lv_group_remove_obj(list1);
            lv_group_remove_obj(list2);
            lv_group_remove_obj(list3);
            lv_group_remove_obj(list4);
            lv_group_add_obj(group, list5);
            break;
    }
    /*  focus on the tab content */
    // lv_tabview_set_tab_act(tv, act_id, false);
    lv_group_focus_next(group);

    return LV_RES_OK;
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
    printf("List element click:%s\n", lv_list_get_btn_text(list_btn));

    return LV_RES_OK; /*Return OK because the list is not deleted*/
}

/*!
 * @brief This function is used to add itens to home list
 */
static void add_home_itens()
{
    /* -| ADD Itens to list1: HOME |- */
    lv_list_add(list1, SYMBOL_FILE, "Recently Played", list_release_action);
    lv_list_add(list1, SYMBOL_SETTINGS, "Settings", list_release_action);
}

/*!
 * @brief This function is used to add itens to nes list
 */
static void add_nes_itens()
{
    /* -| ADD Itens to list2: NES |- */
    lv_list_add(list2, NULL, "Test 1", list_release_action);
    lv_list_add(list2, NULL, "Test 2", list_release_action);
    lv_list_add(list2, NULL, "Test 3", list_release_action);
    lv_list_add(list2, NULL, "Test 4", list_release_action);
}

/*!
 * @brief This function is used to add itens to sms list
 */
static void add_sms_itens()
{
    /* -| ADD Itens to list3: MASTERSYSTEM |- */
    lv_list_add(list3, NULL, "Test 1", list_release_action);
    lv_list_add(list3, NULL, "Test 2", list_release_action);
    lv_list_add(list3, NULL, "Test 3", list_release_action);
    lv_list_add(list3, NULL, "Test 4", list_release_action);
}

/*!
 * @brief This function is used to add itens to gb list
 */
static void add_gb_itens()
{
    /* -| ADD Itens to list4: GAMEBOY |- */
    lv_list_add(list4, NULL, "Test 1", list_release_action);
    lv_list_add(list4, NULL, "Test 2", list_release_action);
    lv_list_add(list4, NULL, "Test 3", list_release_action);
    lv_list_add(list4, NULL, "Test 4", list_release_action); 
}

/*!
 * @brief This function is used to add itens to gg list
 */
static void add_gg_itens()
{
    /* -| ADD Itens to list5: GAMEGEAR |- */
    lv_list_add(list5, NULL, "Test 1", list_release_action);
    lv_list_add(list5, NULL, "Test 2", list_release_action);
    lv_list_add(list5, NULL, "Test 3", list_release_action);
    lv_list_add(list5, NULL, "Test 4", list_release_action);    
}

/*** end of file ***/
