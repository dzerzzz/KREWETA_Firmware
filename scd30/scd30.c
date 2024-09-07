#include "scd30_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec

void create_new_file(FILE **outputFile) {
    if (*outputFile != NULL) {
        fclose(*outputFile);
    }

    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char filename[30];
    strftime(filename, sizeof(filename), "SCD30_%d-%H-%M.txt", local);

    *outputFile = fopen(filename, "w");
    if (*outputFile == NULL) {
        printf("Error opening file.\n");
    }
}

int main(void) {
    int16_t error = NO_ERROR;
    sensirion_i2c_hal_init();
    init_driver(SCD30_I2C_ADDR_61);

    // Soft reset and initial setup
    scd30_stop_periodic_measurement();
    scd30_soft_reset();
    sensirion_hal_sleep_us(2000000);

    error = scd30_start_periodic_measurement(0);
    if (error != NO_ERROR) {
        printf("Error starting periodic measurement: %i\n", error);
        return error;
    }

    FILE *outputFile = NULL;
    create_new_file(&outputFile);

    float co2_concentration = 0.0;
    float temperature = 0.0;
    float humidity = 0.0;

    time_t last_file_time = time(NULL);

    while (1) {
        time_t now = time(NULL);
        if (difftime(now, last_file_time) >= 60.0) {
            create_new_file(&outputFile);
            last_file_time = now;
        }

        sensirion_hal_sleep_us(2000000);
        error = scd30_blocking_read_measurement_data(&co2_concentration, &temperature, &humidity);
        if (error != NO_ERROR) {
            printf("Error reading measurement data: %i\n", error);
            continue;
        }

        // printf("CO2: %.2f ppm, Temp: %.2f °C, Humidity: %.2f %%\n", co2_concentration, temperature, humidity);

        fprintf(outputFile, "%ld, %.2f, %.2f, %.2f\n", now, co2_concentration, temperature, humidity);
        fflush(outputFile);
    }

    error = scd30_stop_periodic_measurement();
    if (error != NO_ERROR) {
        printf("Error stopping periodic measurement: %i\n", error);
        return error;
    }

    if (outputFile != NULL) {
        fclose(outputFile);
    }

    return NO_ERROR;
}
