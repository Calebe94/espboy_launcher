/** @file splash_screen.c
 * 
 * @brief This file has the purpose to create the ESPBoy Splash Screen.
 *  
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group.  All rights reserved.
 */ 

// TODO: This Splash Screen should belong to MinOS and will be displayed in the startup.

/**********************
 *   INCLUDES
 **********************/
#include "lvgl/lvgl.h"
#include "splash_screen.h"

#include "minos_logo.h"
/**********************
 *   MACROS
 **********************/
#define USE_ANIMATED_ARC 0
/**********************
 *   GLOBAL VARIABLES
 **********************/
lv_obj_t * arc;

/**********************
 *   STATIC FUNCTIONS
 **********************/
// void lv_obj_set_height(lv_obj_t * obj, lv_coord_t h);

static void lv_obj_set_angle(lv_obj_t * obj, int value)
{
    lv_arc_set_angles(obj, 90, value);
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*!
 * @brief Creates and initialize the ESPBoy Splash Screen
 */
void splash_screen_init()
{
    lv_obj_t * splash_screen_mbox = lv_mbox_create(lv_scr_act(), NULL);
    lv_obj_align(splash_screen_mbox, NULL, LV_ALIGN_CENTER, 0, -50);
    lv_obj_set_style(splash_screen_mbox, &lv_style_plain);
    lv_mbox_set_text(splash_screen_mbox, "MinOS Loading...");

    lv_obj_t * img1 = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img1, &minos_logo);
    lv_obj_set_width(img1, minos_logo.header.w);
    lv_obj_set_height(img1, minos_logo.header.h);
    lv_obj_align(img1, NULL, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t style_img3;
    lv_style_copy(&style_img3, &lv_style_plain);
    // style_img3.image.color = LV_COLOR_HEX(0x003b75);
    style_img3.image.intense = LV_OPA_90;

    lv_obj_set_style(img1, &style_img3);

    #if USE_ANIMATED_ARC
        static lv_style_t style;
        lv_style_copy(&style, &lv_style_plain);
        style.line.color = LV_COLOR_BLUE;           /*Arc color*/
        style.line.width = 8;                       /*Arc width*/


        /*Copy the previous Arc and set different angles and size*/
        arc = lv_arc_create(splash_screen_mbox, NULL);
        lv_arc_set_style(arc, LV_ARC_STYLE_MAIN, &style);          /*Use the new style*/
        lv_arc_set_angles(arc, 90, 0);
        lv_obj_set_size(arc, 100, 100);
        lv_obj_align(arc, splash_screen_mbox, LV_ALIGN_CENTER, 0, 0);

        lv_anim_t a;                                
        a.var = arc;                             /*Variable to animate*/                        
        a.start = 0;                                 /*Start value*/
        a.end = 360;                                 /*End value*/
        a.fp = (lv_anim_fp_t)lv_obj_set_angle;                                 /*Function to be used to animate*/
        a.path = lv_anim_path_linear;                                            /*Path of animation*/
        a.end_cb = NULL;                               /*Callback when the animation is ready*/
        a.act_time = 0;                                 /*Set < 0 to make a delay [ms]*/
        a.time = 2000;                                 /*Animation length [ms]*/
        a.playback = 0;                                 /*1: animate in reverse direction too when the normal is ready*/
        a.playback_pause = 0;                                 /*Wait before playback [ms]*/
        a.repeat = 1;                                 /*1: Repeat the animation (with or without playback)*/
        a.repeat_pause = 0;                             /*Wait before repeat [ms]*/

        lv_anim_create(&a);                          /*Start the animation*/
    #else
        static lv_style_t style;
        lv_style_copy(&style, &lv_style_plain);
        style.line.width = 10;                         /*10 px thick arc*/
        style.line.color = LV_COLOR_HEX3(0x258);       /*Blueish arc color*/

        style.body.border.color = LV_COLOR_HEX3(0xBBB); /*Gray background color*/
        style.body.border.width = 10;
        style.body.padding.hor = 0;

        /*Create a Preloader object*/
        lv_obj_t * preload = lv_preload_create(splash_screen_mbox, NULL);
        lv_obj_set_size(preload, 100, 100);
        lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_preload_set_style(preload, LV_PRELOAD_STYLE_MAIN, &style);
    #endif
} 

/*!
 * @brief Creates and deinitialize the ESPBoy Splash Screen
 */
void splash_screen_deinit();

/*** end of file ***/
