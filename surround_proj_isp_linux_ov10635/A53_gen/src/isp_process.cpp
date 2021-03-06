/*****************************************************************************
 *
 * Copyright 2020 NXP
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
*****************************************************************************/

// Generated from:
// D:/workspaceS32DS.3.2/ov10635_surround/model/ov10635_surround.isp 

#ifndef __STANDALONE__

#include <signal.h>

#endif // #ifdef __STANDALONE__

#include <string.h>
#include "config.h"

// choose graphical output and parameters
#ifdef __STANDALONE__
  #include "frame_output_dcu.h"
  #define CHNL_CNT io::IO_DATA_CH3
#else // #ifdef __STANDALONE__
  #include "frame_output_v234fb.h"
  #define CHNL_CNT io::IO_DATA_CH3
#endif // else from #ifdef __STANDALONE__

#include "sdi.hpp"
#include "ov10635_surround_c.h"


//Start of user code section_isp_process_cpp

/*You can write your code here. 
This is the region protected from the source code emitter,
all the rest can be changed by the source code emitted without notice*/	

#include "vdb_log.h"
#include "isp_gen.h"
#include "isp_user_define.h"

#ifndef __STANDALONE__ 
  int32_t SigintSetup(void); 
#endif

// DCU input resolution (must be supported by the DCU)
// define IPU engine responsible for exposure computations

//***********************************************************************

/************************************************************************
 * Statically configures frame exposure parameters.
 ************************************************************************/
#ifndef __STANDALONE__ 
  int32_t SigintSetup(void); 
#endif 
#ifndef __STANDALONE__ 
  int32_t SigintSetup(void); 
static bool sStop = false; ///< to signal Ctrl+c from command line  
 
#endif 


int mFrmCnt     = 0;
int mFrmDoneCnt = 0;

void ISP_CALL(int sCamChanel)
{
  //*** Init DCU Output ***
#ifdef __STANDALONE__

  io::FrameOutputDCU  lDcuOutput(WIDTH_DDR,
                                 HEIGHT_DDR,
                                 io::IO_DATA_DEPTH_08,
                                 CHNL_CNT
#ifdef __DCU_BPP
                                 ,DCU_BPP
#endif
                                 ); 
#else // #ifdef __STANDALONE__

  // setup Ctrl+C handler for Linux
  if(SigintSetup() != SEQ_LIB_SUCCESS)
  {
    VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
    return ;
  }

  printf("Press Ctrl+C to terminate the demo.\n");

  io::FrameOutputV234Fb lDcuOutput(WIDTH_DDR,
                                   HEIGHT_DDR,
                                   io::IO_DATA_DEPTH_08,
                                   CHNL_CNT
#ifdef __DCU_BPP
                                   ,DCU_BPP
#endif
                                   ); 

#endif // else from #ifdef __STANDALONE__


  //
  // *** Initialize SDI ***
  //
  sdi::Initialize(0);
  // *** create grabber ***
  sdi_grabber *lpGrabber = new(sdi_grabber);
  if(lpGrabber == NULL)
  {
    printf("Failed to create sdi grabber.\n");
    return;// -1;
  } // if failed to create grabber
  if(lpGrabber->ProcessSet(gpGraph, &gGraphMetadata) != LIB_SUCCESS)
		  {
		    printf("Failed to set ISP graph to grabber.\n");
		    return;// -1;
		  } // if ISP graph not set
  
  // *** prepare IOs ***

  io_config(lpGrabber);
  
   mFrmCnt     = 0;
   mFrmDoneCnt = 0;

  // *** prestart grabber ***
  if(lpGrabber->PreStart() != LIB_SUCCESS)
		  {
		    printf("Failed to prestart the grabber.\n");
		    return;// -1;
		  } // if PreStart() failed

  // *** configure camera parameters ***
  camera_config(); 

/* JD TBD; copied from sdk c demo. not sure how to use arContext
  if(lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &arContext) != LIB_SUCCESS)
  {
    printf("Failed to install Sequencer event callback.\n");
    return -1;
  } // if callback setup failed
*/



  if(lpGrabber->Start() != LIB_SUCCESS)
		  {
		    printf("Failed to start the grabber.\n");
		    return;// -1;
		  } // if Start() failed

  // fetched frame buffer storage
  //SDI_Frame gFrameIsp;

  //uint32_t lLoop;

  uint32_t  lActiveStreamIndex = 0;
    SDI_Frame lpFrame[STREAM_CNT];

  while (1)
  {

	    // pop all
	    for(int i = 0; i < STREAM_CNT; i++)
	    {
	      lpFrame[i] = lpGrabber->FramePop(i);
	      if(lpFrame[i].mUMat.empty())
	      {
	        printf("Failed to grab image number %u\n", mFrmCnt);
	        //arContext.mError = true;
	        return;// -1;
	      } // if pop failed
	    }   // for all channels
	    if(sCamChanel < 5)
	    {
	      lDcuOutput.PutFrame(lpFrame[sCamChanel - 1].mUMat);
	    }
	    else
	    {
	      if(((++mFrmCnt) % STREAM_CYCLE_FRM_CNT) == 0)
	      {
	        ++lActiveStreamIndex;
	        lActiveStreamIndex = lActiveStreamIndex % STREAM_CNT;
	        printf("Selected camera = %u ", lActiveStreamIndex);
	      } // if stream to be switched

	      lDcuOutput.PutFrame(lpFrame[lActiveStreamIndex].mUMat);
	    }

	    for(int i = 0; i < STREAM_CNT; i++)
	    {
	      if(lpGrabber->FramePush(lpFrame[i]) != LIB_SUCCESS)
	      {
	        printf("Failed to push image number %u\n", mFrmCnt);
	        //arContext.mError = true;
	        break;
	      } // if push failed
	    }

    /* only show first camera
     * for(lLoop=0; lLoop<LOOP_NUM; lLoop++)
    {
      gFrameIsp = lpGrabber->FramePop();

      if(gFrameIsp.mUMat.empty())
      {
        break;
      } // if pop failed

      {
        lDcuOutput.PutFrame(gFrameIsp.mUMat);

        if(lpGrabber->FramePush(gFrameIsp) != LIB_SUCCESS)
        {
          break;
        } // if push failed
      } // no error
    } // for LOOP_NUM

    if(gFrameIsp.mUMat.empty())
    {
      break;
    } // if gpFramout == NULL
    */


    // *** log output ****
#ifndef __STANDALONE__
    if(sStop) // if Ctrl+C
    {
      break;
    }
#endif //#ifndef __STANDALONE__
  } // while(1)

  //printf("\nSequencer stopped working in frame %d\n",gFrmCnt);

  if(lpGrabber)
  {
    //*** Stop ISP processing ***
    lpGrabber->Stop();
    lpGrabber->Release();
    delete lpGrabber;
  } // if grabber exists

#ifdef __STANDALONE__
  for(;;);  // don't return on standalone
#endif // #ifdef __STANDALONE__
}

#ifndef __STANDALONE__
void SigintHandler(int aSigNo)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;

  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;

  if( sigaction(SIGINT, &lSa, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered

  return lRet;
} // SigintSetup()

//***************************************************************************
#endif // #ifndef __STANDALONE__
//End of user code
