/* Copyright (c) 2017 NXP.
 * All rights reserved.
 */

//S32V234 includes
#include <stdint.h>
#include <stdio.h>
#include <frame_output_v234fb.h>
#include <opencv2/opencv.hpp>
#include <umat.hpp>
#include "nxpvt_utils.hpp"
#include "nxpvt_imshow_w.hpp"

//S32V234 definitions
#define S32V_FRAMEBUFFER_HEIGHT 720
#define S32V_FRAMEBUFFER_WIDTH 1280

int init_done = 0;

io::FrameOutputV234Fb lDcuOutput(S32V_FRAMEBUFFER_WIDTH, S32V_FRAMEBUFFER_HEIGHT, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3);
vsdk::UMat outBufferUMat = vsdk::UMat(S32V_FRAMEBUFFER_HEIGHT, S32V_FRAMEBUFFER_WIDTH, VSDK_CV_8UC3);

void init_framebuffer()
{
    if (!init_done) {
        // printf("[nxpvt_imshow_c] Initializing framebuffer to %dx%d\n", S32V_FRAMEBUFFER_HEIGHT, S32V_FRAMEBUFFER_WIDTH);
        lDcuOutput.Init(width, height, io::IO_DATA_DEPTH_08, io::IO_DATA_CH3);
        cv::Mat outBufferMat = outBufferUMat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        memset(outBufferMat.data, 0, S32V_FRAMEBUFFER_HEIGHT*S32V_FRAMEBUFFER_WIDTH*3);
        init_done = 1;
    }
}

void nxpvt_imshow_w(uint8_t* img, int height, int width)
{
    //clock_t start, start1, end;
    if ((height <= 0) || (width <= 0)) {
        printf("[nxpvt_imshow_c] Invalid sizes %dx%d\n", height, width);
        return;
    }

    // printf("[nxpvt_imshow_c] height=%d, width=%d\n", height, width);
    init_framebuffer();
    {
        cv::Mat imgCvMat(height, width, CV_8UC3, img);
        cv::Mat outBufferCvMat = outBufferUMat.getMat(cv::ACCESS_WRITE | OAL_USAGE_CACHED);
        //start = clock();
        cv::cvtColor(imgCvMat, imgCvMat, cv::COLOR_RGB2BGR);
        imgCvMat.copyTo(outBufferCvMat(cv::Rect(0, 0, width, height)));
        //end = clock();
        //printf("[nxpvt_imshow_c] neon_memcpy time=%lf\n", double(end - start) / CLOCKS_PER_SEC);
    }
    
    lDcuOutput.PutFrame(outBufferUMat);
}

void nxpvt_imshow_w(vsdk::UMat* imgUMat)
{
    //printf("[nxpvt_imshow_c] height=%d, width=%d, channels=%d\n", imgUMat->rows, imgUMat->cols, imgUMat->channels());
    if ((imgUMat->rows <= 0) || (imgUMat->cols <= 0)) {
        printf("[nxpvt_imshow_c] Invalid sizes %dx%d\n", imgUMat->rows, imgUMat->cols);
        return;
    }
    //printf("[nxpvt_imshow_c] image sizes %dx%dx%d\n", imgUMat->rows, imgUMat->cols, imgUMat->channels());

    init_framebuffer();
    

    cv::Mat imgCvMat;
    if (imgUMat->channels() == 1) {
        cv::Mat grayImgCvMat = imgUMat->getMat(cv::ACCESS_READ | OAL_USAGE_CACHED);
        cv::cvtColor(grayImgCvMat, imgCvMat, cv::COLOR_GRAY2BGR);
    } else {
        #ifdef SNOY
        imgCvMat = imgUMat->getMat(cv::ACCESS_READ | OAL_USAGE_CACHED);
        #else
        cv::Mat imgYUVMat = imgUMat->getMat(cv::ACCESS_READ | OAL_USAGE_CACHED);
        //cvtColor(src,dst,CV_YUV2BGR_YUY2);//CV_YUV2BGR_UYVY);
        //vsdk::UMat output_umat = vsdk::UMat(720, 1280, VSDK_CV_8UC3);
        //cv::Mat output_mat = output_umat.getMat(vsdk::ACCESS_WRITE | OAL_USAGE_CACHED);
        //memset(output_mat.data, 0, 720 * 1280 * 3);
        cv::cvtColor(imgYUVMat,imgCvMat,CV_YUV2BGR_UYVY);//CV_YUV2BGR_UYVY);CV_YUV2BGR_YUY2
        #endif
    }

    //printf("[nxpvt_imshow_c] image imgCvMat size %dx%d\n", imgCvMat.rows, imgCvMat.cols/*, imgCvMat.channels()*/);

    cv::Mat outBufferCvMat = outBufferUMat.getMat(cv::ACCESS_WRITE | OAL_USAGE_CACHED);
    imgCvMat.copyTo(outBufferCvMat(cv::Rect(0, 0, imgCvMat.cols, imgCvMat.rows)));

    lDcuOutput.PutFrame(outBufferUMat);
}
