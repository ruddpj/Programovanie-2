#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define EMPTY_LINE "\n"
#define MAX_LINE 1000
#define MAX_WORD 20
#define WORD sizeof(char *)
#define NUM sizeof(int)

static int size_ws = 0;
static unsigned long longest = 0;

void rem_digit(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            memmove(&str[i], &str[i+1], MAX_LINE-i);
            i--;
        }
    }
}

void rem_punct(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (ispunct(str[i])){
            memmove(&str[i], &str[i+1], MAX_LINE-i);
            i--;
        }
    }
}

unsigned long longest_word(unsigned long n, char *str) {
    if (strlen(str) > n) return strlen(str);
    return 0;
}

char* input_str() {
    char* str;
    str = malloc(MAX_LINE+1);
    if (str == NULL) return NULL;
    str[0] = '\0';
    char buffer[MAX_LINE+1];
    char* tmp;

    fgets(buffer, MAX_LINE, stdin);
    for (int i = 2; strcmp(buffer, EMPTY_LINE) != 0; i++) {
        strcat(str, buffer);
        fgets(buffer, MAX_LINE, stdin);
        tmp = realloc(str, (MAX_LINE+1)*i);
        if (tmp == NULL) return NULL;
        str = tmp;
    }
    return str;
}

void free_2d_array(int n, char** array) {
    if (array == NULL) return;
    for (int i = 0; i < n; i++) {
        free(array[i]);
    }
}

char **word_fndr(char* str, const int* control) {
    char** word_str;
    word_str = malloc(WORD);
    word_str[0] = malloc(MAX_WORD+1);

    int i = 0, j = 0, wrd = 0, n = 0;
    char** tmp;
    while (*str != '\0') {
        if (isalpha(*str) == 0 && control[n] != 2) {
            if (wrd) {
                word_str[i][j] = '\0';
                i++;
                j = 0;
                wrd = 0;

                tmp = realloc(word_str, WORD*(i+1));
                if (tmp == NULL) return NULL;
                tmp[i] = malloc(MAX_WORD+1);
                word_str = tmp;

                unsigned long len = longest_word(longest, word_str[i-1]);
                if (len) longest = len;
            }
        } else {
            word_str[i][j] = *str;
            j++;
            wrd = 1;
        }
        str++;
        n++;
    }
    size_ws = i;
    return word_str;
}

int *row_fndr(char* str) {
    int *rows = (int *) calloc(size_ws, NUM);
    int i = 0, row = 1, wrd = 0;
    while (*str != '\0') {
        char tmp = *str;
        if (isalpha(tmp) == 0) {
            if (wrd) {
                rows[i] = row;
                i++;
                wrd = 0;
            }
        } else {
                wrd = 1;
        }
        if (*str == '\n') row++;
        str++;
    }
    return rows;
}

void letter_swp(int i, char* str, const char *arg, int n, int* control) {
    for (int k = 0; (isalpha(str[i+k]) || control[i+k] == 2) && (k < n); k++) {
        str[i+k] = arg[k];
        if (arg[k] == ' ') control[i+k] = 2;
    }
}

void swp_word(char *str, const char* arg, char **non_opt, int num_no, int sens, int* control) {
    unsigned long sz_str = strlen(str);
    if (sz_str == 0) return;
    sz_str--;
    int wrd = 0;

    for (int i = 0; i < sz_str; i++) {
        if (isalpha(str[i]) && wrd == 0) {
            wrd = 1;
            control[i] = 1;
        } else if (isalpha(str[i]) == 0) {
            wrd = 0;
        }
    }

    for (int i = 0; i < sz_str; i++) {
        if (control[i] == 1) {
            if (num_no == 0) {
                letter_swp(i, str, arg, (int) strlen(arg), control);
            } else {
                for (int j = 0; j < num_no; j++) {
                    int prefix_f = 0;
                    unsigned long n = strlen(non_opt[j]);

                    for (int k = 0; k < n; k++) {
                        if (sens) {
                            if (str[i+k] == non_opt[j][k]) {
                                prefix_f++;
                                control[i+k] = 2;
                            }
                        } else {
                            if (tolower(str[i+k]) == tolower(non_opt[j][k])) {
                                prefix_f++;
                                control[i+k] = 2;
                            }
                        }

                    }

                    if (prefix_f == n) {
                        letter_swp(i, str, arg, (int) strlen(arg), control);
                        break;
                    }
                }
            }
        }
    }
}

void md_spltscrn(char **word_str, char **word_orig, int* rows) {
    for (int i = 0; i < size_ws; i++) {
        printf("%d. %s", rows[i], word_orig[i]);
        for (int j = 0; j < (longest - strlen(word_orig[i])); j++) printf(" ");
        printf(":%s\n", word_str[i]);
    }
}

void normal_print(char *str) {
    while (*str != '\0') {
        printf("%c", *str);
        str++;
    }
}

int main(int argc, char *argv[]) {
    int opt = getopt(argc, argv, ":dpr:R:s");
    int n = 1;

    while (opt != -1) {
        if (opt == '?') {
            printf("E1\n");
            return 0;
        } else if (opt == ':') {
            printf("E2\n");
            return 0;
        } else if (opt == 'r' || opt == 'R') {
            n++;
            if (strlen(optarg) > 20 || strlen(optarg) < 1) {
                printf("E3\n");
                return 0;
            }
        }
        opt = getopt(argc, argv, ":dpr:R:s");
        n++;
    }

    char **non_opt;
    non_opt = malloc(WORD*argc);
    int j = 0;
    for (int i = n; i < argc; i++, j++) {
        non_opt[j] = malloc(MAX_WORD+1);
        strcpy(non_opt[j], argv[i]);
    }

    int print_mode = 0;
    char *str, *orig_str;
    int* control;
    str = input_str();
    orig_str = malloc(strlen(str)+1);
    control = calloc(strlen(str)+1, NUM);

    strcpy(orig_str, str);

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-d") == 0) {
                rem_digit(str);
                strcpy(orig_str, str);
            } else if (strcmp(argv[i], "-p") == 0) {
                rem_punct(str);
                strcpy(orig_str, str);
            } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "-R") == 0) {
                if (strcmp(argv[i], "-r") == 0) {
                    swp_word(str, argv[i+1], non_opt, j, 1, control);
                } else {
                    swp_word(str, argv[i+1], non_opt, j, 0, control);
                }
                i++;
            } else if (strcmp(argv[i], "-s") == 0) {
                print_mode++;
            }
        }
    }

    if (print_mode) {
        char **word_str = word_fndr(str, control);
        char **word_orig = word_fndr(orig_str, control);
        int *rows = row_fndr(orig_str);
        md_spltscrn(word_str, word_orig, rows);

        free_2d_array(size_ws+1, word_str);
        free_2d_array(size_ws+1, word_orig);
        free(word_str);
        free(word_orig);
        free(rows);
    } else {
        normal_print(str);
    }
    free_2d_array(argc, non_opt);
    free(non_opt);
    free(str);
    free(orig_str);
    free(control);

    return 0;
}
