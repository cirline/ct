#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include <ccutils/net.h>
#include <ccutils/log.h>

#include "sinajs.h"

int main_ui(int argc, char *argv[])
{
	GtkWidget *win;
	gtk_init(&argc, &argv);

	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "center");
	gtk_window_set_default_size(GTK_WINDOW(win), 250, 100);
	gtk_widget_show_all(win);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	return 0;
}

void psinajs(struct sinajs *sj)
{
	int i;

	printf("code: %s\n", sj->code);
	printf("open: %f\n", sj->open);
	printf("pre_close: %f\n", sj->pre_close);
	printf("price: %f\n", sj->price);
	printf("high: %f\n", sj->high);
	printf("low: %f\n", sj->low);
	printf("bid: %f\n", sj->bid);
	printf("ask: %f\n", sj->ask);
	printf("volume: %ld\n", sj->volume);
	printf("amount: %ld\n", sj->amount);
	for(i = 0; i < 5; i++) {
		printf("bv%d: %d\n", i, sj->bv[i]);
		printf("bp%d: %f\n", i, sj->bp[i]);
		printf("av%d: %d\n", i, sj->av[i]);
		printf("ap%d: %f\n", i, sj->ap[i]);
	}
	printf("date: %s\n", sj->date);
	printf("time: %s\n", sj->time);
}

void sinajs_decode(char *buffer, struct sinajs *sj)
{
	char *sptr1, *sptr2;
	char *token;
	char *p;
	int i;

	if(!buffer)
		return;

	token = strtok_r(buffer, "\"", &sptr1);
	for(p = token; *p && *p != '='; p++)
		;
	*p = 0;
	sscanf(token, "var hq_str_%s", sj->code);

	token = strtok_r(NULL, "\"", &sptr1);

	token = strtok_r(token, ",", &sptr2);
	strcpy(sj->name, token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->open = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->pre_close = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->price = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->high = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->low = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sj->bid = atof(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->ask = atof(token);

	token = strtok_r(NULL, ",", &sptr2);
	sj->volume = atol(token);
	token = strtok_r(NULL, ",", &sptr2);
	sj->amount = atol(token);

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sj->bv[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sj->bp[i] = atof(token);
	}

	for(i = 0; i < 5; i++) {
		token = strtok_r(NULL, ",", &sptr2);
		sj->av[i] = atoi(token);
		token = strtok_r(NULL, ",", &sptr2);
		sj->ap[i] = atof(token);
	}

	token = strtok_r(NULL, ",", &sptr2);
	strcpy(sj->date, token);
	token = strtok_r(NULL, ",", &sptr2);
	strcpy(sj->time, token);

	//psinajs(sj);
}

int main(int argc, char *argv[])
{
	char buffer[4096];
	char *p;
	struct sinajs data;
	char *token;

	http_get("hq.sinajs.cn/list=sh601766", buffer, 4096);
	p = split_http_response_header(buffer);

	sinajs_decode(p, &data);

	printf("code = %s, price = %.2f\n", data.code, data.price);

	main_ui(argc, argv);
	return 0;
}
