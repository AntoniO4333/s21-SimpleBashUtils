#include "utils.h"

int main(int argc, char** argv) {
  struct Flags_cat fl = Take_cat_flags(argc, argv, fl);
  int FileInd = 1;
  while (FileInd < argc) {
    if (strlen(argv[FileInd]) > 0 && argv[FileInd][0] != '-')
      Print_with_flags(argv, &FileInd, fl);
    FileInd++;
  }
  return 0;
}
