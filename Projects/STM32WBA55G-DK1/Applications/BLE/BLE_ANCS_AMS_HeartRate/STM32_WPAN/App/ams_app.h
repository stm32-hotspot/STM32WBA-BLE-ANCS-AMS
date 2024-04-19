/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ams_app.h
  * @author  MCD Application Team
  * @brief   Header for ams_app.c module
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
#ifndef AMS_APP_H
#define AMS_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
typedef enum {
  WRITE_AMS_CHAR = 0,
  READ_AMS_CHAR,
} AMSCmdToGatt_t;

typedef enum {
  AMS_START_NOTIF = 0,
  AMS_RECEIVE_NOTIF,
  AMS_INIT_HANDLE,
  AMS_DECONNECTION,
} GattCmdToAMS_t;

typedef struct{
  AMSCmdToGatt_t AMSCmdToGatt;
  uint16_t char_UUID;
  uint8_t l_payload;
  uint8_t p_Payload[256];
} ams_interface_gatt_client_t;

#define MAX_DATA_LENGTH_TO_ANCS 256

typedef struct{
  GattCmdToAMS_t GattCmdToAMS;
  uint16_t char_UUID;
  uint8_t l_payload;
  uint8_t p_Payload[MAX_DATA_LENGTH_TO_ANCS];
} gatt_client_interface_ams_t;

typedef struct
{
  uint16_t *connHdl;
  uint16_t *EntityAttributeCharValueHdle;
  uint16_t *RemoteCommandCharValueHdle;
  uint16_t *EntityUpdateCharValueHdle;  
}AMS_init_data_t;

/* ------------ App type def ------------ */

/**
  * @anchor RemoteCommandIDs
  * @name Remote Command ID values
  */
typedef enum {
  RemoteCommandIDPlay                   =  0,
  RemoteCommandIDPause                  =  1,
  RemoteCommandIDTogglePlayPause        =  2,
  RemoteCommandIDNextTrack              =  3,
  RemoteCommandIDPreviousTrack          =  4,
  RemoteCommandIDVolumeUp               =  5,
  RemoteCommandIDVolumeDown             =  6,
  RemoteCommandIDAdvanceRepeatMode      =  7,
  RemoteCommandIDAdvanceShuffleMode     =  8,
  RemoteCommandIDSkipForward            =  9,
  RemoteCommandIDSkipBackward           = 10,
  RemoteCommandIDLikeTrack              = 11,
  RemoteCommandIDDislikeTrack           = 12,
  RemoteCommandIDBookmarkTrack          = 13,
  Nb_of_RemoteCommand                   //Should be last item in the list
} RemoteCommandID;

/**
  * @anchor EntityIDs
  * @name Entity ID values
  */
typedef enum {
  EntityIDPlayer = 0,
  EntityIDQueue = 1,
  EntityIDTrack = 2,
} EntityID;

#define ENTITY_UPDATE_FLAG_TRUNCATED (1 << 0)

/**
  * @anchor PlayerAttributeIDs
  * @name Player Attribute ID  values
  */
typedef enum {
  PlayerAttributeIDName = 0,
  PlayerAttributeIDPlaybackInfo = 1,
  PlayerAttributeIDVolume = 2,
} PlayerAttributeID;

/**
  * @anchor PlaybackState
  * @name Playback State values
  */
typedef enum {
  PlaybackStatePaused = 0,
  PlaybackStatePlaying = 1,
  PlaybackStateRewinding = 2,
  PlaybackStateFastForwarding = 3,
} PlaybackState;

/**
  * @anchor QueueAttributeIDs
  * @name Queue Attribute ID values
  */
typedef enum {
  QueueAttributeIDIndex = 0,
  QueueAttributeIDCount = 1,
  QueueAttributeIDShuffleMode = 2,
  QueueAttributeIDRepeatMode = 3,
} QueueAttributeID;


#define SHUFFLE_MODE_OFF 0
#define SHUFFLE_MODE_ONE 1
#define SHUFFLE_MODE_ALL 2

#define REPEAT_MODE_OFF 0
#define REPEAT_MODE_ONE 1
#define REPEAT_MODE_ALL 2

/**
  * @anchor TrackAttributeIDs
  * @name Track Attribute ID values
  */
typedef enum {
  TrackAttributeIDArtist = 0,
  TrackAttributeIDAlbum = 1,
  TrackAttributeIDTitle = 2,
  TrackAttributeIDDuration = 3,
} TrackAttributeID;

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
void AMS_APP_Init(void);
void start_ams_notif(void);
void AMS_Remote_Cmd(RemoteCommandID RemoteCommand);

void ams_update_singer_song_name(void);
/* USER CODE END EFP */


#ifdef __cplusplus
}
#endif

#endif /*AMS_APP_H */
