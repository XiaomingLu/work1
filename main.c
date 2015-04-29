#include "main.h"

int main(int argc, char const *argv[])
{
    FILE *fp_in = fopen("input.txt", "r");
    if (fp_in == NULL) {
        printf("Error: fail opening \"input.txt\".\n");
        return FILE_ERROR;
    }

    LIST_HEAD(word_list);

    word_t *word = NULL;
    int ret;

    while ((ret = get_word(fp_in, &word)) == SUCCESS) {
        add_word(&word_list, word);
    }

    if (ret != SUCCESS_EOF)
        goto quit;

    if (word)
        add_word(&word_list, word);

    FILE *fp_out = fopen("output.txt", "w");
    if (fp_out == NULL) {
        printf("Error: fail opening \"output.txt\".\n");
        ret = FILE_ERROR;
        goto quit;
    }

    if (print_word_list(fp_out, &word_list) != 0) {
        ret = FILE_ERROR;
        goto quit;
    }
    ret = 0;

quit:
    switch (ret) {
        case MALLOC_ERROR:
        printf("Error: no memory.\n");
        case OVERFLOW_ERROR:
        printf("Error: input word is too long, no more than %d\n", MAX_WORD_STR_SIZE);
        case FILE_ERROR:
        printf("Error: file error\n");
        case UNKNOW_ERROR:
        printf("Error: unkonwn.\n");
        default:
        ;
    }
    clear_word_list(&word_list);
    return ret;
}

int get_word(FILE *fp, word_t **p_word)
{
    int error_num;
    int word_leng = 0;
    char c = '\0';
    *p_word = NULL;
    word_t *word = NULL;
    while ((c = fgetc(fp)) != EOF) {
        if ((c = letter_to_lowercase(c)) == 0) {
            if (word_leng == 0) {
                continue;
            }
            else {
                word->str[word_leng] = '\0';
                *p_word = word;
                return SUCCESS;
            }
        }
        if (word == NULL) {
            word = (word_t *)malloc(sizeof(word_t));
            if (word == NULL)
                return MALLOC_ERROR;
            word->str[0] = '\0';
            // int i;
            // for (i = 0; i < MAX_WORD_VALUE_SIZE; ++i)
            //     word->value[i] = 0;
        }
        word->str[word_leng] = c;
        // word->value[word_leng/LETTER_PER_INT] += (c - 'a' + 1) <<
            // ((LETTER_PER_INT - word_leng % LETTER_PER_INT - 1) * LETTER_VALUE_BIT);
        if (word_leng++ >= MAX_WORD_STR_SIZE - 1) {
            error_num = OVERFLOW_ERROR;
            goto error_handle;
        }
    }
    if (feof(fp)) {
        return SUCCESS_EOF;
    }
    else if (ferror(fp)) {
        error_num = FILE_ERROR;
        goto error_handle;
    }
    else {
        error_num = UNKNOW_ERROR;
        goto error_handle;
    }
error_handle:
    if (word)
        free(word);
    return error_num;
}

char letter_to_lowercase(char c)
{
    if (c >= 'a' && c <= 'z')
        return c;
    else if (c >= 'A' && c <= 'Z')
        return c-'A'+'a';
    else
        return 0;
}

// int word_compare(const word_t *wa, const word_t *wb)
// {
//     int i;
//     for (i = 0; i < MAX_WORD_VALUE_SIZE; ++i)
//     {
//         if (wa->value[i] > wb->value[i])
//             return 1;
//         else if (wa->value[i] < wb->value[i])
//             return -1;
//         else if (wa->value[i] == 0)
//             return 0;
//         else
//             continue;
//     }
//     return 0;
// }

int add_word(struct list_head *wlist, word_t *word)
{
    wlist_t *pos, *newpos;
    struct list_head *p;
    if (list_empty(wlist)) {
        newpos = (wlist_t *)malloc(sizeof(wlist_t));
        if (newpos == NULL)
            return -1;
        newpos->word = word;
        newpos->count = 1;
        list_add(&newpos->list, wlist);
        return 0;
    } else {
        list_for_each (p, wlist) {
            pos = list_entry(p, wlist_t, list);
            // int cmp_res = word_compare(pos->word, word);
            int cmp_res = strncmp(pos->word->str, word->str, MAX_WORD_STR_SIZE);
            if (cmp_res == 0) {
                pos->count++;
                free(word);
                return 0;
            } else if (cmp_res > 0) {
                newpos = (wlist_t *)malloc(sizeof(wlist_t));
                if (newpos == NULL)
                    return -1;
                newpos->word = word;
                newpos->count = 1;
                list_add_tail(&newpos->list, p);
                return 0;
            }
        }
        newpos = (wlist_t *)malloc(sizeof(wlist_t));
        if (newpos == NULL)
            return -1;
        newpos->word = word;
        newpos->count = 1;
        list_add_tail(&newpos->list, wlist);
        return 0;
    }
}

int print_word_list(FILE *fp, struct list_head *wlist)
{
    wlist_t *pos;
    int ret;
    struct list_head *p;
    list_for_each (p, wlist) {
        pos = list_entry(p, wlist_t, list);
        ret = fprintf(fp, "%s,%d\n", pos->word->str, pos->count);
        if (!ret)
            return ret;
    }
    return 0;
}

void clear_word_list(struct list_head *wlist)
{
    wlist_t *pos;
    struct list_head *p, *n;
    list_for_each_safe (p, n, wlist) {
        pos = list_entry(p, wlist_t, list);
        list_del(&pos->list);
        free(pos->word);
        free(pos);
    }
}
