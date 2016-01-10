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

#include "atgpio.h"

gboolean
at_gpio_export(guint gpio_nr, GError **error)
{
        GString *s;
        GIOChannel *ch;

        g_return_val_if_fail(gpio_nr != 0, FALSE);

        ch = g_io_channel_new_file(AT_GPIO_PATH_EXPORT, "w", error);
        if (!ch)
                return FALSE;

        g_print("##%p: %s\n", ch, AT_GPIO_PATH_EXPORT);
        g_io_channel_set_encoding(ch, NULL, NULL);

        s = g_string_new(NULL);
        g_string_printf(s, "%u\n", gpio_nr);
        g_io_channel_write_chars(ch, s->str, s->len, NULL, NULL);
        g_string_free(s, TRUE);

        g_io_channel_unref(ch);

        return TRUE;
}

static void
_at_gpio_set_direction(guint gpio_nr, enum AtGpioDirection dir)
{
        GString *s;
        GIOChannel *ch;

        g_return_if_fail(gpio_nr != 0);

        s = g_string_new(NULL);
        g_string_printf(s, AT_GPIO_PATH_DIRECTION, gpio_nr);
        ch = g_io_channel_new_file(s->str, "w", NULL);
        g_string_free(s, TRUE);

        g_io_channel_set_encoding(ch, NULL, NULL);

        if (dir == AT_GPIO_DIRECTION_IN)
                g_io_channel_write_chars(ch, "in\n", 3, NULL, NULL);
        else
                g_io_channel_write_chars(ch, "out\n", 4, NULL, NULL);

        g_io_channel_unref(ch);
}

void
at_gpio_set_direction_in(guint gpio_nr)
{
        _at_gpio_set_direction(gpio_nr, AT_GPIO_DIRECTION_IN);
}

static void
_at_gpio_set_edge(guint gpio_nr, enum AtGpioEdge edge)
{
        GString *s;
        GIOChannel *ch;

        g_return_if_fail(gpio_nr != 0);

        s = g_string_new(NULL);
        g_string_printf(s, AT_GPIO_PATH_EDGE, gpio_nr);
        ch = g_io_channel_new_file(s->str, "w", NULL);
        g_string_free(s, TRUE);

        g_io_channel_set_encoding(ch, NULL, NULL);

        switch (edge) {
        case AT_GPIO_EDGE_NONE:
                g_io_channel_write_chars(ch, "none\n", 5, NULL, NULL);
                break;
        case AT_GPIO_EDGE_RISING:
                g_io_channel_write_chars(ch, "rising\n", 7, NULL, NULL);
                break;
        case AT_GPIO_EDGE_FALLING:
                g_io_channel_write_chars(ch, "falling\n", 8, NULL, NULL);
                break;
        default:
                g_io_channel_write_chars(ch, "both\n", 5, NULL, NULL);
                break;
        }

        g_io_channel_unref(ch);
}

void
at_gpio_set_edge_none(guint gpio_nr)
{
        _at_gpio_set_edge(gpio_nr, AT_GPIO_EDGE_NONE);
}

void
at_gpio_set_edge_rising(guint gpio_nr)
{
        _at_gpio_set_edge(gpio_nr, AT_GPIO_EDGE_RISING);
}

void
at_gpio_set_edge_falling(guint gpio_nr)
{
        _at_gpio_set_edge(gpio_nr, AT_GPIO_EDGE_FALLING);
}

void
at_gpio_set_edge_both(guint gpio_nr)
{
        _at_gpio_set_edge(gpio_nr, AT_GPIO_EDGE_BOTH);
}

static gboolean
indirector(GIOChannel *ch, GIOCondition c, gpointer d)
{
        struct AtGpioClosure *closure = d;
        (void)c;

        g_io_channel_seek_position(ch, 0, G_SEEK_SET, NULL);
        return closure->function(ch, closure->data);
}

guint
at_gpio_add_full(gint priority,
                 guint gpio_nr,
                 enum AtGpioEdge edge,
                 AtGpioFunc user_func,
                 gpointer user_data,
                 GDestroyNotify notify,
                 GError **error)
{
        GString *s;
        GIOChannel *ch;
        guint id;
        struct AtGpioClosure *closure;
        gboolean ret;
        GError *err = NULL;

        g_return_val_if_fail(user_func != NULL, 0);
        g_return_val_if_fail(gpio_nr != 0, 0);

        ret = at_gpio_export(gpio_nr, &err);
        if (!ret) {
                g_set_error(error, err->domain, 0,
                            "Unable to export gpio %d: %s", gpio_nr,
                            err->message);
                g_error_free(err);
                return ret;
        }
        at_gpio_set_direction_in(gpio_nr);
        _at_gpio_set_edge(gpio_nr, edge);

        s = g_string_new(NULL);
        g_string_printf(s, AT_GPIO_PATH_VALUE, gpio_nr);
        ch = g_io_channel_new_file(s->str, "r", NULL);
        g_string_free(s, TRUE);

        g_io_channel_set_encoding(ch, NULL, NULL);
        g_io_channel_read_to_end(ch, NULL, NULL, NULL);

        closure = g_new(struct AtGpioClosure, 1);
        closure->function = user_func;
        closure->data = user_data;

        id = g_io_add_watch_full(ch, priority, G_IO_PRI,
                                 indirector, closure, notify);
        g_io_channel_unref(ch);

        return id;
}

guint
at_gpio_add(guint gpio_nr,
            enum AtGpioEdge edge,
            AtGpioFunc function,
            gpointer data,
            GError **error)
{
        return at_gpio_add_full(G_PRIORITY_DEFAULT, gpio_nr,
                                edge, function, data, NULL, error);
}
