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
#define USE_BACKGROUND      1

/**********************
 *   STATIC VARIABLES
 **********************/
/* OBJECTS */
static lv_obj_t * preload;
static lv_obj_t * background;

/* STYLES */
static lv_style_t style_background;
static lv_style_t style_preload;

/********************************
 *   STATIC FUNCTIONS PROTOTYPES
 *******************************/
/* STYLES */
static void styles_init(void);

static void set_styles(void);

/* OBJECTS */
static void create_splashscreen(void);

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*!
 * @brief Creates and initialize the ESPBoy Splash Screen
 */
void splash_screen_init(void *this)
{
    styles_init();

    create_splashscreen();

    set_styles();
} 

/*!
 * @brief Creates and deinitialize the ESPBoy Splash Screen
 */
void splash_screen_deinit(void *this)
{
    lv_obj_clean(preload);
    lv_obj_clean(background);
    lv_obj_del(preload);
    lv_obj_del(background);
}


/*********************
 *   STATIC FUNCTIONS 
 *********************/
/* STYLES */
static void styles_init(void)
{
    /* PRELOAD STYLE */
    lv_style_copy(&style_preload, &lv_style_plain);
    style_preload.body.padding.hor = 0;
    style_preload.line.color = LV_COLOR_BLACK;
    style_preload.body.border.color = LV_COLOR_GRAY; 

    #if USE_BACKGROUND
        /* BACKGROUND STYLE */
        lv_style_copy(&style_background, &lv_style_plain);
        style_background.body.opa = LV_OPA_10;
        /* PRELOAD STYLE */
        style_preload.line.width = 14;
        style_preload.body.border.width = 14;
    #else
        style_preload.line.width = 15;
        style_preload.body.border.width = 15;
    #endif

}

static void set_styles(void)
{
    #if  USE_BACKGROUND
        lv_obj_set_style(background, &style_background);
    #endif

    lv_preload_set_style(preload, LV_PRELOAD_STYLE_MAIN, &style_preload);
}

/* OBJECTS */
static void create_splashscreen(void)
{
    #if USE_BACKGROUND
        /* CREATE BACKGROUND IMAGE */
        background = lv_img_create(lv_scr_act(), NULL);
        preload = lv_preload_create(lv_scr_act(), NULL);
        lv_img_set_src(background, &minos_logo);
        lv_obj_set_width(background, minos_logo.header.w);
        lv_obj_set_height(background, minos_logo.header.h);
        lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);
        /*Create a Preloader object*/
        lv_obj_set_size(preload, 50, 50);
        lv_obj_align(preload, NULL, LV_ALIGN_IN_TOP_RIGHT, -104, 24);
    #else
        preload = lv_preload_create(lv_scr_act(), NULL);
        lv_obj_set_size(preload, 115, 115);
        lv_obj_align(preload, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_t * label = lv_label_create(lv_scr_act(), NULL);
        lv_label_set_text(label, "Loading...");
        lv_obj_align(label, preload, LV_ALIGN_CENTER, 0,0);
    #endif
}


/*** end of file ***/
