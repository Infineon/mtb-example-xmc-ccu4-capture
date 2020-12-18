/******************************************************************************
* File Name:   retarget_io.c
*
* Description: This is the source code for the XMC MCU: CCU4 Capture Example
*              for ModusToolbox. This file implements the linking to the
*              printf standard library functions.
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

#include "cybsp.h"
#include "cy_utils.h"

#include "retarget_io.h"

/*******************************************************************************
* Macros
*******************************************************************************/
/* Defines priority level of the DEBUG_UART receive event interrupt */
#define DEBUG_UART_RECEIVE_EVENT_PRIORITY 63

/*******************************************************************************
* Global Variables
*******************************************************************************/
RING_BUFFER_DEF(serial_buffer, SERIAL_BUFFER_SIZE);

/*******************************************************************************
* Function Name: retarget_io_init
********************************************************************************
* Summary:
* Initialize printf retarget
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void retarget_io_init(void)
{
    serial_buffer.head = 0;
    serial_buffer.tail = 0;
    
    /* Set NVIC priority */
    NVIC_SetPriority(CYBSP_DEBUG_UART_RECEIVE_EVENT_IRQN, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), DEBUG_UART_RECEIVE_EVENT_PRIORITY, 0)); 

    /* Enable IRQ */
    NVIC_EnableIRQ(CYBSP_DEBUG_UART_RECEIVE_EVENT_IRQN);
}

/*******************************************************************************
* Function Name: _close
********************************************************************************
* Summary:
* Dummy implementation of std-library call to satisfy linking
*
* Parameters:
*  int fd: file descriptor
*
* Return:
*  int: -1 to indicate error
*
*******************************************************************************/
int _close(int fd)
{
    CY_UNUSED_PARAMETER(fd);
    return -1;
}

/*******************************************************************************
* Function Name: _fstat
********************************************************************************
* Summary:
* Dummy implementation of std-library call to satisfy linking
*
* Parameters:
*  int fd: file descriptor
*  void *buffer: buffer pointer
*
* Return:
*  int: -1 to indicate error
*
*******************************************************************************/
int _fstat(int fd, void *buffer)
{
    CY_UNUSED_PARAMETER(fd);
    CY_UNUSED_PARAMETER(buffer);
    return -1;
}

/*******************************************************************************
* Function Name: _isatty
********************************************************************************
* Summary:
* Dummy implementation of std-library call to satisfy linking
*
* Parameters:
*  int fd: file descriptor
*
* Return:
*  int: 0 to indicate error
*
*******************************************************************************/
int _isatty(int fd)
{
    CY_UNUSED_PARAMETER(fd);
    return 0;
}

/*******************************************************************************
* Function Name: _lseek
********************************************************************************
* Summary:
* Dummy implementation of std-library call to satisfy linking
*
* Parameters:
*  int fd: file descriptor
*  long offset: offset
*  int origin: origin
*
* Return:
*  int: -1 to indicate error
*
*******************************************************************************/
long _lseek(int fd, long offset, int origin)
{
    CY_UNUSED_PARAMETER(fd);
    CY_UNUSED_PARAMETER(offset);
    CY_UNUSED_PARAMETER(origin);
    return -1;
}

/*******************************************************************************
* Function Name: _write
********************************************************************************
* Summary:
* Functional implementation of std-library to map output to UART
*
* Parameters:
*  int fd: file descriptor
*  const void *buf: pointer to buffer
*  size_t count: buffer size
*
* Return:
*  count of written characters
*
*******************************************************************************/
int _write(int fd, const void *buf, size_t count)
{
    CY_UNUSED_PARAMETER(fd);

    for (size_t i = 0; i < count; ++i)
    {
        /* Transmit single characters using UART */
        XMC_UART_CH_Transmit(CYBSP_DEBUG_UART_HW, *(const uint8_t *)buf);
        buf++;
    }
    return count;
}

/*******************************************************************************
* Function Name: _read
********************************************************************************
* Summary:
* Functional implementation of std-library to map input to UART
*
* Parameters:
*  int fd: file descriptor
*  const void *buf: pointer to buffer
*  size_t count: buffer size
*
* Return:
*  count of read characters
*
*******************************************************************************/
int _read(int fd, const void *buf, size_t count)
{
    CY_UNUSED_PARAMETER(fd);
    int char_cnt = 0;

    for (size_t i = 0; i < count; ++i)
    {
        /* Pop single characters from ringbuffer, until ringbuffer is empty. */
        if (ring_buffer_get(&serial_buffer, (uint8_t *)buf) == RING_BUFFER_EMPTY_ERROR)
        {
            break;
        }

        char_cnt++;

        /* Stop reading if CR (Ox0D) character is received */
        /* New line character (CR) received ? */
        if (*(uint8_t *)buf == 0x0DU)
        {
            /* Yes, convert LF to '\n' char. */
            *(uint8_t*)buf = '\n';
            /* Stop loop and return received char(s) */
            break;
        }

        buf++;
    }

    return char_cnt;
}

/*******************************************************************************
* Function Name: CYBSP_DEBUG_UART_RECEIVE_EVENT_HANDLER
********************************************************************************
* Summary:
* Interrupt triggered for every received character on UART
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CYBSP_DEBUG_UART_RECEIVE_EVENT_HANDLER(void)
{
    static uint8_t data;

    /* Receive single characters from UART and push into ringbuffer */
    data = XMC_UART_CH_GetReceivedData(CYBSP_DEBUG_UART_HW);
    ring_buffer_put(&serial_buffer, data);
}

/* [] END OF FILE */
