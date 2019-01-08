
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/encoder.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lv_examples/lv_apps/benchmark/benchmark.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme.h"
#include "lv_examples/lv_tests/lv_test_group/lv_test_group.h"

#include "game_consoles_icons.h"
#include "espboy_wallpaper.h"
#include "espboy_wallpaper1.h"
#include "espboy_wallpaper_big.h"
#include "home_screen_wallpaper.h"
/*********************
 *      DEFINES
 *********************/

/*On OSX SDL needs different handling*/
#if defined(__APPLE__) && defined(TARGET_OS_MAC)
# if __APPLE__ && TARGET_OS_MAC
#define SDL_APPLE
# endif
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static int tick_thread(void * data);
static void memory_monitor(void * param);

static void create_header();

static void theme_init();
static void style_init();

static lv_res_t list_release_action(lv_obj_t * list_btn);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_indev_t * indev;
static lv_group_t *group;
static lv_obj_t *header;

// STYLES
static lv_style_t style_tv_btn_bg;
static lv_style_t style_tv_btn_rel;
static lv_style_t style_tv_btn_pr;

// THEME
static lv_theme_t *th ;

// TABVIEW
static lv_obj_t *tv = NULL;
/**********************
 *  GLOBAL VARIABLES
 **********************/

// OBJECTS
lv_obj_t * actual;

// LISTS
lv_obj_t *list1 = NULL;
lv_obj_t *list2 = NULL;
lv_obj_t *list3 = NULL;
lv_obj_t *list4 = NULL;
lv_obj_t *list5 = NULL;

const char TAB_NAMES[][6] = {
    {"HOME"},
    {"NES"},
    {"SMS"},
    {"GB"},
    {"GG"}
};

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void launcher_init(void);

lv_res_t tb_load_action(lv_obj_t * tabview, uint16_t act_id);

/**********************
 *   MAIN FUNCTION
 **********************/
int main(int argc, char ** argv)
{
    (void) argc;    /*Unused*/
    (void) argv;    /*Unused*/

    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LittlevGL*/
    hal_init();

    /*Load the launcher*/
    launcher_init();

    while(1) 
    {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();

        usleep(10000);       /*Just to let the system breath*/

        #ifdef SDL_APPLE
            SDL_Event event;
            
            while(SDL_PollEvent(&event)) 
            {
                mouse_handler(&event);

                keyboard_handler(&event);

            }
        #endif

    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void theme_init()
{
    th = lv_theme_alien_init(LV_VER_RES, NULL);
    lv_theme_set_current(th);

    // th = lv_theme_get_current();    /*If `LV_THEME_LIVE_UPDATE  1` `th` is not used directly so get the real theme after set*/
    // lv_obj_t * scr = lv_cont_create(NULL, NULL);
}

static void style_init()
{
    lv_style_copy(&style_tv_btn_bg, &lv_style_plain);
    style_tv_btn_bg.body.main_color = LV_COLOR_BLACK;
    style_tv_btn_bg.body.grad_color = LV_COLOR_GRAY;
    // style_tv_btn_bg.body.main_color = LV_COLOR_HEX(0x487fb7);
    // style_tv_btn_bg.body.grad_color = LV_COLOR_HEX(0x487fb7);
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
}

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

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void)
{
    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.disp_flush = monitor_flush;    /*Used when `LV_VDB_SIZE != 0` in lv_conf.h (buffered drawing)*/
    disp_drv.disp_fill = monitor_fill;      /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    disp_drv.disp_map = monitor_map;        /*Used when `LV_VDB_SIZE == 0` in lv_conf.h (unbuffered drawing)*/
    lv_disp_drv_register(&disp_drv);

    /* Add the keyboard as input device
     * Use the 'keyboard' driver which reads the PC's keyboard*/
    keyboard_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read = keyboard_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev = lv_indev_drv_register(&indev_drv);

    /* Tick init.
     * You have to call 'lv_tick_inc()' in periodically to inform LittelvGL about how much time were elapsed
     * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);

    /* Optional:
     * Create a memory monitor task which prints the memory usage in periodically.*/
    lv_task_create(memory_monitor, 3000, LV_TASK_PRIO_LOWEST, NULL);
}

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static int tick_thread(void * data)
{
    (void)data;

    while(1) {
        SDL_Delay(5);   /*Sleep for 5 millisecond*/
        lv_tick_inc(5); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

/**
 * Print the memory usage periodically
 * @param param
 */
static void memory_monitor(void * param)
{
    (void) param; /*Unused*/

    lv_mem_monitor_t mon;
    lv_mem_monitor(&mon);
    printf("used: %6d (%3d %%), frag: %3d %%, biggest free: %6d\n", (int)mon.total_size - mon.free_size,
           mon.used_pct,
           mon.frag_pct,
           (int)mon.free_biggest_size);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void launcher_init(void)
{
    /* Initialization of the ESPBoy Theme (Alien Theme)*/
    theme_init();
    /* Initialization of launcher objects style */
    style_init();
    
    // Set the Theme to actual screen
    lv_cont_set_style(lv_scr_act(), th->bg);
    
    // Create "group" object
    group = lv_group_create();

    // Create the launcher header
    create_header();

    // Create the launcher tabview
    tv = lv_tabview_create(lv_scr_act(), NULL); 
    lv_obj_align(tv, header, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(header)-LV_HOR_RES)/2), 25);    
    lv_obj_set_size(tv, LV_HOR_RES, LV_VER_RES-(lv_obj_get_height(header)+lv_obj_get_height(header)));
    
    // lv_cont_set_style(tv, th->panel);
    
    lv_obj_t *tab1 = lv_tabview_add_tab(tv, SYMBOL_HOME);
    lv_obj_t *tab2 = lv_tabview_add_tab(tv, "NES");
    lv_obj_t *tab3 = lv_tabview_add_tab(tv, "SMS");
    lv_obj_t *tab4 = lv_tabview_add_tab(tv, "GB");
    lv_obj_t *tab5 = lv_tabview_add_tab(tv, "GG");

    // lv_cont_set_style(tab1, th->);
    int tabSize = lv_obj_get_height(tab1);

    /*******************
    * BACKGROUND IMAGES
    *******************/
   
   // ******** BACKGROUND STYLE ********
    static lv_style_t style_background;
    lv_style_copy(&style_background, &lv_style_transp);
    style_background.image.opa = 100;

    // ******** HOME BACKGROUND ********
    lv_obj_t *home_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(home_backgroud, &style_background);
    lv_img_set_src(home_backgroud, &espboy_wallpaper1);
    lv_obj_set_width(home_backgroud, espboy_wallpaper1.header.w*3);
    lv_obj_set_height(home_backgroud, espboy_wallpaper1.header.h*2);
    lv_obj_set_parent(home_backgroud, tab1);
    lv_obj_set_pos(home_backgroud, (espboy_wallpaper1.header.w/5), 0);

    // ******** NES BACKGROUND ********
    lv_obj_t *nes_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(nes_backgroud, &style_background);
    lv_img_set_src(nes_backgroud, &nes_icon_large);
    lv_obj_set_width(nes_backgroud, nes_icon_large.header.w);
    lv_obj_set_height(nes_backgroud, nes_icon_large.header.h);
    lv_obj_set_parent(nes_backgroud, tab2);
    lv_obj_set_pos(nes_backgroud, (LV_HOR_RES/2)-(nes_icon_large.header.w/2), -10);

    // ******** MASTERSYSTEM BACKGROUND ********
    lv_obj_t *sms_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(sms_backgroud, &style_background);
    lv_img_set_src(sms_backgroud, &mastersystem_icon_large);
    lv_obj_set_width(sms_backgroud, mastersystem_icon_large.header.w);
    lv_obj_set_height(sms_backgroud, mastersystem_icon_large.header.h);
    lv_obj_set_parent(sms_backgroud, tab3);
    lv_obj_set_pos(sms_backgroud, (LV_HOR_RES/2)-(mastersystem_icon_large.header.w/2), -10);

    // ******** GAMEBOY BACKGROUND ********
    lv_obj_t *gb_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gb_backgroud, &style_background);
    lv_img_set_src(gb_backgroud, &gameboy_icon_large);
    lv_obj_set_width(gb_backgroud, gameboy_icon_large.header.w);
    lv_obj_set_height(gb_backgroud, gameboy_icon_large.header.h);
    lv_obj_set_parent(gb_backgroud, tab4);
    lv_obj_set_pos(gb_backgroud, (LV_HOR_RES/2)-(gameboy_icon_large.header.w/2), -10);

    // ******** GAMEGEAR BACKGROUND ********
    lv_obj_t *gg_backgroud = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_style(gg_backgroud, &style_background);
    lv_img_set_src(gg_backgroud, &gamegear_icon_large);
    lv_obj_set_width(gg_backgroud, gamegear_icon_large.header.w);
    lv_obj_set_height(gg_backgroud, gamegear_icon_large.header.h);
    lv_obj_set_parent(gg_backgroud, tab5);
    lv_obj_set_pos(gg_backgroud, (LV_HOR_RES/2)-(gamegear_icon_large.header.w/2), -10);

    // ======= SET STYLE TO TABS ======= 
    lv_cont_set_style(tab1, th->bg);
    lv_cont_set_style(tab2, th->bg);
    lv_cont_set_style(tab3, th->bg);
    lv_cont_set_style(tab4, th->bg);
    lv_cont_set_style(tab5, th->bg);

    // ======= SET STYLE TO TABVIEW ======= 
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_BG, &style_tv_btn_bg);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_INDIC, &lv_style_plain);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_PR, &style_tv_btn_pr);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_REL, &style_tv_btn_rel);
    lv_tabview_set_style(tv, LV_TABVIEW_STYLE_BTN_TGL_PR, &style_tv_btn_pr);
    
    /* Crate the list1: HOME */
    list1 = lv_list_create(tab1, NULL);
    lv_obj_set_size(list1, LV_HOR_RES, tabSize);
    lv_list_set_style(list1, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list1, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list1, tab1, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| ADD Itens to list1: HOME |- */
    lv_list_add(list1, SYMBOL_FILE, "Recently Played", list_release_action);
    lv_list_add(list1, SYMBOL_SETTINGS, "Settings", list_release_action);

    /* -| Crate the list2: NES |- */
    list2 = lv_list_create(tab2, NULL);
    lv_obj_set_size(list2, LV_HOR_RES, tabSize);
    lv_list_set_style(list2, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list2, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list2, tab2, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| ADD Itens to list2: NES |- */
    lv_list_add(list2, NULL, "Test 1", list_release_action);
    lv_list_add(list2, NULL, "Test 2", list_release_action);
    lv_list_add(list2, NULL, "Test 3", list_release_action);
    lv_list_add(list2, NULL, "Test 4", list_release_action);

    /* -| Crate the list3: MASTERSYSTEM |- */
    list3 = lv_list_create(tab3, NULL);
    lv_obj_set_size(list3, LV_HOR_RES, tabSize);
    lv_list_set_style(list3, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list3, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list3, tab3, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| ADD Itens to list3: MASTERSYSTEM |- */
    lv_list_add(list3, NULL, "Test 1", list_release_action);
    lv_list_add(list3, NULL, "Test 2", list_release_action);
    lv_list_add(list3, NULL, "Test 3", list_release_action);
    lv_list_add(list3, NULL, "Test 4", list_release_action);

    /*Crate the list4: GAMEBOY */
    list4 = lv_list_create(tab4, NULL);
    lv_obj_set_size(list4, LV_HOR_RES, tabSize);
    lv_list_set_style(list4, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list4, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list4, tab4, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| ADD Itens to list4: GAMEBOY |- */
    lv_list_add(list4, NULL, "Test 1", list_release_action);
    lv_list_add(list4, NULL, "Test 2", list_release_action);
    lv_list_add(list4, NULL, "Test 3", list_release_action);
    lv_list_add(list4, NULL, "Test 4", list_release_action);

    /* -| Crate the list5: GAMEGEAR |- */
    list5 = lv_list_create(tab5, NULL);
    lv_obj_set_size(list5, LV_HOR_RES, tabSize);
    lv_list_set_style(list5, LV_LIST_STYLE_BG, &lv_style_transp_tight);
    lv_list_set_style(list5, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_align(list5, tab5, LV_ALIGN_IN_TOP_LEFT, ((lv_obj_get_width(tab1)-LV_HOR_RES)/2), 0);

    /* -| ADD Itens to list5: GAMEGEAR |- */
    lv_list_add(list5, NULL, "Test 1", list_release_action);
    lv_list_add(list5, NULL, "Test 2", list_release_action);
    lv_list_add(list5, NULL, "Test 3", list_release_action);
    lv_list_add(list5, NULL, "Test 4", list_release_action);
    
    /* -| ADD a action function to tabview |- */
    lv_tabview_set_tab_load_action(tv, tb_load_action);
    
    /* -| ADD itens to group |- */
    lv_group_add_obj(group, tv);
    lv_group_add_obj(group, list1);
    lv_group_focus_obj(list1);

    /* -| SET group to indev |- */
    lv_indev_set_group(indev, group);
}

lv_res_t tb_load_action(lv_obj_t * tabview, uint16_t act_id)
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

static lv_res_t list_release_action(lv_obj_t * list_btn)
{
    printf("List element click:%s\n", lv_list_get_btn_text(list_btn));

    return LV_RES_OK; /*Return OK because the list is not deleted*/
}