## __BLE_ANCS_AMS_HeartRate Application Description__

How to use the Heart Rate profile as specified by the BLE SIG.
How to use the Apple Notification Cetner and Media Services (ANCS & AMS) as specified by Apple.

### __Keywords__

Connectivity, BLE, BLE protocol, BLE pairing, BLE profile

### __Directory contents__

  - BLE_ANCS_AMS_HeartRate/Core/Inc/app_common.h                                             App Common application configuration file for STM32WPAN Middleware
  - BLE_ANCS_AMS_HeartRate/Core/Inc/app_conf.h                                               Application configuration file for STM32WPAN Middleware
  - BLE_ANCS_AMS_HeartRate/Core/Inc/app_entry.h                                              Interface to the application 
  - BLE_ANCS_AMS_HeartRate/Core/Inc/main.h                                                   Header for main.c file. This file contains the common defines of the application
  - BLE_ANCS_AMS_HeartRate/Core/Inc/stm32wbaxx_hal_conf.h                                    HAL configuration file
  - BLE_ANCS_AMS_HeartRate/Core/Inc/stm32wbaxx_it.h                                          This file contains the headers of the interrupt handlers
  - BLE_ANCS_AMS_HeartRate/Core/Inc/stm32wba55g_discovery_conf.h                             STM32WBA55G_DK1 board configuration file.
  - BLE_ANCS_AMS_HeartRate/Core/Inc/utilities_conf.h                                         Header for configuration file for STM32 Utilities
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/app_ble.h                                          Header for ble application 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ble_conf.h                                         Configuration file for BLE Middleware
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ble_dbg_conf.h                                     Debug configuration file for BLE Middleware
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs.h                                              Header for service1.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs_app.h                                          Header for service1_app.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/dis.h                                              Header for dis.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/dis_app.h                                          Header for dis_app.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs.h                                              Header for hrs.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs_app.h                                          Header for hrs_app.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ancs_app.h                                         Header for ancs_app.c    
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ams_app.h                                          Header for ams_app.c  
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/gatt_client_app.h                                  Header for gatt_client_app.c
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/app_menu.h                                         Header for app_menu.c 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/bpka.h                                          This file contains the interface of the BLE PKA module
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/host_stack_if.h                                 This file contains the interface for the stack tasks 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/ll_sys_if.h                                     Header file for ll_sys_if.c
  - BLE_ANCS_AMS_HeartRate/System/Config/Debug_GPIO/app_debug.h                              Real Time Debug module application APIs and signal table 
  - BLE_ANCS_AMS_HeartRate/System/Config/Debug_GPIO/app_debug_signal_def.h                   Real Time Debug module application signal definition 
  - BLE_ANCS_AMS_HeartRate/System/Config/Debug_GPIO/debug_config.h                           Real Time Debug module general configuration file 
  - BLE_ANCS_AMS_HeartRate/System/Config/Flash/simple_nvm_arbiter_conf.h                     Configuration header for simple_nvm_arbiter.c module 
  - BLE_ANCS_AMS_HeartRate/System/Config/LowPower/app_sys.h                                  Header for app_sys.c 
  - BLE_ANCS_AMS_HeartRate/System/Config/LowPower/user_low_power_config.h                    Header for user_low_power_config.c
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw.h                                            This file contains the interface of STM32 HW drivers
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw_if.h                                         Hardware Interface 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/stm32_lpm_if.h                                  Header for stm32_lpm_if.c module (device specific LP management) 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/timer_if.h                                      configuration of the timer_if.c instances 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/usart_if.h                                      Header file for stm32_adv_trace interface file 
  - BLE_ANCS_AMS_HeartRate/System/Modules/adc_ctrl.h                                         Header for ADC client manager module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/ble_timer.h                                        This header defines the timer functions used by the BLE stack 
  - BLE_ANCS_AMS_HeartRate/System/Modules/dbg_trace.h                                        Header for dbg_trace.c 
  - BLE_ANCS_AMS_HeartRate/System/Modules/otp.h                                              Header file for One Time Programmable (OTP) area 
  - BLE_ANCS_AMS_HeartRate/System/Modules/scm.h                                              Header for scm.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/stm_list.h                                         Header file for linked list library
  - BLE_ANCS_AMS_HeartRate/System/Modules/utilities_common.h                                 Common file to utilities 
  - BLE_ANCS_AMS_HeartRate/System/Modules/baes/baes.h                                        This file contains the interface of the basic AES software module
  - BLE_ANCS_AMS_HeartRate/System/Modules/baes/baes_global.h                                 This file contains the internal definitions of the AES software module
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/flash_driver.h                               Header for flash_driver.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/flash_manager.h                              Header for flash_manager.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/rf_timing_synchro.h                          Header for rf_timing_synchro.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/simple_nvm_arbiter.h                         Header for simple_nvm_arbiter.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/simple_nvm_arbiter_common.h                  Common header of simple_nvm_arbiter.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/MemoryManager/advanced_memory_manager.h            Header for advance_memory_manager.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/MemoryManager/stm32_mm.h                           Header for stm32_mm.c module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Nvm/nvm.h                                          This file contains the interface of the NVM manager
  - BLE_ANCS_AMS_HeartRate/System/Modules/RFControl/rf_antenna_switch.h                      RF related module to handle dedictated GPIOs for antenna switch
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/debug_signals.h                            Real Time Debug module System and Link Layer signal definition 
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/local_debug_tables.h                       Real Time Debug module System and Link Layer signal 
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/RTDebug.h                                  Real Time Debug module API declaration 
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/RTDebug_dtb.h                              Real Time Debug module API declaration for DTB usage
  - BLE_ANCS_AMS_HeartRate/Core/Src/app_entry.c                                              Entry point of the application 
  - BLE_ANCS_AMS_HeartRate/Core/Src/main.c                                                   Main program body 
  - BLE_ANCS_AMS_HeartRate/Core/Src/stm32wbaxx_hal_msp.c                                     This file provides code for the MSP Initialization and de-Initialization codes
  - BLE_ANCS_AMS_HeartRate/Core/Src/stm32wbaxx_it.c                                          Interrupt Service Routines
  - BLE_ANCS_AMS_HeartRate/Core/Src/system_stm32wbaxx.c                                      CMSIS Cortex-M33 Device Peripheral Access Layer System Source File 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/app_ble.c                                          BLE Application
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs.c                                              service1 definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs_app.c                                          service1_app application definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/dis.c                                              dis service definition 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/dis_app.c                                          dis service_app application definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs.c                                              heartrate service definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/hrs_app.c                                          heartrate service_app application definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ancs_app.c                                         ANCS client_app application definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/ams_app.c                                          AMS client_app application definition
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/app_menu_cfg.c                                     Config file for app_menu.c
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/App/image.c                                            Image used by LCD
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/bleplat.c                                       This file implements the platform functions for BLE stack library
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/bpka.c                                          This file implements the BLE PKA module
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/host_stack_if.c                                 Source file for the stack tasks 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/linklayer_plat.c                                Source file for the linklayer plateform adaptation layer 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/ll_sys_if.c                                     Source file for initiating the system sequencer 
  - BLE_ANCS_AMS_HeartRate/STM32_WPAN/Target/power_table.c                                   This file contains supported power tables 
  - BLE_ANCS_AMS_HeartRate/System/Config/Debug_GPIO/app_debug.c                              Real Time Debug module application side APIs 
  - BLE_ANCS_AMS_HeartRate/System/Config/LowPower/user_low_power_config.c                    Low power related user configuration
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw_aes.c                                        This file contains the AES driver for STM32WBA 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw_otp.c                                        This file contains the OTP driver
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw_pka.c                                        This file contains the PKA driver for STM32WBA 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/hw_rng.c                                        This file contains the RNG driver for STM32WBA 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/pka_p256.c                                      This file is an optional part of the PKA driver for STM32WBA. It is dedicated to the P256 elliptic curve
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/stm32_lpm_if.c                                  Low layer function to enter/exit low power modes (stop, sleep) 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/timer_if.c                                      Configure RTC Alarm, Tick and Calendar manager 
  - BLE_ANCS_AMS_HeartRate/System/Interfaces/usart_if.c                                      Source file for interfacing the stm32_adv_trace to hardware 
  - BLE_ANCS_AMS_HeartRate/System/Modules/adc_ctrl.c                                         Header for ADC client manager module 
  - BLE_ANCS_AMS_HeartRate/System/Modules/app_sys.c                                          Application system for STM32WPAN Middleware
  - BLE_ANCS_AMS_HeartRate/System/Modules/ble_timer.c                                        This module implements the timer core functions 
  - BLE_ANCS_AMS_HeartRate/System/Modules/otp.c                                              Source file for One Time Programmable (OTP) area 
  - BLE_ANCS_AMS_HeartRate/System/Modules/scm.c                                              Functions for the System Clock Manager
  - BLE_ANCS_AMS_HeartRate/System/Modules/stm_list.c                                         TCircular Linked List Implementation
  - BLE_ANCS_AMS_HeartRate/System/Modules/baes/baes_cmac.c                                   This file contains the AES CMAC implementation
  - BLE_ANCS_AMS_HeartRate/System/Modules/baes/baes_ecb.c                                    This file contains the AES ECB functions implementation
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/flash_driver.c                               The Flash Driver module is the interface layer between Flash management modules and HAL Flash drivers
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/flash_manager.c                              The Flash Manager module provides an interface to write raw data from SRAM to FLASH
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/rf_timing_synchro.c                          The RF Timing Synchronization module provides an interface to synchronize the flash processing versus the RF activity to make sure the RF timing is not broken
  - BLE_ANCS_AMS_HeartRate/System/Modules/Flash/simple_nvm_arbiter.c                         The Simple NVM arbiter module provides an interface to write and/or restore data from SRAM to FLASH with use of NVMs
  - BLE_ANCS_AMS_HeartRate/System/Modules/MemoryManager/advanced_memory_manager.c            Memory Manager 
  - BLE_ANCS_AMS_HeartRate/System/Modules/MemoryManager/stm32_mm.c                           Memory Manager 
  - BLE_ANCS_AMS_HeartRate/System/Modules/Nvm/nvm_emul.c                                     This file implements the RAM version of the NVM manager for STM32WBX. It is made for test purpose
  - BLE_ANCS_AMS_HeartRate/System/Modules/RFControl/rf_antenna_switch.c                      RF related module to handle dedictated GPIOs for antenna switch
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/RTDebug.c                                  Real Time Debug module API definition 
  - BLE_ANCS_AMS_HeartRate/System/Modules/RTDebug/RTDebug_dtb.c                              Real Time Debug module API definition for DTB usage

### __Hardware and Software environment__

  - This example runs on STM32WBA55xx Discovery kit.
  - Connect the Discovery Kit Board to your PC with a USB cable type A to type C to ST-LINK connector (USB_STLINK). 

### __How to use it?__

If not already done, have a look at the readme file in the home directory of this git.

In order to make the program work, you must do the following:
 - Open IAR toolchain
 - Rebuild all files and flash the board with the executable file

 On the android/ios device, enable the Bluetooth communications, and if not done before:

   - Install the ST BLE Toolbox application on the ios device:
     - <a href="https://apps.apple.com/us/app/st-ble-toolbox/id1531295550"> ST BLE Toolbox iOS</a>

   - You can also install the ST BLE Sensor application on the ios device:
       - <a href="https://itunes.apple.com/us/App/st-bluems/id993670214?mt=8"> ST BLE Sensor iOS</a>

 Power on the Discovery Kit board with the BLE_ANCS_AMS_HeartRate application.

- Then, click on the App icon, ST BLE Toolbox (ios device),
   You can either open ST BLE Sensor application (ios device).

- When connected with the client, pairing request is send, a popup asks you to associate your device. Once done, another popup asks the notification allowance, if not, leave the app without closing it, the popu should appear.

- On the board, you should be able to see "connected" as well as the current time on the phone and the battery.
- With the right joystick, you can access the selection menu where you can :
   - change the phone's volume with up/down joystick.
   - display song/singer, pause/play/change current music track (or media) with right/up/down joystick.
   - display your notification, and go to the next/previous one with up/down joystick. You can also do an action on the current displayed notification by pressing right joystick. The possible action will be displayed on the screen. (Must of the time the down action will be to delete the notification)
   - display the HeartRate and energy measurement.

To go on the last element shown, use the left joystick.