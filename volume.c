/** @file launcher.c
 * 
 * @brief This module has the purpose to create the ESPBoy's game launcher. 
 *
 * @par       
 * COPYRIGHT NOTICE: (c) 2018 Barr Group. All rights reserved.
 */

/**************
 *   INCLUDES
 **************/

#include <stdbool.h>
#include "lvgl/lvgl.h"
#include "volume.h"
#include "launcher_config.h"

/**********************
 *   GLOBAL VARIABLES
 **********************/

/********************************
 *   STATIC FUNCTIONS PROTOTYPES
 ********************************/


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
lv_res_t show_volume_control(lv_style_t *style_base,  uint8_t volume)
{   
    char * volume_symbol = return_volume_symbol(volume);
    if( volume_symbol == NULL)
    {
        return LV_RES_INV;
    }

    static lv_style_t style;
    lv_style_copy(&style, style_base);
    style.text.font = &lv_font_dejavu_40;   
    // CREATE THE MESSAGE BOX
    lv_obj_t * volume_mbox = lv_mbox_create(lv_scr_act(), NULL);
    lv_obj_set_style(volume_mbox, &style);
    // ADD A BAR TO INDICATE THE VOLUME LEVEL 
    lv_obj_t * bar1 = lv_bar_create(volume_mbox, NULL);
    lv_obj_set_size(bar1, 100, 20);
    lv_obj_align(bar1, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 20);
    lv_bar_set_value(bar1, volume);
    // SET MESSAGE BOX TEXT
    lv_mbox_set_text(volume_mbox, volume_symbol);
    lv_obj_align(volume_mbox, NULL, LV_ALIGN_IN_TOP_MID, 10, 70); /*Align to the corner*/
        
    lv_mbox_start_auto_close(volume_mbox, 1500);

    return LV_RES_OK;
}

/*!
 * @brief Check the actual volume and returns the corresponding symbol
 *
 * @param[in] volume wich goes from 0(mute) to 100(max)
 *
 * @return a char * which represents the symbol.
 */
char * return_volume_symbol(uint8_t volume)
{
    if( (volume>(0)) && (volume<20) )
    {
        return SYMBOL_MUTE;
    }
    else if( (volume>19) && (volume<80) )
    {
        return SYMBOL_VOLUME_MID;
    }
    else if( (volume>79) && (volume<101) )
    {
        return SYMBOL_VOLUME_MAX;
    }
    else
    {
        return NULL;
    }
}



/**********************
 *   STATIC FUNCTIONS
 **********************/
