// gcc -Wall -Werror -Wpedantic fun.c && cp fun.c{,c} && g++ -Wall -Werror -Wpedantic fun.cc && ./a.out
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#define MACHINE_INIT_CAPACITY 100

enum TokenType {
  TOKEN_TYPE_UNKNOWN,
  TOKEN_TYPE_STR,
  TOKEN_TYPE_NUM,
  TOKEN_TYPE_NAME
};
typedef enum TokenType TokenType;

typedef struct Token Token;
struct Token {
  TokenType type;
  union {
    char *string;
    double number;
  } value;
};

enum ValueType {
  VALUE_TYPE_UNKNOWN,
  VALUE_TYPE_STR,
  VALUE_TYPE_NUM,

  /* pseudo values */
  VALUE_TYPE_LIST_START
};
typedef enum ValueType ValueType;

typedef struct Value Value;
struct Value {
  ValueType type;
  union {
    char *string;
    double number;
  } value;
};

typedef struct Machine Machine;
struct Machine {
  Token *tokens;
  int tokensSize; /* number of tokens actually represented by 'tokens' */
  int tokensCapacity; /* number of tokens 'tokens' buffer can hold */
  int instructionPointer;

  Value *stack;
  int stackSize;
  int stackCapacity;
};

void initMachine(Machine *machine) {
  machine->tokens = (Token*) malloc(sizeof(Token)*MACHINE_INIT_CAPACITY);
  machine->tokensSize = 0;
  machine->tokensCapacity = MACHINE_INIT_CAPACITY;
  machine->instructionPointer = 0;
}

int startsWith(const char *string, const char *prefix) {
  return strncmp(prefix, string, strlen(prefix)) == 0;
}

void skipSpaces(char **stringPointer) {
  while (isspace(**stringPointer)) {
    (*stringPointer)++;
  }
}

int extractString(char **stringPointer, Token *token) {
  if (startsWith(*stringPointer, "'") ||
      startsWith(*stringPointer, "r'") ||
      startsWith(*stringPointer, "\"") ||
      startsWith(*stringPointer, "r\"")) {
    int raw, stringLength;
    char quote[4], *afterFirstQuotes, *p;

    token->type = TOKEN_TYPE_STR;

    if (**stringPointer == 'r') {
      (*stringPointer)++;
      raw = 1;
    } else {
      raw = 0;
    }

    if (startsWith(*stringPointer, "'''") ||
        startsWith(*stringPointer, "\"\"\"")) {
      quote[0] = quote[1] = quote[2] = (*stringPointer)[0];
      quote[3] = '\0';
    } else {
      quote[0] = (*stringPointer)[0];
      quote[1] = '\0';
    }
    (*stringPointer) += strlen(quote);

    /* Here, we just run through the string to get the string size. */
    afterFirstQuotes = *stringPointer;
    stringLength = 0;
    while (!startsWith(*stringPointer, quote)) {
      if (!raw && **stringPointer == '\\')
        (*stringPointer) += 2;
      else
        (*stringPointer)++;
      stringLength++;
    }

    /* Now run it again, but copy over the string this time. */
    p = token->value.string = (char*) malloc(sizeof(char) * (stringLength+1));
    (*stringPointer) = afterFirstQuotes;
    while (!startsWith(*stringPointer, quote)) {
      if (!raw && **stringPointer == '\\') {
        (*stringPointer)++;
        switch (*((*stringPointer)++)) {
        case 'n':
          *p++ = '\n';
          break;
        case 't':
          *p++ = '\t';
          break;
        case '\\':
          *p++ = '\\';
          break;
        default:
          /* Error */
          return 0;
        }
      } else {
        *p++ = *((*stringPointer)++);
      }
    }
    *p = '\0';
    (*stringPointer) += strlen(quote);
    return 1;
  }
  return 0;
}

int extractNumber(char **stringPointer, Token *token) {
  char *start = *stringPointer;
  token->type = TOKEN_TYPE_NUM;
  token->value.number = strtod(start, stringPointer);
  return start < *stringPointer;
}

int extractName(char **stringPointer, Token *token) {
  int length;
  char *start = *stringPointer;

  token->type = TOKEN_TYPE_NAME;
  length = 0;
  while (**stringPointer != '\0' && !isspace(**stringPointer)) {
    length++;
    (*stringPointer)++;
  }

  if (!length)
    return 0;

  token->value.string = (char*) malloc(sizeof(char) * (length+1));
  token->value.string[length] = '\0';
  strncpy(token->value.string, start, length);

  return 1;
}

int loadTokens(Machine *machine, char *string) {
  skipSpaces(&string);
  while (*string != '\0') {
    Token *token;

    if (machine->tokensSize >= machine->tokensCapacity) {
      machine->tokensCapacity = 2*machine->tokensCapacity+10;
      machine->tokens =
          (Token*) realloc(machine->tokens, sizeof(Token)*machine->tokensCapacity);
    }

    token = machine->tokens+machine->tokensSize++;

    if (!(extractString(&string, token) ||
          extractNumber(&string, token) ||
          extractName(&string, token))) {
      /* Error */
      return 0;
    }

    skipSpaces(&string);
  }
  return 1;
}

int processName(Machine *machine, Token *token) {
  return 0;
}

int stepMachine(Machine *machine) {
  Token *token;

  if (machine->instructionPointer >= machine->tokensSize)
    return 0;

  token = machine->tokens + (machine->instructionPointer++);

  switch (token->type) {
  case TOKEN_TYPE_STR:
    break;
  case TOKEN_TYPE_NUM:
    break;
  case TOKEN_TYPE_NAME:
    break;
  default:
    /* Error */
    return 0;
  }

  return 1;
}

void testExtractString() {
  Token token;
  char text[] = "'hello world' '\\n' r'\\n'", *p;

  p = text;
  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractString(&p, &token));
  assert(token.type == TOKEN_TYPE_STR);
  assert(strcmp(token.value.string, "hello world") == 0);

  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractString(&p, &token));
  assert(token.type == TOKEN_TYPE_STR);
  assert(strcmp(token.value.string, "\n") == 0);

  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractString(&p, &token));
  assert(token.type == TOKEN_TYPE_STR);
  assert(strcmp(token.value.string, "\\n") == 0);
}

void testExtractNumber() {
  Token token;
  char text[] = "123 54", *p = text;

  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(!extractString(&p, &token));
  assert(extractNumber(&p, &token));
  assert(token.type == TOKEN_TYPE_NUM);
  assert(token.value.number == 123);

  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractNumber(&p, &token));
  assert(token.type == TOKEN_TYPE_NUM);
  assert(token.value.number == 54);
}

void testExtractName() {
  Token token;
  char text[] = "hello world", *p;

  p = text;
  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractName(&p, &token));
  assert(token.type == TOKEN_TYPE_NAME);
  assert(strcmp(token.value.string, "hello") == 0);

  token.type = TOKEN_TYPE_UNKNOWN;
  skipSpaces(&p);
  assert(extractName(&p, &token));
  assert(token.type == TOKEN_TYPE_NAME);
  assert(strcmp(token.value.string, "world") == 0);
}

void testStepMachine() {
  Machine machine;
  char text[] = "55 55 +";

  initMachine(&machine);
  loadTokens(&machine, text);
}

void testAll() {
  testExtractString();
  testExtractNumber();
  testExtractName();
  testStepMachine();
  fprintf(stderr, "Tests successful!\n");
}

int main(int argc, char **argv) {
  testAll();
}
