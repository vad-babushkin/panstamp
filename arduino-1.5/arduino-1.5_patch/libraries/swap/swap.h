/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 07/10/2014
 */

#ifndef _SWAP_H
#define _SWAP_H

#include "register.h"
#include "config.h"
#include "repeater.h"

/**
 * Macros
 */
#define eepromToFactoryDefaults()                     \
  EEPROM.write(EEPROM_SYNC_WORD, CCDEF_SYNC1);        \
  EEPROM.write(EEPROM_SYNC_WORD + 1, CCDEF_SYNC0);    \
  EEPROM.write(EEPROM_DEVICE_ADDR, CCDEF_ADDR);       \
  EEPROM.write(EEPROM_FREQ_CHANNEL, CCDEF_CHANNR);    \
  EEPROM.write(EEPROM_TX_INTERVAL, 0xFF);             \
  EEPROM.write(EEPROM_TX_INTERVAL + 1, 0xFF)

#define enableAntiPlayback()    security |= 0x01

#define swapNetworkId    radio.syncWord

#define setSwapStatusCallBack(ptrFunc)     statusReceived = ptrFunc

/**
 * System states
 */
enum SYSTATE
{
  SYSTATE_RESTART = 0,
  SYSTATE_RXON,
  SYSTATE_RXOFF,
  SYSTATE_SYNC,
  SYSTATE_LOWBAT,
  SYSTATE_UPGRADE
};

/**
 * Types of SWAP message for attachInterrupt
 */
#define STATUS  0
#define QUERY   1
#define COMMAND 2

/**
 * Class: SWAP
 * 
 * Description:
 * SWAP protocol class
 */
class SWAP
{   
  public:   
    /**
     * Pointer to array of registers
     */
    REGISTER **regTable;
    
    /**
     * Size of the array of registers
     */
    uint8_t regTableSize;
    
    /**
     * Pointer to repeater object
     */
    REPEATER *repeater;

    /**
     * SWAP extended address
     */
    uint16_t devAddress;

    /**
     * Security options
     */
    uint8_t security;

    /**
     * Security cyclic nonce
     */
    uint8_t nonce;
    
    /**
     * System state
     */
    uint8_t systemState;

    /**
     * Interval between periodic transmissions. 0 for asynchronous transmissions
     */
    uint16_t txInterval;

    /**
     * Smart encryption password
     */
    uint8_t *encryptPwd;

    /**
     * SWAP status packet received. Callback function
     */
    void (*statusReceived)(SWPACKET *status);
    
    /**
     * enableRepeater
     *
     * Enable repeater mode
     *
     * @param maxHop Maximum repeater count. Zero if omitted
     */
    void enableRepeater(uint8_t maxHop=0);

    /**
     * SWAP
     *
     * Class constructor
     * 
     * @param regTbl  Pointer to table of SWAP registers
     * @param numRegs  Number of registers
     */
    SWAP(REGISTER** regTbl, uint8_t numRegs);

    /**
     * enterSystemState
     *
     * Enter system state
     *
     * @param state New system state
     */
    void enterSystemState(SYSTATE state);
    /**
     * goToSleep
     *
     * put the MCU in sleep mode during txInterval seconds
     */
    void goToSleep(void);

    /**
     * setSmartPassword
     * 
     * Set Smart Encryption password
     * 
     * @param password Encryption password
     */
    void setSmartPassword(unsigned char* password);
    
    /**
     * attachInterrupt
     * 
     * Declare custom ISR, to be called whenever a SWAP packet is received
     * 
     * @param type of packet that triggers the user function
     * @param funct pointer to the custom function
     */
     inline void attachInterrupt(uint8_t type, void (*funct)(SWPACKET*))
     {
       if (type == STATUS)
         statusReceived = funct;
     }
};

/**
 * Global SWAP object
 */
extern SWAP swap;

/**
 * getRegister
 *
 * Return pointer to register with ID = regId
 *
 * @param regId Register ID
 */
REGISTER * getRegister(uint8_t regId);

#endif
