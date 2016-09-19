#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <gtk/gtk.h>

#include <ccutils/net.h>
#include <ccutils/log.h>

#include "sinajs.h"
#include "stkmon.h"

struct stkui {
	char *code;
	GtkWidget *label_code;
	GtkWidget *label_price;
};

gboolean hdlr_1s(gpointer *);
int parse_xmlconfig(struct sm_desc *);

int main_ui(int argc, char *argv[], struct sm_desc *desc)
{
	int i;
	int px, py;
	struct sm_stock *stock;

	gtk_init(&argc, &argv);

	GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), "stkmon");
	gtk_window_set_default_size(GTK_WINDOW(win), 0, 0);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	gtk_window_get_position(GTK_WINDOW(win), &px, &py);
	gtk_window_move(GTK_WINDOW(win), px * 2, py);
	gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);

	GtkWidget *mbox = gtk_vbox_new(TRUE, 1);
	gtk_container_add(GTK_CONTAINER(win), mbox);

	for(stock = desc->stock; stock; stock = stock->next) {
		GtkWidget *align;
		GtkWidget *label;

		GtkWidget *hbox = gtk_hbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(mbox), hbox, FALSE, FALSE, 0);

		align = gtk_alignment_new(0, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 5, 5);
		label = gtk_label_new(stock->code);
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_code = label;
		//
		align = gtk_alignment_new(1, 0, 0, 0);
		gtk_alignment_set_padding(GTK_ALIGNMENT(align), 0, 0, 5, 5);
		label = gtk_label_new("0");
		gtk_container_add(GTK_CONTAINER(align), label);
		gtk_container_add(GTK_CONTAINER(hbox), align);
		stock->ui.label_price = label;
	}

	g_timeout_add(desc->cfg.delay_ms, (GSourceFunc)hdlr_1s, (gpointer)desc);
	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(win);
	hdlr_1s((gpointer)desc);

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
}

gboolean hdlr_1s(gpointer *p)
{
	char buffer[4096];
	char *bp;
	struct sinajs data;
	int i;
	struct sm_desc *desc = (struct sm_desc *)p;
	struct sm_stock *stock;
	int rc;

	for(stock = desc->stock; stock; stock = stock->next) {
		char *url;
		rc = asprintf(&url, "hq.sinajs.cn/list=%s%s", stock->stkex, stock->code);
		if(rc < 0) {
			g_printf("%d: %s\n", __LINE__, strerror(errno));
			continue;
		}
		http_get(url, buffer, 4096);
		free(url);

		bp = split_http_response_header(buffer);
		sinajs_decode(bp, &data);
		sprintf(buffer, "%.2f", data.price);
		gtk_label_set_text(GTK_LABEL(stock->ui.label_price), buffer);
		pr_info("code = %s, price = %.2f\n", data.code, data.price);
	}

	return TRUE;
}

int main(int argc, char *argv[])
{
	struct sm_desc desc;

	parse_xmlconfig(&desc);
	main_ui(argc, argv, &desc);

	return 0;
}
