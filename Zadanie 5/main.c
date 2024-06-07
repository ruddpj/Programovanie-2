#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "monopoly.h"

static int prop_spaces = NUM_PROPERTIES;
static int num_prop = NUM_PROPERTIES;
static int num_spaces = NUM_SPACES;

void free_my_memories(int b, int t, SPACE *gameboard, PROPERTY *property) {
    if (b) {
        free(gameboard);
    }
    if (t) {
        free(property);
    }
}

void list_props(PROPERTY *props) {
    if (props == NULL) return;
    for (int i = 0; i < num_prop; i++) {
        printf("%-20s %d  %-10s\n", props[i].name, props[i].price, property_colors[props[i].color]);
    }
}

void line_print(int i, char *name) {
    if (i >= 9) {
        printf("%d.  %s\n", i+1, name);
    } else {
        printf("%d.   %s\n", i+1, name);
    }
}

void print_board(SPACE *gameboard, PROPERTY *props) {
    if (gameboard == NULL || props == NULL) return;
    int j = 0;
    for (int i = 0; i < num_spaces; i++) {
        switch (gameboard[i].type) {
            case Start:
                line_print(i, "START");
                break;
            case Go_to_jail:
                line_print(i, "GO TO JAIL");
                break;
            case In_jail:
                line_print(i, "IN JAIL");
                break;
            case Jail_pass:
                line_print(i, "JAIL PASS");
                break;
            case Free_parking:
                line_print(i, "FREE PARKING");
                break;
            case Property:
                if (i >= 9) {
                    printf("%d.  %-20s %d  %-10s\n", i+1, props[j].name, props[j].price, property_colors[props[j].color]);
                } else {
                    printf("%d.   %-20s %d  %-10s\n", i+1, props[j].name, props[j].price, property_colors[props[j].color]);
                }
                j++;
                break;
            default:
                break;
        }
    }
}

int color_select(char *color) {
    for (int i = 0; i < NUM_COLORS; i++) {
        if (strcmp(property_colors[i], color) == 0) return i;
    }
    return 0;
}

PROPERTY *p_file_handle(FILE *p_file) {
    int tmp;
    char color[MAX_NAME+1], name[MAX_NAME+1];
    int x, k = 0;
    PROPERTY *props = malloc(sizeof(PROPERTY));
    PROPERTY *temp;

    while ((tmp = fgetc(p_file)) != EOF) {
        if (tmp == '[') {
            x = 0;
            bzero(color, MAX_NAME+1);
            while ((tmp = fgetc(p_file)) != ']') {
                color[x] = (char) tmp;
                x++;
            }
        } else if (tmp == '"') {
            x = 0;
            bzero(name, MAX_NAME+1);
            while ((tmp = fgetc(p_file)) != '"') {
                name[x] = (char) tmp;
                x++;
            }
            if (fgetc(p_file) == ':') {
                int cost = fgetc(p_file) - '0';
                temp = realloc(props, sizeof(PROPERTY)*(k+1));
                if (temp != NULL) props = temp;
                strcpy(props[k].name, name);
                props[k].price = cost;
                props[k].color = color_select(color);
                k++;
            }
        }
    }
    num_prop = k;
    return props;
}

SPACE *g_file_handle(FILE *g_file, PROPERTY *prop) {
    char buffer[MAX_NAME+1];
    int j = 0;
    fgets(buffer, MAX_NAME+1, g_file);
    num_spaces = atoi(buffer);
    SPACE *layout = malloc(sizeof(SPACE)*num_spaces);

    for (int i = 0; i < num_spaces; i++) {
        bzero(buffer, MAX_NAME+1);
        int tmp, x = 0;
        while ((tmp = fgetc(g_file)) != '\n' && tmp != EOF) {
            buffer[x] = (char) tmp;
            x++;
        }
        if (strcmp(buffer, "START") == 0) {
            layout[i].type = Start;
            layout[i].property = NULL;
        } else if (strcmp(buffer, "IN JAIL") == 0) {
            layout[i].type = In_jail;
            layout[i].property = NULL;
        } else if (strcmp(buffer, "JAIL PASS") == 0) {
            layout[i].type = Jail_pass;
            layout[i].property = NULL;
        } else if (strcmp(buffer, "GO TO JAIL") == 0) {
            layout[i].type = Go_to_jail;
            layout[i].property = NULL;
        } else if (strcmp(buffer, "FREE PARKING") == 0) {
            layout[i].type = Free_parking;
            layout[i].property = NULL;
        } else {
            layout[i].type = Property;
            layout[i].property = &prop[j];
            j++;
        }
    }
    prop_spaces = j;
    return layout;
}

int main(int argc, char *argv[]) {
    SPACE *gameboard = game_board;
    PROPERTY *property = properties;

    FILE *g_file = NULL;
    FILE *p_file = NULL;

    int opt;
    int b_control = 0, t_control = 0;
    while ((opt = getopt(argc, argv, ":b:t:")) != -1) {
        switch (opt){
            case 'b':
                if ((g_file = fopen(optarg, "r")) == NULL) {
                    printf("E1\n");
                    return 0;
                }
                b_control = 1;
                break;
            case 't':
                if ((p_file = fopen(optarg, "r")) == NULL) {
                    printf("E1\n");
                    return 0;
                }
                t_control = 1;
                break;
            default:
                break;
        }
    }

    if (t_control) {
        property = p_file_handle(p_file);
        fclose(p_file);
    }
    if (b_control) {
        gameboard = g_file_handle(g_file, property);
        fclose(g_file);
    }

    if (b_control) {
        if (prop_spaces > num_prop) {
            free_my_memories(b_control, t_control, gameboard, property);
            printf("E2\n");
            return 0;
        }
        print_board(gameboard, property);
    } else if (t_control) {
        list_props(property);
    }

    free_my_memories(b_control, t_control, gameboard, property);
    return 0;
}
