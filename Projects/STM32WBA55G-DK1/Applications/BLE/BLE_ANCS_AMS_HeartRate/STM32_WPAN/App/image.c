/**
  ******************************************************************************
  * @file    image.c
  * @author  MCD Application Team
  * @brief   image used for the LCD
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"

const uint8_t volume_up_byteicon[] = {
/* volume-up, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x03, 0x0c, 0x07, 0x04, 0x1f, 0x66, 0x7f, 0x36, 0x7f, 0x32,
0x7f, 0x32, 0x7f, 0x36, 0x1f, 0x66, 0x07, 0x0c, 0x03, 0x0c, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00};

const uint8_t volume_mute_byteicon[] = {
/* volume-mute, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x07, 0x00, 0x1f, 0x00, 0x7f, 0x24, 0x7f, 0x18,
0x7f, 0x18, 0x7f, 0x24, 0x1f, 0x00, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t volume_down_byteicon[] = {
/* volume-down, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x07, 0x00, 0x1f, 0x60, 0x7f, 0x30, 0x7f, 0x30,
0x7f, 0x30, 0x7f, 0x30, 0x1f, 0x60, 0x07, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t initiate_call_byteicon[] = {
/* initiate-call, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x3c, 0x3c, 0x3e, 0x0c, 0x3e, 0x14, 0x3c, 0x24, 0x1c, 0x40,
0x1c, 0x00, 0x0e, 0x00, 0x07, 0xb8, 0x03, 0xfc, 0x01, 0xfc, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00};

const uint8_t answer_call_byteicon[] = {
/* answer-call, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x3c, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x3c, 0x00, 0x1c, 0x00,
0x1c, 0x00, 0x0e, 0x00, 0x07, 0xb8, 0x03, 0xfc, 0x01, 0xfc, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00};

const uint8_t terminate_call_byteicon[] = {
/* terminate-call, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x3f, 0xfc, 0x7f, 0xfe,
0x78, 0x1e, 0x78, 0x1e, 0x70, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t next_track_byteicon[] = {
/* next-track, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x06, 0x61, 0x86, 0x71, 0xc6, 0x7d, 0xf6,
0x7d, 0xf6, 0x71, 0xc6, 0x61, 0x86, 0x41, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t play_pause_byteicon[] = {
/* play-pause, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x66, 0x78, 0x66, 0x7c, 0x66, 0x7f, 0x66,
0x7f, 0x66, 0x7c, 0x66, 0x78, 0x66, 0x60, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t previous_track_byteicon[] = {
/* previous-track, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x82, 0x61, 0x86, 0x63, 0x8e, 0x6f, 0xbe,
0x6f, 0xbe, 0x63, 0x8e, 0x61, 0x86, 0x60, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t hourglass_byteicon[] = {
/* hourglass, 16x16px */
0x00, 0x00, 0x0f, 0xf0, 0x08, 0x10, 0x08, 0x10, 0x08, 0x10, 0x04, 0x20, 0x06, 0x60, 0x02, 0x40,
0x02, 0x40, 0x06, 0x60, 0x04, 0x20, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00};

const uint8_t advertising_byteicon[] = {
/* advertising, 16x16px */
0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x1f, 0xf8, 0x3c, 0x3c, 0x71, 0x8e, 0x6f, 0xf6, 0x1e, 0x78,
0x19, 0x98, 0x07, 0xe0, 0x06, 0x60, 0x01, 0x80, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t arrow_up_byteicon[] = {
/* arrow up, 16x6px */ 
0x01, 0x00, 0x03, 0x80, 0x07, 0xc0, 0x0f, 0xe0, 0x1f, 0xf0, 0x3f, 0xf8};

const uint8_t arrow_down_byteicon[] = {
/* arrow down, 16x6px */
0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00};

const uint8_t arrow_return_byteicon[] = {
/* arrow down, 8x8px */
0x20, 0x60, 0xfc, 0x62, 0x21, 0x01, 0x02, 0x7c};

const uint8_t empty_heart_byteicon[] = {
 /* heart empty, 32x32px */
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x03, 0xc0, 0x07, 0x80,
0x0c, 0x30, 0x18, 0x60,
0x10, 0x08, 0x20, 0x10,
0x10, 0x04, 0x40, 0x10,
0x20, 0x02, 0x80, 0x08,
0x20, 0x01, 0x00, 0x08, 
0x20, 0x00, 0x00, 0x08, 
0x20, 0x00, 0x00, 0x08, 
0x10, 0x00, 0x00, 0x10, 
0x10, 0x00, 0x00, 0x10, 
0x08, 0x00, 0x00, 0x20,
0x04, 0x00, 0x00, 0x40, 
0x02, 0x00, 0x00, 0x80, 
0x01, 0x00, 0x01, 0x00, 
0x00, 0x80, 0x02, 0x00, 
0x00, 0x40, 0x04, 0x00,
0x00, 0x20, 0x08, 0x00,
0x00, 0x10, 0x10, 0x00,
0x00, 0x08, 0x20, 0x00,
0x00, 0x04, 0x40, 0x00,
0x00, 0x02, 0x80, 0x00,
0x00, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00};

const uint8_t full_heart_byteicon[] = {
  /* heart empty, 32x32px */
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x03, 0xc0, 0x07, 0x80,
0x0F, 0xF0, 0x1F, 0xE0,
0x1F, 0xF8, 0x3F, 0xF0,
0x1F, 0xFC, 0x7F, 0xF0,
0x3F, 0xFE, 0xFF, 0xF8,
0x3F, 0xFF, 0xFF, 0xF8,
0x3F, 0xFF, 0xFF, 0xF8,
0x3F, 0xFF, 0xFF, 0xF8,
0x1F, 0xFF, 0xFF, 0xF0,
0x1F, 0xFF, 0xFF, 0xF0,
0x0F, 0xFF, 0xFF, 0xE0,
0x07, 0xFF, 0xFF, 0xC0, 
0x03, 0xFF, 0xFF, 0x80,
0x01, 0xFF, 0xFF, 0x00, 
0x00, 0xFF, 0xFE, 0x00, 
0x00, 0x7F, 0xFC, 0x00,
0x00, 0x3F, 0xF8, 0x00,
0x00, 0x1F, 0xF0, 0x00,
0x00, 0x0F, 0xE0, 0x00,
0x00, 0x07, 0xC0, 0x00,
0x00, 0x03, 0x80, 0x00,
0x00, 0x01, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00};