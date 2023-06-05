// #include "s21_smartcalc.h"

// gcc graph.c -o graph `pkg-config --cflags --libs gtk+-3.0`

// double calculate_expression(const char *expression, double value) {
//     return s21_smartcalc(expression, value);
// }
#include "s21_smartcalc.h"
#include <gtk/gtk.h>

GtkWidget *expression_entry;
GtkWidget *result_label;
GtkWidget *drawing_area;
gboolean is_calculated = FALSE;

double calculate_expression(const char *expression, double value) {
  return s21_smartcalc(expression, value);
}

gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
  if (!is_calculated) {
    return FALSE;
  }

  GtkAllocation allocation;
  gtk_widget_get_allocation(widget, &allocation);
  gint width = allocation.width;
  gint height = allocation.height;

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  // Настройка параметров сетки
  double grid_spacing = 20.0;
  double grid_line_width = 0.5;
  double grid_line_dash[] = {2.0, 2.0};
  int grid_line_dash_count = 2;

  cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
  cairo_set_line_width(cr, grid_line_width);
  cairo_set_dash(cr, grid_line_dash, grid_line_dash_count, 0);

  // Рисуем вертикальные линии сетки
  for (double x = -100; x <= 100; x += grid_spacing) {
    double x_screen = width / 2 + (x / 200.0) * width;
    cairo_move_to(cr, x_screen, 0);
    cairo_line_to(cr, x_screen, height);
  }

  // Рисуем горизонтальные линии сетки
  for (double y = -100; y <= 100; y += grid_spacing) {
    double y_screen = height / 2 - (y / 200.0) * height;
    cairo_move_to(cr, 0, y_screen);
    cairo_line_to(cr, width, y_screen);
  }

  cairo_stroke(cr);

  // Настройка параметров осей координат
  double axis_line_width = 1.0;

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, axis_line_width);
  cairo_set_dash(cr, NULL, 0, 0);

  // Рисуем ось x
  double x_axis_y = height / 2;
  cairo_move_to(cr, 0, x_axis_y);
  cairo_line_to(cr, width, x_axis_y);

  // Рисуем ось y
  double y_axis_x = width / 2;
  cairo_move_to(cr, y_axis_x, 0);
  cairo_line_to(cr, y_axis_x, height);

  cairo_stroke(cr);

  // Рисуем график функции
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);

  double step = 0.1;
  double x, y;

  const char *expression = gtk_entry_get_text(GTK_ENTRY(expression_entry));

  for (x = -100; x <= 100; x += step) {
    y = calculate_expression(expression, x);
    double x_screen = width / 2 + (x / 200.0) * width;
    double y_screen = height / 2 - y;

    if (x == -100) {
      cairo_move_to(cr, x_screen, y_screen);
    } else {
      cairo_line_to(cr, x_screen, y_screen);
    }
  }

  cairo_stroke(cr);

  return FALSE;
}

void on_calculate_button_clicked(GtkWidget *widget, gpointer data) {
  is_calculated = TRUE;

  const char *expression = gtk_entry_get_text(GTK_ENTRY(expression_entry));
  double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(data));
  double result = calculate_expression(expression, value);

  char result_str[50];
  snprintf(result_str, sizeof(result_str), "Result: %.2f", result);
  gtk_label_set_text(GTK_LABEL(result_label), result_str);

  gtk_widget_queue_draw(drawing_area);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Graph");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(window), main_box);

  drawing_area = gtk_drawing_area_new();
  gtk_box_pack_start(GTK_BOX(main_box), drawing_area, TRUE, TRUE, 0);
  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_graph),
                   NULL);

  GtkWidget *expression_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(main_box), expression_box, FALSE, FALSE, 0);

  GtkWidget *expression_label = gtk_label_new("Expression:");
  gtk_box_pack_start(GTK_BOX(expression_box), expression_label, FALSE, FALSE,
                     0);

  expression_entry = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(expression_box), expression_entry, TRUE, TRUE, 0);

  GtkWidget *value_spin = gtk_spin_button_new_with_range(-100.0, 100.0, 0.1);
  gtk_box_pack_start(GTK_BOX(expression_box), value_spin, FALSE, FALSE, 0);

  GtkWidget *calculate_button = gtk_button_new_with_label("Calculate");
  g_signal_connect(G_OBJECT(calculate_button), "clicked",
                   G_CALLBACK(on_calculate_button_clicked), value_spin);
  gtk_box_pack_start(GTK_BOX(expression_box), calculate_button, FALSE, FALSE,
                     0);

  result_label = gtk_label_new("Result: ");
  gtk_box_pack_start(GTK_BOX(main_box), result_label, FALSE, FALSE, 0);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}