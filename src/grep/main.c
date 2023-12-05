#include "utils.h"

int main(int argc, char **argv) {
  if (argc >= 3) {
    option flags = {0};
    char f[10];  // промежуточное хранение флагов
    char patterns[10000];  // хранение паттерна
    int files[20];         // индексы файлов в аргв
    int flag_pars = pars(argc, argv, f, patterns, files);
    int flag_set = set_flags(f, &flags);

    if (flag_pars && flag_set) {
      int count_files = 0;  // сколько файлов считано
      while (files[count_files] != -1) {
        count_files++;
      }
      regex_t regex;
      int regresult = compilation(&regex, patterns, flags);

      if (!regresult) {
        for (int i = 0; i < count_files; i++) {
          FILE *file = fopen(argv[files[i]], "r");
          if (file) {
            output(regex, flags, file, argv[files[i]], count_files);
            fclose(file);
          } else if (flags.s == 0)
            fprintf(stderr, "Can't open file: %s\n", argv[files[i]]);
        }
        regfree(&regex);
      } else {
        fprintf(stderr, "Error with compile regex\n");
      }
    }
  } else
    fprintf(stderr, "Too few arguments\n");
  return 0;
}