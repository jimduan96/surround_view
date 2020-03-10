##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2015 Freescale Semiconductor;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################

SDK_ROOT := $(realpath $(S32V234_SDK_ROOT))

ARM_DEFS += -DVSDK_UMAT_USE_OPENCV

ARM_APP = s32v234

ISP_GRAPH = mipi_simple

PRE_BUILT_SYSTEM_LIBS = 1 # 0 to face rebuild sdk

SEQ_MODE = dynamic
SEQ_FW = $(SEQ_MODE)_$(ISP_GRAPH)

VPATH += $(VPATH_LIST)
ARM_APP_SRCS = $(ARM_APP_SRCS_LIST)

ARM_INCS =                                                               \
    -I$(NXPVT_ROOT)/internals/include/s32v                               \
    -I$(NXPVT_ROOT)/internals/src/s32v                                   \
    -I$(SDK_ROOT)/include                                                \
    -I$(SDK_ROOT)/isp/inc                                                \
    -I$(SDK_ROOT)/isp/graphs/mipi_simple/inc                             \
    -I$(SDK_ROOT)/libs/apex/acf/include                                  \
    -I$(SDK_ROOT)/libs/apex/acf_offline/include                          \
    -I$(SDK_ROOT)/libs/apex/drivers/user/include                         \
    -I$(SDK_ROOT)/libs/apex/icp/include                                  \
	-I$(SDK_ROOT)/libs/io/dcu/include                                    \
	-I$(SDK_ROOT)/libs/io/frame_io/include                               \
    -I$(SDK_ROOT)/platform/s32_v234                                      \
    -I$(OPENCV_ROOT)/include                                             \
	-I.                                                                  \
    -I..                                                                 \
    -I../graphs                                                          \
    -I../graphs/build-apu-nxp-sa-d                                       \
    -I$(MATLAB_ROOT)/toolbox/vision/builtins/src/ocv/include             \
    -I$(MATLAB_ROOT)/toolbox/vision/builtins/src/ocv                     \

ARM_APP_LIBS =                                                           \
    $(SDK_ROOT)/3rdparty/ocv/linux-arm64/lib/libopencv_calib3d.so        \
    $(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(SEQ_FW)/$(ODIR)/lib$(SEQ_FW).a \
    $(SDK_ROOT)/libs/io/sdi/$(ODIR)/libsdi.a                             \
    $(SDK_ROOT)/libs/isp/cam_generic/user/$(ODIR)/libcamdrv.a            \
    $(SDK_ROOT)/libs/isp/csi/user/$(ODIR)/libcsidrv.a                    \
    $(SDK_ROOT)/libs/isp/fdma/user/$(ODIR)/libfdmadrv.a                  \
    $(SDK_ROOT)/libs/isp/h264enc/user/$(ODIR)/libh264encdrv.a            \
    $(SDK_ROOT)/libs/isp/h264dec/user/$(ODIR)/libh264decdrv.a            \
    $(SDK_ROOT)/libs/isp/jpegdec/user/$(ODIR)/libjpegdecdrv.a            \
    $(SDK_ROOT)/libs/isp/sequencer/user/$(ODIR)/libseqdrv.a              \
    $(SDK_ROOT)/libs/isp/viu/user/$(ODIR)/libviudrv.a                    \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                    \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                          \
    $(SDK_ROOT)/libs/utils/umat/$(ODIR)/libumat.a                        \
    $(SDK_ROOT)/libs/utils/sumat/$(ODIR)/libsumat.a                      \

# dependency project for the ISP graph
ARM_LIB_LIBS = $(SDK_ROOT)/isp/firmware/$(ODIR)/libsequencer.a

##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################	
ifneq (,$(findstring -sa-d,$(ODIR)))

    ARM_LDOPTS +=                                                            \
        -lopencv_core                                                        \
        -lopencv_features2d                                                  \
        -lopencv_flann                                                       \
        -lopencv_highgui                                                     \
        -lopencv_imgcodecs                                                   \
        -lopencv_imgproc                                                     \
        -lopencv_ml                                                          \
        -lopencv_videoio                                                     \
		-lzlib

##############################################################################
# LINUX SPECIFIC INCLUDES
##############################################################################			
else
    ARM_LDOPTS +=                                                            \
        -lopencv_core                                                        \
        -lopencv_features2d                                                  \
        -lopencv_flann                                                       \
        -lopencv_highgui                                                     \
        -lopencv_imgcodecs                                                   \
        -lopencv_imgproc                                                     \
        -lopencv_ml                                                          \
        -lopencv_objdetect                                                   \
        -lopencv_videoio                                                     \
        -lavcodec                                                            \
        -lavdevice                                                           \
        -lavfilter                                                           \
        -lavformat                                                           \
        -lavutil                                                             \
        -lswresample                                                         \
        -lswscale
endif