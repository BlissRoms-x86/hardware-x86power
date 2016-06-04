/*
 *
 * Copyright (C) 2016 The Android-x86 Open Source Project
 *
 * Licensed under the GNU General Public License Version 2 or later.
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.gnu.org/licenses/gpl.html
 *
 *
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOG_TAG "PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define SYS_CPU "/sys/devices/system/cpu"
#define CPU_ONLINE "1"
#define CPU_OFFLINE "0"

static void set_nonboot_cpu_state(const char *state)
{
    static int cpu_n_fd;
    char p[PATH_MAX];
    int cpu_n = 1;

    for (;;) {
        snprintf(p, PATH_MAX, SYS_CPU "/cpu%d/online", cpu_n);
        cpu_n_fd = open(p, O_RDWR);
        if (cpu_n_fd < 0) {
            return;
        }
        ALOGV("Set CPU%d_online state %s ", cpu_n, state);
        write(cpu_n_fd, state, 1);
        close(cpu_n_fd);
		cpu_n++;
    }
}

static void power_init(struct power_module *module)
{
}

static void power_set_interactive(struct power_module *module, int on)
{
    set_nonboot_cpu_state(on ? CPU_ONLINE : CPU_OFFLINE);
}

static void power_hint(struct power_module *module, power_hint_t hint,
                       void *data) {
    switch (hint) {
    default:
        break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_2,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "x86 Power HAL",
        .author = "The Android-x86 Project",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
};
