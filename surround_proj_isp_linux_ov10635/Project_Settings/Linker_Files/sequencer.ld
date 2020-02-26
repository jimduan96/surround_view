/**************************************************************************************************
 * 
 * NXP Confidential Proprietary
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
 **************************************************************************************************/
 
ENTRY(resetEntry)

MEMORY
{
  intvec_base	: ORIGIN = 0x0,			        LENGTH = 0xC0
  sram_RAM	  : ORIGIN = 0x100,		        LENGTH = 64K - 16K - 4K - 0x100
  stack		    : ORIGIN = 64K - 16K - 4K,  LENGTH = 4K
  graph		    : ORIGIN = 64K - 16K,		    LENGTH = 16K
}

SECTIONS
{
  .vectable	ORIGIN(intvec_base) :
  {
    LD_vectorTable = .; 
    KEEP(*(.vectable))
  }
  
  .init		  ORIGIN(sram_RAM)  : { *(.init)	  }
  .text		  ALIGN(8)			    : { *(.text*)	  }
  .rodata	  ALIGN(4)			    : { *(.rodata*)	}
  .sdabase  ALIGN(16)			    : { *(.sdabase*)}
  .sdata	  ALIGN(4)			    : { *(.sdata*)	}
  .sdata2	  ALIGN(4)			    : { *(.sdata2*)	}
  .sbss		  ALIGN(4)			    : { *(.sbss*)	  }
  .data		  ALIGN(4)			    : { *(.data*)	  }

  .bss ALIGN(16) :
  {
    LD_bssStart = .;
    *(.bss*)
    LD_bssEnd   = .;
  }
    
  .stack	ORIGIN(stack) :
  {
    LD_stackBottom = .;
  } > stack

  .graph	ORIGIN(graph) :
  {
    LD_stackTop = . -4; 
    LD_graphStart = .; 
  } > graph

} /* SECTIONS */
