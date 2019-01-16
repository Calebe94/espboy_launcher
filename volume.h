/** @file volume.h
 * 
 * @brief This module has the purpose to create the launcher audio volume visualization. 
 *  
 * It consists of one LittleVGL Message Box with a label and a bar.
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group.  All rights reserved.
 */ 
#ifndef VOLUME_H
#define VOLUME_H

/**************
 *   INCLUDES
 **************/

#include <stdint.h>
#include "lv_obj.h"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/*!
 * @brief Creates and initialize the Volume indicator Box
 *
 * @param[in] volume wich goes from 0(mute) to 100(max)
 *
 * @return LV_RES_OK if the Volume Indicator was showed or LV_RES_INV if the box is being shown.
 */
lv_res_t show_volume_control(lv_style_t *,  uint8_t );

/*!
 * @brief Check the actual volume and returns the corresponding symbol
 *
 * @param[in] volume wich goes from 0(mute) to 100(max)
 *
 * @return a char * which represents the symbol.
 */
char * return_volume_symbol(uint8_t volume);

#endif /* VOLUME_H */

/*** end of file ***/