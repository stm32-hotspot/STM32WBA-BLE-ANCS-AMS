/**
  ******************************************************************************
  * @file    ancs_app.c
  * @author  MCD Application Team
  * @brief   ANCS definition
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
#include "ble.h"
#include "stm32_timer.h"
#include "ancs_app.h"
#include "app_menu.h"
/* Private includes ----------------------------------------------------------*/

/* Private defines ------------------------------------------------------------*/
#define UNPACK_2_BYTE_PARAMETER(ptr)  \
        (uint16_t)((uint16_t)(*((uint8_t *)ptr))) |   \
        (uint16_t)((((uint16_t)(*((uint8_t *)ptr + 1))) << 8))

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  char AppID[MAX_NBR_OF_APP_NAME_STORED][MAX_CHAR_LENGTH];
  char AppName[MAX_NBR_OF_APP_NAME_STORED][MAX_CHAR_LENGTH];
  uint16_t free_index;
} App_Name_List_t;


/* Private macros -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static Notif_List_t Notif_List[MAX_NBR_OF_NOTIF];
static tListNode Notif_HeadList;
static uint32_t List_used = 0;

static App_Name_List_t App_Name_List;

static Notif_List_t *Notif_Displayed;
    
static UTIL_TIMER_Object_t Wait_Notification_Init_Id;

/* Global variables ----------------------------------------------------------*/
gatt_client_interface_ancs_t gatt_client_interface_ancs;
ANCS_init_data_t ANCS_init_data;

extern Menu_Content_Text_t notif_display_text;
extern Menu_Content_Text_t notif_control_text;
extern Menu_Content_Text_t call_text;

extern Menu_Page_t *p_call_control_menu;
extern Menu_Page_t *p_notif_control_menu;
extern Menu_Page_t *p_notif_display_menu;

extern Menu_Icon_t answer_call_icon;
/* Private function prototypes -----------------------------------------------*/
/* Used to log notif value */
static void gatt_client_to_ancs(void);
static void send_gatt_cmd_to_client(ANCSCmdToGatt_t, uint16_t, uint8_t, uint8_t*);

static void ANCS_Show_Notif_Source_Update(void);
static void ANCS_Show_Notif_Data_Source(void);
static void ANCS_get_detail(void);
static void removed_notif(Notif_List_t *);
static void reset_app_name_list(void);
static void ANCS_Display_Notif(Notif_List_t *Notif);
static void ANCS_Display_Notif_List(void);
static void ANCS_Display_App_Name_List(void);

static Notif_List_t *retrieve_notif_in_list_UID(tListNode *Notif_HeadList, uint32_t UID);
static void Update_AppName(tListNode *Notif_HeadList, char *, uint16_t, char *, uint16_t);
static uint8_t retrieve_notif_index(tListNode *Node);

static void Wait_Notification_Init_cb(void *arg);
/* Functions Definition ------------------------------------------------------*/
/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void ANCS_APP_Init(void)
{
  
  UTIL_SEQ_RegTask(1U << CFG_TASK_GATT_CLIENT_TO_ANCS_ID, UTIL_SEQ_RFU, gatt_client_to_ancs);
  UTIL_SEQ_RegTask(1U << CFG_TASK_ANCS_GET_DETAIL_ID, UTIL_SEQ_RFU, ANCS_get_detail);
  reset_app_name_list();
  LST_init_head(&Notif_HeadList);
  
  
  UTIL_TIMER_Create(&Wait_Notification_Init_Id,
                100,
                UTIL_TIMER_ONESHOT,
                &Wait_Notification_Init_cb, 0);  
  return;
}

static void gatt_client_to_ancs(void)
{
  
  switch ( gatt_client_interface_ancs.GattCmdToANCS )
  {
  case ANCS_INIT_HANDLE:
  {
    memcpy(&ANCS_init_data, gatt_client_interface_ancs.p_Payload, gatt_client_interface_ancs.l_payload);
    break;
  }    
  case ANCS_RECEIVE_NOTIF:
     switch (gatt_client_interface_ancs.char_UUID )
     {
     case ANCS_NOTIFICATION_SOURCE_CHAR_UUID :
       //LOG_INFO_APP("New ANCS notif from ANCS_NOTIFICATION_SOURCE_CHAR\n");
       ANCS_Show_Notif_Source_Update();
       break;
     case ANCS_DATA_SOURCE_CHAR_UUID :
       //LOG_INFO_APP("New ANCS notif from ANCS_DATA_SOURCE_CHAR\n");
       ANCS_Show_Notif_Data_Source();
       break;       
     default:
       break;
     }
    break;   
  case ANCS_DECONNECTION :
    for (uint8_t index = 0; index < MAX_NBR_OF_NOTIF; index++)
    {
      removed_notif(&(Notif_List[index]));
    }
    ANCS_init_data.app_init = false;
    break;
  default:
    break;
  }
  UTIL_SEQ_SetEvt(1U << CFG_IDLEEVT_GATT_CMD_TO_ANCS_COMPLETE);  
  return;
};

static void send_gatt_cmd_to_client(ANCSCmdToGatt_t ANCSCmdToGatt, uint16_t char_UUID, uint8_t l_payload, uint8_t *p_Payload)
{
  tBleStatus result = BLE_STATUS_SUCCESS;  
  uint16_t CharValueHdle;
  
  switch ( ANCSCmdToGatt )
  {
  case WRITE_ANCS_CHAR:
    CharValueHdle = 0;
      if ( char_UUID == ANCS_CONTROL_POINT_CHAR_UUID)
      {
        CharValueHdle = *(ANCS_init_data.CtrlPointCharValueHdle);
        LOG_INFO_APP("Write ANCS Control Point Char");
      }
      
      if (CharValueHdle == 0)
      {
        LOG_INFO_APP("PROC_GATT_WRITE_ANCS_CHAR failed, UUID=%x not found \n", char_UUID);
        return;
      }
    
      result = aci_gatt_write_char_value( *(ANCS_init_data.connHdl),
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
        LOG_INFO_APP(" NOT SUCCESSFULLY status =0x%02X\n", result);
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
 
static void ANCS_Show_Notif_Source_Update(void)
{
  Notif_List_t *Notif;
  uint16_t Notif_index = 0;
  gatt_client_interface_ancs_t *p_Notif = &gatt_client_interface_ancs;
  EventID_t evtID = (EventID_t) p_Notif->p_Payload[0];
  EventFlags_t evtFlag = (EventFlags_t) p_Notif->p_Payload[1];
  CategoryID_t CategoryID = (CategoryID_t) p_Notif->p_Payload[2];
  uint8_t CategoryCount =  p_Notif->p_Payload[3];
  uint32_t NotificationUID = (uint32_t) (((uint32_t) p_Notif->p_Payload[7] << 24 ) |
                                         ((uint32_t) p_Notif->p_Payload[6] << 16 ) |
                                         ((uint32_t) p_Notif->p_Payload[5] <<  8 ) |
                                         ((uint32_t) p_Notif->p_Payload[4]) );

  UNUSED(CategoryCount);

  switch ( evtID )
  {
    case EventIDNotificationAdded:
    {
      //LOG_INFO_APP("New Notif added \n");
      for (Notif_index = 0; Notif_index <= MAX_NBR_OF_NOTIF; Notif_index++)
      {
        if (Notif_index == MAX_NBR_OF_NOTIF)
        {
          LOG_INFO_APP("Too many notifications have already been registered. If you want to store more, you can increment the MAX_NBR_OF_NOTIF (currently %d)\n", MAX_NBR_OF_NOTIF);
          return;
        }

        if ( (List_used & (1 << Notif_index)) == 0 )
        {
          List_used |= (1 << Notif_index);
          //LOG_INFO_APP(" List_used add : %d --> index %d\n", List_used, Notif_index);
          LST_insert_tail(&Notif_HeadList, &Notif_List[Notif_index].Node);
          Notif = &Notif_List[Notif_index];
          break;
        }
      }
    }
    break;
    
    case EventIDNotificationModified:
    {
      LOG_INFO_APP("New Notif modified");
      Notif = retrieve_notif_in_list_UID(&Notif_HeadList, NotificationUID);
      if (Notif == NULL)
      {
        LOG_INFO_APP("Erreur : Impossible to retrieve the Notif with UID : %x\n",NotificationUID);
        return;
      }
      ANCS_Display_Notif_List();
    }
    break;
      
    case EventIDNotificationRemoved:
    {
      Notif = retrieve_notif_in_list_UID(&Notif_HeadList, NotificationUID);
      if (Notif == NULL)
      {
        LOG_INFO_APP("Erreur : Impossible to retrieve the Notif with UID : %x\n",NotificationUID);
        return;
      }
      else if (Notif->catID == CategoryIDActiveCall)
      {
        Menu_SetActivePage(p_notif_display_menu); 
      }
      
      if (Notif_Displayed == Notif)
      {
        if ( ((tListNode *)Notif_Displayed)->prev == &Notif_HeadList)
          LST_get_next_node( (tListNode *)Notif_Displayed, (tListNode **) &Notif_Displayed);
        else
          LST_get_prev_node( (tListNode *)Notif_Displayed, (tListNode **) &Notif_Displayed);
      }

      removed_notif(Notif);
      LOG_INFO_APP("==> ANCS Removed Notif\n");
      ANCS_Display_Notif_List();
      return;
    }
    break;      
    default:
    break;
  } 
  
  Notif->evtFlag = evtFlag;
  Notif->catID  = CategoryID;
  Notif->UID    = NotificationUID;  

//  LOG_INFO_APP("Flag : %x :",evtFlag);
//  if (evtFlag & EventFlagSilent)
//      LOG_INFO_APP(",          Silent Flag");
//  if (evtFlag & EventFlagImportant)
//      LOG_INFO_APP(",       Important Flag");
//  if (evtFlag & EventFlagPreExisting)
//      LOG_INFO_APP(",    Pre Existing Flag");
//  if (evtFlag & EventFlagPositiveAction)
//      LOG_INFO_APP(", Positive Action Flag");
//  if (evtFlag & EventFlagNegativeAction)
//      LOG_INFO_APP(", Negative Action Flag");

  switch ( CategoryID )
  {
    case CategoryIDOther:
//      LOG_INFO_APP(",          Other Category\n");
      break;
    case CategoryIDIncomingCall:
//      LOG_INFO_APP(", Incoming Call Category\n");
      answer_call_icon.Image_height = 16;
      answer_call_icon.Image_width = 16;
      Notif_Displayed = Notif;
      strcpy(call_text.Lines[0], "Incoming Call");
      Menu_SetActivePage(p_call_control_menu);
      break;
    case CategoryIDMissedCall:
//      LOG_INFO_APP(", Missed Call Category\n");
      if (Menu_GetActivePage() == p_call_control_menu)
      {
        Notif_Displayed = Notif;
        Menu_SetActivePage(p_notif_display_menu);
      }
      break;
    case CategoryIDVoicemail:
//      LOG_INFO_APP(", Voice mail Category\n");
      break;
    case CategoryIDSocial:
//      LOG_INFO_APP(", Social Category\n");
      break;
    case CategoryIDSchedule:
//      LOG_INFO_APP(", Schedule Category\n");
      break;
    case CategoryIDEmail:
//      LOG_INFO_APP(", Email Category\n");
      break;
    case CategoryIDNews:
//      LOG_INFO_APP(", News Category\n");
      break;
    case CategoryIDHealthAndFitness:
//      LOG_INFO_APP(", Health And Fitness Category\n");
      break;
    case CategoryIDBusinessAndFinance:
//      LOG_INFO_APP(", Business And Finance Category\n");
      break;
    case CategoryIDLocation:
//      LOG_INFO_APP(", Location Category\n");
      break;
    case CategoryIDEntertainment:
//      LOG_INFO_APP(", Entertainment Category\n");
      break;
    case CategoryIDActiveCall:
//      LOG_INFO_APP(", Active Call Category\n");
      answer_call_icon.Image_height = 0;
      answer_call_icon.Image_width = 0;
      Notif_Displayed = Notif;
      Menu_SetActivePage(p_call_control_menu); 
      strcpy(call_text.Lines[0], "Active Call");
      strcpy(call_text.Lines[1], "");
    break;
    default:
//      LOG_INFO_APP(", Unknown Category : %d\n", CategoryID);
      break;
  }
  
  if (evtID == EventIDNotificationAdded)
  {
    Notif->retrieve_more_data_flag = true;
    Notif->Data_To_retrieve = CommandIDGetNotificationAttributes; 

    if (ANCS_init_data.app_init == false)
    {
      UTIL_TIMER_Start(&Wait_Notification_Init_Id);
    }
    else
    {
      UTIL_SEQ_SetTask(1U << CFG_TASK_ANCS_GET_DETAIL_ID, CFG_SEQ_PRIO_0);
    }
  }
  return;  
}

static void ANCS_Show_Notif_Data_Source(void)
{
  uint16_t p_index = 0;
  Notif_List_t *Notif;
  NotificationAttributeID AttributeID;
  uint16_t AttributeID_length, AppID_length;
  char payload[MAX_CHAR_LENGTH + MAX_CHAR_LENGTH_MESSAGE];
  char payload_AppID[MAX_CHAR_LENGTH];
  gatt_client_interface_ancs_t *p_Notif = &gatt_client_interface_ancs;
    
  CommandID_t CommandID = (CommandID_t) p_Notif->p_Payload[p_index++];
  
  if (CommandID == CommandIDGetAppAttributes)
  {
    AppID_length = strlen((char *) &(p_Notif->p_Payload[p_index]));     //Not Including '\0'
    
    if (AppID_length >= MAX_CHAR_LENGTH)
    {
      AppID_length = MAX_CHAR_LENGTH - 1;       //-1 to include the null terminated string
    }

    strncpy(payload_AppID, (char *) &(p_Notif->p_Payload[p_index]), AppID_length);
    payload_AppID[AppID_length] = '\0';
    p_index += AppID_length + 1;                //To Include '\0' 
    p_index += 1;                               //AttributeID
    AttributeID_length = (uint16_t) (((uint16_t) p_Notif->p_Payload[p_index+1] <<  8 ) |
                                     ((uint16_t) p_Notif->p_Payload[p_index]) );
    p_index += 2;                               //Attribute length sizeof (uint16_t)
    
    if (AttributeID_length >= MAX_CHAR_LENGTH)
    {
      LOG_INFO_APP("MAX_NBR_OF_CHAR isn't big enought to store the app name\n");
      AttributeID_length = MAX_CHAR_LENGTH - 1;    //-1 to include the null terminated string
    }

    strncpy(payload, (char *) &(p_Notif->p_Payload[p_index]), AttributeID_length);
    

    if (AttributeID_length == 0)
    {
      Update_AppName(&Notif_HeadList, payload_AppID, AppID_length, "Unknown App", strlen("Unknown App") + 1);
    }        
    else
    { 
      payload[AttributeID_length] = '\0';
      for (uint16_t index = 0; index < App_Name_List.free_index; index ++)
      {
        if (strcmp(App_Name_List.AppID[index], payload_AppID) == 0)
        {
          strncpy(App_Name_List.AppName[index], payload, AttributeID_length+1);  
          break;
        }
      }
      Update_AppName(&Notif_HeadList, payload_AppID, AppID_length, payload, AttributeID_length+1);
    }
    
    ANCS_Display_App_Name_List();
    ANCS_Display_Notif_List();
    //LOG_INFO_APP("App Name : %s \n", payload);
    return;
  }
  else if (CommandID == CommandIDGetNotificationAttributes)
  {
    uint32_t NotificationUID = (uint32_t) (((uint32_t) p_Notif->p_Payload[p_index+3] << 24 ) |
                                           ((uint32_t) p_Notif->p_Payload[p_index+2] << 16 ) |
                                           ((uint32_t) p_Notif->p_Payload[p_index+4] <<  8 ) |
                                           ((uint32_t) p_Notif->p_Payload[p_index]) );
    p_index += 4;
    
    Notif = retrieve_notif_in_list_UID(&Notif_HeadList, NotificationUID);
    if (Notif->UID == RESET_UID)
    {
      LOG_INFO_APP("Unknown UID : %d\n",NotificationUID);
      return;
    }
    
    while (p_index < p_Notif->l_payload)
    {
      AttributeID = (NotificationAttributeID) p_Notif->p_Payload[p_index++];
      AttributeID_length = (uint16_t) (((uint16_t) p_Notif->p_Payload[p_index+1] <<  8 ) |
                                       ((uint16_t) p_Notif->p_Payload[p_index]) );
      p_index += 2;
      
      strncpy(payload, (char *) &(p_Notif->p_Payload[p_index]), AttributeID_length);
      payload[AttributeID_length] ='\0';
    
      p_index += AttributeID_length;
      
      switch ( AttributeID )
        {
        case NotificationAttributeIDAppIdentifier:
          strncpy(Notif->AppIdentifier, payload, AttributeID_length + 1);       //+1 ==> include '\0'
          //LOG_INFO_APP("  - New Notification App Identifier : ");
          break;
        case NotificationAttributeIDTitle:
          //strncpy(Notif->Title, payload, AttributeID_length + 1);
          cpy_ext_utf8_data(payload, Notif->Title, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Title : ");
          snprintf(call_text.Lines[1], 13, "%s", Notif->Title);           
          break;
        case NotificationAttributeIDSubtitle:
          strncpy(Notif->SubTitle, payload, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Subtitle : ");
          break;
        case NotificationAttributeIDMessage:
          //strncpy(Notif->Message, payload, AttributeID_length + 1);
          cpy_ext_utf8_data(payload, Notif->Message, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Message : ");
          break;
        case NotificationAttributeIDMessageSize:
          strncpy(Notif->MessageSize, payload, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Message Size : ");
          break;
        case NotificationAttributeIDDate:
          strncpy(Notif->Date, payload, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Date : ");
          break;
        case NotificationAttributeIDPositiveActionLabel:
          //strncpy(Notif->PositiveActionLabel, payload, AttributeID_length + 1);
          cpy_ext_utf8_data(payload, Notif->PositiveActionLabel, AttributeID_length + 1);        
          //LOG_INFO_APP("  - New Notification Positive Action Label : ");
          break;
        case NotificationAttributeIDNegativeActionLabel:
          //strncpy(Notif->NegativeActionLabel, payload, AttributeID_length + 1);
          cpy_ext_utf8_data(payload, Notif->NegativeActionLabel, AttributeID_length + 1);
          //LOG_INFO_APP("  - New Notification Negative Action Label : ");
          break;
        default:
          break;
      }
    }
    //LOG_INFO_APP("%s\n", payload);
    uint16_t App_name_index;
    
    for (App_name_index = 0; App_name_index < MAX_NBR_OF_APP_NAME_STORED; App_name_index++)
    {
      if (strcmp(Notif->AppIdentifier, App_Name_List.AppID[App_name_index]) == 0)
      {
        if (strlen(App_Name_List.AppName[App_name_index]) == 0)                         //App are already in app name list, but no app name correspond
        {
          LOG_INFO_APP("Unknown Name, but procedure to retrieve %s have already started\n", Notif->AppIdentifier);
          return;
        }
        else                                                                            //App are already in app name list, one app name correspond
        {
          strcpy(Notif->AppName, App_Name_List.AppName[App_name_index]);
          ANCS_Display_Notif(Notif);
          ANCS_Display_Notif_List();
          return;
        }
      }
    }

    if (App_name_index == MAX_NBR_OF_APP_NAME_STORED)                                   //Unknown app ID
    {
      LOG_INFO_APP("Unknown Name, Start procedure to retrieve name\n");
      strncpy(App_Name_List.AppID[App_Name_List.free_index++], Notif->AppIdentifier, strlen(Notif->AppIdentifier) + 1);     //+1 --> include '\0'     
      Notif->retrieve_more_data_flag = true;
      Notif->Data_To_retrieve = CommandIDGetAppAttributes;
      UTIL_SEQ_SetTask(1U << CFG_TASK_ANCS_GET_DETAIL_ID, CFG_SEQ_PRIO_0);
    } 
  }
  else if (CommandID == CommandIDPerformNotificationAction)
  {
    LOG_INFO_APP("CommandIDPerformNotificationAction not implemented\n");       //the code shouldn't be here
  }
  return;
}

static void removed_notif(Notif_List_t *Notif)
{
  uint8_t notif_index = 0;
  Notif->evtFlag = (EventFlags_t) 0;
  Notif->catID  = (CategoryID_t) 0;
  Notif->UID = RESET_UID;
  
  memset(Notif->AppIdentifier,       '\0', MAX_CHAR_LENGTH);
  memset(Notif->AppName,             '\0', MAX_CHAR_LENGTH);
  memset(Notif->Title,               '\0', MAX_CHAR_LENGTH);
  memset(Notif->SubTitle,            '\0', MAX_CHAR_LENGTH);
  memset(Notif->Message,             '\0', MAX_CHAR_LENGTH_MESSAGE);
  memset(Notif->MessageSize,         '\0',   2);
  memset(Notif->Date,                '\0',   7);
  memset(Notif->PositiveActionLabel, '\0', MAX_CHAR_LENGTH);
  memset(Notif->NegativeActionLabel, '\0', MAX_CHAR_LENGTH);
  
  for (notif_index = 0; notif_index < 32; notif_index++)
  {
    if (&Notif_List[notif_index] == Notif)
    {
      List_used &= ~(1 << notif_index);
      //LOG_INFO_APP(" List_used remove : %d --> notif_index %d\n", List_used, notif_index);
      break;
    }
  }
  if ( retrieve_notif_index((tListNode *)Notif) != 0xFF)
  {
      LST_remove_node((tListNode *)Notif);  
  }
}

static void reset_app_name_list(void)
{
  for (uint16_t index = 0; index < MAX_NBR_OF_APP_NAME_STORED; index++)
  {
    memset(&(App_Name_List.AppID[index]),'\0', 50);
    memset(&(App_Name_List.AppName[index]),'\0', 50);
  }
  App_Name_List.free_index = 0;
}

static void ANCS_get_detail(void)
{
  uint16_t index = 0;
  uint8_t tab[MAX_CHAR_LENGTH + 3] = {'\0'};           //Max size happens for CommandIDGetAppAttributes => App ID length = MAX_CHAR_LENGTH + '\0' + command ID + Attribut ID
                                                       //                                                    Nb of bytes = MAX_CHAR_LENGTH +  1   +     1      +      1
  uint8_t AppID_size;
  
  for (index = 0; index < MAX_NBR_OF_NOTIF; index ++)  //Retrieve the detail for all notif
  {
    if ( (List_used >> index) & 1)                     //Retrieve the detail for the activ notif only
    {
      if (Notif_List[index].retrieve_more_data_flag == true)
      {
        if (Notif_List[index].Data_To_retrieve == CommandIDGetNotificationAttributes)
        {
          tab[0]  = CommandIDGetNotificationAttributes;
          memcpy(&tab[1], &Notif_List[index].UID, 4 * sizeof (uint8_t));
          tab[5]  = NotificationAttributeIDAppIdentifier;
          tab[6]  = NotificationAttributeIDTitle;
          tab[7]  = (uint8_t)  ((MAX_CHAR_LENGTH - 2) &0x00FF);                                   //On 2 bytes / litlle endian / - 2 to include '\n' + '\0'
          tab[8]  = (uint8_t) (((MAX_CHAR_LENGTH - 2) &0xFF00) >> 8);
          tab[9]  = NotificationAttributeIDSubtitle;
          tab[10] = (uint8_t)  ((MAX_CHAR_LENGTH - 2) &0x00FF);
          tab[11] = (uint8_t) (((MAX_CHAR_LENGTH - 2) &0xFF00) >> 8);
          tab[12] = NotificationAttributeIDMessage;
          tab[13] = (uint8_t)  ((MAX_CHAR_LENGTH_MESSAGE - 2) &0x00FF);
          tab[14] = (uint8_t) (((MAX_CHAR_LENGTH_MESSAGE - 2) &0xFF00) >> 8);
          tab[13] = NotificationAttributeIDMessageSize;
          tab[14] = NotificationAttributeIDDate;
          tab[15] = NotificationAttributeIDPositiveActionLabel;
          tab[16] = NotificationAttributeIDNegativeActionLabel;
          
          LOG_INFO_APP("ANCS retrieve notif data --> ");
          send_gatt_cmd_to_client(WRITE_ANCS_CHAR, ANCS_CONTROL_POINT_CHAR_UUID, 17, tab);
        }
        else if (Notif_List[index].Data_To_retrieve == CommandIDGetAppAttributes)
        {
          AppID_size = strlen(Notif_List[index].AppIdentifier);

          tab[0] = CommandIDGetAppAttributes;
          strncpy((char *) &(tab[1]), Notif_List[index].AppIdentifier, AppID_size + 1);      //+1 -- > ANCS Doc : "AppIdentifier: The string identifier of the app the client wants information about. 
                                                                                             //                    This string must be NULL-terminated."
          tab[AppID_size + 1] = AppAttributeIDDisplayName;

          LOG_INFO_APP("ANCS retrieve app name --> ");
          send_gatt_cmd_to_client(WRITE_ANCS_CHAR, ANCS_CONTROL_POINT_CHAR_UUID, AppID_size + 3, tab);      
        }
      }
    }
  }
}

void ANCS_Perform_Notification_Action(ActionID_t ActionID)
{   
  if (Notif_Displayed == NULL)
    return;
  
  switch (ActionID)
  {
  case ActionIDPositive:
    if ((Notif_Displayed->evtFlag & EventFlagPositiveAction) == false)
    {
      LOG_INFO_APP("No Positive Action to do on this notif\n");
      return;
    }    
    break;
  case ActionIDNegative:
    if ((Notif_Displayed->evtFlag & EventFlagNegativeAction) == false)
    {
      LOG_INFO_APP("No Negative Action to do on this notif\n");
      return;
    }    
    break;
  default:
    break;
  }
  
  uint8_t tab[6] = {0};
  
  tab[0] = CommandIDPerformNotificationAction;
  memcpy(&tab[1], &(Notif_Displayed->UID), 4 * sizeof (uint8_t));
  tab[5] = ActionID;  
  
  LOG_INFO_APP("ANCS perform action --> ");
  send_gatt_cmd_to_client(WRITE_ANCS_CHAR, ANCS_CONTROL_POINT_CHAR_UUID, 6, tab);      
}

static Notif_List_t *retrieve_notif_in_list_UID(tListNode *Notif_HeadList, uint32_t UID)
{
  Notif_List_t *currentNode = (Notif_List_t*)Notif_HeadList->next;
  while((tListNode *)currentNode != Notif_HeadList)
  {
    if(currentNode->UID == UID)
    {
      return currentNode;
    }
    LST_get_next_node((tListNode *)currentNode, (tListNode **)&currentNode);
  }
  return NULL;
}

static void Update_AppName(tListNode *Notif_HeadList, char *AppID, uint16_t AppID_length, char *AppName, uint16_t App_length)
{
  Notif_List_t *currentNode = (Notif_List_t*)Notif_HeadList->next;
  while((tListNode *)currentNode != Notif_HeadList)
  {
    if( (strncmp(currentNode->AppIdentifier, AppID, AppID_length) == 0) && (strlen(currentNode->AppName) == 0))
    {
      strncpy(currentNode->AppName, AppName, App_length);
      ANCS_Display_Notif( (Notif_List_t *)currentNode);
    }
    LST_get_next_node((tListNode *)currentNode, (tListNode **)&currentNode);
  }
  return;
}

static uint8_t retrieve_notif_index(tListNode *Node)
{
  uint8_t index = 0xFF;
  tListNode* currentNode = Notif_HeadList.next;
  while((tListNode *)currentNode != &Notif_HeadList)
  {
    index++;
    if(currentNode == Node)
    {
      return index;
    }
    LST_get_next_node((tListNode *)currentNode, (tListNode **)&currentNode);
  }
  return 0xFF;
}  

/* USER CODE BEGIN LF */

void ancs_update_notif(ModifNotifDisplay_t ModifNotifDisplay)
{
  static int Sub_Title = -2;
  static int Sub_Message = -2;
  static int Sub_PosAction = -2;
  static int Sub_NegAction = -2;

  if (LST_is_empty(&Notif_HeadList))
  {
    strcpy(notif_display_text.Lines[0], "00/00");
    strcpy(notif_display_text.Lines[1], "No");
    strcpy(notif_display_text.Lines[2], "Notif");  
    strcpy(notif_control_text.Lines[0], "No Action");
    strcpy(notif_control_text.Lines[1], "To Do");
    return;
  }

  if (Notif_Displayed == NULL)
  {
    Notif_Displayed = (Notif_List_t *) Notif_HeadList.next;
  }
  else if ( (tListNode *) Notif_Displayed == &Notif_HeadList)
  {
    LST_get_next_node( (tListNode *)Notif_Displayed, (tListNode **) &Notif_Displayed);
  }
  
    
  switch (ModifNotifDisplay)
  {
  case ShowPrevious :
    LST_get_prev_node ((tListNode *) Notif_Displayed, (tListNode **) &Notif_Displayed);
    if ((tListNode *)Notif_Displayed == &Notif_HeadList)
      LST_get_prev_node ((tListNode *) Notif_Displayed, (tListNode **) &Notif_Displayed);
    Sub_Title = -2;
    Sub_Message = -2;
    break;
  case ShowNext :
    LST_get_next_node ((tListNode *) Notif_Displayed, (tListNode **) &Notif_Displayed);
    if ((tListNode *)Notif_Displayed == &Notif_HeadList)
      LST_get_next_node ((tListNode *) Notif_Displayed, (tListNode **) &Notif_Displayed);    
    Sub_Title = -2;
    Sub_Message = -2;
    break;
  case ShowWithoutModif:
    break;
  default:
    break;
  }

  Menu_Page_t* Current_Menu = Menu_GetActivePage();
  if (Current_Menu == p_notif_display_menu)
  {
    uint8_t refresh_index_display = (strlen(Notif_Displayed->Title) > strlen(Notif_Displayed->Message)) ? strlen(Notif_Displayed->Title) : strlen(Notif_Displayed->Message);
    char tmp[5];
    snprintf(tmp, 6, "%02d/%02d", retrieve_notif_index((tListNode *) Notif_Displayed)+1, LST_get_size(&Notif_HeadList));
    strncpy(notif_display_text.Lines[0], tmp, 5);
    if (strlen(Notif_Displayed->Title) > 13)
    {
      if (Sub_Title < 0)
      {
        strncpy(notif_display_text.Lines[1], Notif_Displayed->Title, 13);
      }      
      else if ((Sub_Title >= 0) && (Sub_Title + 13 <= strlen(Notif_Displayed->Title)))
      {
        strncpy(notif_display_text.Lines[1], &(Notif_Displayed->Title[Sub_Title]), 13);
      }
      else if (Sub_Title + 10 == refresh_index_display)
      {
        Sub_Title = -2;
        strncpy(notif_display_text.Lines[1], Notif_Displayed->Title, 13);
      }  
      Sub_Title++;
    }
    else
    {
      strcpy(notif_display_text.Lines[1], Notif_Displayed->Title);
    }
    
    if (strlen(Notif_Displayed->Message) > 13)
    {
      if (Sub_Message < 0)
      {
        strncpy(notif_display_text.Lines[2], Notif_Displayed->Message, 13);
      }      
      else if ((Sub_Message >= 0) && (Sub_Message + 13 <= strlen(Notif_Displayed->Message)))
      {
        strncpy(notif_display_text.Lines[2], &(Notif_Displayed->Message[Sub_Message]), 13);
      }
      else if (Sub_Message + 10 == refresh_index_display)
      {
        Sub_Message = -2;
        strncpy(notif_display_text.Lines[2], Notif_Displayed->Message, 13);
      }  
      Sub_Message++;
    }
    else
    {
      strcpy(notif_display_text.Lines[2], Notif_Displayed->Message);
    }
  }
  else if (Current_Menu == p_notif_control_menu)
  {
    uint8_t refresh_index_control = (strlen(Notif_Displayed->PositiveActionLabel) > strlen(Notif_Displayed->NegativeActionLabel)) ? strlen(Notif_Displayed->PositiveActionLabel) : strlen(Notif_Displayed->NegativeActionLabel);  
    if (strlen(Notif_Displayed->PositiveActionLabel) > 13)
    {
      if (Sub_PosAction < 0)
      {
        strncpy(notif_control_text.Lines[0], Notif_Displayed->PositiveActionLabel, 13);
      }      
      else if ((Sub_PosAction >= 0) && (Sub_PosAction + 13 <= strlen(Notif_Displayed->PositiveActionLabel)))
      {
        strncpy(notif_control_text.Lines[0], &(Notif_Displayed->PositiveActionLabel[Sub_PosAction]), 13);
      }
      else if (Sub_PosAction + 10 == refresh_index_control)
      {
        Sub_PosAction = -2;
        strncpy(notif_control_text.Lines[0], Notif_Displayed->PositiveActionLabel, 13);
      }  
      Sub_PosAction++;
    }
    else
    {
      strcpy(notif_control_text.Lines[0], Notif_Displayed->PositiveActionLabel);
    }    
    
    if (strlen(Notif_Displayed->NegativeActionLabel) > 13)
    {
      if (Sub_NegAction < 0)
      {
        strncpy(notif_display_text.Lines[1], Notif_Displayed->NegativeActionLabel, 13);
      }      
      else if ((Sub_NegAction >= 0) && (Sub_NegAction + 13 <= strlen(Notif_Displayed->NegativeActionLabel)))
      {
        strncpy(notif_display_text.Lines[1], &(Notif_Displayed->NegativeActionLabel[Sub_NegAction]), 13);
      }
      else if (Sub_NegAction + 10 == refresh_index_control)
      {
        Sub_NegAction = -2;
        strncpy(notif_display_text.Lines[1], Notif_Displayed->NegativeActionLabel, 13);
      }  
      Sub_NegAction++;
    }
    else
    {
      strcpy(notif_control_text.Lines[1], Notif_Displayed->NegativeActionLabel);
    }
  }
}

static void ANCS_Display_Notif(Notif_List_t *Notif)
{
  LOG_INFO_APP("==> ANCS New Notification/Modified\n");
  LOG_INFO_APP("  - App Name : %s\n", Notif->AppName);
  LOG_INFO_APP("  - App ID : %s\n", Notif->AppIdentifier);
  LOG_INFO_APP("  - Title : %s\n", Notif->Title);
  LOG_INFO_APP("  - Subtitle : %s\n", Notif->SubTitle);
  LOG_INFO_APP("  - Message : %s\n", Notif->Message);
  LOG_INFO_APP("  - Message Size : %s\n", Notif->MessageSize);
  LOG_INFO_APP("  - Date : %s\n", Notif->Date);
  LOG_INFO_APP("  - Positive Action Label : %s\n", Notif->PositiveActionLabel);
  LOG_INFO_APP("  - Negative Action Label : %s\n", Notif->NegativeActionLabel);
  
  LOG_INFO_APP("  - Flag : 0x%x :", Notif->evtFlag);
  if (Notif->evtFlag & EventFlagSilent)
      LOG_INFO_APP(" Silent Flag |");
  if (Notif->evtFlag & EventFlagImportant)
      LOG_INFO_APP(" Important Flag |");
  if (Notif->evtFlag & EventFlagPreExisting)
      LOG_INFO_APP(" Pre Existing Flag |");
  if (Notif->evtFlag & EventFlagPositiveAction)
      LOG_INFO_APP(" Positive Action Flag |");
  if (Notif->evtFlag & EventFlagNegativeAction)
      LOG_INFO_APP(" Negative Action Flag |");
  LOG_INFO_APP("\n");
  
  LOG_INFO_APP("  - Category ID : ");
  switch ( Notif->catID )
  {
    case CategoryIDOther:
      LOG_INFO_APP("Other Category\n");
      break;
    case CategoryIDIncomingCall:
      LOG_INFO_APP("Incoming Call Category\n");
      break;
    case CategoryIDMissedCall:
      LOG_INFO_APP("Missed Call Category\n"); 
      break;
    case CategoryIDVoicemail:
      LOG_INFO_APP("Voice mail Category\n");
      break;
    case CategoryIDSocial:
      LOG_INFO_APP("Social Category\n");
      break;
    case CategoryIDSchedule:
      LOG_INFO_APP("Schedule Category\n");
      break;
    case CategoryIDEmail:
      LOG_INFO_APP("Email Category\n");
      break;
    case CategoryIDNews:
      LOG_INFO_APP("News Category\n");
      break;
    case CategoryIDHealthAndFitness:
      LOG_INFO_APP("Health And Fitness Category\n");
      break;
    case CategoryIDBusinessAndFinance:
      LOG_INFO_APP("Business And Finance Category\n");
      break;
    case CategoryIDLocation:
      LOG_INFO_APP("Location Category\n");
      break;
    case CategoryIDEntertainment:
      LOG_INFO_APP("Entertainment Category\n");
      break;
    case CategoryIDActiveCall:
      LOG_INFO_APP("Active Call Category\n");
    break;
    default:
      LOG_INFO_APP("Unknown Category : %d\n", Notif->catID);
      break;
  }    
}

static void ANCS_Display_Notif_List(void)
{
  uint8_t nb_of_item = LST_get_size(&Notif_HeadList);
  LOG_INFO_APP("==> Display Notif List : %d items\n", nb_of_item);
  for (uint8_t index = 0; index < nb_of_item; index++)
  {
    LOG_INFO_APP("      %2d/%2d : App name : %20s | Title : %20s | Message : %20s \n", index + 1, nb_of_item, Notif_List[index].AppName, Notif_List[index].Title, Notif_List[index].Message);
  }
};

static void ANCS_Display_App_Name_List(void)
{
  uint8_t nb_of_item = App_Name_List.free_index;
  LOG_INFO_APP("==> Display App Name List : %d items\n", nb_of_item);
  for (uint8_t index = 0; index < nb_of_item; index++)
  {
    LOG_INFO_APP("      %2d/%2d : App name : %20s | App ID : %20s\n", index + 1, nb_of_item, App_Name_List.AppName[index], App_Name_List.AppID[index]);
  }
};

static void Wait_Notification_Init_cb(void *arg)
{
  if (ANCS_init_data.app_init == false)
  {
    UTIL_TIMER_Start(&Wait_Notification_Init_Id);
  }
  else
  {
    UTIL_SEQ_SetTask(1U << CFG_TASK_ANCS_GET_DETAIL_ID, CFG_SEQ_PRIO_0);    
  }
  return;  
}
/* USER CODE END LF */
