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

$(call inherit-product, device/wmt/wmid/common.mk)
$(call inherit-product, device/wmt/wmid/device.mk)
# to have B2G install
$(call inherit-product, build/target/product/full_base.mk)
#
# Overrides
#
PRODUCT_NAME := full_wmid
PRODUCT_DEVICE := wmid
PRODUCT_BRAND := WonderMedia
PRODUCT_MODEL := MID
PRODUCT_MODEL_UNIQUE_ID := WM8950S
PRODUCT_MANUFACTURER := WonderMedia

