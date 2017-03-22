#include <gtk/gtk.h>

void add_to_list(GtkWidget *list, char *str)
{
	GtkTreeIter iter;

	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, str, -1);
	gtk_list_store_set_column_types
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

int main(int argc, char *argv[])
{
	GtkWidget *win;

	gtk_init(&argc, &argv);
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	GtkWidget *list = gtk_tree_view_new();

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);

	GtkLabel *label = gtk_label_new("");
	gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);

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

	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);

	g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(win);

	gtk_main();

	return 0;
}
