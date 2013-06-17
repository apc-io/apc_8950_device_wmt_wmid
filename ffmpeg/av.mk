# LOCAL_PATH is one of libavutil, libavcodec, libavformat, or libswscale

# include $(LOCAL_PATH)/../config-$(TARGET_ARCH).mak
include $(LOCAL_PATH)/../config.mak

OBJS :=
OBJS-yes :=
MMX-OBJS-yes :=
include $(LOCAL_PATH)/Makefile

ifeq ($(NAME),avcodec)
include $(LOCAL_PATH)/arm/Makefile
endif

# collect objects
OBJS-$(HAVE_MMX) += $(MMX-OBJS-yes)
OBJS += $(OBJS-yes)

FFNAME := lib$(NAME)
FFLIBS := $(foreach,NAME,$(FFLIBS),lib$(NAME))

FFCFLAGS  = -DHAVE_AV_CONFIG_H -DCONFIG_ANDROID -Wno-sign-compare -Wno-switch -Wno-pointer-sign -mcpu=cortex-a9 -mfpu=neon -fPIC -mfloat-abi=softfp -fsigned-char
FFASFLAGS = -mcpu=cortex-a9 -mfpu=neon -fPIC -mfloat-abi=softfp

ALL_S_FILES := $(wildcard $(LOCAL_PATH)/$(TARGET_ARCH)/*.S)
ALL_S_FILES := $(addprefix $(TARGET_ARCH)/, $(notdir $(ALL_S_FILES)))

ifneq ($(ALL_S_FILES),)
ALL_S_OBJS := $(patsubst %.S,%.o,$(ALL_S_FILES))
C_OBJS := $(filter-out $(ALL_S_OBJS),$(OBJS))
S_OBJS := $(filter $(ALL_S_OBJS),$(OBJS))
else
C_OBJS := $(OBJS)
S_OBJS :=
endif

C_FILES := $(patsubst %.o,%.c,$(C_OBJS))
S_FILES := $(patsubst %.o,%.S,$(S_OBJS))

FFFILES := $(sort $(S_FILES)) $(sort $(C_FILES))
