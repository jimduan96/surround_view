/*****************************************************************************
 *
 * Copyright 2019 NXP
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/*
 * User's configurations
 *
 */
#ifndef	__USER_CONFIG_H
#define	__USER_CONFIG_H

#include "dcu.h"
 
#define WIDTH_DDR       2560//JD 1280    // < width of DDR buffer in pixels
#define HEIGHT_DDR      800//JD 720     // < height of DDR buffer in pixels
#define M_NUM_OUT_LINES (720)
#define  MULTITRIGGER 0  // one trigger processes 1<<MULTITRIGGER lines
#define DDR_OUT_BUFFER_CNT 3    // < number of DDR buffers per ISP stream
#define LOOP_NUM        30

#define DCU_BPP DCU_BPP_YCbCr422
#define __DCU_BPP 

#endif // __USER_CONFIG_H
