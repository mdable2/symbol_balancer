/**
 *  Description: This program is an introduction to the stack structure.
 *      Project: Balance symbols
 *        Class: CS 211
 *         Name: Mark Dabler
 *       NET ID: mdable2
 *    Last edit: 2/15/18
 * Confirmation: 1e5154e5-fd7c-4156-8eb9-d5ea35653e88
 **/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Typical C boolean set-up
#define TRUE 1
#define FALSE 0

int DebugMode;  // As implemented in CS 211 notes, debug will allow the user to see info on each expression
                // Can be global as this is a development program


//----------------------------------------------------
// Struct data type for a stack
typedef struct StackStruct
{
 char*   darr;  /* pointer to dynamic array  */
 int     size;  /* amount of space allocated */
 int     inUse; /* top of stack indicator - counts how many values are on the stack   */
} Stack;


//----------------------------------------------------
// Initilize stack structure size
void init (Stack *s)
{
 s->size = 2;
 s->darr = (char*) malloc ( sizeof (char) * s->size );
 s->inUse  = 0;
}


//----------------------------------------------------
// Standard message to be printed after analyzing text
void printMessage(int x, int counter, char match)
{
  int i;
  switch(x) {
    // Balanced
    case 1: {
      printf("Expression is balanced\n");
      break;
    }
    // Stack is empty means a closing symbol should not be encountered yet
    case 2: {
      for (i = 0; i < counter; i++) {
        printf(" ");
      }
      printf("^   missing %c\n", match);
      break;
    }
    // The symbol on the top of stack is not the corresponding symbol
    case 3: {
      for (i = 0; i < counter; i++) {
        printf(" ");
      }
      printf("^   expecting %c\n", match);
      break;
    }
    // Else stack is NOT empty which means not balanced and missing closing symbol
    case 4: {
      for (i = 0; i < counter; i++) {
        printf(" ");
      }
      printf("^ missing %c\n", match);
      break;
    }
    default: {
      printf("Error in printMessage!\n");
    }
  }
}


//----------------------------------------------------
// Check if char symbol is opening
int isOpeningSymbol(char ch)
{
  if (ch == '{' || ch == '(' || ch == '[' || ch == '<')
    return TRUE;
  else
    return FALSE;
}


//----------------------------------------------------
// Return symbol that matches parameter char
char getMatchingSymbol(char ch)
{
  if (ch == '(') return ')';
  if (ch == ')') return '(';
  if (ch == '{') return '}';
  if (ch == '}') return '{';
  if (ch == '[') return ']';
  if (ch == ']') return '[';
  if (ch == '<') return '>';
  if (ch == '>') return '<';

  return '?'; // TODO: this needs to be changed!
}


//----------------------------------------------------
// Check if parameter symbols match and return TRUE or FALSE; code adopted from CS 211 notes
int doSymbolsMatch(char ch1, char ch2)
{
  char symbols[8] = {'(', '{', '[', '<', ')', '}', ']', '>'}; // the 8 symbols this program uses
  int i;

  // Loop through each possible
  for (i = 0; i < 8; i++) {
    int j = (i + 4) % 8;
    if (ch1 == symbols[i] && ch2 == symbols[j])
      return TRUE;
  }
  return FALSE;
}


//----------------------------------------------------
// Check if char symbol is closing
int isClosingSymbol(char ch)
{
  if (ch == '}' || ch == ')' || ch == ']' || ch == '>')
    return TRUE;
  else
    return FALSE;
}


//----------------------------------------------------
// Dynamically allocate memory to grow the stack by 2
void grow (Stack *s)
{
  char *temp = (char*) malloc((s->size + 2) * (sizeof(char)));
  int i;
  for (i = 0; i < s->size; i++) {
    temp[i] = s->darr[i];
  }
  if (DebugMode == TRUE) {
    printf("[Grow Called] Old Size: %d   New Size: %d   Values Copied: %d\n", s->size, s->size + 2, i);
  }
  free(s->darr);
  s->darr = temp;
  s->size = s->size + 2;
}


//----------------------------------------------------
// Push value onto stack
void push (Stack *s, char val)
{
  // Check if enough space currently on stack and grow if needed
  if (s->inUse == s->size - 1) {
    grow(s);
  }

  if (DebugMode == TRUE) {
    printf("[Push Called] Character Pushed: %c\n", val);
  }
  // Add val onto stack
  s->darr[s->inUse] = val;
  s->inUse = s->inUse + 1;
}


//----------------------------------------------------
// Check if stack is empty
int isEmpty (Stack *s)
{
  if (s->inUse == 0)
    return TRUE;
  else
    return FALSE;
}


//----------------------------------------------------
// Returns the top element of the stack
int top (Stack *s)
{
  // Check if stack is empty
  if (isEmpty(s) == 1) {
    return -1;
  }

  // Else return the pointer to the top stack element
  return (s->darr[s->inUse-1]);
}


//----------------------------------------------------
// Pop (delete) top of stack
void pop (Stack *s)
{
  // Check if stack is empty
  if (isEmpty(s) == 1) {
    printf("ERROR void pop(): Stack is empty. Nothing to pop.\n");
    return;
  }

  if (DebugMode == TRUE) {
    printf("[Pop Called] Character Popped: %c\n", getMatchingSymbol(s->darr[s->inUse - 1]));
  }

  // Else delete the top element
  s->inUse = s->inUse - 1;
}


//----------------------------------------------------
// Make the stack empty, but the stack still exists
void reset (Stack *s)
{
  // Else delete each element on the stack to make an empty stack
  while (s->inUse > 0) {
    pop(s);
  }
  //free(s->darr);
  s->size = 2;
}


//----------------------------------------------------
// Driver function of program
int main (int argc, char** argv)
{
  char input[300];    // User input that can take 300 characters
  int errorFound = FALSE;

  // Check if debug mode argument was entered in command line
  int i;
  for (i = 0 ; i < argc ; i++) {
    if (strcmp (argv[i], "-d") == 0 ) {
      DebugMode = TRUE;
      printf("\n\n****DEBUG MODE ENABLED****\n\n");
    }
    else {
      DebugMode = FALSE;
    }
  }

  // Initilize the stack
  Stack st1;
  init(&st1);

  printf("Please enter an expression and this program will check if it is balanced (q/Q to quit):\n");
  fgets (input, 300, stdin);

  // Main while loop for program to keep prompting user to input string
  while ((input[0] != 'q' || input[0] != 'Q') && input[1] != '\n') {

    // Print out user input that will be analyzed
    printf("\nYour Input:\n");
    printf("%s", input);

    int counter = 0;
    while (input[counter] != '\0') {
      // Take out all new lines and treat as end of line character
      if ('\n' == input[counter]) {
        input[counter] = '\0';
        break;
      }

      // Check if input is an opening symbol this program checks
      if (isOpeningSymbol(input[counter]) == TRUE) {
        push(&st1, input[counter]);
      }

      // Check if input is a closing symbol this program checks
      if (isClosingSymbol(input[counter]) == TRUE) {
        // Correct symbol
        if (doSymbolsMatch(top(&st1), input[counter]) == 1) {
          pop(&st1);
        }
        // Stack is empty means a closing symbol should not be encountered yet
        else if (isEmpty(&st1) == TRUE) {
          printMessage(2, counter, getMatchingSymbol(input[counter]));
          errorFound = TRUE;
          break;
        }
        // The symbol on the top of stack is not the corresponding symbol
        else {
          printMessage(3, counter, getMatchingSymbol(input[counter]));
          errorFound = TRUE;
          break;
        }
      }
      counter++;
    }

    if (errorFound == FALSE) {
      // Check if expression is balanced
      if (isEmpty(&st1) == 1) {
        printMessage(1, counter, 'x');
        errorFound = FALSE;
      }
      // Else stack is NOT empty which means not balanced and missing closing symbol
      else {
        printMessage(4, counter, getMatchingSymbol(top(&st1)));
        errorFound = FALSE;
      }
    }

    // Reset stack for next iteration
    reset(&st1);

    printf("\nEnter your next expression:\n");
    fgets (input, 300, stdin);
  }

  printf("Exiting expression balancer...\n");
}
