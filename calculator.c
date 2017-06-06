#include <stdio.h>

#define MAX_SIZE 1000

int ParseNumbersAndOperators(char* expression, int* numbers, char* operators) {
  int op_count = 0;
  int i = 0;
  char ch = 0;
  // NOTE: '\0' is the last charactor of string, when reaching it,
  // we know we have done reading all charactors in string.
  for (op_count = 0, i = 0; i < MAX_SIZE && expression[i] != '\0'; ++i) {
    ch = expression[i];
    // printf("current character: %c\n", ch);
    switch (ch) {
      // Handles operators.
      case '+':
      case '-':
      case '*':
      case '/': {
        operators[op_count] = ch;
        printf("operator is: '%c'\n", operators[op_count]);
        ++op_count;
        break;
      }

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
      case '9': {
        numbers[op_count] = numbers[op_count] * 10 + (ch - '0');
        printf("number is: %d\n", numbers[op_count]);
        break;
      }
      default:
        printf("unsupported character expression[%d]: '%c'\n", i, ch);
        return -1;
    }
  }
  return op_count;
}

// NOTE: count of numbers = count of operators (op_count) + 1
int PriorityEvaluateOneOperator(int* numbers, char* operators, int op_count) {
  int op_chosen = 0;
  int i = 0;
  int result = 0, num_left = 0, num_right = 0;
  if (0 == op_count) {
    return 0;
  }

  // Chooses an operator to evaluate.
  for (op_chosen = -1, i = 0; i < op_count; ++i) {
    switch (operators[i]) {
      case '*':
      case '/':
        if (-1 == op_chosen || '+' == operators[op_chosen] || '-' == operators[op_chosen]) {
          // * / has higher priority than + -.
          op_chosen = i;
        }
        break;
      case '+':
      case '-':
        if (-1 == op_chosen) {
          op_chosen = i;
        }
        break;
      case 0:  // Skips placeholder operator.
        break;
      default:
        printf("unsupported operator %c\n", operators[i]);
        return -1;
    }
  }

  // Evaluates chosen operator.
  num_left = numbers[op_chosen];
  num_right = numbers[op_chosen+1];
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
  }
  numbers[op_count] = 0;
  operators[op_count-1] = 0;
  return op_count - 1;
}

void PrintExpression(int* numbers, char* operators, int op_count) {
  int i = 0;
  printf("expression is: '%d", numbers[0]);
  for (i = 0; i < op_count; ++i) {
    printf("%c%d", operators[i], numbers[i+1]);
  }
  printf("'\n");
}

int main() {
  char expression[MAX_SIZE] = {0};
  int numbers[MAX_SIZE] = {0};
  char operators[MAX_SIZE] = {0};
  int op_count = 0;

  // Reads expression from standard input.
  scanf("%1000s", expression);
  printf("input expression is: '%s'\n", expression);

  // Parses numbers and operators.
  op_count = ParseNumbersAndOperators(expression, numbers, operators);

  // Evaluates operators one by one.
  PrintExpression(numbers, operators, op_count);
  for (; op_count > 0; ) {
    op_count = PriorityEvaluateOneOperator(numbers, operators, op_count);
    PrintExpression(numbers, operators, op_count);
  }
  if (op_count == 0) {
    printf("result of expression '%s' is %d\n", expression, numbers[0]);
  } else {
    printf("failed to evaluate expression '%s'\n", expression);
  }
}