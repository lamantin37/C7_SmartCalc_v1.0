#include "s21_stack.h"

void init(Stack *stack) { stack->top = -1; }

int isEmpty(Stack *stack) { return stack->top == -1; }

int isFull(Stack *stack) { return stack->top == MAX_SIZE - 1; }

void push(Stack *stack, double value, int is_num) {
  if (isFull(stack)) {
    return;
  }
  stack->top++;
  stack->data[stack->top] = value;
  stack->is_num[stack->top] = is_num;
}

double pop(Stack *stack) {
  if (isEmpty(stack)) {
    return -1;
  }
  double value = stack->data[stack->top];
  stack->data[stack->top] = 0;
  stack->is_num[stack->top] = 0;
  stack->top--;
  return value;
}