# Makefile - make rules for target/config/BJUT_9200
#
# 2004/10/23 this file is modified form VxWorks demo bsp integrator920t
#
# DESCRIPTION
# This file contains rules for building VxWorks for the ARM INTEGRATOR AP
# fitted with an ARM920T core module.
#
# INCLUDES
#     makeTarget
#*/

CPU		= ARMARCH4
TOOL		= gnu
EXTRA_DEFINE	= -Wcomment -DCPU_920T \
		  -DARMMMU=ARMMMU_920T -DARMCACHE=ARMCACHE_920T

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= BJUT_9200
VENDOR		= BJUT
BOARD		= 	AT91RM9200
MACH_EXTRA = 	at91EMAC.o romInit_C.o 
BOOT_EXTRA  = romInit_C.o


RELEASE		+= bootrom.bin

#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h and Makefile.
# All definitions for these constants must be identical.
#

ROM_TEXT_ADRS	= 10000000 # ROM entry address
ROM_SIZE	= 00200000 # 2M number of bytes of ROM space

RAM_LOW_ADRS	= 20001000 # VxWorks image entry point
RAM_HIGH_ADRS	= 23E00000 # RAM address for ROM boot

VMA_START	= 0x$(ROM_TEXT_ADRS)

# Binary version of VxWorks ROM images, suitable for programming
# into Flash using tools provided by ARM.  If other ROM images need to
# be put into Flash, add similar rules here.

bootrom.bin: bootrom
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary bootrom $@

bootrom_res.bin: bootrom_res
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary bootrom_res $@

bootrom_uncmp.bin: bootrom_uncmp
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary bootrom_uncmp $@

vxWorks_rom.bin: vxWorks_rom
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary vxWorks_rom $@

vxWorks.st_rom.bin: vxWorks.st_rom
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary vxWorks.st_rom $@

vxWorks.res_rom.bin: vxWorks.res_rom
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary vxWorks.res_rom $@

vxWorks.res_rom_nosym.bin: vxWorks.res_rom_nosym
	- @ $(RM) $@
	$(EXTRACT_BIN) -O binary vxWorks.res_rom_nosym $@

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

