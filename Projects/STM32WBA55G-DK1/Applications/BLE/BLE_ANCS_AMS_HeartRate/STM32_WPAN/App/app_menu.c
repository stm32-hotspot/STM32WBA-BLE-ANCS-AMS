/**
******************************************************************************
* @file    app_menu.c
* @author  MCD Application Team
* @brief   Application interface for menu
******************************************************************************
* @attention
*
* Copyright (c) 2020-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "app_menu.h"
#include "app_conf.h"
#include <stdio.h>
#include "stm32_lcd.h"
#include "stm32wba55g_discovery_lcd.h"
#include "stm32wba55g_discovery_bus.h"
#include "stm32_seq.h"

/* External variables ------------------------------------------------------- */

/* Private defines ---------------------------------------------------------- */

/* Private variables -------------------------------------------------------- */
Menu_Page_t Menu_PagePool[MENU_MAX_PAGE];
tListNode Menu_PagePoolList;
tListNode Menu_PageActiveList;

extern uint8_t current_battery_level;
extern uint8_t current_time_hour;
extern uint8_t current_time_min;
extern Menu_Page_t *p_waitcon_menu;

Menu_Page_t *pCurrentPage;

extern uint8_t arrow_return_byteicon[];

/* Private functions prototypes-----------------------------------------------*/
static int32_t LCD_DrawBitmapArray(uint8_t xpos, uint8_t ypos, uint8_t xlen, uint8_t ylen, uint8_t *data);
static char ext_ascii_to_ascii(char p_data);
/* Exported Functions Definition -------------------------------------------- */

/**
 * @brief Initialize the Menu module
 */
void Menu_Init(void)
{
  uint8_t i;

  LST_init_head(&Menu_PagePoolList);
  LST_init_head(&Menu_PageActiveList);

  for (i = 0; i < MENU_MAX_PAGE; i++)
  {
    Menu_PagePool[i].NumEntry = 0;
    Menu_PagePool[i].SelectedEntry = 0;
    LST_insert_tail(&Menu_PagePoolList, &Menu_PagePool[i].Node);
  }
}

/**
 * @brief Remove a Menu Page
 * @param MenuType: The type of the menu page
 * @retval A pointer to the menu page created, 0 if the pool list is empty
 */
Menu_Page_t* Menu_CreatePage(Menu_Type_t MenuType)
{
  Menu_Page_t *p_menu_page;
  if (LST_is_empty(&Menu_PagePoolList) == FALSE)
  {
    LST_remove_head(&Menu_PagePoolList, (tListNode **)&p_menu_page);
    p_menu_page->MenuType = MenuType;
    LST_insert_tail(&Menu_PageActiveList, (tListNode *)p_menu_page);
    return p_menu_page;
  }
  else
  {
    return 0;
  }
}

/**
 * @brief Remove a Menu Page
 * @param pMenuPage: A pointer to the menu page
 */
void Menu_RemovePage(Menu_Page_t* pMenuPage)
{
  if (LST_is_empty(&Menu_PageActiveList) == FALSE)
  {
    LST_remove_node((tListNode *) pMenuPage);
    LST_insert_tail(&Menu_PagePoolList, (tListNode *)pMenuPage);
  }
}

/**
 * @brief Add a list entry to a List menu page
 * @param pMenuPage: A pointer to the menu page
 * @param pText: The text of the entry
 * @param Action: The Action to assign to the list entry
 */
void Menu_AddListEntry(Menu_Page_t* pMenuPage, char *pText, Menu_Action_t Action)
{
  snprintf((char *)&pMenuPage->ListEntry[pMenuPage->NumEntry].Text, MENU_LIST_ENTRY_MAX_TEXT_LEN, "%s",pText);
  pMenuPage->ListEntry[pMenuPage->NumEntry].Action = Action;
  pMenuPage->NumEntry++;
}

/**
 * @brief Set the content of a Control Menu page
 * @param pMenuPage: A pointer to the menu page
 * @param pText: A pointer to a content text structure to display
 * @param pIcon: A pointer to an icon structure to discplay
 */
void Menu_SetControlContent(Menu_Page_t* pMenuPage, Menu_Content_Text_t *pText, Menu_Icon_t* pIcon)
{
  pMenuPage->pIcon = pIcon;
  pMenuPage->pText = pText;
}

/**
 * @brief Assign an action related to a direction on a menu page
 * @param pMenuPage: A pointer to the menu page
 * @param Direction: The direction to assign the action to
 * @param Action: The Action to assign
 */
void Menu_SetControlAction(Menu_Page_t* pMenuPage, Menu_Action_Direction_t Direction, Menu_Action_t Action)
{
  switch (Direction)
  {
    case MENU_DIRECTION_LEFT:
    {
      pMenuPage->ActionLeft = Action;
      break;
    }
    case MENU_DIRECTION_RIGHT:
    {
      pMenuPage->ActionRight = Action;
      break;
    }
    case MENU_DIRECTION_DOWN:
    {
      pMenuPage->ActionDown = Action;
      break;
    }
    case MENU_DIRECTION_UP:
    {
      pMenuPage->ActionUp = Action;
      break;
    }
  }
}

/**
 * @brief Get current active page
 * @return pMenuPage: A pointer to the current menu page
 */
Menu_Page_t* Menu_GetActivePage(void)
{
  return pCurrentPage;
}

/**
 * @brief Set new page as active page
 * @param pMenuPage: A pointer to the menu page to set
 */
void Menu_SetActivePage(Menu_Page_t* pMenuPage)
{
  pCurrentPage = pMenuPage;
  Menu_Print();
}

/**
 * @brief Navigate in the menu, direction Left
 */
void Menu_Left(void)
{
  if (pCurrentPage->pReturnPage != 0)
  {
    pCurrentPage = (Menu_Page_t *) pCurrentPage->pReturnPage;
    Menu_Print();
  }
}

/**
 * @brief Navigate in the menu, direction Right
 */
void Menu_Right(void)
{
  switch (pCurrentPage->MenuType)
  {
    case MENU_TYPE_LIST:
    {
      if (pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.ActionType == MENU_ACTION_MENU_PAGE
          && pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.pPage != 0)
      {
        ((Menu_Page_t *) pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.pPage)->pReturnPage = (struct Menu_Page_t *)pCurrentPage;
        pCurrentPage = (Menu_Page_t *) pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.pPage;
        Menu_Print();
      }
      else if(pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.ActionType == MENU_ACTION_CALLBACK
              && pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.Callback != 0)
      {
        pCurrentPage->ListEntry[pCurrentPage->SelectedEntry].Action.Callback();
      }
      break;
    }
    case MENU_TYPE_CONTROL:
    {
      if (pCurrentPage->ActionRight.ActionType == MENU_ACTION_CALLBACK && pCurrentPage->ActionRight.Callback != 0)
      {
        pCurrentPage->ActionRight.Callback();
      }
      else if (pCurrentPage->ActionRight.ActionType == MENU_ACTION_MENU_PAGE && pCurrentPage->ActionRight.pPage != 0)
      {
        ((Menu_Page_t *) pCurrentPage->ActionRight.pPage)->pReturnPage = (struct Menu_Page_t *)pCurrentPage;
        pCurrentPage = (Menu_Page_t *) pCurrentPage->ActionRight.pPage;
        Menu_Print();
      }
      break;
    }
  }
}

/**
 * @brief Navigate in the menu, direction Up
 */
void Menu_Up(void)
{
  switch (pCurrentPage->MenuType)
  {
    case MENU_TYPE_LIST:
    {
      if (pCurrentPage->SelectedEntry == 0)
      {
        pCurrentPage->SelectedEntry = MENU_NUM_LIST_ENTRY - 1;
      }
      else
      {
        pCurrentPage->SelectedEntry = (pCurrentPage->SelectedEntry - 1) % MENU_NUM_LIST_ENTRY;
      }
      Menu_Print();
      break;
    }
    case MENU_TYPE_CONTROL:
    {
      if (pCurrentPage->ActionUp.Callback != 0)
      {
        pCurrentPage->ActionUp.Callback();
      }
      break;
    }
  }
}

/**
 * @brief Navigate in the menu, direction Down
 */
void Menu_Down(void)
{
  switch (pCurrentPage->MenuType)
  {
    case MENU_TYPE_LIST:
    {
      pCurrentPage->SelectedEntry = (pCurrentPage->SelectedEntry + 1) % MENU_NUM_LIST_ENTRY;
      Menu_Print();
    }
    case MENU_TYPE_CONTROL:
    {
      if (pCurrentPage->ActionDown.Callback != 0)
      {
        pCurrentPage->ActionDown.Callback();
      }
    }
  }
}

/**
 * @brief Request execution of the Print Task
 */
void Menu_Print(void)
{
  UTIL_SEQ_SetTask( 1U << CFG_TASK_MENU_PRINT_ID, CFG_SEQ_PRIO_0);
}

/**
 * @brief Print the current menu on the screen
 */
void Menu_Print_Task(void)
{
  BSP_SPI3_Init();
  BSP_LCD_Clear(0,SSD1315_COLOR_BLACK);

  switch (pCurrentPage->MenuType)
  {
    case MENU_TYPE_LIST:
    {
      uint8_t i;
      for (i = 0; i < MENU_NUM_LIST_ENTRY; i++)
      {
        if (i == pCurrentPage->SelectedEntry)
        {
          UTIL_LCD_FillRect(0, i*MENU_ENTRY_LINE_HEIGHT, SSD1315_LCD_PIXEL_WIDTH, MENU_ENTRY_LINE_HEIGHT, SSD1315_COLOR_WHITE);
          UTIL_LCD_SetTextColor(SSD1315_COLOR_BLACK);
          UTIL_LCD_SetBackColor(SSD1315_COLOR_WHITE);
          UTIL_LCD_DisplayStringAt(0, i*MENU_ENTRY_LINE_HEIGHT + MENU_ENTRY_TEXT_X_OFFSET, (uint8_t *) ">", RIGHT_MODE);
        }
        else
        {
          UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
          UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);
        }
        UTIL_LCD_DisplayStringAt(0, i*MENU_ENTRY_LINE_HEIGHT + MENU_ENTRY_TEXT_X_OFFSET,
                                 (uint8_t *) pCurrentPage->ListEntry[i].Text, CENTER_MODE);
      }
      break;
    }

    case MENU_TYPE_CONTROL:
    {
      uint8_t num_lines = 0;

      UTIL_LCD_SetTextColor(SSD1315_COLOR_WHITE);
      UTIL_LCD_SetBackColor(SSD1315_COLOR_BLACK);

      if (pCurrentPage->pIcon != 0)
      {
        num_lines++;
      }

      num_lines += pCurrentPage->pText->NumLines;

      if (num_lines == 1)
      {
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 6,
                                 (uint8_t *) pCurrentPage->pText->Lines[0], CENTER_MODE);
      }
      else if (num_lines == 2)
      {
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 14,
                                 (uint8_t *) pCurrentPage->pText->Lines[0], CENTER_MODE);
        if (pCurrentPage->pIcon == 0)
        {
          UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 2,
                                   (uint8_t *) pCurrentPage->pText->Lines[1], CENTER_MODE);
        }
        else
        {
          if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_IMAGE)
          {
            LCD_DrawBitmapArray((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - pCurrentPage->pIcon->Image_width/2,
                                (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2), 
                                pCurrentPage->pIcon->Image_width, 
                                pCurrentPage->pIcon->Image_height, 
                                pCurrentPage->pIcon->pImage);
          }
          else if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_CHAR)
          {
            UTIL_LCD_DisplayChar((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 3,
                                 (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 2, pCurrentPage->pIcon->Character);
          }
        }
      }
      else if (num_lines == 3)
      {
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 22,
                                 (uint8_t *) pCurrentPage->pText->Lines[0], CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 6,
                                 (uint8_t *) pCurrentPage->pText->Lines[1], CENTER_MODE);
        if (pCurrentPage->pIcon == 0)
        {
          UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 10,
                                   (uint8_t *) pCurrentPage->pText->Lines[2], CENTER_MODE);
        }
        else
        {
          if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_IMAGE)
          {
            LCD_DrawBitmapArray((uint8_t) SSD1315_LCD_PIXEL_WIDTH  / 2 - pCurrentPage->pIcon->Image_width/2,
                                (uint8_t) SSD1315_LCD_PIXEL_HEIGHT / 2 + pCurrentPage->pIcon->Image_height/2,
                                pCurrentPage->pIcon->Image_width, 
                                pCurrentPage->pIcon->Image_height,                                 
                                pCurrentPage->pIcon->pImage);
          }
          else if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_CHAR)
          {
            UTIL_LCD_DisplayChar((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 3,
                                 (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 10, pCurrentPage->pIcon->Character);
          }
        }
      }
      else if (num_lines == 4)
      {
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 30,
                                 (uint8_t *) pCurrentPage->pText->Lines[0], CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 14,
                                 (uint8_t *) pCurrentPage->pText->Lines[1], CENTER_MODE);
        UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 2,
                                 (uint8_t *) pCurrentPage->pText->Lines[2], CENTER_MODE);
        if (pCurrentPage->pIcon == 0)
        {
          UTIL_LCD_DisplayStringAt(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 18,
                                   (uint8_t *) pCurrentPage->pText->Lines[3], CENTER_MODE);
        }
        else
        {
          if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_IMAGE)
          {
            LCD_DrawBitmapArray((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - pCurrentPage->pIcon->Image_width/2,
                                (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + pCurrentPage->pIcon->Image_height, 
                                pCurrentPage->pIcon->Image_width, 
                                pCurrentPage->pIcon->Image_height,  
                                pCurrentPage->pIcon->pImage);
          }
          else if (pCurrentPage->pIcon->IconType == MENU_ICON_TYPE_CHAR)
          {
            UTIL_LCD_DisplayChar((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 3,
                                 (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) + 18, pCurrentPage->pIcon->Character);
          }
        }
      }
      else
      {
        /* Error */
      }

      if ((pCurrentPage->ActionRight.ActionType == MENU_ACTION_CALLBACK && pCurrentPage->ActionRight.Callback != 0)
          || (pCurrentPage->ActionRight.ActionType == MENU_ACTION_MENU_PAGE && pCurrentPage->ActionRight.pPage != 0))
      {
        if (pCurrentPage->ActionRight.pIcon->IconType == MENU_ICON_TYPE_CHAR)
        {
          UTIL_LCD_DisplayChar(SSD1315_LCD_PIXEL_WIDTH - 7, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 6,
                               pCurrentPage->ActionRight.pIcon->Character);
        }
        else if (pCurrentPage->ActionRight.pIcon->IconType == MENU_ICON_TYPE_IMAGE)
        {
          LCD_DrawBitmapArray(SSD1315_LCD_PIXEL_WIDTH - pCurrentPage->ActionRight.pIcon->Image_width, 
                              (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - pCurrentPage->ActionRight.pIcon->Image_height/2, 
                              pCurrentPage->ActionRight.pIcon->Image_width, 
                              pCurrentPage->ActionRight.pIcon->Image_height,  
                              pCurrentPage->ActionRight.pIcon->pImage);
        }
      }

      if ((pCurrentPage->ActionUp.ActionType == MENU_ACTION_CALLBACK && pCurrentPage->ActionUp.Callback != 0)
          || (pCurrentPage->ActionUp.ActionType == MENU_ACTION_MENU_PAGE && pCurrentPage->ActionUp.pPage != 0))
      {
        if (pCurrentPage->ActionUp.pIcon->IconType == MENU_ICON_TYPE_CHAR)
        {
          UTIL_LCD_DisplayChar((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 3, 0, pCurrentPage->ActionUp.pIcon->Character);
        }
        else if (pCurrentPage->ActionUp.pIcon->IconType == MENU_ICON_TYPE_IMAGE)
        {
          LCD_DrawBitmapArray((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - pCurrentPage->ActionUp.pIcon->Image_width/2, 
                              0, 
                              pCurrentPage->ActionUp.pIcon->Image_width, 
                              pCurrentPage->ActionUp.pIcon->Image_height,
                              pCurrentPage->ActionUp.pIcon->pImage);
        }
      }

      if ((pCurrentPage->ActionDown.ActionType == MENU_ACTION_CALLBACK && pCurrentPage->ActionDown.Callback != 0)
          || (pCurrentPage->ActionDown.ActionType == MENU_ACTION_MENU_PAGE && pCurrentPage->ActionDown.pPage != 0))
      {
        if (pCurrentPage->ActionDown.pIcon->IconType == MENU_ICON_TYPE_CHAR)
        {
          UTIL_LCD_DisplayChar((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 3, SSD1315_LCD_PIXEL_HEIGHT - 12,
                               pCurrentPage->ActionDown.pIcon->Character);
        }
        else if (pCurrentPage->ActionDown.pIcon->IconType == MENU_ICON_TYPE_IMAGE)
        {
          LCD_DrawBitmapArray((uint8_t) SSD1315_LCD_PIXEL_WIDTH / 2 - 8, SSD1315_LCD_PIXEL_HEIGHT - pCurrentPage->ActionDown.pIcon->Image_height, 
                              pCurrentPage->ActionDown.pIcon->Image_width, 
                              pCurrentPage->ActionDown.pIcon->Image_height,
                              pCurrentPage->ActionDown.pIcon->pImage);
        }
      }

      if ((pCurrentPage->ActionLeft.ActionType == MENU_ACTION_CALLBACK && pCurrentPage->ActionLeft.Callback != 0)
          || (pCurrentPage->ActionLeft.ActionType == MENU_ACTION_MENU_PAGE && pCurrentPage->ActionLeft.pPage != 0))
      {
        if (pCurrentPage->ActionLeft.pIcon->IconType == MENU_ICON_TYPE_CHAR)
        {
          UTIL_LCD_DisplayChar(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 6, pCurrentPage->ActionLeft.pIcon->Character);
        }
        else if (pCurrentPage->ActionLeft.pIcon->IconType == MENU_ICON_TYPE_IMAGE)
        {
          LCD_DrawBitmapArray(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - pCurrentPage->ActionLeft.pIcon->Image_height/2, 
                              pCurrentPage->ActionLeft.pIcon->Image_width, 
                              pCurrentPage->ActionLeft.pIcon->Image_height,                              
                              pCurrentPage->ActionLeft.pIcon->pImage);
        }
      }
      else if (pCurrentPage->pReturnPage != 0)
      {
        //UTIL_LCD_DisplayChar(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 6, '<');
        LCD_DrawBitmapArray(0, (uint8_t) (SSD1315_LCD_PIXEL_HEIGHT / 2) - 8/2, 
                    8, 
                    8,                              
                    arrow_return_byteicon);
        
      }
      
      if (pCurrentPage != p_waitcon_menu)
      {
        if (current_battery_level == 100)
        {
          UTIL_LCD_DisplayChar((uint8_t) 128-29, 0, '1');
          UTIL_LCD_DisplayChar((uint8_t) 128-22, 0, '0');
          UTIL_LCD_DisplayChar((uint8_t) 128-15, 0, '0');
        }
        else if (current_battery_level >= 10)
        {        
          UTIL_LCD_DisplayChar((uint8_t) 128-22, 0, (current_battery_level/10) + '0');
          UTIL_LCD_DisplayChar((uint8_t) 128-15, 0, (current_battery_level%10) + '0');
        }
        else
        {        
          UTIL_LCD_DisplayChar((uint8_t) 128-15, 0, (current_battery_level) + '0');
        }
        
        UTIL_LCD_DisplayChar((uint8_t) 128-6 , 0, '%');
        
        if (current_time_hour == 0)
        {
          UTIL_LCD_DisplayChar((uint8_t) 3, 0, '0');
          UTIL_LCD_DisplayChar((uint8_t) 10, 0, '0');
        }
        else if (current_time_hour >= 10)
        {        
          UTIL_LCD_DisplayChar((uint8_t)  3, 0, (current_time_hour/10) + '0');
          UTIL_LCD_DisplayChar((uint8_t) 10, 0, (current_time_hour%10) + '0');
        }
        else
        { 
          UTIL_LCD_DisplayChar((uint8_t) 3, 0, '0');
          UTIL_LCD_DisplayChar((uint8_t) 10, 0, (current_time_hour) + '0');
        }
        UTIL_LCD_DisplayChar((uint8_t) 17 , 0, ':');      
        if (current_time_min == 0)
        {
          UTIL_LCD_DisplayChar((uint8_t) 24, 0, '0');
          UTIL_LCD_DisplayChar((uint8_t) 31, 0, '0');
        }
        else if (current_time_min >= 10)
        {        
          UTIL_LCD_DisplayChar((uint8_t) 24, 0, (current_time_min/10) + '0');
          UTIL_LCD_DisplayChar((uint8_t) 31, 0, (current_time_min%10) + '0');
        }
        else
        { 
          UTIL_LCD_DisplayChar((uint8_t) 24, 0, '0');
          UTIL_LCD_DisplayChar((uint8_t) 31, 0, (current_time_min) + '0');
        }
      }
      break;
    }
  }

  BSP_LCD_Refresh(0);
  BSP_SPI3_DeInit();
}

/* Private Functions Definition --------------------------------------------- */

/**
 * @brief Draw an array of bits at the specified offsets starting from corner top left. Ensure xlen is multiple of 8
 * @param xpos: X coordinate to print at
 * @param ypos: Y coordinate to print at
 * @param xlen: Width of the bitmap array
 * @param ylen: Height of the bitmap array
 * @param data: Pointer to the bitmap array
 * @retval 0 if success, -1 if the coordinates are out of screen
 */
static int32_t LCD_DrawBitmapArray(uint8_t xpos, uint8_t ypos, uint8_t xlen, uint8_t ylen, uint8_t *data){

  int32_t i,j,k;
  uint8_t mask;
  uint8_t* pdata = data;

  if (((xpos+xlen) > 128) || ((ypos+ylen) > 64))
  {
    /*out of screen*/
    return -1;
  }

  for (j=0 ; j < ylen ; j++)
  {
    for (i=0 ; i < xlen/8 ; i++)
    {
      mask = 0x80;
      for (k=0 ; k < 8 ; k++)
      {
        if ( mask & *pdata)
        {
          UTIL_LCD_SetPixel(i*8+k+xpos, j+ypos, SSD1315_COLOR_WHITE);
        }
        else
        {
          UTIL_LCD_SetPixel(i*8+k+xpos, j+ypos, SSD1315_COLOR_BLACK);
        }
        mask = mask >> 1;
      }
      pdata++;
    }
  }

  //BSP_LCD_Refresh(0);

  return 0;
}

void cpy_ext_utf8_data(char *p_data, char *ascii_converted_data, uint8_t l_data)
{
  char converted_extended_ascii_to_utf8;
  uint8_t ascii_index = 0;
    
  for (uint8_t index = 0; index < l_data;)
  {
    if ( (p_data[index] & 0x80) == 0)
    {
      ascii_converted_data[ascii_index] = p_data[index];
      
      index++;
      ascii_index ++;
    }

    else if ( (p_data[index] & 0xF0) == 0xF0)   //Extended ascii char detected on 4 bytes => 0b11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    {
      ascii_converted_data[ascii_index] = '?';   //this shouldn't happens

      ascii_index++;      
      index += 4;         
    }

    else if ( (p_data[index] & 0xE0) == 0xE0)   //Extended ascii char detected on 3 bytes => 0b1110xxxx 10xxxxxx 10xxxxxx
    {
      ascii_converted_data[ascii_index] = '?';   //this shouldn't happens

      ascii_index++;      
      index += 3;      
    }

    else if ( (p_data[index] & 0xC0) == 0xC0)   //Extended ascii char detected on 2 bytes => 0b110xxxXX 10XXXXXX   (with x unused data and X usefull data)
    {
      converted_extended_ascii_to_utf8 = (p_data[index] & 0x3) << 6;
      converted_extended_ascii_to_utf8 |= p_data[index+1] & 0x3F;
      
      ascii_converted_data[ascii_index] = ext_ascii_to_ascii(converted_extended_ascii_to_utf8);
        
      index += 2;
      ascii_index++;
    }
    
    else if ( (p_data[index] & 0xC0) == 0x80)
    {
      ascii_converted_data[ascii_index] = '?';   //this shouldn't happens
      
      index++;
      ascii_index++;
    }        
  }
}

static char ext_ascii_to_ascii(char p_data)
{
  switch(p_data)
  {
  case 0xC0 : // 'À'
  case 0xC1 : // 'Á'
  case 0xC2 : // 'Â'
  case 0xC3 : // 'Ã'
  case 0xC4 : // 'Ä'      
  case 0xC5 : // 'Å'
    return 'A';

  case 0xC7 : // 'Ç'      
    return 'C';
    
  case 0xC8 : // 'È'
  case 0xC9 : // 'É'      
  case 0xCA : // 'Ê'
  case 0xCB : // 'Ë'
    return 'E';
    
  case 0xCC : // 'Ì'
  case 0xCD : // 'Í'      
  case 0xCE : // 'Î'
  case 0xCF : // 'Ï'
    return 'I';
    
  case 0xD0 : // 'Ð'
    return 'D';
    
  case 0xD1 : // 'Ñ'
    return 'N';
    
  case 0xD2 : // 'Ò'
  case 0xD3 : // 'Ó'
  case 0xD4 : // 'Ô'
  case 0xD5 : // 'Õ'
  case 0xD6 : // 'Ö'
    return 'O';
    
  case 0xD9 : // 'Ù'
  case 0xDA : // 'Ú'
  case 0xDB : // 'Û'
  case 0xDC : // 'Ü'
    return 'U';
    
  case 0xDD : // 'Ý'
    return 'Y';
    
  case 0xDF : // 'ß'
    return 'B';
    
  case 0xE0 : // 'à'
  case 0xE1 : // 'á'
  case 0xE2 : // 'â'
  case 0xE3 : // 'ã'
  case 0xE4 : // 'ä'
  case 0xE5 : // 'å'
    return 'a';

  case 0xE7 : // 'ç'
    return 'c';

  case 0xE8 : // 'è'
  case 0xE9 : // 'é'
  case 0xEA : // 'ê'
  case 0xEB : // 'ë'
    return 'e';
    
  case 0xEC : // 'ì'
  case 0xED : // 'í'
  case 0xEE : // 'î'
  case 0xEF : // 'ï'
    return 'i';

  case 0xF1 : // 'ñ'
    return 'n';

  case 0xF0 : // 'ð'
  case 0xF2 : // 'ò'
  case 0xF3 : // 'ó'
  case 0xF4 : // 'ô'
  case 0xF5 : // 'õ'
  case 0xF6 : // 'ö'
    return 'o';
    
  case 0xF9 : // 'ù'
  case 0xFA : // 'ú'
  case 0xFB : // 'û'
  case 0xFC : // 'ü'
    return 'u';
    
  case 0xFD : // 'ý'
  case 0xFF : // 'ÿ'      
    return 'y';
    
  default :
    return '?';
  }
}