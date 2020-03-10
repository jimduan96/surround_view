//****************************************************************************
//
//  Freescale Confidential Proprietary
//
//  Copyright (c) 2016 Freescale Semiconductor;
//  All Rights Reserved
//
// ****************************************************************************
//
//  THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
//  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
//  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//  THE POSSIBILITY OF SUCH DAMAGE.
//
// **************************************************************************

#include "camera_input.h"

extern char sequencer_srec[];
extern char kmem_srec[];
//extern SEQ_Head_Ptr_t   gpGraph_mipi_simple[];
//extern GraphMetadata_t  gGraphMetadata_mipi_simple;

uint32_t gGammaOn = 1;
uint16_t gKneeVal = 256;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
ISPCameraInputStereo::ISPCameraInputStereo(int csiIdx)
{
    mCsiIdx = csiIdx;
}

ISPCameraInputStereo::~ISPCameraInputStereo()
{
    if (mpGrabber) {
        Stop();
        Deinit();
        delete(mpGrabber);
    }
}

bool ISPCameraInputStereo::isCameraInited()
{
    return mCameraInited;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Init(int height, int width, bool verticalFlip, bool horizontalFlip)
{
  // *** Initialize SDI ***
  if (sdi::Initialize(0) != LIB_SUCCESS) {
    VDB_LOG_ERROR("Failed to initialized SDI.");
    return(-1);
  }                                    // if sdi init failed  
  // *** create grabber ***
  mpGrabber = new(sdi_grabber);
  if(mpGrabber == NULL)
  {
    printf("Failed to create sdi grabber.\n");
    return -1;
  } // if failed to create grabber

#ifdef SONY
  if (mpGrabber->ProcessSet(gpGraph_mipi_simple, &gGraphMetadata_mipi_simple,
          kmem_srec_mipi_simple, sequencer_srec_mipi_simple) != LIB_SUCCESS)
  {
    printf("Failed to set ISP graph to grabber.\n");
    return -1;
  }
  #else
    if(mpGrabber->ProcessSet(gpGraph, &gGraphMetadata, 
      kmem_srec, sequencer_srec) != LIB_SUCCESS)
    {
      printf("Failed to set ISP graph to grabber.\n");
      return -1;
    } // if ISP graph not set
  #endif

#ifdef SONY
  // Switch to camera 2 !
  if (mCsiIdx == CAMERA2) {
      if(mpGrabber->IoGet(SEQ_OTHRIX_MIPICSI0)) {
          mpGrabber->CsiSwap(1 + SEQ_OTHRIX_MIPICSI0, SEQ_OTHRIX_MIPICSI0);
      }
  }
 #else
  
  if(mpGrabber->IoGet(SEQ_OTHRIX_MIPICSI0) && mCsiIdx > 0)
  {
    mpGrabber->CsiSwap(mCsiIdx + SEQ_OTHRIX_MIPICSI0, SEQ_OTHRIX_MIPICSI0);
  } // if(!lCamIo.mCsiIdxs.empty() && sCsiPort > 0)
   
  #endif

  // *** prepare IOs ***
  sdi_FdmaIO *lpFdma = (sdi_FdmaIO*)mpGrabber->IoGet(SEQ_OTHRIX_FDMA);  
  if(lpFdma == NULL)
    {
      printf("Failed to get FDMA object.\n");
      return -1;
    }
  
#ifdef SONY  
  // *** RGB888 images ***
  SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(width, height, RGB888);
  if (lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out_MIPI_SIMPLE, lFrmDesc) != LIB_SUCCESS) {
    printf("Failed to set image descriptor.\n");
    return -1;
  } // if frame descriptor setup failed
#else
  SDI_ImageDescriptor lFrmDesc;
  lFrmDesc = SDI_ImageDescriptor(width, height, YUV422Stream_UYVY);

  if(lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out, lFrmDesc) != LIB_SUCCESS) //JD needs DC
  {
    printf("Failed to set image descriptor 0.\n");
    return -1;
  } // if frame descriptor setup failed

  if(lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out1, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 1.\n");
    return -1;
  } // if frame descriptor setup failed

  if(lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out2, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 2.\n");
    return -1;
  } // if frame descriptor setup failed

  if(lpFdma->DdrBufferDescSet(FDMA_IX_FastDMA_Out3, lFrmDesc) != LIB_SUCCESS)
  {
    printf("Failed to set image descriptor 3.\n");
    return -1;
  } // if frame descriptor setup failed

#endif
  
  if (lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT) != LIB_SUCCESS) {
    printf("Failed to allocate DDR buffers.\n");
    return -1;
  }
  
#ifdef SONY  
  // *** prestart grabber ***
  mpGrabber->PreStart();
#else
  if(mpGrabber->PreStart() != LIB_SUCCESS)
  {
    printf("Failed to prestart the grabber.\n");
    return -1;
  } // if PreStart() failed
#endif

#ifdef SONY  
   /* Rotate image from camera */
  if (verticalFlip || horizontalFlip) {
      SONY_Geometry_t lGeo;
      lGeo.mCsiIdx = (mCsiIdx == 1)?CSI_IDX_1:CSI_IDX_0;
      SONY_GeometryGet(lGeo);
      if (verticalFlip) {
        lGeo.mVerFlip = 1;
      };
      
      if (horizontalFlip) {
        lGeo.mHorFlip = 1;
      };
      
      SONY_GeometrySet(lGeo);
  }
#endif

  mCameraInited = true;
  
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int ISPCameraInputStereo::getCsiIdx()
{
    return mCsiIdx;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Start()
{
  //*** Start ISP processing ***
  if(mpGrabber->Start() != LIB_SUCCESS)
      {
        printf("Failed to start the grabber.\n");
        return -1;
      }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Stop()
{
  mpGrabber->Stop();
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
int32_t ISPCameraInputStereo::Deinit()
{
  mpGrabber->Release();
  mCameraInited = false;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
vsdk::UMat ISPCameraInputStereo::GetFrame(uint32_t aIdx, SDI_Frame &arFrame)
{
  arFrame = mpGrabber->FramePop(aIdx);
  return arFrame.mUMat;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void ISPCameraInputStereo::PutFrame(SDI_Frame &arFrame)
{
  mpGrabber->FramePush(arFrame);
}
