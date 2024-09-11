scd30_dir = /home/pi/sensors/scd30
iis3d_dir = /home/pi/sensors/iis3d
temperature_dir = /home/pi/sensors/temperature

common_sources = $(scd30_dir)/sensirion_config.h $(scd30_dir)/sensirion_common.h $(scd30_dir)/sensirion_common.c
i2c_sources = $(scd30_dir)/sensirion_i2c_hal.h $(scd30_dir)/sensirion_i2c.h $(scd30_dir)/sensirion_i2c.c
driver_sources = $(scd30_dir)/scd30_i2c.h $(scd30_dir)/scd30_i2c.c

i2c_implementation ?= $(scd30_dir)/sensirion_i2c_hal.c

CFLAGS = -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC -I.

ifdef CI
    CFLAGS += -Werror
endif

.PHONY: all clean scd30 iis_recorder uart temp set_temp

all: scd30 iis_recorder uart temp set_temp

# SCD30
scd30:
	$(CC) $(CFLAGS) -o $(scd30_dir)/scd30 $(driver_sources) $(i2c_sources) \
		$(i2c_implementation) $(common_sources) $(scd30_dir)/scd30.c

# IIS3D
iis_recorder:
	$(CC) $(CFLAGS) -o $(iis3d_dir)/iis_recorder $(iis3d_dir)/iis_recorder.c $(iis3d_dir)/iis3dwb_reg.c -lm

# Temperature
uart:
	$(CC) $(CFLAGS) -o $(temperature_dir)/uart $(temperature_dir)/iface.c $(temperature_dir)/uart.c -lm

temp:
	$(CC) $(CFLAGS) -o $(temperature_dir)/temp $(temperature_dir)/iface.c $(temperature_dir)/temp.c -lm

set_temp:
	$(CC) $(CFLAGS) -o $(temperature_dir)/set_temp $(temperature_dir)/iface.c $(temperature_dir)/set_temp.c -lm

clean:
	$(RM) $(scd30_dir)/scd30
	$(RM) $(iis3d_dir)/iis_recorder
	$(RM) $(temperature_dir)/uart
	$(RM) $(temperature_dir)/temp
	$(RM) $(temperature_dir)/set_temp
