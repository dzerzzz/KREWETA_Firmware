common_sources = sensirion_config.h sensirion_common.h sensirion_common.c
i2c_sources = sensirion_i2c_hal.h sensirion_i2c.h sensirion_i2c.c
driver_sources = scd30_i2c.h scd30_i2c.c

i2c_implementation ?= sensirion_i2c_hal.c

CFLAGS = -Os -Wall -fstrict-aliasing -Wstrict-aliasing=1 -Wsign-conversion -fPIC -I.

ifdef CI
    CFLAGS += -Werror
endif

.PHONY: all clean scd30 iis_recorder uart temp set_temp

all: scd30 iis_recorder uart temp set_temp

# SCD30
scd30:
	$(CC) $(CFLAGS) -o /home/pi/sensors/scd30/scd30 ${driver_sources} ${i2c_sources} \
		${i2c_implementation} ${common_sources} scd30.c

# IIS3D
iis_recorder:
	$(CC) $(CFLAGS) -o /home/pi/sensors/iis3d/iis_recorder /home/pi/sensors/iis3d/iis_recorder.c /home/pi/sensors/iis3d/iis3dwb_reg.c -lm

# Temperature
uart:
	$(CC) $(CFLAGS) -o /home/pi/sensors/temperature/uart /home/pi/sensors/temperature/iface.c /home/pi/sensors/temperature/uart.c -lm

temp:
	$(CC) $(CFLAGS) -o /home/pi/sensors/temperature/temp /home/pi/sensors/temperature/iface.c /home/pi/sensors/temperature/temp.c -lm

set_temp:
	$(CC) $(CFLAGS) -o /home/pi/sensors/temperature/set_temp /home/pi/sensors/temperature/iface.c /home/pi/sensors/temperature/set_temp.c -lm

clean:
	$(RM) /home/pi/sensors/scd30/scd30
	$(RM) /home/pi/sensors/iis3d/iis_recorder
	$(RM) /home/pi/sensors/temperature/uart
	$(RM) /home/pi/sensors/temperature/temp
	$(RM) /home/pi/sensors/temperature/set_temp
