// gcc graph.c -o graph `pkg-config --cflags --libs gtk+-3.0`
// gcc graph.c s21_smartcalc.c s21_stack.c s21_smartcalc.h s21_stack.h -o graph
// -lm `pkg-config --cflags --libs gtk+-3.0`

#include "s21_smartcalc.h"
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *expression_label;

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, char *expression) {
      printf("%s\n", expression);
  // Устанавливаем цвет заливки поля
  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Белый цвет
  cairo_paint(cr);

  // Устанавливаем параметры линии
  cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Черный цвет
  cairo_set_line_width(cr, 1.0);           // Толщина линии

  int number_of_vars = 0;
  double step = 0.01;
  double x_fixed = 0.f;
  double y_fixed = 0.f;
  int size_x = 800;
  int size_y = 600;

  for (double x = -100; x <= 100; x += step) {
    if (s21_smartcalc(expression, x, &y_fixed, &number_of_vars) == 0) {
      y_fixed = size_y / 2 - y_fixed;
      x_fixed = size_x / 2 + (x / 200.0) * size_x;
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
  char buffer[256] = "\0";
  char buffer_expression[256] = "\0";
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
    gtk_window_set_default_size(GTK_WINDOW(new_window), 800, 600);

    // Создаем область для рисования
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(new_window), drawing_area);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), buffer_expression);
    gtk_widget_show_all(new_window);
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
  // Создание нового окна
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Калькулятор");
  gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 200);

  // Создание контейнера
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);
  gtk_container_add(GTK_CONTAINER(new_window), box);

  // Создание полей ввода
  GtkWidget *entry1 = gtk_entry_new();
  GtkWidget *entry2 = gtk_entry_new();

  // Создание подписей для полей ввода
  GtkWidget *label1 = gtk_label_new("Выражение:");
  GtkWidget *label2 = gtk_label_new("Значение переменной (опционально):");

  // Создание кнопки "Рассчитать"
  GtkWidget *calculate_button = gtk_button_new_with_label("Рассчитать");

  // Добавление виджетов в контейнер
  gtk_box_pack_start(GTK_BOX(box), label1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), entry1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), label2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), entry2, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(box), calculate_button, FALSE, FALSE, 0);

  expression_label = gtk_label_new(NULL);
  gtk_box_pack_start(GTK_BOX(box), expression_label, FALSE, FALSE, 0);

  g_object_set_data(G_OBJECT(calculate_button), "entry2", entry2);

  // Подключение функции обратного вызова к кнопке "Рассчитать"
  g_signal_connect(G_OBJECT(calculate_button), "clicked",
                   G_CALLBACK(calculate_button_clicked), entry1);
  // Отображение всех виджетов
  gtk_widget_show_all(new_window);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  // Создание основного окна
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Введение в GTK");
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);

  // Установка начального размера окна
  gtk_window_set_default_size(
      GTK_WINDOW(window), 400,
      215); // Ширина: 400 пикселей, Высота: 300 пикселей

  // Создание контейнера
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL,
                               0); // 10 - отступы между элементами
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);

  // Создание кнопок
  GtkWidget *button1 = gtk_button_new_with_label("Калькулятор");
  GtkWidget *button2 = gtk_button_new_with_label("Кредитный калькулятор");
  GtkWidget *button3 = gtk_button_new_with_label("Депозитный калькулятор");

  // Увеличение размера кнопок
  gtk_widget_set_size_request(button1, 400, 100);
  gtk_widget_set_size_request(button2, 400, 100);
  gtk_widget_set_size_request(button3, 400, 100);

  // Добавление кнопок в контейнер
  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE,
                     15); // FALSE - кнопка не растягивается, 0 - отступ
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 15);
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 15);

  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(button1_clicked),
                   NULL);

  // Отображение всех виджетов
  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}
