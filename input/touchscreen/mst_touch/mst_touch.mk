#
# Copyright (c) 2011-201_ WonderMedia Technologies, Inc.

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#        http://www.apache.org/licenses/LICENSE-2.0.

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

TARGET_IDC_DIR := system/usr/idc
LOCAL_IDC_DIR  := device/wmt/wmid/input/touchscreen/mst_touch

PRODUCT_COPY_FILES += \
	$(LOCAL_IDC_DIR)/mst_touch.idc:$(TARGET_IDC_DIR)/mst_touch.idc

