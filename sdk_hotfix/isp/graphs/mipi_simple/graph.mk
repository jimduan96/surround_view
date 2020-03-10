##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2015 Freescale Semiconductor;
# Copyright 2017 NXP;
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
#
#  CONTENT
#    JPEG 4 stream ISP graph. Includes build of relevant kernels
#    Sequencer Firmware for both dynamic and static mode (for M0+)
#
#  AUTHOR
#    Armin Strobl
#    Mihail Marian Nistor
#
#  DATE
#    2018-01-09
#
#  LANGUAGE
#    make
#
##############################################################################

##############################################################################
# check required parameters
##############################################################################

ifeq ($(origin SDK_ROOT),undefined)
  $(error error: SDK_ROOT undefined)
endif

include $(SDK_ROOT)/isp/firmware/seq_fw.mk

ifeq ($(origin TOOLCH),undefined)
  $(error error: TOOLCH undefined)
endif

GRAPH_NAME = mipi_simple

##############################################################################
# Sequencer Firmware
##############################################################################
SEQ_APP =sequencer

VPATH +=$(SDK_ROOT)/isp/graphs/$(GRAPH_NAME)/src

SEQ_APP_SRCS = \
	$(SEQ_MODE)_$(GRAPH_NAME).c

SEQ_DEFS +=
SEQ_INCS +=
SEQ_CCOPTS +=

SEQ_APP_LIBS = $(SDK_ROOT)/isp/firmware/$(ODIR)/libsequencer.a

ifeq ($(TOOLCH),ghs)	### GHS ###########################

SEQ_LDSCRIPT =$(SDK_ROOT)/isp/firmware/$(TOOLCH)/cm0_ram.ld

else	### GCC ###########################################

SEQ_LDOPTS =$(DBG_CCOPTS) -Xlinker -Map=$(SEQ_APP).map -Xlinker -L.

SEQ_LDSCRIPT =-Xlinker -T -Xlinker $(SDK_ROOT)/isp/firmware/$(TOOLCH)/cm0_ram.ld

endif

##############################################################################
# ISP Graph
##############################################################################
ARM_LIB = lib$(SEQ_MODE)_$(GRAPH_NAME).a

ARM_LIB_SRCS = \
	$(GRAPH_NAME).c \
	sequencer_srec.c

ARM_INCS = \
	-I$(SDK_ROOT)/include                                                     \
	-I$(SDK_ROOT)/isp/graphs/$(GRAPH_NAME)/inc                                \
	-I$(SDK_ROOT)/isp/inc                                                     \
	-I$(SDK_ROOT)/platform/s32_v234

GDT_OPTS = -m

##############################################################################
# IPUx kernels
##############################################################################
IPUS_APP = ipus.elf
IPUS_APP_SRCS = \
    $(SDK_ROOT)/isp/kernels/generic/src/yuv42210bit_to_8bit.ipus \


KMEM = kmem
