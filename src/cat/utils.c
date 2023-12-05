#include "utils.h"

struct Flags_cat Take_cat_flags(int argc, char** argv, struct Flags_cat fl) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (argv[i][1] == '-') {
        if (strcmp(argv[i], "--number-nonblank") == 0) {
          fl.b = 1;
        } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
          fl.s = 1;
        } else if (strcmp(argv[i], "--number") == 0) {
          fl.n = 1;
        }
      } else {
        for (size_t j = 1; j < strlen(argv[i]); j++) {
          if (argv[i][j] == 'b') {
            fl.b = 1;
          } else if (argv[i][j] == 'e') {
            fl.e = 1;
            fl.v = 1;

          } else if (argv[i][j] == 'E') {
            fl.e = 1;

          } else if (argv[i][j] == 'n') {
            fl.n = 1;

          } else if (argv[i][j] == 's') {
            fl.s = 1;

          } else if (argv[i][j] == 't') {
            fl.t = 1;
            fl.v = 1;

          } else if (argv[i][j] == 'T') {
            fl.t = 1;

          } else if (argv[i][j] == 'v') {
            fl.v = 1;
          }
        }
      }
    }
  }
  return fl;
}

bool IsPathRight(char* path) {  // ????
  FILE* file = fopen(path, "r");
  int f = 0;
  if (file) {
    fclose(file);
    f = 1;
  }
  return f;
}

bool Print_with_flags(char** argv, int* FileInd, struct Flags_cat fl) {
  if (*FileInd == 0) {
    fprintf(stderr, "stdin по заданию не обрабатывается\n");
  } else if (!IsPathRight(argv[*FileInd])) {
    fprintf(stderr, "cat: %s: No such file or directory\n", argv[*FileInd]);
  } else {
    if (fl.b == 1) {
      fl.n = 0;
    }

    FILE* file = fopen(argv[*FileInd], "r");
    size_t i = 0;
    int strnum = 1;  // для флагов -n, -b, нумерация строк
    char prev_symb = ' ';  // для флага -s, запоминает первый элемент строки
    size_t lenmas = 100;
    char* mas = (char*)malloc(lenmas * sizeof(char));

    while (!feof(file)) {
      do {
        fscanf(file, "%c", &mas[i]);  // чаровский массив
        if (feof(file)) break;
        i++;
        if (i == lenmas) {
          lenmas *= 2;
          mas = (char*)realloc(mas, lenmas * sizeof(char));
        }
      } while (mas[i - 1] != '\n');
      PrintStr(mas, prev_symb, &strnum, fl, i);
      prev_symb = mas[0];
      i = 0;
    }
    free(mas);
    fclose(file);
  }
  return 0;
}

void PrintStr(char* mas, char prev_symb, int* strnum, struct Flags_cat fl,
              size_t len) {
  if (len == 0) return;
  if (fl.s == 1 && prev_symb == '\n' && mas[0] == prev_symb) return;
  // flag -s
  if (fl.b == 1 && mas[0] != '\n' && mas[0]) {  // флаг -b
    printf("% 6d\t", *strnum);
    (*strnum)++;
  } else if (fl.n == 1) {  // флаг -n
    printf("% 6d\t", *strnum);
    (*strnum)++;
  }
  for (size_t i = 0; i < len; i++) {
    if (fl.v == 1) {  // flag -v
      if (mas[i] == 127) {
        printf("^%c", mas[i] - 64);
        continue;
      } else if ((0 <= mas[i]) && (31 >= mas[i]) && (mas[i] != '\n') &&
                 (mas[i] != '\t')) {
        printf("^%c", mas[i] + 64);
        continue;
      }
    }
    if (fl.e == 1) {  // flag -e
      if (mas[i] == '\n') {
        printf("$\n");
        continue;
      }
    }
    if (fl.t == 1) {  // flag -t
      if (mas[i] == '\t') {
        printf("^I");
        continue;
      }
    }
    printf("%c", mas[i]);
  }
}
