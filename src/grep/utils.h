#ifndef GREP_FUNC_H
#define GREP_FUNC_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct option {
  int e, b, n, s, t, v, i, c, l, o, h;
} option;

int pars(int argc, char **argv, char *f, char *patterns, int *files);
int set_flags(char *f, option *flags);
void output(regex_t regex, option flags, FILE *file, char *filename, int count);
int compilation(regex_t *regex, char *patterns, option flags);
int find_e(int argc, char **argv);
void add_pattern(const char *buf, int start, char *patterns, int *count_temp);
int compilation(regex_t *regex, char *patterns, option flags);
int use_flag_c(regex_t *regex, char *buffer, option flags);
void use_flag_l(char *filename, int *flag);
void use_flag_o(regex_t *regex, char *buffer, option flags, int *f,
                int str_count);
void flag_nomatch_c(char *filename, option flags, int count);
void print_match_count(char *filename, option flags, int count,
                       int number_match);
void use_flag_f(char *buf, int start, char *patterns, int *count_temp,
                int *flag_parser);

#endif