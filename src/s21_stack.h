#include <stdio.h>
#define MAX_SIZE 255

typedef struct {
  double data[MAX_SIZE];
  int is_num[MAX_SIZE];
  int top;
} Stack;

void init(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push(Stack *stack, double value, int is_num);
double pop(Stack *stack);
void printStack(Stack* stack);