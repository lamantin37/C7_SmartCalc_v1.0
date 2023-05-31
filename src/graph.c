#include <gtk/gtk.h>

GtkWidget *expression_entry;
GtkWidget *result_label;
GtkWidget *drawing_area;

void draw_graph(cairo_t *cr, double width, double height) {
    // Здесь реализуйте логику отрисовки графика с использованием Cairo

    // Пример отрисовки графика
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // Цвет черный
    cairo_set_line_width(cr, 1.0); // Толщина линии
    cairo_move_to(cr, 0, 0); // Перемещение к начальной точке
    cairo_line_to(cr, width, height); // Рисование линии
    cairo_stroke(cr); // Отрисовка
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    gint width = allocation.width;
    gint height = allocation.height;

    draw_graph(cr, width, height);

    return FALSE;
}

void calculate_expression(GtkWidget *widget, gpointer data) {
    // Получаем текст из поля ввода выражения
    const gchar *expression = gtk_entry_get_text(GTK_ENTRY(expression_entry));

    // Здесь вызываем вашу функцию s21_smartcalc с передачей выражения и значения переменной x

    // Пример результата вычислений
    double result = 42.0;

    // Преобразуем результат в строку и выводим его на метку
    gchar *result_str = g_strdup_printf("Result: %.2f", result);
    gtk_label_set_text(GTK_LABEL(result_label), result_str);
    g_free(result_str);

    // Перерисовываем график
    gtk_widget_queue_draw(drawing_area);
}

int main(int argc, char *argv[]) {
    // Инициализация GTK+
    gtk_init(&argc, &argv);

    // Создание главного окна
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Smart Calculator");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 500, 350);

    // Создание вертикального контейнера
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Создание поля ввода выражения
    expression_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(expression_entry), "Enter expression");
    gtk_box_pack_start(GTK_BOX(vbox), expression_entry, FALSE, FALSE, 0);

    // Создание кнопки "Calculate"
    GtkWidget *calculate_button = gtk_button_new_with_label("Calculate");
    g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(calculate_expression), NULL);
    gtk_box_pack_start(GTK_BOX(vbox), calculate_button, FALSE, FALSE, 0);

    // Создание метки для отображения результата
    result_label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(vbox), result_label, FALSE, FALSE, 0);

    // Создание области для рисования (GtkDrawingArea)
    drawing_area = gtk_drawing_area_new();
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    // Задаем обработчик события отрисовки
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_event), NULL);

    // Завершение работы при закрытии окна
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Отображение всех виджетов
    gtk_widget_show_all(window);

    // Запуск основного цикла GTK+
    gtk_main();

    return 0;
}
