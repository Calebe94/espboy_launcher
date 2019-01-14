/** @file splash_screen.h
 * 
 * @brief This file has the purpose to create the ESPBoy Splash Screen.
 *  
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group.  All rights reserved.
 */ 
#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

// TODO: This Splash Screen should belong to MinOS and will be displayed in the startup.

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*!
 * @brief Creates and initialize the ESPBoy Splash Screen
 */
void splash_screen_init(); 

/*!
 * @brief Creates and deinitialize the ESPBoy Splash Screen
 */
void splash_screen_deinit();

#endif
/*** end of file ***/
