/******************************************************************************
* File Name:   retarget_io.h
*
* Description: This is the source code for the XMC MCU: CCU4 Capture Example
*              ModusToolbox. This file is the public interface of retarget_io.c
*              source file.
*
* Related Document: See README.md
*
******************************************************************************
*
* Copyright (c) 2015-2020, Infineon Technologies AG
* All rights reserved.
*
* Boost Software License - Version 1.0 - August 17th, 2003
*
* Permission is hereby granted, free of charge, to any person or organization
* obtaining a copy of the software and accompanying documentation covered by
* this license (the "Software") to use, reproduce, display, distribute,
* execute, and transmit the Software, and to prepare derivative works of the
* Software, and to permit third-parties to whom the Software is furnished to
* do so, all subject to the following:
*
* The copyright notices in the Software and this entire statement, including
* the above license grant, this restriction and the following disclaimer,
* must be included in all copies of the Software, in whole or in part, and
* all derivative works of the Software, unless such copies or derivative
* works are solely in the form of machine-executable object code generated by
* a source language processor.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

#ifndef RETARGET_IO_H
#define RETARGET_IO_H

#include "ring_buffer.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define SERIAL_BUFFER_SIZE 128

/*******************************************************************************
* Global variables
*******************************************************************************/
extern ring_buffer_t serial_buffer;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Function prototypes
*******************************************************************************/
void retarget_io_init(void);

#ifdef __cplusplus
}
#endif

#endif

/* [] END OF FILE */
