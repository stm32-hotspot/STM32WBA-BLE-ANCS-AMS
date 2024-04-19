/**
******************************************************************************
* @file    app_menu_cfg.c
* @author  MCD Application Team
* @brief   Configuration interface of menu for application
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
#include "ams_app.h"
#include "ancs_app.h"

/* External variables ------------------------------------------------------- */
extern uint8_t volume_mute_byteicon[];
extern uint8_t volume_up_byteicon[];
extern uint8_t volume_down_byteicon[];
extern uint8_t initiate_call_byteicon[];
extern uint8_t answer_call_byteicon[];
extern uint8_t terminate_call_byteicon[];
extern uint8_t play_pause_byteicon[];
extern uint8_t next_track_byteicon[];
extern uint8_t previous_track_byteicon[];
extern uint8_t advertising_byteicon[];
extern uint8_t hourglass_byteicon[];
extern uint8_t arrow_up_byteicon[];
extern uint8_t arrow_down_byteicon[];
extern uint8_t empty_heart_byteicon[];
extern uint8_t full_heart_byteicon[];
/* Private defines ---------------------------------------------------------- */

/* Private variables -------------------------------------------------------- */
Menu_Icon_t access_menu_icon    = { MENU_ICON_TYPE_CHAR, 0, '>'};
Menu_Icon_t notif_menu_up       = { MENU_ICON_TYPE_CHAR, 0, ' '};
Menu_Icon_t notif_menu_done     = { MENU_ICON_TYPE_CHAR, 0, ' '};
Menu_Icon_t volume_up_icon      = {MENU_ICON_TYPE_IMAGE, (uint8_t *)      &volume_up_byteicon, 0, 16, 16};
Menu_Icon_t volume_down_icon    = {MENU_ICON_TYPE_IMAGE, (uint8_t *)    &volume_down_byteicon, 0, 16, 16};
Menu_Icon_t volume_mute_icon    = {MENU_ICON_TYPE_IMAGE, (uint8_t *)    &volume_mute_byteicon, 0, 16, 16};
//Menu_Icon_t initiate_call_icon  = {MENU_ICON_TYPE_IMAGE, (uint8_t *)  &initiate_call_byteicon, 0, 16, 16};
Menu_Icon_t answer_call_icon    = {MENU_ICON_TYPE_IMAGE, (uint8_t *)    &answer_call_byteicon, 0, 16, 16};
Menu_Icon_t terminate_call_icon = {MENU_ICON_TYPE_IMAGE, (uint8_t *) &terminate_call_byteicon, 0, 16, 16};
Menu_Icon_t next_track_icon     = {MENU_ICON_TYPE_IMAGE, (uint8_t *)     &next_track_byteicon, 0, 16, 16};
Menu_Icon_t play_pause_icon     = {MENU_ICON_TYPE_IMAGE, (uint8_t *)     &play_pause_byteicon, 0, 16, 16};
Menu_Icon_t previous_track_icon = {MENU_ICON_TYPE_IMAGE, (uint8_t *) &previous_track_byteicon, 0, 16, 16};
Menu_Icon_t advertising_icon    = {MENU_ICON_TYPE_IMAGE, (uint8_t *)    &advertising_byteicon, 0, 16, 16};
Menu_Icon_t hourglass_icon      = {MENU_ICON_TYPE_IMAGE, (uint8_t *)      &hourglass_byteicon, 0, 16, 16};
Menu_Icon_t arrow_up_icon       = {MENU_ICON_TYPE_IMAGE, (uint8_t *)       &arrow_up_byteicon, 0, 16,  6};
Menu_Icon_t arrow_down_icon     = {MENU_ICON_TYPE_IMAGE, (uint8_t *)     &arrow_down_byteicon, 0, 16,  6};
Menu_Icon_t empty_heart_icon    = {MENU_ICON_TYPE_IMAGE, (uint8_t *)    &empty_heart_byteicon, 0, 32, 32};
Menu_Icon_t full_heart_icon     = {MENU_ICON_TYPE_IMAGE, (uint8_t *)     &full_heart_byteicon, 0, 32, 32};

Menu_Content_Text_t waitcon_text = {1, {"Wait Connection"}};
Menu_Content_Text_t connecting_text = {1, {"Connecting"}};
Menu_Content_Text_t connected_text = {1, {"Connected"}};
Menu_Content_Text_t volume_text = {1, {"  0%"}};
Menu_Content_Text_t call_text = {2, {"Call", "Control"}};
Menu_Content_Text_t media_text = {2, {"    Media    ", "   Control   "}};
Menu_Content_Text_t notif_display_text = {3, {"00/00", "No", "Notif"}};
Menu_Content_Text_t notif_control_text = {2, {"No Action", "To Do"}};
Menu_Content_Text_t hrs_text = {1, {"  xxbpm - xxxkcal"}};

Menu_Page_t *p_waitcon_menu;
Menu_Page_t *p_connecting_menu;
Menu_Page_t *p_connected_menu;
Menu_Page_t *p_control_menu;
Menu_Page_t *p_volume_control_menu;
Menu_Page_t *p_call_control_menu;
Menu_Page_t *p_media_control_menu;
Menu_Page_t *p_notif_display_menu;
Menu_Page_t *p_notif_control_menu;
Menu_Page_t *p_hrs_menu;

/* Private functions prototypes-----------------------------------------------*/
static void Menu_Volume_Up(void);
static void Menu_Volume_Down(void);
static void Menu_Call_Answer(void);
static void Menu_Call_Terminate(void);
static void Menu_Media_NextTrack(void);
static void Menu_Media_PlayPause(void);
static void Menu_Media_PreviousTrack(void);
static void Menu_Notif_Display_Previous(void);
static void Menu_Notif_Display_Next(void);
static void Menu_Notif_Control_Up(void);
static void Menu_Notif_Control_Down(void);
/* Exported Functions Definition -------------------------------------------- */
/**
 * @brief Initialize and setup the menu
 */
void Menu_Config(void)
{
  Menu_Action_t access_menu_action = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};

  Menu_Action_t entry_menu_volume_action = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};
  //Menu_Action_t entry_menu_call_action = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};
  Menu_Action_t entry_menu_media_action = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};
  Menu_Action_t entry_menu_notif_display = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};
  Menu_Action_t entry_menu_hrs = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};

  Menu_Action_t volume_control_up_action = {MENU_ACTION_CALLBACK, &Menu_Volume_Up, 0, &volume_up_icon};
  Menu_Action_t volume_control_down_action = {MENU_ACTION_CALLBACK, &Menu_Volume_Down, 0, &volume_down_icon};
  
  Menu_Action_t call_control_up_action = {MENU_ACTION_CALLBACK, &Menu_Call_Answer, 0, &answer_call_icon};
  Menu_Action_t call_control_down_action = {MENU_ACTION_CALLBACK, &Menu_Call_Terminate, 0, &terminate_call_icon};

  Menu_Action_t media_control_up_action = {MENU_ACTION_CALLBACK, &Menu_Media_NextTrack, 0, &next_track_icon};
  Menu_Action_t media_control_down_action = {MENU_ACTION_CALLBACK, &Menu_Media_PreviousTrack, 0, &previous_track_icon};
  Menu_Action_t media_control_right_action = {MENU_ACTION_CALLBACK, &Menu_Media_PlayPause, 0, &play_pause_icon};
  
  Menu_Action_t notif_display_up_action = {MENU_ACTION_CALLBACK, &Menu_Notif_Display_Previous, 0, &arrow_up_icon};
  Menu_Action_t notif_display_down_action = {MENU_ACTION_CALLBACK, &Menu_Notif_Display_Next, 0, &arrow_down_icon};
  Menu_Action_t notif_display_right_action = {MENU_ACTION_MENU_PAGE, 0, 0, &access_menu_icon};

  Menu_Action_t notif_control_up_action = {MENU_ACTION_CALLBACK, &Menu_Notif_Control_Up, 0, &arrow_up_icon};
  Menu_Action_t notif_control_down_action = {MENU_ACTION_CALLBACK, &Menu_Notif_Control_Down, 0, &arrow_down_icon};

  Menu_Init();

  p_waitcon_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_connecting_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_connected_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_control_menu = Menu_CreatePage(MENU_TYPE_LIST);
  p_volume_control_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_call_control_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_media_control_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_notif_display_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_notif_control_menu = Menu_CreatePage(MENU_TYPE_CONTROL);
  p_hrs_menu = Menu_CreatePage(MENU_TYPE_CONTROL);

  access_menu_action.pPage = (struct Menu_Page_t*) p_control_menu;
  entry_menu_volume_action.pPage = (struct Menu_Page_t*) p_volume_control_menu;
//  entry_menu_call_action.pPage = (struct Menu_Page_t*) p_call_control_menu;
  entry_menu_media_action.pPage = (struct Menu_Page_t*) p_media_control_menu;
  entry_menu_notif_display.pPage = (struct Menu_Page_t*) p_notif_display_menu;
  entry_menu_hrs.pPage = (struct Menu_Page_t*) p_hrs_menu;
  notif_display_right_action.pPage = (struct Menu_Page_t*) p_notif_control_menu;

  Menu_SetControlContent(p_waitcon_menu, &waitcon_text, &advertising_icon);
  Menu_SetControlContent(p_connecting_menu, &connecting_text, &hourglass_icon);
  Menu_SetControlContent(p_connected_menu, &connected_text, 0);
  Menu_SetControlContent(p_hrs_menu, &hrs_text, &empty_heart_icon);
  
  Menu_SetControlAction(p_connected_menu, MENU_DIRECTION_RIGHT, access_menu_action);

  Menu_AddListEntry(p_control_menu, "Volume...", entry_menu_volume_action);
//  Menu_AddListEntry(p_control_menu, "Call...", entry_menu_call_action);
  Menu_AddListEntry(p_control_menu, "Media...", entry_menu_media_action);
  Menu_AddListEntry(p_control_menu, "Notif...", entry_menu_notif_display);
  Menu_AddListEntry(p_control_menu, "HRS...", entry_menu_hrs);

  Menu_SetControlContent(p_volume_control_menu, &volume_text, 0);
  Menu_SetControlAction(p_volume_control_menu, MENU_DIRECTION_UP, volume_control_up_action);
  Menu_SetControlAction(p_volume_control_menu, MENU_DIRECTION_DOWN, volume_control_down_action);

  Menu_SetControlContent(p_call_control_menu,&call_text, 0);
  Menu_SetControlAction(p_call_control_menu, MENU_DIRECTION_UP, call_control_up_action);
  Menu_SetControlAction(p_call_control_menu, MENU_DIRECTION_DOWN, call_control_down_action);

  Menu_SetControlContent(p_media_control_menu, &media_text, 0);
  Menu_SetControlAction(p_media_control_menu, MENU_DIRECTION_UP, media_control_up_action);
  Menu_SetControlAction(p_media_control_menu, MENU_DIRECTION_DOWN, media_control_down_action);
  Menu_SetControlAction(p_media_control_menu, MENU_DIRECTION_RIGHT, media_control_right_action);

  Menu_SetControlContent(p_notif_display_menu, &notif_display_text, 0);
  Menu_SetControlAction(p_notif_display_menu, MENU_DIRECTION_UP, notif_display_up_action);
  Menu_SetControlAction(p_notif_display_menu, MENU_DIRECTION_DOWN, notif_display_down_action);
  Menu_SetControlAction(p_notif_display_menu, MENU_DIRECTION_RIGHT, notif_display_right_action);
  
  Menu_SetControlContent(p_notif_control_menu, &notif_control_text, 0);
  Menu_SetControlAction(p_notif_control_menu, MENU_DIRECTION_UP, notif_control_up_action);
  Menu_SetControlAction(p_notif_control_menu, MENU_DIRECTION_DOWN, notif_control_down_action);
  
  Menu_SetActivePage(p_waitcon_menu);

  Menu_Print();
}

/**
 * @brief Set the connecting page as active
 */
void Menu_SetConnectingPage(void)
{
  Menu_SetActivePage(p_connecting_menu);
}

/**
 * @brief Set the NoStream Page as active
 */
void Menu_SetConnectedPage(void)
{
  Menu_SetActivePage(p_connected_menu);
}


/**
 * @brief Set the WaitConnection Page as active
 */
void Menu_SetWaitConnPage(void)
{
  Menu_SetActivePage(p_waitcon_menu);
}

/**
 * @brief Set the WaitConnection Page as active
 */
void Menu_SetIdentifier(char *pId)
{
  UTIL_MEM_cpy_8(&waitcon_text.Lines[1][9], pId, 4);
}

/* Private Functions Definition --------------------------------------------- */
/**
 * @brief Volume Up Callback
 */
static void Menu_Volume_Up(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Volume+\n");
  AMS_Remote_Cmd(RemoteCommandIDVolumeUp);
}

/**
 * @brief Volume Down Callback
 */
static void Menu_Volume_Down(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Volume-\n");
  AMS_Remote_Cmd(RemoteCommandIDVolumeDown);
}

/**
 * @brief Next Track Callback
 */
static void Menu_Media_NextTrack(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Next Track\n");
  AMS_Remote_Cmd(RemoteCommandIDNextTrack);
}

/**
 * @brief Play Pause Callback
 */
static void Menu_Media_PlayPause(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Play/Pause\n");
  AMS_Remote_Cmd(RemoteCommandIDTogglePlayPause);
}

/**
 * @brief Previous track Callback
 */
static void Menu_Media_PreviousTrack(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Previous Track\n");
  AMS_Remote_Cmd(RemoteCommandIDPreviousTrack);
}

/**
 * @brief Disconnect Callback
 */
__USED static void Menu_Disconnect(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Disconnect\n");
}

static void Menu_Notif_Display_Previous(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Previous Notif Disply\n");
  ancs_update_notif(ShowPrevious);
  Menu_Print();
};

static void Menu_Notif_Display_Next(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Next Notif Display\n"); 
  ancs_update_notif(ShowNext);
  Menu_Print(); 
};

static void Menu_Call_Answer(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Answer Call\n");
  ANCS_Perform_Notification_Action(ActionIDPositive);
};

static void Menu_Call_Terminate(void)
{
  APP_DBG_MSG("[APP_MENU_CONF] Terminate Call\n");
  ANCS_Perform_Notification_Action(ActionIDNegative);
};

static void Menu_Notif_Control_Up(void)
{
  ANCS_Perform_Notification_Action(ActionIDPositive);
  Menu_SetActivePage(p_notif_display_menu);
};

static void Menu_Notif_Control_Down(void)
{
  ANCS_Perform_Notification_Action(ActionIDNegative);
  Menu_SetActivePage(p_notif_display_menu);
};