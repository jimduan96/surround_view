/*****************************************************************************
 *
 * Copyright (c) 2017-2018 NXP
 * All Rights Reserved
 *
 *****************************************************************************
 *
 * THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


#pragma once

/*****************************************************************************
* consts
*****************************************************************************/

enum {
  FDMA_IX_FastDMA_Out,
  FDMA_IX_FastDMA_Out1,
  FDMA_IX_FastDMA_Out2,
  FDMA_IX_FastDMA_Out3,
  FDMA_IX_END
};

/*****************************************************************************
* exported variables
*****************************************************************************/
extern char kmem_srec[];
extern char sequencer_srec[];

extern SEQ_Head_Ptr_t   gpGraph[];
extern GraphMetadata_t  gGraphMetadata;

extern SEQ_Buf_t	FastDMA_OutBuffer;
extern SEQ_Buf_t	MIPI_port_ABuffer0;
extern SEQ_Buf_t	YUV41110bit_to_8bitBuffer0;
extern SEQ_Buf_t	YUV41110bit_to_8bitBuffer1;
extern SEQ_Buf_t	YUV41110bit_to_8bitBuffer2;
extern SEQ_Buf_t	FastDMA_Out1Buffer;
extern SEQ_Buf_t	FastDMA_Out2Buffer;
extern SEQ_Buf_t	YUV42210bit_to_8bit_ipus2Buffer0;
extern SEQ_Buf_t	FastDMA_Out3Buffer;
