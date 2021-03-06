/**
* \copyright
* Copyright (c) 2018, Infineon Technologies AG
* All rights reserved.                                                         
*
* This software is provided with terms and conditions as specified in OPTIGA(TM) Trust X Evaluation Kit License Agreement.
* \endcopyright
*
* \author Infineon AG
*
* \file ifx_i2c_config.c
*
* \brief   This file provides the ifx i2c platform specific context configurations. 
*
* \addtogroup  grIFXI2C
* @{
*/

/***********************************************************************************************************************
* HEADER FILES
**********************************************************************************************************************/
// Protocol Stack Includes
#include "pal_ifx_i2c_config.h"
#include "ifx_i2c_config.h"

/***********************************************************************************************************************
* MACROS
**********************************************************************************************************************/


/***********************************************************************************************************************
* ENUMS
**********************************************************************************************************************/
/***********************************************************************************************************************
* DATA STRUCTURES
***********************************************************************************************************************/

/** @brief This is IFX I2C context. Only one context is supported per slave.*/
//lint --e{785} suppress "Only required fields are initialized, the rest are handled by consumer of this structure"
ifx_i2c_context_t ifx_i2c_context_0 =
{
    /// Slave address
    0x30,
    /// i2c-master frequency
    400,
    /// IFX-I2C frame size
    0x3B,
    /// Vdd pin
    &optiga_vdd_0,
    /// Reset pin
    &optiga_reset_0,
    /// optiga pal i2c context
    &optiga_pal_i2c_context_0
};

/***********************************************************************************************************************
* GLOBAL
***********************************************************************************************************************/
/***********************************************************************************************************************
* LOCAL ROUTINES
***********************************************************************************************************************/
/***********************************************************************************************************************
* API PROTOTYPES
**********************************************************************************************************************/

/**
 * @}
 **/
