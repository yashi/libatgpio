/*
 * Library for Linux GPIO Sysfs Interface with Glib
 *
 * Copyright (c) 2016 Yasushi SHOJI <yashi@atmark-techno.com>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __AT_GPIO_H__
#define __AT_GPIO_H__

#include <glib.h>

#ifndef SYSFS_MNT_PATH
#define SYSFS_MNT_PATH "/sys"
#endif

#define AT_GPIO_PATH_EXPORT    SYSFS_MNT_PATH "/class/gpio/export"
#define AT_GPIO_PATH_DIRECTION SYSFS_MNT_PATH "/class/gpio/gpio%d/direction"
#define AT_GPIO_PATH_EDGE      SYSFS_MNT_PATH "/class/gpio/gpio%d/edge"
#define AT_GPIO_PATH_VALUE     SYSFS_MNT_PATH "/class/gpio/gpio%d/value"

enum AtGpioDirection {
        AT_GPIO_DIRECTION_IN,
        AT_GPIO_DIRECTION_OUT,
};

enum AtGpioEdge {
        AT_GPIO_EDGE_NONE,
        AT_GPIO_EDGE_RISING,
        AT_GPIO_EDGE_FALLING,
        AT_GPIO_EDGE_BOTH,
};

typedef gboolean (*AtGpioFunc) (GIOChannel   *source,
                                gpointer      data);

struct AtGpioClosure {
        AtGpioFunc function;
        gpointer data;
};

gboolean  at_gpio_export           (guint           gpio_nr,
                                    GError        **error);

void      at_gpio_set_direction_in (guint           gpio_nr);

void      at_gpio_set_edge_none    (guint           gpio_nr);
void      at_gpio_set_edge_rising  (guint           gpio_nr);
void      at_gpio_set_edge_falling (guint           gpio_nr);
void      at_gpio_set_edge_both    (guint           gpio_nr);

guint     at_gpio_add_full         (gint            priority,
                                    guint           gpio_nr,
                                    enum AtGpioEdge edge,
                                    AtGpioFunc      user_func,
                                    gpointer        user_data,
                                    GDestroyNotify  notify,
                                    GError        **error);

guint     at_gpio_add              (guint           gpio_nr,
                                    enum AtGpioEdge edge,
                                    AtGpioFunc      function,
                                    gpointer        data,
                                    GError        **error);

#endif /* __AT_GPIO_H__ */
