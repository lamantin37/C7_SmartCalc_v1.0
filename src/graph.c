#include <gtk/gtk.h>
#include "s21_smartcalc.h"

// gcc graph.c -o graph `pkg-config --cflags --libs gtk+-3.0`

double calculate_expression(const char *expression, double value) {
    return s21_smartcalc(expression, value);
}

gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
    gint width, height;
    gtk_widget_get_size_request(widget, &width, &height);

    cairo_set_source_rgb(cr, 1, 1, 1); // Задайте цвет фона
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0, 0, 0); // Задайте цвет графика
    cairo_set_line_width(cr, 1); // Задайте толщину линии

    double step = 0.1; // Шаг между точками графика
    double x, y;

    for (x = -100; x <= 100; x += step) {
        y = calculate_expression("sin(x)", x); // Здесь передайте нужное выражение
        double x_screen = width / 2 + x; // Преобразование координаты x на экране
        double y_screen = height / 2 - y; // Преобразование координаты y на экране

        if (x == -100) {
            cairo_move_to(cr, x_screen, y_screen);
        } else {
            cairo_line_to(cr, x_screen, y_screen);
        }
    }

    cairo_stroke(cr);

    return FALSE;
}

int main(int argc, char *argv[]) {
    // Инициализация GTK+
    gtk_init(&argc, &argv);

    // Создание главного окна
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Graph");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Создание области для рисования (GtkDrawingArea)
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Задание обработчика события отрисовки
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_graph), NULL);

    // Отображение всех виджетов
    gtk_widget_show_all(window);

    // Запуск основного цикла GTK+
    gtk_main();

    return 0;
}
