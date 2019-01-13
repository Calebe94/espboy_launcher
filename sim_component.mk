CSRCS += launcher.c volume.c
#CSRCS += img_bubble_pattern.c

DEPPATH += --dep-path espboy_launcher
VPATH += :espboy_launcher

#DEPPATH += --dep-path lv_examples/lv_apps/demo
#VPATH += :lv_examples/lv_apps/demo

CFLAGS += "-I$(LVGL_DIR)/espboy_launcher"
#CFLAGS += "-I$(LVGL_DIR)/lv_examples/lv_apps/demo"
