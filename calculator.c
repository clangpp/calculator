#include <stdio.h>

#define MAX_SIZE 1000

int Calculate(int num_left, char op, int num_right) {
  switch (op) {
    case '+':
      return num_left + num_right;
    case '-':
      return num_left - num_right;
    case '*':
      return num_left * num_right;
    case '/':
      return num_left / num_right;
    default:
      printf("unsupported operation %d %c %d\n", num_left, op, num_right);
      return 0;
  }
}

int main() {
  char expression[MAX_SIZE] = {0};
  int i = 0;
  char ch = 0;
  int num = 0, result = 0;
  char op = 0;
  
  // Reads expression from standard input.
  scanf("%1000s", expression);
  printf("input expression is: %s\n", expression);
  
  // Parses numbers and operators.
  // NOTE: '\0' is the last charactor of string, when reaching it,
  // we know we have done reading all charactors in string.
  for (i = 0; i < MAX_SIZE && expression[i] != '\0'; ++i) {
    ch = expression[i];
    printf("current character: %c\n", ch);
    switch (ch) {
      // Handles operators.
      case '+':
      case '-':
      case '*':
      case '/': {
        if (op == 0) {
          result = num;  // First number in expression.
        } else {
          result = Calculate(result, op, num);  // Updates result.
        }
        num = 0;  // Clears current number to be parsed from expression.
        op = ch;  // Updates operator.
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
        num = num * 10 + (ch - '0');
        break;
      }
      default:
        printf("unsupported character expression[%d]: '%c'\n", i, ch);
        return 0;
    }
  }
  if (op != 0) {
    // Don't forget last number.
    result = Calculate(result, op, num);
  }
  
  printf("result is: %d\n", result);
}