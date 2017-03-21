#include <gtk/gtk.h>

GtkWidget *list;

void add_to_list(GtkWidget *list, char *str)
{
	GtkTreeIter iter;

	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, str, -1);
}

void on_changed(GtkWidget *widget, gpointer label)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	char *value;

	if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {
		gtk_tree_model_get(model, &iter, 0, &value, -1);
		gtk_label_set_text(GTK_LABEL(label), value);
		g_free(value);
	}
}

void on_clicked_add(GtkWidget *widget, gpointer data)
{
	add_to_list(list, "new a");
}

void on_clicked_dela(GtkWidget *widget, gpointer selection)
{
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter_first(model, &iter) == FALSE)
		return;

	if(gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter))
		gtk_list_store_remove(store, &iter);
}

void on_clicked_delall(GtkWidget *widget, gpointer data)
{
	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
	GtkTreeIter iter;

	if(gtk_tree_model_get_iter_first(model, &iter) == FALSE)
		return;

	gtk_list_store_clear(store);
}

int main(int argc, char *argv[])
{
	GtkWidget *win;

	gtk_init(&argc, &argv);
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(win, 300, 300);

	GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
	list = gtk_tree_view_new();
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), TRUE);
	gtk_container_add(GTK_CONTAINER(sw), list);

	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);

	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);

	GtkLabel *label = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);

	GtkWidget *add = gtk_button_new_with_label("add");
	GtkWidget *dela = gtk_button_new_with_label("dela");
	GtkWidget *delall = gtk_button_new_with_label("delall");
	gtk_box_pack_start(GTK_BOX(vbox), add, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), dela, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(vbox), delall, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(win), vbox);

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("List Items", renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
	GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));
	g_object_unref(store);

	add_to_list(list, "string a");
	add_to_list(list, "I love you");
	add_to_list(list, "Good afternoon");

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));

	g_signal_connect(G_OBJECT(add), "clicked", G_CALLBACK(on_clicked_add), NULL);
	g_signal_connect(G_OBJECT(dela), "clicked", G_CALLBACK(on_clicked_dela), selection);
	g_signal_connect(G_OBJECT(delall), "clicked", G_CALLBACK(on_clicked_delall), selection);

	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}
