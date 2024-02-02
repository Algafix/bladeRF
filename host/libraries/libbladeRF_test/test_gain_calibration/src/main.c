/*
 * This file is part of the bladeRF project:
 *   http://www.github.com/nuand/bladeRF
 *
 * Copyright (C) 2024 Nuand LLC
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "libbladeRF.h"
#include "file_operations.h"

#include "log.h"
#include "helpers/file.h"

#define CHECK(func_call) do { \
    status = (func_call); \
    if (status != 0) { \
        fprintf(stderr, "Failed at %s: %s\n", #func_call, bladerf_strerror(status)); \
        status = EXIT_FAILURE; \
        goto out; \
    } \
} while (0)

int main(int argc, char *argv[]) {
    int status;
    struct bladerf *dev;
    char *devstr = NULL;
    const char *cal_rx_file_loc_csv = "rx_pwr_cal.csv";
    bladerf_channel ch;

    bladerf_log_set_verbosity(BLADERF_LOG_LEVEL_DEBUG);

    CHECK(bladerf_open(&dev, devstr));

    ch = BLADERF_CHANNEL_RX(0);
    CHECK(bladerf_load_gain_calibration(dev, ch, cal_rx_file_loc_csv));

    ch = BLADERF_CHANNEL_TX(0);
    CHECK(bladerf_load_gain_calibration(dev, ch, cal_rx_file_loc_csv));

    for (int i = 0; i < 4; i++) {
        CHECK(bladerf_print_gain_calibration(dev, i, false));
    }

out:
    bladerf_close(dev);
    return status;
}

