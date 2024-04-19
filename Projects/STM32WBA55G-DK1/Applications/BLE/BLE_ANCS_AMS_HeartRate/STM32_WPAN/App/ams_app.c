/**
  ******************************************************************************
  * @file    ams_app.c
  * @author  MCD Application Team
  * @brief   AMS definition 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "uuid.h"
#include "stm32_seq.h"
#include "dbg_trace.h"
#include "ble.h"

/* Private includes ----------------------------------------------------------*/
#include "ams_app.h"
#include "app_menu.h"

/* Private typedef -----------------------------------------------------------*/

/* Private defines ------------------------------------------------------------*/
#define UNPACK_2_BYTE_PARAMETER(ptr)  \
        (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
        (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

/* Private macros -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static bool Remote_Cmd_Available[(RemoteCommandID) Nb_of_RemoteCommand] = {false};
static char SingerName[100] = {0};
static char SongName[100] = {0};
static int Sub_SingerIndex;
static int Sub_SongIndex;

/* Global variables ----------------------------------------------------------*/
gatt_client_interface_ams_t gatt_client_interface_ams;
AMS_init_data_t AMS_init_data;
static int Volume = 0;
extern Menu_Content_Text_t media_text;
extern Menu_Content_Text_t volume_text;
extern Menu_Page_t *p_media_control_menu;
/* USER CODE END GV */

/* Private function prototypes -----------------------------------------------*/
/* Used to log notif value */
static void gatt_client_to_ams(void);
static void send_gatt_cmd_to_client(AMSCmdToGatt_t, uint16_t, uint8_t, uint8_t*);

static void AMS_Show_Notif_Entity_Update(void);
static void AMS_Update_Available_Remote_Cmd(void);
static void AMS_Retrieve_Value_Cmd(EntityID Entity, uint8_t AttributID);
static void ams_start_notification(void);
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void AMS_APP_Init(void)
{
  UTIL_SEQ_RegTask(1U << CFG_TASK_GATT_CLIENT_TO_AMS_ID, UTIL_SEQ_RFU, gatt_client_to_ams);
  UTIL_SEQ_RegTask(1U << CFG_TASK_AMS_START_NOTIF_ID, UTIL_SEQ_RFU, ams_start_notification);
  return;
}

static void gatt_client_to_ams(void)
{
  
  switch ( gatt_client_interface_ams.GattCmdToAMS )
  {
    case AMS_START_NOTIF :
      UTIL_SEQ_SetTask(1U << CFG_TASK_AMS_START_NOTIF_ID, CFG_SEQ_PRIO_0);
      break;
    case AMS_INIT_HANDLE:
    {
      memcpy(&AMS_init_data, gatt_client_interface_ams.p_Payload, gatt_client_interface_ams.l_payload);
      break;
    }
    case AMS_RECEIVE_NOTIF:
    {
     switch (gatt_client_interface_ams.char_UUID )
     {
     case AMS_ENTITY_UPDATE_CHAR_UUID :
       AMS_Show_Notif_Entity_Update();
       break;
     case AMS_REMOTE_COMMAND_CHAR_UUID :
       AMS_Update_Available_Remote_Cmd();
       break;       
     default:
       break;
     }
    break;
  }
  case AMS_DECONNECTION:
  {
    strcpy(SongName, "    Media    ");
    strcpy(media_text.Lines[0], "    Media    ");
    strcpy(SingerName, "   Control   ");
    strcpy(media_text.Lines[1], "   Control   ");
    break;
  }  
  default:
    break;
  }
  UTIL_SEQ_SetEvt(1U << CFG_IDLEEVT_GATT_CMD_TO_AMS_COMPLETE); // This should have been implemented earlier, after data have been processed
};

static void send_gatt_cmd_to_client(AMSCmdToGatt_t AMSCmdToGatt, uint16_t char_UUID, uint8_t l_payload, uint8_t *p_Payload)
{
  tBleStatus result = BLE_STATUS_SUCCESS;  
  uint16_t CharValueHdle;
  
  switch ( AMSCmdToGatt )
  {
  case WRITE_AMS_CHAR:
    CharValueHdle = 0;
    if ( char_UUID == AMS_ENTITY_ATTRIBUTE_CHAR_UUID)
    {
      CharValueHdle = *(AMS_init_data.EntityAttributeCharValueHdle);
      LOG_INFO_APP("Write AMS Entity Attribute Char");
    }
    else if ( char_UUID == AMS_REMOTE_COMMAND_CHAR_UUID)
    {
      CharValueHdle = *(AMS_init_data.RemoteCommandCharValueHdle);
      LOG_INFO_APP("Write AMS Remote Command Char");
    }
    else if ( char_UUID == AMS_ENTITY_UPDATE_CHAR_UUID)
    {
      CharValueHdle = *(AMS_init_data.EntityUpdateCharValueHdle);
      LOG_INFO_APP("Write AMS Entity Update Char");
    }
    
    if (CharValueHdle == 0)
    {
      LOG_INFO_APP("PROC_GATT_WRITE_AMS_CHAR failed, UUID=%x not found \n", char_UUID);
      return;
    }
    result = aci_gatt_write_char_value( *(AMS_init_data.connHdl),
                        CharValueHdle,
                        l_payload,
                        (uint8_t *) p_Payload);        
    
    if (result == BLE_STATUS_SUCCESS)
    {
      LOG_INFO_APP(" Successfully\n");      
      UTIL_SEQ_WaitEvt(1U << CFG_IDLEEVT_PROC_GATT_COMPLETE);
    }
    else
    {
      LOG_INFO_APP(" NOT SUCCESSFULLY status = 0x%02X\n", result);
    }      
    break;
  case READ_AMS_CHAR:
    CharValueHdle = 0;
    if ( char_UUID == AMS_ENTITY_ATTRIBUTE_CHAR_UUID)
    {
      CharValueHdle = *(AMS_init_data.EntityAttributeCharValueHdle);
    }
    if (CharValueHdle == 0)
    {
      LOG_INFO_APP("PROC_GATT_READ_AMS_CHAR failed, UUID=%x not found \n", char_UUID);
      return;
    }    

    result = aci_gatt_read_char_value( *(AMS_init_data.connHdl),
                                       CharValueHdle);
    
    if (result == BLE_STATUS_SUCCESS)
    {
      UTIL_SEQ_WaitEvt(1U << CFG_IDLEEVT_PROC_GATT_COMPLETE);
      LOG_INFO_APP("Read AMS Char Successfully | UUID=%x\n\n", char_UUID);
    }
    else
    {
      LOG_INFO_APP("Entity Update cmd NOK status =0x%02X | UUID=%x\n\n", result, char_UUID);
    }
    break;
  default:
    break;
  }
};
/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void AMS_Show_Notif_Entity_Update(void){
  gatt_client_interface_ams_t *p_Notif = &gatt_client_interface_ams;
  EntityID entID = (EntityID) p_Notif->p_Payload[0];
  uint8_t AttrID = p_Notif->p_Payload[1];
  uint8_t TruncatedFlag = p_Notif->p_Payload[2];
  
  char Notif_Value[257] = {'\0'};
  memcpy(Notif_Value, &(p_Notif->p_Payload[3]), p_Notif->l_payload);
  Notif_Value[p_Notif->l_payload - 3] = '\0'; 
                 
  UTIL_SEQ_SetEvt(1U << CFG_IDLEEVT_GATT_CMD_TO_AMS_COMPLETE);  
    
  if (TruncatedFlag & ENTITY_UPDATE_FLAG_TRUNCATED)
  {
    AMS_Retrieve_Value_Cmd(entID,AttrID);
  }

  switch ( entID )
  {
    case EntityIDPlayer:
        switch ( (PlayerAttributeID) AttrID )
        {
          case PlayerAttributeIDName:
            LOG_INFO_APP("      [AMS] New App Name                     : %s\n", Notif_Value);
            break;
          case PlayerAttributeIDPlaybackInfo:
            LOG_INFO_APP("      [AMS] Playback Info                    :"); 
           switch ( (PlaybackState) (strtok((char *) Notif_Value, ",")[0] - '0'))
           {
           case PlaybackStatePaused : 
             LOG_INFO_APP("     PAUSED      |");
             break;
           case PlaybackStatePlaying : 
             LOG_INFO_APP("     PLAYING     |");
             break;
           case PlaybackStateRewinding :
             LOG_INFO_APP("     REWIDING    |");
             break;
           case PlaybackStateFastForwarding :
             LOG_INFO_APP(" FAST FORWARDING |");
             break;             
           default: 
             LOG_INFO_APP(" Unknown Playback State |");
             break;             
           }
           LOG_INFO_APP(" Play Back Rate : %s |", strtok(NULL, ",")); 
           LOG_INFO_APP(" Elapsed Time : %s sec \n", strtok(NULL, ","));
            break;
          case PlayerAttributeIDVolume:
            LOG_INFO_APP("      [AMS] New Volume                       : %s\n", Notif_Value);
            Volume = (int) (atof(Notif_Value) * 100.0);
            snprintf(volume_text.Lines[0], 5, "%d%%", Volume);
            UTIL_SEQ_SetTask( 1U << CFG_TASK_MENU_PRINT_ID, CFG_SEQ_PRIO_0);
            break;      
          default:
            LOG_INFO_APP("      [AMS] Unknown Player Attribute ID %d\n", AttrID);
            break;
        }
      break;
      
      
    case EntityIDQueue:
        switch ( (QueueAttributeID ) AttrID )
        {
          case QueueAttributeIDIndex:
            LOG_INFO_APP("      [AMS] New Queue Attribute Index        : %s\n", Notif_Value);
            break;
            
          case QueueAttributeIDCount:
            LOG_INFO_APP("      [AMS] New Queue Attribute Count        : %s\n", Notif_Value);
            break;
            
          case QueueAttributeIDShuffleMode:
            LOG_INFO_APP("      [AMS] New Queue Attribute Shuffle Mode :");
            switch ( Notif_Value[0] - '0')
             {
             case SHUFFLE_MODE_OFF : 
               LOG_INFO_APP(" Shuffle Mode Off \n");
               break;
             case SHUFFLE_MODE_ONE : 
               LOG_INFO_APP(" Shuffle Mode One \n");
               break;
             case SHUFFLE_MODE_ALL :
               LOG_INFO_APP(" Shuffle Mode All \n");
               break;          
             default: 
               LOG_INFO_APP(" Unknown Shuffle Mode \n");
               break;             
             }
            break;
            
          case QueueAttributeIDRepeatMode:
            LOG_INFO_APP("      [AMS] New Queue Attribute Repeat Mode  :");
            switch ( Notif_Value[0] - '0')
             {
             case REPEAT_MODE_OFF : 
               LOG_INFO_APP(" Repeat Mode Off \n");
               break;
             case REPEAT_MODE_ONE : 
               LOG_INFO_APP(" Repeat Mode One \n");
               break;
             case REPEAT_MODE_ALL :
               LOG_INFO_APP(" Repeat Mode All \n");
               break;          
             default: 
               LOG_INFO_APP(" Unknown Repeat Mode \n");
               break;             
             }            
            break;
          default:
            LOG_INFO_APP("      [AMS] Unknown Queue Attribute ID %d\n", AttrID);
            break;
        }
      break;
      
      
    case EntityIDTrack:
        switch ( (QueueAttributeID ) AttrID )
        {
          case TrackAttributeIDArtist:
            LOG_INFO_APP("      [AMS] New Track Attribute Artist       : %s\n", Notif_Value);
            Sub_SingerIndex = -2;
            cpy_ext_utf8_data((char *) Notif_Value, SingerName, strlen((char *) Notif_Value) + 1);
            //strcpy(SingerName, (char *) Notif_Value);
            strncpy(media_text.Lines[1], SingerName, 13);
            UTIL_SEQ_SetTask( 1U << CFG_TASK_MENU_PRINT_ID, CFG_SEQ_PRIO_0);
            break;
            
          case TrackAttributeIDAlbum:
            LOG_INFO_APP("      [AMS] New Track Attribute Album        : %s\n", Notif_Value);
            break;
            
          case TrackAttributeIDTitle:
            LOG_INFO_APP("      [AMS] New Track Attribute Title        : %s\n", Notif_Value);
            Sub_SongIndex = -2;
            memset(SongName, '\0' , 100);
            cpy_ext_utf8_data((char *) Notif_Value, SongName, strlen((char *) Notif_Value) + 1);
            //strcpy(SongName, (char *) Notif_Value);
            strncpy(media_text.Lines[0], SongName, 13);
            UTIL_SEQ_SetTask( 1U << CFG_TASK_MENU_PRINT_ID, CFG_SEQ_PRIO_0);
            break;
            
          case TrackAttributeIDDuration:
            LOG_INFO_APP("      [AMS] New Track Attribute Duration     : %s\n",Notif_Value);
            break;
            
          default:
            LOG_INFO_APP("      [AMS] Unknown Track Attribute ID %d\n", AttrID);
            break;
        }
      break;   
      
      
    default:
      LOG_INFO_APP(" Unknown Entity ID %d\n", entID);
      break;
  }
}

static void AMS_Update_Available_Remote_Cmd(void)
{
  gatt_client_interface_ams_t *p_Notif = &gatt_client_interface_ams;
  memset(Remote_Cmd_Available, false, (RemoteCommandID) Nb_of_RemoteCommand);
  for (uint8_t index = 0; index < p_Notif->l_payload; index++)
  {
    Remote_Cmd_Available[p_Notif->p_Payload[index]] = true;
  }
 
  UTIL_SEQ_SetEvt(1U << CFG_IDLEEVT_GATT_CMD_TO_AMS_COMPLETE);
}

static void ams_start_notification(void)
{
  uint8_t CmdStartNotification[5] = {0, 0, 1, 2, 3};
  LOG_INFO_APP("AMS request player notification --> ");
  send_gatt_cmd_to_client(WRITE_AMS_CHAR, AMS_ENTITY_UPDATE_CHAR_UUID, 4, &(CmdStartNotification[0]));
  
  LOG_INFO_APP("AMS request queue notification --> ");
  CmdStartNotification[0] = 1;
  send_gatt_cmd_to_client(WRITE_AMS_CHAR, AMS_ENTITY_UPDATE_CHAR_UUID, 5, &(CmdStartNotification[0]));
  
  LOG_INFO_APP("AMS request AMS track notification --> ");
  CmdStartNotification[0] = 2;
  send_gatt_cmd_to_client(WRITE_AMS_CHAR, AMS_ENTITY_UPDATE_CHAR_UUID, 5, &(CmdStartNotification[0]));
}

void AMS_Remote_Cmd(RemoteCommandID RemoteCommand)
{
  if (Remote_Cmd_Available[RemoteCommand] != true)
  {
    LOG_INFO_APP("This Remote Command isn't available on that app\n");
    return;
  }
  uint8_t RemoteCmdAtt = RemoteCommand;
  LOG_INFO_APP("AMS command --> ");
  send_gatt_cmd_to_client(WRITE_AMS_CHAR, AMS_REMOTE_COMMAND_CHAR_UUID, 1, &RemoteCmdAtt);
}

static void AMS_Retrieve_Value_Cmd(EntityID Entity, uint8_t AttributID)
{
  uint8_t EntityCmdAtt[2] = {Entity, AttributID};
  LOG_INFO_APP("AMS retrieve value --> ");
  send_gatt_cmd_to_client(WRITE_AMS_CHAR, AMS_ENTITY_ATTRIBUTE_CHAR_UUID, 2, EntityCmdAtt);
}

void ams_update_singer_song_name(void)
{
  Menu_Page_t *Current_Menu = Menu_GetActivePage();
  if (Current_Menu == p_media_control_menu)
  {
    uint8_t refresh_index = (strlen(SongName) > strlen(SingerName)) ? strlen(SongName) : strlen(SingerName) ;
    if (strlen(SongName) > 13)
    {
      if ((Sub_SongIndex >= 0) && (Sub_SongIndex + 13 <= strlen(SongName)))
      {
        strncpy(media_text.Lines[0], &(SongName[Sub_SongIndex]), 13);
      }
      else if (Sub_SongIndex + 10 == refresh_index)
      {
        Sub_SongIndex = -2;
        strncpy(media_text.Lines[0], SongName, 13);
      }  
      Sub_SongIndex++;
    }
    
    if (strlen(SingerName) > 13)
    {
      if ((Sub_SingerIndex >= 0) && (Sub_SingerIndex + 13 <= strlen(SingerName)))
      {
        strncpy(media_text.Lines[1], &(SingerName[Sub_SingerIndex]), 13);
      }
      else if (Sub_SingerIndex + 10 == refresh_index)
      {
        Sub_SingerIndex = -2;
        strncpy(media_text.Lines[1], SingerName, 13);
      }  
      Sub_SingerIndex++;
    }
  }
}
