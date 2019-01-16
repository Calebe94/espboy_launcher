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
#include <lvgl.h>

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void launcher_init(void *);

void launcher_deinit(void *);

void launcher_update(void *);

#endif /* LAUNCHER_H */

/*** end of file ***/
