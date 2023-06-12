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

GtkWidget *deposit_window;
GtkWidget *deposit_amount_entry;
GtkWidget *placement_term_entry;
GtkWidget *interest_rate_entry;
GtkWidget *tax_rate_entry;
GtkWidget *payment_frequency_combo;
GtkWidget *interest_compound_combo;
GtkWidget *interest_type_combo;
GtkWidget *deposit_additions_entry;
GtkWidget *partial_withdrawals_entry;

GtkWidget *payment_frequency_label;
GtkWidget *interest_compound_label;

void calculate_deposit(GtkWidget *widget, gpointer data) {
  const gchar *deposit_amount_str = gtk_entry_get_text(GTK_ENTRY(deposit_amount_entry));
  const gchar *placement_term_str = gtk_entry_get_text(GTK_ENTRY(placement_term_entry));
  const gchar *interest_rate_str = gtk_entry_get_text(GTK_ENTRY(interest_rate_entry));
  const gchar *tax_rate_str = gtk_entry_get_text(GTK_ENTRY(tax_rate_entry));
  const gchar *payment_frequency_str = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(payment_frequency_combo));
  const gchar *interest_compound_str = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(interest_compound_combo));
  const gchar *deposit_additions_str = gtk_entry_get_text(GTK_ENTRY(deposit_additions_entry));
  const gchar *partial_withdrawals_str = gtk_entry_get_text(GTK_ENTRY(partial_withdrawals_entry));

  double deposit_amount = atof(deposit_amount_str);
  double placement_term = atof(placement_term_str);
  double interest_rate = atof(interest_rate_str);
  double tax_rate = atof(tax_rate_str);
  double calculated_amount = deposit_amount;
  calculated_amount += calculated_amount * (interest_rate / 100);
  double deposit_additions = atof(deposit_additions_str);
  calculated_amount += deposit_additions;
  double partial_withdrawals = atof(partial_withdrawals_str);
  calculated_amount -= partial_withdrawals;
  calculated_amount -= calculated_amount * (tax_rate / 100);

  GtkWidget *result_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(result_window), "Результат расчета");

  gchar *result_text = g_strdup_printf("Сумма вклада с начисленными процентами: %.2f", calculated_amount);
  GtkWidget *result_label = gtk_label_new(result_text);
  g_free(result_text);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(result_window), box);
  gtk_box_pack_start(GTK_BOX(box), result_label, FALSE, FALSE, 15);

  gtk_widget_show_all(result_window);
}

void on_interest_type_changed(GtkWidget *widget, gpointer data) {
  const gchar *selected_type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

  if (strcmp(selected_type, "Добавлять к вкладу") == 0) {
    gtk_widget_show(GTK_WIDGET(payment_frequency_label));
    gtk_widget_show(GTK_WIDGET(interest_compound_label));
  } else {
    gtk_widget_hide(GTK_WIDGET(payment_frequency_label));
    gtk_widget_hide(GTK_WIDGET(interest_compound_label));
  }

  g_free((gchar *) selected_type);
}

void button3_clicked(GtkWidget *widget, gpointer data) {
  deposit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(deposit_window), "Депозитный калькулятор");

  gtk_window_set_default_size(GTK_WINDOW(deposit_window), 400, 200);
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(deposit_window), box);

  GtkWidget *grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(box), grid, FALSE, FALSE, 15);

  GtkWidget *deposit_amount_label = gtk_label_new("Сумма вклада:");
  GtkWidget *placement_term_label = gtk_label_new("Срок размещения:");
  GtkWidget *interest_rate_label = gtk_label_new("Процентная ставка:");
  GtkWidget *tax_rate_label = gtk_label_new("Налоговая ставка:");
  GtkWidget *interest_type_label = gtk_label_new("Начислено процентов:");
  GtkWidget *deposit_additions_label = gtk_label_new("Сумма пополнений:");
  GtkWidget *partial_withdrawals_label = gtk_label_new("Сумма частичных снятий:");

  deposit_amount_entry = gtk_entry_new();
  placement_term_entry = gtk_entry_new();
  interest_rate_entry = gtk_entry_new();
  tax_rate_entry = gtk_entry_new();
  interest_type_combo = gtk_combo_box_text_new();
  deposit_additions_entry = gtk_entry_new();
  partial_withdrawals_entry = gtk_entry_new();

  gtk_grid_attach(GTK_GRID(grid), deposit_amount_label, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), placement_term_label, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), interest_rate_label, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), tax_rate_label, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), interest_type_label, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), deposit_additions_label, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), partial_withdrawals_label, 0, 6, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), deposit_amount_entry, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), placement_term_entry, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), interest_rate_entry, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), tax_rate_entry, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), interest_type_combo, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), deposit_additions_entry, 1, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), partial_withdrawals_entry, 1, 6, 1, 1);

  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interest_type_combo), "Добавлять к вкладу");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(interest_type_combo), "Выплачивать");

  gtk_widget_set_margin_start(GTK_WIDGET(deposit_amount_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(deposit_amount_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(placement_term_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(placement_term_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(interest_rate_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(interest_rate_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(tax_rate_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(tax_rate_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(interest_type_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(interest_type_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(deposit_additions_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(deposit_additions_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(partial_withdrawals_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(partial_withdrawals_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(deposit_amount_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(deposit_amount_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(placement_term_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(placement_term_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(interest_rate_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(interest_rate_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(tax_rate_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(tax_rate_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(interest_type_combo), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(interest_type_combo), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(deposit_additions_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(deposit_additions_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(partial_withdrawals_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(partial_withdrawals_entry), 5);

  GtkWidget *calculate_button = gtk_button_new_with_label("Рассчитать");
  g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(calculate_deposit), NULL);
  gtk_box_pack_start(GTK_BOX(box), calculate_button, FALSE, FALSE, 0);

  interest_compound_label = gtk_label_new("Периодичность капитализации:");
  payment_frequency_label = gtk_label_new("Периодичность выплат:");

  g_signal_connect(G_OBJECT(interest_type_combo), "changed", G_CALLBACK(on_interest_type_changed), NULL);

  gtk_widget_set_margin_start(GTK_WIDGET(interest_compound_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(interest_compound_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(payment_frequency_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(payment_frequency_label), 5);

  gtk_widget_show_all(deposit_window);
}

gboolean draw_chart(GtkWidget *widget, cairo_t *cr, double *data_pay) {
  GtkAllocation allocation;

  gtk_widget_get_allocation(widget, &allocation);

  guint width = allocation.width;
  guint height = allocation.height;

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
  cairo_rectangle(cr, 0, 0, width, height);
  cairo_fill(cr);

  double padding = 10.0;
  double corner_radius = 5.0;

  cairo_set_source_rgb(cr, 0.1, 0.2, 0.7);
  double initial_width = (data_pay[0] - data_pay[1]) / data_pay[0] * (width - 2 * padding);
  cairo_move_to(cr, padding + corner_radius, padding);
  cairo_line_to(cr, padding + initial_width - corner_radius, padding);
  cairo_arc(cr, padding + initial_width - corner_radius, padding + corner_radius, corner_radius, 1.5 * M_PI, 2 * M_PI);
  cairo_line_to(cr, padding + initial_width, height - padding - corner_radius);
  cairo_arc(cr, padding + initial_width - corner_radius, height - padding - corner_radius, corner_radius, 0, 0.5 * M_PI);
  cairo_line_to(cr, padding + corner_radius, height - padding);
  cairo_arc(cr, padding + corner_radius, height - padding - corner_radius, corner_radius, 0.5 * M_PI, M_PI);
  cairo_line_to(cr, padding, padding + corner_radius);
  cairo_arc(cr, padding + corner_radius, padding + corner_radius, corner_radius, M_PI, 1.5 * M_PI);
  cairo_close_path(cr);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 0.7, 0.2, 0.1);
  double overpayment_width = data_pay[1] / data_pay[0] * (width - 2 * padding);
  double overpayment_x = padding + initial_width;
  cairo_move_to(cr, overpayment_x + corner_radius, padding);
  cairo_line_to(cr, overpayment_x + overpayment_width - corner_radius, padding);
  cairo_arc(cr, overpayment_x + overpayment_width - corner_radius, padding + corner_radius, corner_radius, 1.5 * M_PI, 2 * M_PI);
  cairo_line_to(cr, overpayment_x + overpayment_width, height - padding - corner_radius);
  cairo_arc(cr, overpayment_x + overpayment_width - corner_radius, height - padding - corner_radius, corner_radius, 0, 0.5 * M_PI);
  cairo_line_to(cr, overpayment_x + corner_radius, height - padding);
  cairo_arc(cr, overpayment_x + corner_radius, height - padding - corner_radius, corner_radius, 0.5 * M_PI, M_PI);
  cairo_line_to(cr, overpayment_x, padding + corner_radius);
  cairo_arc(cr, overpayment_x + corner_radius, padding + corner_radius, corner_radius, M_PI, 1.5 * M_PI);
  cairo_close_path(cr);
  cairo_fill(cr);

  return FALSE;
}

void calculate_credit(GtkWidget *widget, gpointer data) {
  const gchar *amount_str = gtk_entry_get_text(GTK_ENTRY(amount_entry));
  const gchar *rate_str = gtk_entry_get_text(GTK_ENTRY(rate_entry));
  const gchar *term_str = gtk_entry_get_text(GTK_ENTRY(term_entry));
  const gchar *type_str =
      gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(type_combo));

  gdouble amount = g_strtod(amount_str, NULL);
  gdouble rate = g_strtod(rate_str, NULL);
  gint term = g_ascii_strtoll(term_str, NULL, 10);

  gdouble total_payment = 0.0;
  gdouble monthly_payment = 0.0;
  gdouble overpayment = 0.0;
  gdouble initial_amount = amount;

  if (g_strcmp0(type_str, "Аннуитетный") == 0) {
    gdouble monthly_rate = rate / 100.0 / 12.0;
    gdouble monthly_rate_plus_one = monthly_rate + 1.0;
    gdouble power_term = pow(monthly_rate_plus_one, term);
    monthly_payment = amount * monthly_rate * power_term / (power_term - 1.0);
    total_payment = monthly_payment * term;
    overpayment = total_payment - amount;
  } else if (g_strcmp0(type_str, "Дифференцированный") == 0) {
    gdouble monthly_rate = rate / 100.0 / 12.0;
    monthly_payment = amount / term + amount * monthly_rate;
    total_payment = monthly_payment * term;
    overpayment = total_payment - amount;
  }

  GtkWidget *result_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(result_window), "Результаты расчета");
  gtk_window_set_default_size(GTK_WINDOW(result_window), 500, 300);

  GtkWidget *result_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(result_window), result_box);

  GtkWidget *total_label = gtk_label_new(NULL);
  GtkWidget *overpay_label = gtk_label_new(NULL);
  GtkWidget *monthly_label = gtk_label_new(NULL);

  gchar *total_text =
      g_strdup_printf("Общая сумма платежей: %.2f", total_payment);
  gchar *overpay =
      g_strdup_printf("Переплата по кредиту: %.2f", overpayment);
  gchar *monthly_text =
      g_strdup_printf("Ежемесячный платеж: %.2f", monthly_payment);

  gtk_label_set_text(GTK_LABEL(total_label), total_text);
  gtk_label_set_text(GTK_LABEL(overpay_label), overpay);
  gtk_label_set_text(GTK_LABEL(monthly_label), monthly_text);

  gtk_widget_set_margin_start(total_label, 10);
  gtk_widget_set_margin_top(total_label, 5);
  gtk_widget_set_margin_start(overpay_label, 10);
  gtk_widget_set_margin_top(overpay_label, 5);
  gtk_widget_set_margin_start(monthly_label, 10);
  gtk_widget_set_margin_top(monthly_label, 5);

  gtk_box_pack_start(GTK_BOX(result_box), total_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(result_box), overpay_label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(result_box), monthly_label, FALSE, FALSE, 0);

  GtkWidget *drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 400, 200);

  double data_pay[2];
  data_pay[0] = total_payment;
  data_pay[1] = overpayment;
  g_signal_connect(G_OBJECT(drawing_area), "draw",
                   G_CALLBACK(draw_chart), g_memdup2(data_pay, 2 * sizeof(double)));
  gtk_box_pack_start(GTK_BOX(result_box), drawing_area, FALSE, FALSE, 0);

  g_free(total_text);
  g_free(monthly_text);

  gtk_widget_show_all(result_window);
}

void button2_clicked(GtkWidget *widget, gpointer data) {
  credit_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(credit_window), "Кредитный калькулятор");

  gtk_window_set_default_size(GTK_WINDOW(credit_window), 400, 200);
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(credit_window), box);

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
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(type_combo),
                                 "Дифференцированный");

  gtk_widget_set_margin_start(GTK_WIDGET(amount_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(amount_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(rate_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(rate_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(term_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(term_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(type_label), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(type_label), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(amount_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(amount_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(rate_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(rate_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(term_entry), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(term_entry), 5);
  gtk_widget_set_margin_start(GTK_WIDGET(type_combo), 10);
  gtk_widget_set_margin_top(GTK_WIDGET(type_combo), 5);

  GtkWidget *calculate_button = gtk_button_new_with_label("Рассчитать");
  g_signal_connect(G_OBJECT(calculate_button), "clicked",
                   G_CALLBACK(calculate_credit), NULL);
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

  for (double x = -width / 2; x <= width / 2; x += step) {
    if (s21_smartcalc(expression, x, &y_fixed, &number_of_vars) == 0) {
      x_fixed = x + width / 2;
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
  char *buffer = (char *)malloc(256 * sizeof(char));
  char *buffer_expression = (char *)malloc(256 * sizeof(char));
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
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback),
                     g_strdup(buffer_expression));
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

  gtk_window_set_default_size(GTK_WINDOW(window), 400, 215);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(window), box);
  gtk_container_set_border_width(GTK_CONTAINER(box), 10);

  GtkWidget *button1 = gtk_button_new_with_label("Калькулятор");
  GtkWidget *button2 = gtk_button_new_with_label("Кредитный калькулятор");
  GtkWidget *button3 = gtk_button_new_with_label("Депозитный калькулятор");

  gtk_widget_set_size_request(button1, 400, 100);
  gtk_widget_set_size_request(button2, 400, 100);
  gtk_widget_set_size_request(button3, 400, 100);

  gtk_box_pack_start(GTK_BOX(box), button1, FALSE, FALSE, 15);
  gtk_box_pack_start(GTK_BOX(box), button2, FALSE, FALSE, 15);
  gtk_box_pack_start(GTK_BOX(box), button3, FALSE, FALSE, 15);

  g_signal_connect(G_OBJECT(button1), "clicked", G_CALLBACK(button1_clicked),
                   NULL);
  g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(button2_clicked),
                   NULL);
  g_signal_connect(G_OBJECT(button3), "clicked", G_CALLBACK(button3_clicked),
                   NULL);

  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}
