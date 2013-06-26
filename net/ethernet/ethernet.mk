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

TARGET_IDC_DIR := system/etc/wmt/script
LOCAL_IDC_DIR  := device/wmt/wmid/net/ethernet

TARGET_VIA_DIR := system/etc/via/script

PRODUCT_COPY_FILES += \
	$(LOCAL_IDC_DIR)/run_staticip.sh:$(TARGET_IDC_DIR)/run_staticip.sh \
	$(LOCAL_IDC_DIR)/update_dns.sh:$(TARGET_VIA_DIR)/update_dns.sh


