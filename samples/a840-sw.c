#include <glib.h>
#include "atgpio.h"

static gboolean on_quit(GIOChannel *ch, gpointer d)
{
	GMainLoop *loop = d;

	g_io_channel_read_to_end(ch, NULL, NULL, NULL);
	g_main_loop_quit(loop);
	g_print("bye\n");

	return FALSE;
}

static gboolean on_print(GIOChannel *ch, gpointer d)
{
	gchar *str = d;

	g_print("%s\n", str);

	g_io_channel_read_to_end(ch, NULL, NULL, NULL);

	return TRUE;
}

int main()
{
	GMainLoop *loop;
	GError *error = NULL;

	loop = g_main_loop_new(NULL, FALSE);

	at_gpio_add(97,  AT_GPIO_EDGE_BOTH,    on_quit,  loop,  &error);
	if (error)
		g_error("%s\n", error->message);
	at_gpio_add(98,  AT_GPIO_EDGE_RISING,  on_print, "98",  NULL);
	at_gpio_add(99,  AT_GPIO_EDGE_FALLING, on_print, "99",  NULL);
	at_gpio_add(100, AT_GPIO_EDGE_NONE,    on_print, "100", NULL);

	g_main_loop_run(loop);
	g_main_loop_unref(loop);

	return 0;
}
