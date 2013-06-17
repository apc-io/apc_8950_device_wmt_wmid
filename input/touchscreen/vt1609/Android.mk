TARGET_KO_DIR := root/lib/modules
LOCAL_KO_DIR  := device/wmt/wmid/input/touchscreen/vt1609

PRODUCT_COPY_FILES += \
	$(LOCAL_KO_DIR)/lib/modules/vt1609_spi_dual_touch_support.ko:$(TARGET_KO_DIR)/vt1609_spi_dual_touch_support.ko

