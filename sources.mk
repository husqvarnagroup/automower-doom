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
# Source files
# -----------------------------------------------------------------------
SOURCE_FILES =

# Application files
SOURCE_FILES += Automower-Doom/main.c
SOURCE_FILES += Automower-Doom/Application/Application.c

# PORT sources
SOURCE_FILES += Port/stm32f469/Src/CanBroadcaster.c
SOURCE_FILES += Port/stm32f469/Src/DigitalInput.c
SOURCE_FILES += Port/stm32f469/Src/DigitalOutput.c
SOURCE_FILES += Port/stm32f469/Src/Draw.c
SOURCE_FILES += Port/stm32f469/Src/Encoder.c
SOURCE_FILES += Port/stm32f469/Src/I2c.c
SOURCE_FILES += Port/stm32f469/Src/Interrupt.c
SOURCE_FILES += Port/stm32f469/Src/JoystickNewHal.c
SOURCE_FILES += Port/stm32f469/Src/LCD_Backlight.c
SOURCE_FILES += Port/stm32f469/Src/LCD_ST7789VI.c
SOURCE_FILES += Port/stm32f469/Src/LCD.c
SOURCE_FILES += Port/stm32f469/Src/Magnetic3dSensor.c
SOURCE_FILES += Port/stm32f469/Src/Restart.c
SOURCE_FILES += Port/stm32f469/Src/Spi9bit.c
SOURCE_FILES += Port/stm32f469/Src/SystemPower.c
SOURCE_FILES += Port/stm32f469/Src/SystemTick.c
SOURCE_FILES += Port/stm32f469/Src/Watchdog.c
SOURCE_FILES += Port/stm32f469/Adapter/myff.c
SOURCE_FILES += Port/stm32f469/Adapter/mylcd.c
SOURCE_FILES += Port/stm32f469/Adapter/mymain.c

# CUBEMX configs
SOURCE_FILES += Port/stm32f469/CubeMX/startup_stm32f469nihx.s
SOURCE_FILES += Port/stm32f469/CubeMX/can.c
SOURCE_FILES += Port/stm32f469/CubeMX/dma.c
SOURCE_FILES += Port/stm32f469/CubeMX/gpio.c
SOURCE_FILES += Port/stm32f469/CubeMX/i2c.c
SOURCE_FILES += Port/stm32f469/CubeMX/main_hal.c
SOURCE_FILES += Port/stm32f469/CubeMX/spi.c
SOURCE_FILES += Port/stm32f469/CubeMX/stm32f4xx_hal_msp.c
SOURCE_FILES += Port/stm32f469/CubeMX/stm32f4xx_it.c
SOURCE_FILES += Port/stm32f469/CubeMX/sysmem_sdram.c
SOURCE_FILES += Port/stm32f469/CubeMX/syscalls.c
SOURCE_FILES += Port/stm32f469/CubeMX/system_stm32f4xx.c
SOURCE_FILES += Port/stm32f469/CubeMX/tim.c
SOURCE_FILES += Port/stm32f469/CubeMX/ltdc.c
SOURCE_FILES += Port/stm32f469/CubeMX/dma2d.c
SOURCE_FILES += Port/stm32f469/CubeMX/wwdg.c

# CUBEMX Drives
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dsi.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_ltdc_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma2d.c
SOURCE_FILES += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c

# DOOM sources
SOURCE_FILES += Doom/stm32doom/src/chocodoom/dummy.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/am_map.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/doomdef.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/doomstat.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/dstrings.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_event.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_items.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_iwad.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_loop.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_main.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_mode.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/d_net.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/f_finale.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/f_wipe.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/g_game.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/hu_lib.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/hu_stuff.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/info.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_cdmus.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_endoom.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_joystick.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_main.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_scale.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_sound.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_system.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_timer.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/i_video.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/memio.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_argv.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_bbox.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_cheat.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_config.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_controls.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_fixed.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_menu.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_misc.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/m_random.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_ceilng.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_doors.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_enemy.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_floor.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_inter.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_lights.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_map.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_maputl.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_mobj.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_plats.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_pspr.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_saveg.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_setup.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_sight.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_spec.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_switch.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_telept.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_tick.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/p_user.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_bsp.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_data.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_draw.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_main.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_plane.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_segs.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_sky.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/r_things.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/sha1.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/sounds.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/statdump.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/st_lib.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/st_stuff.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/s_sound.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/tables.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/v_video.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/wi_stuff.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/w_checksum.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/w_file.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/w_file_stdc.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/w_main.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/w_wad.c
SOURCE_FILES += Doom/stm32doom/src/chocodoom/z_zone.c


# -----------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------
INC_DIRS =
INC_DIRS += Automower-Doom/Application
INC_DIRS += Automower-Doom/Config
INC_DIRS += Doom/stm32doom/src/chocodoom
INC_DIRS += Doom/stm32doom/src
INC_DIRS += Port/stm32f469/CubeMX/Drivers/CMSIS/Include
INC_DIRS += Port/stm32f469/CubeMX/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INC_DIRS += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
INC_DIRS += Port/stm32f469/CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc
INC_DIRS += Port/stm32f469/CubeMX
INC_DIRS += Port/stm32f469/Adapter
INC_DIRS += Port/stm32f469/Inc