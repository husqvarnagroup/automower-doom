#
# Copyright(C) 2023 Husqvarna AB
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

# -----------------------------------------------------------------------
# Include other makefiles
# -----------------------------------------------------------------------
include sources.mk

# -----------------------------------------------------------------------
# Configuration
# -----------------------------------------------------------------------

# Directories
OUT_DIR := ./out
OBJ_DIR := $(OUT_DIR)/obj

# Out file(s)
OUT_ELF := $(OUT_DIR)/doom.elf
OUT_BIN := $(OUT_DIR)/doom.bin
OUT_MAP := $(OUT_DIR)/doom.map

# Compiler etc
GCC_PREFIX = arm-none-eabi-

# Compiler flags and defines
DEFINES := STM32F4XX STM32F469_479xx STM32F469xx USE_STDPERIPH_DRIVER HSE_VALUE=24000000 PLL_M=24 PLL_N=360 PLL_Q=2 PLL_R=2 PLL_P=2 MYFF_HANDLE_WAD_IN_RAM ISOFTWAREEXCEPTIONCFG_NO_FILE_INFO DATA_IN_ExtSDRAM DATA_IN_SDRAM USER_VECT_TAB_ADDRESS VECT_TAB_SRAM
CFLAGS := -Wall -std=gnu99 -ffunction-sections -fdata-sections -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os

# Linker flags
LINKSCRIPT := ./Automower-Doom/Config/STM32F469NI.ld
LDFLAGS := -static -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T$(LINKSCRIPT) -Wl,-cref,-Map=$(OUT_MAP) --specs=nano.specs --specs=nosys.specs

# -----------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------

# Tools
CC = $(GCC_PREFIX)gcc
LD = $(GCC_PREFIX)gcc
SIZE = $(GCC_PREFIX)size
OBJCOPY = $(GCC_PREFIX)objcopy

# Generated variables
OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o,$(SOURCE_FILES))  # C-files
OBJS := $(patsubst %.s, $(OBJ_DIR)/%.o,$(OBJS))          # Assembly files

# Properly prefixed flags
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPP_DEFS := $(addprefix -D,$(DEFINES))

# -----------------------------------------------------------------------
# Rules / Targets
# -----------------------------------------------------------------------
# Build, link and create binary
all: $(OUT_BIN)

# Conversion to BIN
$(OUT_BIN): $(OUT_ELF)
	@echo "Creating binary $(notdir $@)"
	@$(OBJCOPY) -O binary $< $@

# Linking to ELF
$(OUT_ELF): $(OBJS)
	@echo "Linking $(notdir $@)"
	@$(LD) $(INC_FLAGS) $(LDFLAGS) -o $@ $^
	@$(SIZE) $@

# Assembly files
$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo
	@echo "Assembling $(notdir $<)"
	@$(CC) $(INC_FLAGS) $(CPP_DEFS) $(CFLAGS) -c $< -o $@

# C files
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo
	@echo "Compiling $(notdir $<)"
	@$(CC) $(INC_FLAGS) $(CPP_DEFS) $(CFLAGS) -c $< -o $@

# GZIP of binfile
gzip: $(OUT_BIN)
	@echo "Compressing $(OUT_BIN) -> $(OUT_BIN).gz "
	@gzip -fk $(OUT_BIN)

# -----------------------------------------------------------------------
# .PHONY targets
# -----------------------------------------------------------------------
.PHONY: clean

clean:
	@rm -rf $(OUT_DIR)