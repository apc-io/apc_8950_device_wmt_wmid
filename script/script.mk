PRODUCT_COPY_FILES := \
	device/wmt/wmid/script/pm.sh:system/etc/wmt/pm.sh \
	device/wmt/wmid/script/poweroff.sh:system/etc/wmt/poweroff.sh \
	device/wmt/wmid/script/usbfs.sh:system/etc/wmt/script/usbfs.sh \
	device/wmt/wmid/script/init_vt1609dual.sh:system/etc/wmt/script/init_vt1609dual.sh \
	device/wmt/wmid/script/runallscript.sh:system/etc/rc.d/rc0.d/runallscript.sh \
	device/wmt/wmid/script/S06speedup:system/etc/rc.d/rcS.d/S06speedup \
	device/wmt/wmid/script/sdio_wifi_pwron.sh:system/etc/wmt/sdio_wifi_pwron.sh\
	device/wmt/wmid/script/sdio_wifi_pwroff.sh:system/etc/wmt/sdio_wifi_pwroff.sh\
	device/wmt/wmid/script/force.sh:system/etc/wmt/script/force.sh\
	device/wmt/wmid/script/crontab.sh:system/etc/wmt/script/crontab.sh \
	device/wmt/wmid/script/force.sh:system/etc/rc.d/rc0.d/S66lowmemorythreshold
