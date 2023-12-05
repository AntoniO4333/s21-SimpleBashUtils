#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Flags_cat {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
};

struct Flags_cat Take_cat_flags(int argc, char** argv, struct Flags_cat fl);
bool IsPathRight(char* path);
bool Print_with_flags(char** argv, int* FileInd, struct Flags_cat fl);
void PrintStr(char* mas, char prev_symb, int* strnum, struct Flags_cat fl,
              size_t len);