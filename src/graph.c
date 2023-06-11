// gcc graph.c -o graph `pkg-config --cflags --libs gtk+-3.0`
// gcc graph.c s21_smartcalc.c s21_stack.c s21_smartcalc.h s21_stack.h -o graph
// -lm `pkg-config --cflags --libs gtk+-3.0`

#include "s21_smartcalc.h"
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *expression_label;

GtkWidget *credit_window;
GtkWidget *credit_window2;
GtkWidget *amount_entry;
GtkWidget *rate_entry;
GtkWidget *term_entry;
GtkWidget *type_combo;
GtkWidget *chart_area;
GtkWidget *payment_label;
GtkWidget *total_label;

void calculate_credit(GtkWidget *widget, gpointer data) {
  // Получение значений из полей
  const gchar *amount_str = gtk_entry_get_text(GTK_ENTRY(amount_entry));
  const gchar *rate_str = gtk_entry_get_text(GTK_ENTRY(rate_entry));
  const gchar *term_str = gtk_entry_get_text(GTK_ENTRY(term_entry));
  const gchar *type_str = gtk_combo_box_text_get_active_text(
      GTK_COMBO_BOX_TEXT(type_combo));

  // Преобразование строковых значений в числа
  double amount = atof(amount_str);
  double rate = atof(rate_str);
  int term = atoi(term_str);

  // Выполнение расчетов и отрисовка графика

  // Размеры полей и графика
  int field_width = 200;
  int field_height = 25;
  int chart_width = 200;
  int chart_height = 100;

  // Создание полей для отображения результатов
  GtkWidget *amount_label = gtk_label_new(NULL);
  GtkWidget *rate_label = gtk_label_new(NULL);
  GtkWidget *term_label = gtk_label_new(NULL);
  GtkWidget *type_label = gtk_label_new(NULL);

  // Создание графика
  GtkWidget *chart = gtk_drawing_area_new();
  gtk_widget_set_size_request(chart, chart_width, chart_height);

  // Создание меток для отображения ежемесячного платежа и общей суммы кредита
  payment_label = gtk_label_new(NULL);
  total_label = gtk_label_new(NULL);

  credit_window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(credit_window2), "Кредитный калькулятор");
  // Расположение полей, графика и меток в окне
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(credit_window2), box);

  gtk_box_pack_start(GTK_BOX(box), amount_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), rate_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), term_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), type_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), chart, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), payment_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), total_label, FALSE, FALSE, 0);

  // Отображение результатов
  char result_str[100];

  sprintf(result_str, "Сумма кредита: %.2f", amount);
  gtk_label_set_text(GTK_LABEL(amount_label), result_str);

  sprintf(result_str, "Ставка процента: %.2f", rate);
  gtk_label_set_text(GTK_LABEL(rate_label), result_str);

  sprintf(result_str, "Срок кредита: %d", term);
  gtk_label_set_text(GTK_LABEL(term_label), result_str);

  sprintf(result_str, "Тип кредита: %s", type_str);
  gtk_label_set_text(GTK_LABEL(type_label), result_str);

  gtk_widget_show_all(credit_window2);
  // Очистка памяти и освобождение ресурсов
  g_free((gpointer)type_str);
}

void button2_clicked(GtkWidget *widget, gpointer data) {
  // Создание окна для кредитного калькулятора
  credit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(credit_window), "Кредитный калькулятор");
  
  // Создание контейнера для кнопок
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(credit_window), box);

  // Создание полей ввода и кнопки "Рассчитать"
  GtkWidget *grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 15);

  GtkWidget *amount_label = gtk_label_new("Сумма кредита:");
  GtkWidget *rate_label = gtk_label_new("Ставка процента:");
  GtkWidget *term_label = gtk_label_new("Срок кредита:");
  GtkWidget *type_label = gtk_label_new("Тип кредита:");

  amount_entry = gtk_entry_new();
  rate_entry = gtk_entry_new();
  term_entry = gtk_entry_new();
  type_combo = gtk_combo_box_text_new();

  gtk_grid_attach(GTK_GRID(grid), amount_label, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rate_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), term_label, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), type_label, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), amount_entry, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rate_entry, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), term_entry, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), type_combo, 1, 3, 1, 1);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Аннуитетный");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo), "Дифференцированный");

  GtkWidget *calculate_button = gtk_button_new_with_label("Рассчитать");
  g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(calculate_credit), NULL);
  gtk_box_pack_start(GTK_BOX(box), calculate_button, FALSE, FALSE, 0);

  gtk_widget_show_all(credit_window);
}

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
  gtk_window_set_title(GTK_WINDOW(window), "s21_smartcalc");
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
  g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(button2_clicked), NULL);

  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}
