#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "monopoly.h"

struct _player *player_maker(int num_players) {
    struct _player *players = malloc(num_players*sizeof(struct _player));
    for (int i = 0; i < num_players; i++) {
        players[i].number = i+1;
        players[i].space_number = 1;
        players[i].cash = 20-(num_players-2)*2;
        players[i].num_jail_pass = 0;
        players[i].is_in_jail = 0;
        players[i].num_properties = 0;
        for (int j = 0; j < 16; j++) {
            players[i].owned_properties[j] = malloc(sizeof (PROPERTY));
        }
    }
    return players;
}

void turn_info(int roll, int turn, int player) {
    printf("R: %d\nTurn: %d\nPlayer on turn: P%d\n\n", roll, turn, player);
}

void game_plan(const int *owned_property) {
    int first = 1;
    printf("Game board:\n");
    for (int i = 0; i < 24; i++) {
        printf("%d.", i+1);
        (i > 8) ? printf("  ") : printf("   ");
        if (game_board[i].property == NULL) {
            printf("%s", space_types[game_board[i].type]);
        } else {
            printf("%-17s %d  %-10s", game_board[i].property->name,
                   game_board[i].property->price,
                   property_colors[game_board[i].property->color]);
            if (owned_property[i]) {
                printf("P%d ", owned_property[i]);
                if (first) {
                    (owned_property[i] == owned_property[i+1]) ? printf("yes") : printf("no");
                } else {
                    (owned_property[i] == owned_property[i-1]) ? printf("yes") : printf("no");
                }
            }
            if (first) {
                first = 0;
            } else {
                first = 1;
            }
        }
        printf("\n");
    }
}

void player_stats(int num_players,  struct _player *players) {
    printf("Players:\n");
    for (int i = 0; i < num_players; i++) {
        printf("%d. S: %d, C: %d, JP: %d, IJ: %s\n", players[i].number, players[i].space_number, players[i].cash,
               players[i].num_jail_pass, (players[i].is_in_jail) ? "yes" : "no");
        for (int j = 0; j < players[i].num_properties; j++) {
                printf("      %-17s %d  %-9s ", players[i].owned_properties[j]->name, players[i].owned_properties[j]->price,
                       property_colors[players[i].owned_properties[j]->color]);
                for (int k = 0; k < 24; k++) {
                    if (game_board[k].property != NULL && strcmp(game_board[k].property->name, players[i].owned_properties[j]->name) == 0) {
                        printf("S%d\n", k+1);
                        break;
                    }
                }
        }
    }
}

void print_mode_decider(int print_mode, struct _player *players, int *owned_property, int num_players) {
    switch (print_mode) {
        case 1:
            player_stats(num_players, players);
            printf("\n");
            break;
        case 2:
            game_plan(owned_property);
            printf("\n");
            break;
        case 3:
            player_stats(num_players, players);
            game_plan(owned_property);
            printf("WINNER: -\n\n");
            break;
        default:
            break;
    }
}

int eval_space(struct _player *players, int player_turn, int *owned_property) {
    int temp = players[player_turn].space_number-1;
    if (game_board[temp].property == NULL) {
        switch (players[player_turn].space_number) {
            case 4:
            case 10:
            case 16:
            case 22:
                players[player_turn].num_jail_pass++;
                break;
            case 19:
                if (players[player_turn].num_jail_pass > 0) {
                    players[player_turn].num_jail_pass--;
                } else {
                    players[player_turn].is_in_jail = 1;
                    players[player_turn].space_number = 7;
                }
                break;
            default:
                break;
        }
    } else {
        if (owned_property[temp] == 0) {
            if (players[player_turn].cash >= game_board[temp].property->price) {
                players[player_turn].cash -= game_board[temp].property->price;
                owned_property[temp] = player_turn+1;
                strcpy(players[player_turn].owned_properties[players[player_turn].num_properties]->name, game_board[temp].property->name);
                players[player_turn].owned_properties[players[player_turn].num_properties]->color = game_board[temp].property->color;
                players[player_turn].owned_properties[players[player_turn].num_properties]->price = game_board[temp].property->price;
                players[player_turn].num_properties++;
            } else return 1;
        } else if (owned_property[temp] != player_turn+1) {
            if (owned_property[temp] == owned_property[temp+1] || owned_property[temp] == owned_property[temp-1]) {
                if (players[player_turn].cash >= (game_board[temp].property->price)*2) {
                    players[player_turn].cash -= (game_board[temp].property->price)*2;
                    players[owned_property[temp]-1].cash += (game_board[temp].property->price)*2;
                } else return 1;
            } else {
                if (players[player_turn].cash >= game_board[temp].property->price) {
                    players[player_turn].cash -= game_board[temp].property->price;
                    players[owned_property[temp]-1].cash += game_board[temp].property->price;
                } else return 1;
            }
        }
    }
    return 0;
}

int playing_the_game(struct _player *players, int num_players, int print_mode, int *owned_property) {
    int player_turn = 0, roll, turn = 1, win_condition;

    player_stats(num_players, players);
    game_plan(owned_property);
    printf("WINNER: -\n\n");
    while (1) {
        if (scanf("%d", &roll) != 1) break;
        turn_info(roll, turn, player_turn+1);

        if (players[player_turn].is_in_jail) {
            if (players[player_turn].cash <= 0) return player_turn;
            players[player_turn].cash--;
            players[player_turn].is_in_jail = 0;
        }

        players[player_turn].space_number += roll;
        if (players[player_turn].space_number > 24) {
            players[player_turn].space_number = players[player_turn].space_number % 24;
            players[player_turn].cash += 2;
        }

        win_condition = eval_space(players, player_turn, owned_property);
        if (win_condition != 0) return player_turn;

        print_mode_decider(print_mode, players, owned_property, num_players);
        turn++;
        player_turn++;
        if (player_turn > num_players-1) player_turn = 0;
    }
    return 0;
}

void decide_the_winner(struct _player *players, int num_players, int cant_win) {
    int max_cash = -1, winner = 0, draw_scenario = 0;
    int draw[4] = {0};
    for (int i = 0; i < num_players; i++) {
        if (i != cant_win) {
            if (players[i].cash > max_cash) {
                winner = i;
                draw_scenario = 0;
                max_cash = players[i].cash;
            } else if (players[i].cash == max_cash) {
                draw_scenario = 1;
                draw[winner] = 1;
                draw[i] = 1;
            }
        }
    }
    if (draw_scenario) {
        int double_draw = 0, temp_cash;
        max_cash = -1;
        for (int i = 0; i < num_players; i++) {
            if (draw[i]) {
                temp_cash = players[i].cash;
                for (int j = 0; j < players[i].num_properties; j++) {
                    temp_cash += players[i].owned_properties[j]->price;
                }
                if (temp_cash > max_cash) {
                    winner = i;
                    max_cash = temp_cash;
                    double_draw = 0;
                } else if (temp_cash == max_cash) {
                    double_draw = 1;
                }
            }
        }
        if (double_draw) {
            printf("WINNER: ?\n\n");
            return;
        }
    }
    printf("WINNER: P%d\n\n", winner+1);
}

int main(int argc, char *argv[]) {
    int num_players = 2, print_mode = 0;
    int opt = getopt(argc, argv, ":n:spg");
    while (opt != -1) {
        switch (opt) {
            case 'n':
                sscanf(optarg, "%d", &num_players);
                break;
            case 's':
                print_mode = 2;
                break;
            case 'p':
                print_mode = 1;
                break;
            case 'g':
                print_mode = 3;
                break;
            default:
                break;
        }
        opt = getopt(argc, argv, ":n:spg");
    }

    struct _player *players = player_maker(num_players);
    int owned_property[NUM_SPACES] = {0};

    int cant_win = playing_the_game(players, num_players, print_mode, owned_property);
    player_stats(num_players, players);
    game_plan(owned_property);
    decide_the_winner(players, num_players, cant_win);

    for (int i = 0; i < num_players; i++) {
        for (int j = 0; j < 16; j++) {
            free(players[i].owned_properties[j]);
        }
    }
    free(players);
    return 0;
}
