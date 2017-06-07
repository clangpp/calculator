#include <stdio.h>

#define MAX_SIZE 1000

const int PARENTHESIS_PRIORITY = 10;
const int PLUS_MINUS_PRIORITY = 1;
const int MULTIPLY_DIVIDE_PRIORITY = 2;

int ParseExpression(char* expression, int* numbers, char* operators, int* priorities) {
  int op_count = 0;
  int i = 0;
  char ch = 0;
  int open_parenthesis_count = 0;
  // NOTE: '\0' is the last charactor of string, when reaching it,
  // we know we have done reading all charactors in string.
  for (op_count = 0, open_parenthesis_count = 0, i = 0;
       i < MAX_SIZE && expression[i] != '\0'; ++i) {
    ch = expression[i];
    // printf("current character: %c\n", ch);
    switch (ch) {
      // Handles numbers.
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        numbers[op_count] = numbers[op_count] * 10 + (ch - '0');
        printf("number is: %d\n", numbers[op_count]);
        break;

      // Handles parentheses.
      case '(':
        ++open_parenthesis_count;
        break;
      case ')':
        --open_parenthesis_count;
        break;

      // Handles operators.
      case '+':
      case '-':
      case '*':
      case '/':
        operators[op_count] = ch;
        priorities[op_count] = PARENTHESIS_PRIORITY * open_parenthesis_count;
        if ('*' == ch || '/' == ch) {
          priorities[op_count] += MULTIPLY_DIVIDE_PRIORITY;
        } else {  // '+' == ch || '-' == ch
          priorities[op_count] += PLUS_MINUS_PRIORITY;
        }
        printf("operator is: '%c', priority is: %d\n", operators[op_count], priorities[op_count]);
        ++op_count;
        break;
      default:
        printf("unsupported character expression[%d]: '%c'\n", i, ch);
        return -1;
    }
    if (open_parenthesis_count < 0) {
      printf("more ')' than '(' found at expression[%d]\n", i);
      return -1;
    }
  }
  if (open_parenthesis_count != 0) {
    printf("parentheses '()' don't match: num of '(' - num of ')' = %d\n", open_parenthesis_count);
    return -1;
  }
  return op_count;
}

// NOTE: count of numbers = count of operators (op_count) + 1
int EvaluateHighestPriorityOperator(int* numbers, char* operators, int* priorities, int op_count) {
  int op_chosen = 0;
  int i = 0;
  int result = 0, num_left = 0, num_right = 0;
  if (0 == op_count) {
    return 0;
  }

  // Chooses highest priority operator to evaluate.
  for (op_chosen = -1, i = 0; i < op_count; ++i) {
    if (-1 == op_chosen || priorities[op_chosen] < priorities[i]) {
      op_chosen = i;
    }
  }

  // Evaluates chosen operator.
  num_left = numbers[op_chosen];
  num_right = numbers[op_chosen+1];
  printf("evaluating: '%d%c%d'\n", num_left, operators[op_chosen], num_right);
  switch (operators[op_chosen]) {
    case '+':
      result = num_left + num_right;
      break;
    case '-':
      result = num_left - num_right;
      break;
    case '*':
      result = num_left * num_right;
      break;
    case '/':
      result = num_left / num_right;
      break;
    default:
      printf("unsupported operation %d %c %d\n", num_left, operators[op_chosen], num_right);
      return -1;
  }

  // Replaces evaluated operation with result.
  numbers[op_chosen] = result;
  for (i = op_chosen+1; i < op_count; ++i) {
    // Left shifts rest of expression.
    numbers[i] = numbers[i+1];
    operators[i-1] = operators[i];
    priorities[i-1] = priorities[i];
  }
  numbers[op_count] = 0;
  operators[op_count-1] = 0;
  priorities[op_count-1] = 0;
  return op_count - 1;
}

// No need to understand this, this is just a helper function to print expression
// to help you understand how the program works.
void PrintExpression(int* numbers, char* operators, int* priorities, int op_count) {
  int i = 0, j = 0;
  int parenthesis_delta = 0;
  int next_op_priority = 0;
  if (op_count < 0) {
    return;
  }
  printf("expression is: '");
  for (parenthesis_delta = priorities[0]/PARENTHESIS_PRIORITY, j = 0;
       j < parenthesis_delta; ++j) {
    printf("(");
  }
  printf("%d", numbers[0]);
  for (i = 0; i < op_count; ++i) {
    next_op_priority = 0;
    if (i < op_count-1) {  // not last operator
      next_op_priority = priorities[i+1];
    }
    parenthesis_delta =
      next_op_priority/PARENTHESIS_PRIORITY - priorities[i]/PARENTHESIS_PRIORITY;

    printf("%c", operators[i]);
    for (j = 0; j < parenthesis_delta; ++j) {
      printf("(");
    }
    printf("%d", numbers[i+1]);
    for (j = 0; j < -parenthesis_delta; ++j) {
      printf(")");
    }
  }
  printf("'\n");
}

int main() {
  char expression[MAX_SIZE] = {0};
  int numbers[MAX_SIZE] = {0};
  char operators[MAX_SIZE] = {0};
  int priorities[MAX_SIZE] = {0};
  int op_count = 0;

  // Reads expression from standard input.
  scanf("%1000s", expression);
  printf("input expression is: '%s'\n", expression);

  // Parses expression into information arrays.
  op_count = ParseExpression(expression, numbers, operators, priorities);

  // Evaluates operators one by one.
  PrintExpression(numbers, operators, priorities, op_count);
  for (; op_count > 0; ) {
    op_count = EvaluateHighestPriorityOperator(numbers, operators, priorities, op_count);
    PrintExpression(numbers, operators, priorities, op_count);
  }
  if (op_count == 0) {
    printf("result of expression '%s' is %d\n", expression, numbers[0]);
  } else {
    printf("failed to evaluate expression '%s'\n", expression);
  }
}