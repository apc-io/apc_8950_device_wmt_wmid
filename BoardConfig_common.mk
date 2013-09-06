#
# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###include build/target/board/generic/BoardConfig.mk

# The generic product target doesn't have any hardware-specific pieces.
TARGET_NO_BOOTLOADER := true
TARGET_NO_KERNEL := false
USE_OPENGL_RENDERER := true
BOARD_USES_GENERIC_AUDIO := false
BOARD_USES_ALSA_AUDIO := true
BUILD_WITH_ALSA_UTILS := true 
ALSA_DEFAULT_SAMPLE_RATE := 48000
BOARD_HAVE_BLUETOOTH := true

# init.rc
TARGET_PROVIDES_INIT_RC := true
TARGET_PROVIDES_B2G_INIT_RC := true

# no hardware camera
#USE_CAMERA_STUB := true

# Set /system/bin/sh to ash, not mksh, to make sure we can switch back.
#TARGET_SHELL := ash

# Build OpenGLES emulation guest and host libraries
#BUILD_EMULATOR_OPENGL := true

TARGET_BOARD_PLATFORM := wmt

TARGET_ARCH_VARIANT := armv7-a-neon
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi

ARCH_ARM_HAVE_TLS_REGISTER := true

WITH_DEXPREOPT := false

# Wifi related defines
WPA_SUPPLICANT_VERSION := VER_0_8_X

# enable librecovery
ENABLE_LIBRECOVERY := true
