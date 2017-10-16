/**************************************************************************//**
* @file          RZ_A1H_GENMAI_Init.h
* $Rev: 531 $
* $Date:: 2013-04-16 13:07:35 +0900#$
* @brief         RZ_A1H_GENMAI Board Initialize
******************************************************************************/

/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved */

#ifndef RZ_A1H_GENMAI_INIT_H
#define RZ_A1H_GENMAI_INIT_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "iodefine.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/

void    RZ_A1H_GENMAI_SetSramWriteEnable(void);
void    RZ_A1H_GENMAI_InitClock(void);
int     RZ_A1H_GENMAI_IsClockMode0(void);
void    RZ_A1H_GENMAI_InitBus(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* RZ_A1H_GENMAI_INIT_H */
