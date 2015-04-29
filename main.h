#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define LETTER_VALUE_BIT        5
#define LETTER_PER_INT          (32/LETTER_VALUE_BIT)

#define MAX_WORD_STR_SIZE       80
#define MAX_WORD_VALUE_SIZE     ((int)(MAX_WORD_STR_SIZE+1)/LETTER_PER_INT-1)

#define SUCCESS        0
#define SUCCESS_EOF    1
#define MALLOC_ERROR   -1
#define OVERFLOW_ERROR -2
#define FILE_ERROR     -3
#define UNKNOW_ERROR   -4

typedef struct {
    char str[MAX_WORD_STR_SIZE];
    // unsigned int value[MAX_WORD_VALUE_SIZE];
}word_t;

typedef struct {
    word_t *word;
    unsigned int count;
    struct list_head list;
}wlist_t;

int main(int argc, char const *argv[]);

int get_word(FILE *fp, word_t **p_word);

int add_word(struct list_head *wlist, word_t *word);
int print_word_list(FILE *fp, struct list_head *wlist);
void clear_word_list(struct list_head *wlist);

char letter_to_lowercase(char c);
// int word_compare(const word_t *wa, const word_t *wb);


#endif
