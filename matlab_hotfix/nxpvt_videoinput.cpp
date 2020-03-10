/* Copyright (c) 2017 NXP.
 * All rights reserved.
 */

#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <opencv2/opencv.hpp>
#include "nxpvt_videoinput.hpp"
#include "nxpvt_utils.hpp"
  #include "frame_output_v234fb.h"

using namespace std;

namespace nxpvt
{
    
    static nxpvt::videoinput *objPtr = NULL;
    #ifdef SONY
    static SDI_Frame lFrameRgb;
    #else
    #define STREAM_CNT 4

       int mFrmCnt     = 0;
   int mFrmDoneCnt = 0;
     uint32_t  lActiveStreamIndex = 0;
    static SDI_Frame lpFrame0;
    static SDI_Frame lpFrame1;
    static SDI_Frame lpFrame2;
    static SDI_Frame lpFrame3;//[STREAM_CNT];
    #endif
    
    static void sighandler(int signal)
    {
        if (signal == SIGINT) {
            cout << "Ctrl + C detected"<<endl;
        }
        else {
            cout << "Signal detected (" << signal << ")" << endl;
        }
        if (objPtr)
            delete objPtr;
        exit(0);
    }
    
    /*static io::FrameOutputV234Fb lDcuOutput(1280,
                                   720,
                                   io::IO_DATA_DEPTH_08,
                                   io::IO_DATA_CH3,
                                   DCU_BPP_YCbCr422 ); */

    videoinput::videoinput(const char cameraIdx, int frameHeight, int frameWidth, bool verticalFlip, bool horizontalFlip)
    {
        mHeight = frameHeight;
        mWidth = frameWidth;
        mCameraIdx = cameraIdx;
        
        cameraInput = new ISPCameraInputStereo(mCameraIdx);
        cameraInput->Init(mHeight, mWidth, verticalFlip, horizontalFlip);
        cameraInput->Start();
        signal(SIGINT, sighandler);
        signal(SIGSEGV, sighandler);
        signal(SIGPIPE, sighandler);
        objPtr = this;
        
        
    }
    
      
    vsdk::UMat videoinput::getSnapshot()
    {
    #ifdef SONY
        // Grab the image
        if (!lFrameRgb.mUMat.empty()) {
            cameraInput->PutFrame(lFrameRgb);
        }
        cameraInput->GetFrame(FDMA_IX_FastDMA_Out_MIPI_SIMPLE, lFrameRgb);
        if(lFrameRgb.mUMat.empty()) {
            cout << "Error getting camera frame!" <<endl;
        }  
        return lFrameRgb.mUMat;
    #else
        // PutFrame->FramePush
        //for(int i = 0; i < STREAM_CNT; i++)
        {
            if (!lpFrame0.mUMat.empty()) 
            cameraInput->PutFrame(lpFrame0);
            if (!lpFrame1.mUMat.empty()) 
            cameraInput->PutFrame(lpFrame1);
            if (!lpFrame2.mUMat.empty()) 
            cameraInput->PutFrame(lpFrame2);
            if (!lpFrame3.mUMat.empty()) 
            cameraInput->PutFrame(lpFrame3);
        }

        
        //GetFrame->FramePop
          cameraInput->GetFrame(FDMA_IX_FastDMA_Out, lpFrame0);
          if(lpFrame0.mUMat.empty())
          {
            cout << "Error getting camera 0 frame!" <<endl;
          } // if pop failed
         /* cameraInput->GetFrame(FDMA_IX_FastDMA_Out1, lpFrame1);
          cameraInput->GetFrame(FDMA_IX_FastDMA_Out2, lpFrame2);
          cameraInput->GetFrame(FDMA_IX_FastDMA_Out3, lpFrame3);*/

//cout << "Well getting camera 0 frame!" <<endl;
//lDcuOutput.PutFrame(lpFrame0.mUMat);
        return lpFrame0.mUMat;

    #endif  
    }
    
    videoinput::~videoinput()
    {
        cameraInput->Stop();
        cameraInput->Deinit();
        delete cameraInput;
        cameraInput = NULL;
    }

    
}
