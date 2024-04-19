/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ancs_app.h
  * @author  MCD Application Team
  * @brief   Header for ancs_app.c module
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ANCS_APP_H
#define ANCS_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm_list.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum {
  WRITE_ANCS_CHAR = 0,
} ANCSCmdToGatt_t;

typedef enum {
  ANCS_RECEIVE_NOTIF = 0,
  ANCS_DECONNECTION,
  ANCS_INIT_HANDLE,
} GattCmdToANCS_t;

typedef struct{
  GattCmdToANCS_t GattCmdToANCS;
  uint16_t char_UUID;
  uint8_t l_payload;
  uint8_t p_Payload[256];
} gatt_client_interface_ancs_t;

typedef struct
{
  bool app_init;
  uint16_t *connHdl;
  uint16_t *NotifSourceCharValueHdle;
  uint16_t *DataSourceCharValueHdle;
  uint16_t *CtrlPointCharValueHdle;  
}ANCS_init_data_t;

/* ------------ App type def ------------ */
/**
  * @anchor CategoryIDs
  * @name Category ID values
  */
typedef enum {
  CategoryIDOther = 0,
  CategoryIDIncomingCall = 1,
  CategoryIDMissedCall = 2,
  CategoryIDVoicemail = 3,
  CategoryIDSocial = 4,
  CategoryIDSchedule = 5,
  CategoryIDEmail = 6,
  CategoryIDNews = 7,
  CategoryIDHealthAndFitness = 8,
  CategoryIDBusinessAndFinance = 9,
  CategoryIDLocation = 10,
  CategoryIDEntertainment = 11,
  CategoryIDActiveCall = 12,    //New CategoryID since IOS 13
} CategoryID_t;

/**
  * @anchor EventIDs
  * @name Event ID values
  */
typedef enum {
  EventIDNotificationAdded = 0,
  EventIDNotificationModified = 1,
  EventIDNotificationRemoved = 2,
} EventID_t;

/**
  * @anchor EventFlags
  * @name Event Flag values
  */
typedef enum {
  EventFlagSilent = (1 << 0),
  EventFlagImportant = (1 << 1),
  EventFlagPreExisting = (1 << 2),
  EventFlagPositiveAction = (1 << 3),
  EventFlagNegativeAction = (1 << 4),
} EventFlags_t;

/**
  * @anchor CommandIDs to control point characteristic commands
  * @name Command ID values
  */
typedef enum {
  CommandIDGetNotificationAttributes = 0,
  CommandIDGetAppAttributes = 1,
  CommandIDPerformNotificationAction = 2,
} CommandID_t;

/**
  * @anchor NotificationAttributeIDs for Get Notification Attribute command
  * @name Notification Attribute ID values for Get notifications attributes command
  */
typedef enum {
  NotificationAttributeIDAppIdentifier = 0,
  NotificationAttributeIDTitle = 1, 
  NotificationAttributeIDSubtitle = 2,
  NotificationAttributeIDMessage = 3,
  NotificationAttributeIDMessageSize = 4,
  NotificationAttributeIDDate = 5,
  NotificationAttributeIDPositiveActionLabel = 6,
  NotificationAttributeIDNegativeActionLabel = 7,
} NotificationAttributeID;

/**
  * @anchor ActionIDs
  * @name Action ID values
  */
typedef enum {
  ActionIDPositive = 0,
  ActionIDNegative = 1,
}ActionID_t;

/**
  * @anchor AppAttributeIDs for Get App Attribute command
  * @name App Attribute ID values for Get notifications attributes command
  */
typedef enum {
  AppAttributeIDDisplayName = 0,
} AppAttributeID;

typedef enum {
  ShowWithoutModif = 0,
  ShowPrevious,
  ShowNext,
} ModifNotifDisplay_t;


#define MAX_CHAR_LENGTH                 100
#define MAX_CHAR_LENGTH_MESSAGE         500

#define MAX_NBR_OF_APP_NAME_STORED      20
#define MAX_NBR_OF_NOTIF                32
#define RESET_UID                       0xFFFFFFFF
/**
 * @anchor notificationAttrS
 * @name Notification Attributes typedef
 */
typedef struct{
  tListNode     Node;

  uint8_t       evtFlag;
  CategoryID_t  catID;
  uint32_t      UID;
  
  bool          retrieve_more_data_flag;  /* flag to indicate that data should be retrieved */
  CommandID_t   Data_To_retrieve;

  char  AppIdentifier[MAX_CHAR_LENGTH];
  char  AppName[MAX_CHAR_LENGTH];
  char  Title[MAX_CHAR_LENGTH];
  char  SubTitle[MAX_CHAR_LENGTH];
  char  Message[MAX_CHAR_LENGTH_MESSAGE];
  char  MessageSize[2];
  char  Date[7];
  char  PositiveActionLabel[MAX_CHAR_LENGTH];
  char  NegativeActionLabel[MAX_CHAR_LENGTH];  
} Notif_List_t;
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Exported macros ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/

/* USER CODE BEGIN EFP */
void ANCS_APP_Init(void);
void ANCS_Perform_Notification_Action(ActionID_t ActionID);

void ancs_update_notif(ModifNotifDisplay_t);
/* USER CODE END EFP */


#ifdef __cplusplus
}
#endif

#endif /*ANCS_APP_H */
