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
#include "lvgl/lvgl.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void launcher_init(lv_indev_t *);

void launcher_update();

#endif /* LAUNCHER_H */

/*** end of file ***/
