// gcc graph.c -o graph `pkg-config --cflags --libs gtk+-3.0`
// gcc graph.c s21_smartcalc.c s21_stack.c s21_smartcalc.h s21_stack.h -o graph
// -lm `pkg-config --cflags --libs gtk+-3.0`

#include "s21_smartcalc.h"
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *expression_label;

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, const char *expression) {
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_paint(cr);

  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(cr, 0.6); 

  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);

  double x_axis_y = height / 2;
  cairo_move_to(cr, 0, x_axis_y);
  cairo_line_to(cr, width, x_axis_y);

  double y_axis_x = width / 2;
  cairo_move_to(cr, y_axis_x, 0);
  cairo_line_to(cr, y_axis_x, height);

  cairo_stroke(cr);
  cairo_set_line_width(cr, 1.0);

  int number_of_vars = 0;
  double step = 0.01;
  double x_fixed = 0.f;
  double y_fixed = 0.f;
  double max_diff = 100;

  for (double x = -width/2; x <= width/2; x += step) {
    if (s21_smartcalc(expression, x, &y_fixed, &number_of_vars) == 0) {
      x_fixed = x + width/2;
      y_fixed = height / 2 - y_fixed;
      if (x == -100) {
        cairo_move_to(cr, x_fixed, y_fixed);
      } else {
        cairo_line_to(cr, x_fixed, y_fixed);
      }
    }
  }
  cairo_stroke(cr);

  return FALSE;
}

void calculate_button_clicked(GtkWidget *widget, gpointer data) {
  GtkWidget *entry1 = GTK_WIDGET(data);
  GtkWidget *entry2 = g_object_get_data(G_OBJECT(widget), "entry2");

  const gchar *expression = gtk_entry_get_text(GTK_ENTRY(entry1));
  const gchar *variable = gtk_entry_get_text(GTK_ENTRY(entry2));
  char* buffer = (char*)malloc(256 * sizeof(char));
  char* buffer_expression = (char*)malloc(256 * sizeof(char));
  strncpy(buffer, variable, 256);
  strncpy(buffer_expression, expression, 256);
  REPLACE_CHAR(buffer, '.', ',');
  REPLACE_CHAR(buffer_expression, '.', ',');
  double number = 0.f;
  number = atof(buffer);

  int number_of_vars = 0;
  double res = 0.f;
  int ret = s21_smartcalc(buffer_expression, number, &res, &number_of_vars);

  if (g_strcmp0(variable, "") == 0 && ret != -3 && number_of_vars != 0) {
    GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "График");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 1200, 900);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(new_window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), g_strdup(buffer_expression));
    gtk_widget_show_all(new_window);
    free(buffer);
    free(buffer_expression);
  } else if ((g_strcmp0(variable, "") != 0 || number_of_vars == 0) &&
             ret != -3) {
    int num_vars = 0;
    if (s21_smartcalc(buffer_expression, number, &number, &num_vars) == 0) {
      snprintf(buffer, 256, "%0.3lf\n", number);
      gtk_label_set_text(GTK_LABEL(expression_label), buffer);
    } else {
      gtk_label_set_text(GTK_LABEL(expression_label), "ERROR");
    }
  } else if (ret == -3) {
    gtk_label_set_text(GTK_LABEL(expression_label), "ERROR");
  }
}

void button1_clicked(GtkWidget *widget, gpointer data) {
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Калькулятор");
  gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 200);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);
  gtk_container_add(GTK_CONTAINER(new_window), box);

  GtkWidget *entry1 = gtk_entry_new();
  GtkWidget *entry2 = gtk_entry_new();

  GtkWidget *label1 = gtk_label_new("Выражение:");
  GtkWidget *label2 = gtk_label_new("Значение переменной (опционально):");

  GtkWidget *calculate_button = gtk_button_new_with_label("Рассчитать");

  gtk_box_pack_start(GTK_BOX(box), label1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), entry1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), entry2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), calculate_button, FALSE, FALSE, 0);

  expression_label = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(box), expression_label, FALSE, FALSE, 0);

  g_object_set_data(G_OBJECT(calculate_button), "entry2", entry2);

  g_signal_connect(G_OBJECT(calculate_button), "clicked",
                   G_CALLBACK(calculate_button_clicked), entry1);
  gtk_widget_show_all(new_window);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Введение в GTK");
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);

  gtk_window_set_default_size(
      GTK_WINDOW(window), 400,
      215);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,
                               0);
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);

  GtkWidget *button1 = gtk_button_new_with_label("Калькулятор");
  GtkWidget *button2 = gtk_button_new_with_label("Кредитный калькулятор");
  GtkWidget *button3 = gtk_button_new_with_label("Депозитный калькулятор");

  gtk_widget_set_size_request(button1, 400, 100);
  gtk_widget_set_size_request(button2, 400, 100);
  gtk_widget_set_size_request(button3, 400, 100);

  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE,
                     15);
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 15);
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 15);

  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(button1_clicked),
                   NULL);

  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}
