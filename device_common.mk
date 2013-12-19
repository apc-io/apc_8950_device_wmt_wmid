#
# Copyright (C) 2011 The Android Open-Source Project
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

PRODUCT_PROPERTY_OVERRIDES += \
	ro.opengles.version=131072
	
PRODUCT_PROPERTY_OVERRIDES += \
	hwui.render_dirty_regions=false

DEVICE_PACKAGE_OVERLAYS := \
    device/wmt/wmid/overlay_common

#
# Copy product files
#
PRODUCT_COPY_FILES := \

# Touch
PRODUCT_COPY_FILES += \
	frameworks/base/data/etc/android.hardware.touchscreen.multitouch.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.xml

# Wifi
PRODUCT_COPY_FILES += \
	frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
	frameworks/base/data/etc/android.hardware.location.xml:system/etc/permissions/android.hardware.location.xml \
	device/wmt/wmid/wifi/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf \
	device/wmt/wmid/wifi/RT2870STACard.dat:system/etc/Wireless/RT2870STA/RT2870STACard.dat \
	device/wmt/wmid/wifi/RT2870STA.dat:/system/etc/Wireless/RT2870STA/RT2870STA.dat \
	device/wmt/wmid/wifi/rt2870.bin:/system/vendor/firmware/rt2870.bin \
	device/wmt/wmid/wifi/wifi_list:/system/etc/wmt/wifi_list \
	device/wmt/wmid/wifi/broadcom/fw_bcmdhd.bin:/system/vendor/firmware/fw_bcmdhd.bin \
	device/wmt/wmid/wifi/broadcom/bcmdhd.cal:/system/etc/wifi/bcmdhd.cal \
	device/wmt/wmid/bluetooth/rtk8723a:/system/etc/firmware/rtk8723a \

PRODUCT_PROPERTY_OVERRIDES += \
	wifi.interface=wlan0

# Bluetooth configuration files
PRODUCT_COPY_FILES += \
        frameworks/base/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml \

# Get the TTS language packs
$(call inherit-product-if-exists, external/svox/pico/lang/all_pico_languages.mk)

# Inside of wmt chip:
$(call inherit-product-if-exists, device/wmt/wmid/script/script.mk)
#$(call inherit-product-if-exists, device/wmt/wmid/net/net.mk)
$(call inherit-product-if-exists, device/wmt/wmid/audio/alsa.mk)
$(call inherit-product-if-exists, device/wmt/wmid/audio/sound_effect.mk)
$(call inherit-product-if-exists, device/wmt/wmid/input/touchscreen/mst_touch/mst_touch.mk)
$(call inherit-product-if-exists, device/wmt/wmid/input/touchscreen/it7260/it7260.mk)
$(call inherit-product-if-exists, device/wmt/wmid/input/touchscreen/vt1603_ts/vt1603_ts.mk)
$(call inherit-product-if-exists, device/wmt/wmid/input/touchscreen/vt1609/Android.mk)
$(call inherit-product-if-exists, device/wmt/wmid/net/ethernet/ethernet.mk)
$(call inherit-product-if-exists, device/wmt/wmid/inhouse/serial_tool/serialber_tool.mk)
#$(call inherit-product-if-exists, device/wmt/wmid/ffmpeg/libavcodec/wmt/system/lib/wmt_codec.mk)
$(call inherit-product-if-exists, device/wmt/wmid/cursor/cursor.mk)

# Outside of wmt chip:
$(call inherit-product-if-exists, vendor/wmt/wmid/device-vendor.mk)

# Release
$(call inherit-product-if-exists, device/wmt/wmid/device.rel.mk)

# initial script
$(shell chmod 777 device/wmt/wmid/script/S66lowmemorythreshold)
$(shell chmod 777 device/wmt/wmid/script/runallscript.sh)
$(shell chmod 777 device/wmt/wmid/script/S06speedup)

