#include "utils.h"

int compilation(regex_t *regex, char *patterns, option flags) {
  int regresult;
  if (flags.i)  // компиляция регулярки с учетом регистра или без него (-i)
    regresult = regcomp(regex, patterns, (REG_ICASE | REG_EXTENDED));
  else
    regresult = regcomp(regex, patterns, REG_EXTENDED);
  return regresult;
}

int use_flag_c(regex_t *regex, char *buffer, option flags) {
  int result = 0;
  int flag_match = 0;
  if (flags.v) flag_match = REG_NOMATCH;
  if (regexec(regex, buffer, 0, NULL, 0) == flag_match) result = 1;
  return result;
}

void use_flag_l(char *filename, int *flag) {
  printf("%s\n", filename);
  *flag = 0;
}

void use_flag_o(regex_t *regex, char *buffer, option flags, int *f,
                int str_count) {
  *f = 0;
  regmatch_t pmatch;
  int start = 0, flag_match = 0;
  if (flags.v) flag_match = REG_NOMATCH;
  while (regexec(regex, buffer + start, 1, &pmatch, 0) == flag_match) {
    if (start != 0 && flags.n) {
      printf("%d:", str_count);
    }
    for (int i = pmatch.rm_so + start; i < pmatch.rm_eo + start; i++) {
      printf("%c", buffer[i]);
    }

    printf("\n");
    start += pmatch.rm_eo;
  }
}

void flag_nomatch_c(char *filename, option flags, int count) {
  if (flags.h == 0 && count > 1)
    printf("%s:%d\n", filename, 0);
  else
    printf("%d\n", 0);
}

void print_match_count(char *filename, option flags, int count,
                       int match_count) {
  if (flags.h == 0 && count > 1)
    printf("%s:%d\n", filename, match_count);
  else
    printf("%d\n", match_count);
}

int find_e(int argc, char **argv) {
  int flag_e = 0;
  for (int i = 1; i < argc; i++)
    if ((strcmp(argv[i], "-e") == 0) ||
        (argv[i][0] == '-' && strstr(argv[i], "e") != NULL))
      flag_e = 1;
  return flag_e;
}

void write_pattern(const char *buf, int start, char *patterns,
                   int *count_temp) {
  for (int i = start; buf[i] != 0; i++) {
    patterns[*count_temp] = buf[i];
    (*count_temp) += 1;
  }
  patterns[*count_temp] = '|';
  (*count_temp) += 1;
}

void use_flag_f(char *buf, int start, char *patterns, int *count_temp,
                int *flag_pars) {
  FILE *file = fopen(buf + start, "r");
  if (file) {
    char c = fgetc(file);
    char prev = '\n';
    while (!feof(file)) {
      if (c == '\n' && prev != '\n') {
        patterns[*count_temp] = '|';
        (*count_temp) += 1;
      } else if (c != '\n') {
        patterns[*count_temp] = c;
        (*count_temp) += 1;
      }
      prev = c;
      c = fgetc(file);
    }
    patterns[*count_temp] = c;
    (*count_temp) += 1;
    fclose(file);
  } else {
    *flag_pars = 0;
    fprintf(stderr, "Error with opening file_pattern: %s", buf + start);
  }
}

int set_flags(char *f, option *flags) {
  int flag = 1;
  for (int i = 0; f[i] != 0; i++) {
    switch (f[i]) {
      case 'i':
        flags->i = 1;
        break;
      case 'l':
        flags->l = 1;
        flags->o = 0;
        flags->n = 0;
        flags->c = 0;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        if (flags->l == 0) {
          flags->c = 1;
          flags->n = 0;
          flags->o = 0;
        }
        break;

      case 'n':
        if ((flags->l == 0) && (flags->c == 0)) flags->n = 1;
        break;
      case 'o':
        if ((flags->l == 0) && (flags->c == 0)) flags->o = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      default:
        fprintf(stderr, "Unknown option: %c\n", f[i]);
        flag = 0;
    }
  }
  return flag;
}

void output(regex_t regex, option flags, FILE *file, char *filename,
            int count) {
  char buffer[10000];
  int flag = 1, str_count = 0, match_count = 0;
  int at_least_one_match = 0;
  while (!feof(file) && flag) {
    if (!(fgets(buffer, 10000, file))) {
      flag = 0;
      continue;
    }
    if (flags.v && flags.o && !flags.l &&
        !flags.c) {  // сочетание -o и -v всегда пусто
      flag = 0;
      continue;
    }
    if (flags.c && flags.l == 0) {
      match_count += use_flag_c(&regex, buffer, flags);
      continue;
    }
    int f = 1;  // флаг-разрешение на печать строки
    str_count++;  // счетчик строк в файле для -n
    int regresult = regexec(&regex, buffer, 0, NULL, 0);  // 0 - нашел
    if (!flags.v)                                         // инверсия
      regresult = !regresult;
    if (regresult) {
      at_least_one_match = 1;  // хотя бы что-то нашлось
      if (flags.l) {
        use_flag_l(filename, &flag);
        continue;
      }
      if ((flags.h == 0) && (count > 1)) printf("%s:", filename);
      if (flags.n) printf("%d:", str_count);
      if (flags.o) use_flag_o(&regex, buffer, flags, &f, str_count);
      if (f) printf("%s", buffer);
      if (strstr(buffer, "\n") == NULL && f)
        printf("\n");  // если в конце файла нет \n
    }
  }
  // в файле НИЧЕГО не нашлось
  if (at_least_one_match == 0 && flags.c && flags.l)
    flag_nomatch_c(filename, flags, count);
  if (flags.c && flags.l == 0)  // вывод от -с
    print_match_count(filename, flags, count, match_count);
}

int pars(int argc, char **argv, char *f, char *patterns, int *files) {
  int count_f = 0, count_temp = 0, count_files = 0, flag_pars = 1;
  int flag_e = find_e(argc, argv);  // проверка на наличие флага -е
  for (int i = 1; i < argc; i++) {
    // это шаблон единственный, без -е, возможно до него есть флаги
    if ((argv[i][0] != '-') && (count_temp == 0) && (flag_e == 0)) {
      write_pattern(argv[i], 0, patterns, &count_temp);
      continue;
    }
    if (flag_e) {  // если есть -е, то все шаблоны обязаны быть с -е
      // это флаг шаблона, все что после флага - в шаблоны
      if (strcmp(argv[i], "-e") == 0) {
        if (i == argc - 1) {  // если -е последний аргв - ошибка
          flag_pars = 0;
          fprintf(stderr, "Pattern after -e required\n");
        } else {
          i++;
          write_pattern(argv[i], 0, patterns, &count_temp);
        }
        continue;
        // флаг -е и шаблон слепили в одно
      } else if ((argv[i][0] == '-') && (argv[i][1] == 'e')) {
        write_pattern(argv[i], 2, patterns, &count_temp);
        continue;
      }
    }
    // это просто флаг или несколько флагов
    if (argv[i][0] == '-') {
      int noend_argv = 1;
      for (int j = 1; argv[i][j] != 0 && noend_argv; j++) {
        if (argv[i][j] != 'e' && argv[i][j] != 'f')
          f[count_f++] = argv[i][j];
        else if (argv[i][j] == 'e') {
          noend_argv = 0;
          if (argv[i][j + 1] == 0) {  // конец аргв с -е
            if (i == argc - 1) {      // нет шаблона после -е
              fprintf(stderr, "Pattern after -e required\n");
              flag_pars = 0;
            } else {  // нет ошибки - пишем шаблон
              i++;
              write_pattern(argv[i], 0, patterns, &count_temp);
            }
          } else {  // комбинация с -е и шаблон слеплены
            j++;
            write_pattern(argv[i], j, patterns, &count_temp);
          }
        } else if (argv[i][j] == 'f') {
          noend_argv = 0;
          if (argv[i][j + 1] == 0) {  // это конец аргв с -f
            if (i == argc - 1) {
              fprintf(stderr, "File after -f required\n");
              flag_pars = 0;
            } else {
              i++;
              use_flag_f(argv[i], 0, patterns, &count_temp, &flag_pars);
            }
          } else {
            j++;
            use_flag_f(argv[i], j, patterns, &count_temp, &flag_pars);
          }
        }
      }
    } else  // это имя файла
      files[count_files++] = i;
  }
  f[count_f] = 0;
  patterns[--count_temp] = 0;
  files[count_files] = -1;
  return flag_pars;
}