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

PRODUCT_PACKAGES := \
    Camera \
    camera.default \
    sensors.wmt \
    acoustics.default \
    alsa.default \
    audio.a2dp.default \
    audio.primary.wmt \
    acoustics.default \
    audio.primary.default \
    audio_policy.default \
    libgetubootenv \
    lights.wmt \
    libwmth264adp \
    libwmtapi \
    libwmtcodec \
    libwmtthumb \
    libwmtnativecal \
    libwmtegl \
    libwmtpe \
    libwplayer \
    libwmtavcap \
    libavcodec \
    libavformat \
    libavutil \
    libswscale \
    libOMX.WMT.Video.Decoder \
    libOMX.WMT.Video.Encoder \
    libpagemap \
    libc_malloc_debug_leak \
    libc_malloc_debug_qemu \
    invoke_mock_media_player \
    libstagefrighthw \
    camera.goldfish \
    gps.goldfish \
    gralloc.android \
    lights.goldfish \
    sensors.goldfish \
    OTAUpdate \
    File_Browser

# Live Wallpapers
PRODUCT_PACKAGES += \
        LiveWallpapers \
        LiveWallpapersPicker \
        MagicSmokeWallpapers \
        VisualizationWallpapers \
        libRS librs_jni

# ARMv5:
$(call inherit-product, $(SRC_TARGET_DIR)/product/generic_no_telephony.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/core.mk)

# ARMv7: ?
# Inherit from those products. Most specific first.
#$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Get a list of languages.
$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

$(call inherit-product, device/wmt/wmid/device_common.mk)

WMT_PREBUILT=device/wmt/wmid/wmt_prebuilt.mk
