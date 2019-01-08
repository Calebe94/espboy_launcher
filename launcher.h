/** @file launcher.h
 * 
 * @brief This module has the purpose to create the ESPBoy's game launcher. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group.  All rights reserved.
 */ 

#ifndef LAUNCHER_H
#define LAUNCHER_H

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
#include "lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      MACROS
 **********************/

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

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void launcher_create();

void launcher_update();


#endif /* LAUNCHER_H */

/*** end of file ***/
