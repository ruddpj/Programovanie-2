// Základný layout prebratý od ostatných zadaní Programovania 2
// Inšpirácia podľa hry DOOM

//README
// Compile with: gcc main.c -o main -lncurses
// pozri ako linkovať library cez compiler
// Po stlačení šípok sa kurzor zafarbý na červeno, ak nevidíš farby doinštaluj farby pre ncurses
// V0.3 pridal možnosť hrať aj bez farieb

#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define SPACE_X 14
#define SPACE_Y 6

#define MAX_NAME 100
#define NUM_SPACES 16
#define NUM_DEMONS 8

#define DOOMGUYCOLOR 9
#define TERRORBILLYCOLOR 10

typedef enum space_type { // Types of spaces in the program
    Demon,
    Start,
    Camp,
    In_hell,
    Go_to_Hell,
    Chainsaw_fuel
} SPACE_TYPE;

typedef struct property { // Structure for demons
    char name[MAX_NAME + 1];
    int price;
    short color;
} PROPERTY;

typedef struct space { // Structure for spaces
    SPACE_TYPE type;
    PROPERTY *property;
} SPACE;

typedef struct players { // Structure for players
    char name[MAX_NAME+1];
    int num_kills;
    int pos;
    int hp;
    int fuel;
    int in_hell;
} PLAYERS;

char *color_list[4] = { // List of colors
        "Red",
        "Green",
        "Yellow",
        "Blue"
};

PROPERTY demons[NUM_DEMONS] = { // Demon names
        {.name = "ZOMBIEMAN", .price = 1, .color = COLOR_RED},
        {.name = "SHOTGUN GUY", .price = 1, .color = COLOR_RED},
        {.name = "REVENANT", .price = 2, .color = COLOR_BLUE},
        {.name = "IMP", .price = 2, .color = COLOR_BLUE},
        {.name = "PINKY", .price = 3, .color = COLOR_GREEN},
        {.name = "CACODEMON", .price = 3, .color = COLOR_GREEN},
        {.name = "THE BARON", .price = 4, .color = COLOR_YELLOW},
        {.name = "MANCUBUS", .price = 4, .color = COLOR_YELLOW},

};

SPACE game_board[NUM_SPACES] = { // Spaces
        {.type = Start, .property=NULL},
        {.type = Demon, .property=&demons[0]},
        {.type = Chainsaw_fuel, .property=NULL},
        {.type = Demon, .property=&demons[1]},
        {.type = In_hell, .property=NULL},
        {.type = Demon, .property=&demons[2]},
        {.type = Chainsaw_fuel, .property=NULL},
        {.type = Demon, .property=&demons[3]},
        {.type = Camp, .property=NULL},
        {.type = Demon, .property=&demons[4]},
        {.type = Chainsaw_fuel, .property=NULL},
        {.type = Demon, .property=&demons[5]},
        {.type = Go_to_Hell, .property=NULL},
        {.type = Demon, .property=&demons[6]},
        {.type = Chainsaw_fuel, .property=NULL},
        {.type = Demon, .property=&demons[7]}

};

char *names[2] = { // The names of our protagonists
        "DOOM GUY",
        "TERROR BILLY"
};

void main_title(WINDOW *title) { // Print the title
    mvwprintw(title, 1, 2, " ______   _______  _______  _______         _______  _______  _______  _");
    mvwprintw(title, 2, 2, "(  __  \\ (  ___  )(  ___  )(       )       (  ___  )(  ____ )(  ___  )( \\   |\\     /|");
    mvwprintw(title, 3, 2, "| (  \\  )| (   ) || (   ) || () () |       | (   ) || (    )|| (   ) || (   ( \\   / )");
    mvwprintw(title, 4, 2, "| |   ) || |   | || |   | || || || | _____ | |   | || (____)|| |   | || |    \\ (_) /");
    mvwprintw(title, 5, 2, "| |   | || |   | || |   | || |(_)| |(_____)| |   | ||  _____)| |   | || |     \\   /");
    mvwprintw(title, 6, 2, "| |   ) || |   | || |   | || |   | |       | |   | || (      | |   | || |      ) (");
    mvwprintw(title, 7, 2, "| (__/  )| (___) || (___) || )   ( |       | (___) || )      | (___) || (____/\\| |");
    mvwprintw(title, 8, 2, "(______/ (_______)(_______)|/     \\|       (_______)|/       (_______)(_______/\\_/");
}

void select_menu(WINDOW **menus, int val, char **options) { // Main menu highlight options
    for (int i = 0; i < 3; i++) {
        if (i == val) {
            box(menus[i], 0, 0);
            wattron(menus[i], COLOR_PAIR(5));
            mvwprintw(menus[i], 1, 6, "> %s", options[i]);
            wattroff(menus[i], COLOR_PAIR(5));
        } else {
            box(menus[i], 0, 0);
            mvwprintw(menus[i], 1, 6, " %s ", options[i]);
        }
        wrefresh(menus[i]);
    }
}

void scoreboard(const int *spaces, PLAYERS *players, int x, int pl) { // Prints the scoreboards
    WINDOW *board = newwin(24, 35, 1, x);
    box(board, 0, 0);

    wattron(board, COLOR_PAIR(9+pl));
    mvwprintw(board, 1, 2, "%s", players[pl].name);
    wattroff(board, COLOR_PAIR(9+pl));
    mvwprintw(board, 2, 2, "HP: %d FUEL: %d IN HELL: %s", players[pl].hp, players[pl].fuel,
              (players[pl].in_hell) ? "yes" : "no");
    mvwprintw(board, 4, 2, "KILL LIST:");
    for (int i = 0, j = 0; i < NUM_SPACES; i++) {
        if (spaces[i] == pl+1){
            mvwprintw(board, j+5, 4, "%-12s %d  %s", game_board[i].property->name, game_board[i].property->price,
                      color_list[game_board[i].property->color-1]);
            j++;
        }
    }
    wrefresh(board);
}

int main_menu() { // Prints and handles the main menu
    WINDOW *title = newwin(11, 89, 8, 43);
    refresh();
    box(title, 0, 0);
    main_title(title);
    wrefresh(title);

    WINDOW *menus[3] = { // 3 menu options
            menus[0] = newwin(4, 35, 20, 70),
            menus[1] = newwin(4, 35, 25, 70),
            menus[2] = newwin(4, 35, 30, 70)
    };
    char *options[3] = {"          DUEL          ", "    MAN VS. MACHINE    ", "          QUIT          "};
    for (int i = 0; i < 3; i++) { // Boxes the options
        box(menus[i], 0, 0);
        mvwprintw(menus[i], 1, 6, " %s ", options[i]);
        wrefresh(menus[i]);
    }
    mvprintw(35, 78, "PRESS 'F' TO START");
    mvprintw(38, 70, "Tip: Use arrow keys no navigate menu");
    mvprintw(45, 68, "All rights reserved (C) Fero Stromokocúr"); // Context clues
    refresh();

    int c = getch(), val = 0;
    while (c != 'f') { // Control up and down to select mode
        if (c == KEY_UP && val > 0) {
            val--;
        } else if (c == KEY_DOWN && val < 2) {
            val++;
        }
        select_menu(menus, val, options);
        c = getch();
    }
    return val;
}

void handle_win_content(WINDOW *win, int x, const int* spaces, int p1, int p2) {
    switch (game_board[x].type) { // Different space types
        case Start:
            wattron(win, COLOR_PAIR(6));
            mvwprintw(win, 2, 1, "   START    ");
            wattroff(win, COLOR_PAIR(6));
            break;
        case Chainsaw_fuel:
            wattron(win, COLOR_PAIR(7));
            mvwprintw(win, 2, 1, "    FUEL    ");
            wattroff(win, COLOR_PAIR(7));
            break;
        case In_hell:
            wattron(win, COLOR_PAIR(8));
            mvwprintw(win, 2, 1, "  IN HELL   ");
            wattroff(win, COLOR_PAIR(8));
            break;
        case Go_to_Hell:
            wattron(win, COLOR_PAIR(8));
            mvwprintw(win, 2, 1, " GO TO HELL ");
            wattroff(win, COLOR_PAIR(8));
            break;
        case Camp:
            wattron(win, COLOR_PAIR(6));
            mvwprintw(win, 2, 1, "    CAMP    ");
            wattroff(win, COLOR_PAIR(6));
            break;
        case Demon:
            wattron(win, COLOR_PAIR(game_board[x].property->color));
            mvwprintw(win, 1, 1, "%-12s", game_board[x].property->name);
            wattroff(win, COLOR_PAIR(8+spaces[x]));
            if (spaces[x]) {
                wattron(win, COLOR_PAIR(8+spaces[x]));
                mvwprintw(win, 2, 1, "%-12s", names[spaces[x]-1]);
                wattroff(win, COLOR_PAIR(8+spaces[x]));
            }
            break;
        default:
            break;
    }
    if (x == p1) { // Handles printing the player pieces
        wattron(win, COLOR_PAIR(11));
        mvwprintw(win, 4, 2, "DG");
        wattroff(win, COLOR_PAIR(11));
    }
    if (x == p2) {
        wattron(win, COLOR_PAIR(12));
        mvwprintw(win, 4, 10, "TB");
        wattroff(win, COLOR_PAIR(12));
    }
}

int demon_popup(int* spaces, PLAYERS *players, int sp_num, int pl, int ai) { // Demon context menu
    WINDOW *demon = newwin(13, 37, 30, 5+pl*135);
    box(demon, 0, 0);
    mvwprintw(demon, 1, 1, "        O              L           ");
    mvwprintw(demon, 2, 1, "       <|r=l-         [o\\          ");
    mvwprintw(demon, 3, 1, "        |            --|#|         ");
    mvwprintw(demon, 4, 1, "       /\\            _/ |L*        ");
    mvwprintw(demon, 5, 1, "___________________________________");
    mvwprintw(demon, 7, 1, "You are approached by %s", game_board[sp_num].property->name);
    mvwprintw(demon, 8, 1, "COST: %d HP", game_board[sp_num].property->price);
    mvwprintw(demon, 11, 1, "Press F to FIGHT or press P to PASS");
    wrefresh(demon); // Art

    int c;
    if (ai == 0 || pl == 0) { // Skips waiting for computer turn
        c = getch();
        while (c != 'f' && c != 'p') c = getch();
    } else {
        c = 'f';
    }
    if (c == 'f') { // Either kills you or lowers your health and you get the spot
        if (players[pl].hp >= game_board[sp_num].property->price) {
            players[pl].hp -= game_board[sp_num].property->price;
            players[pl].num_kills++;
            spaces[sp_num] = pl+1;
        } else {
            return 1;
        }
    }
    wclear(demon);
    wrefresh(demon);
    return 0;
}

int player_popup(const int* spaces, PLAYERS *players, int sp_num, int pl, int ai) { // Player context menu
    WINDOW *player = newwin(13, 37, 30, 5+pl*135);
    box(player, 0, 0);
    mvwprintw(player, 1, 1, "        O             \\ <O> /      ");
    mvwprintw(player, 2, 1, "       <|r=l-           | |        ");
    mvwprintw(player, 3, 1, "        |               | |        ");
    mvwprintw(player, 4, 1, "       /\\             _/   \\_      ");
    mvwprintw(player, 5, 1, "___________________________________");
    mvwprintw(player, 7, 1, "You stepped into territory of");
    mvwprintw(player, 8, 1, "%s", names[spaces[sp_num]-1]);
    mvwprintw(player, 9, 1, "COST: %d HP", game_board[sp_num].property->price);
    mvwprintw(player, 11, 1, "Press any key to transfer HP");
    wrefresh(player); // Art

    if (ai == 0) getch(); // Skip if CPU
    if (players[pl].hp >= game_board[sp_num].property->price) { // Transfers health or kills you
        players[pl].hp -= game_board[sp_num].property->price;
        players[spaces[sp_num]-1].hp += game_board[sp_num].property->price;
    } else {
        return 1;
    }
    wclear(player);
    wrefresh(player);
    return 0;
}

void hell_popup(PLAYERS *players, int pl, int ai) { // Hell context menu
    WINDOW *hell = newwin(13, 37, 30, 5+pl*135);
    box(hell, 0, 0);
    mvwprintw(hell, 1, 1, "    0B           O            ___ ");
    mvwprintw(hell, 2, 1, "   <|-r==       /|^D==z      />:(\\ ");
    mvwprintw(hell, 3, 1, "    |            |           \\___/ ");
    mvwprintw(hell, 4, 1, "   /|       (*/--/\\/--\\*)          ");
    mvwprintw(hell, 5, 1, "___________________________________");
    mvwprintw(hell, 7, 1, "You are surrounded by demons");
    mvwprintw(hell, 9, 1, "Press F to use CHAINSAW FUEL");
    mvwprintw(hell, 10, 1, "Press P to GO TO HELL");
    mvwprintw(hell, 11, 1, "YOUR FUEL: %d", players[pl].fuel);
    wrefresh(hell); // Art

    int c;
    if (ai == 0 || pl == 0) { // Skip if cpu
        c = getch();
        while (c != 'f' && c != 'p') c = getch();
    } else {
        c = 'f';
    }
    if (c == 'f' && players[pl].fuel > 0) { // Either you go to health or use you chainsaw fuel
        players[pl].fuel--;
    } else {
        players[pl].pos = 4;
        players[pl].in_hell = 1;
    }
    wclear(hell);
    wrefresh(hell);
}

void print_gameboard(const int* spaces, PLAYERS *players) { // Print the actual board
    WINDOW *game_spaces[NUM_SPACES];

    int base_x = 115, base_y = 30, dir = 0;
    for (int i = 0; i < NUM_SPACES; i++) {
        game_spaces[i] = newwin(SPACE_Y, SPACE_X, base_y, base_x);
        box(game_spaces[i], 0, 0);
        handle_win_content(game_spaces[i], i, spaces, players[0].pos, players[1].pos);

        wrefresh(game_spaces[i]);
        switch (dir) { // The direction the next space needs to be moved to print it
            case 0:
                base_x -= SPACE_X+1;
                break;
            case 1:
                base_y -= SPACE_Y+1;
                break;
            case 2:
                base_x += SPACE_X+1;
                break;
            case 3:
                base_y += SPACE_Y+1;
                break;
            default:
                break;
        }
        if (i%4 == 3) { // Every 4 spaces it changes
            dir = (dir+1)%4;
        }
    }
}

void roll_vis(int roll) { // Prints the dice roll
    WINDOW *rollwin = newwin(5, 11, 20, 85);
    box(rollwin, 0, 0);
    switch (roll) {
        case 1:
            mvwprintw(rollwin, 2, 5, "O");
            break;
        case 2:
            mvwprintw(rollwin, 1, 3, "O");
            mvwprintw(rollwin, 3, 7, "O");
            break;
        case 3:
            mvwprintw(rollwin, 1, 3, "O");
            mvwprintw(rollwin, 2, 5, "O");
            mvwprintw(rollwin, 3, 7, "O");
            break;
        case 4:
            mvwprintw(rollwin, 1, 3, "O   O");
            mvwprintw(rollwin, 3, 3, "O   O");
            break;
        case 5:
            mvwprintw(rollwin, 1, 3, "O   O");
            mvwprintw(rollwin, 2, 5, "O");
            mvwprintw(rollwin, 3, 3, "O   O");
            break;
        case 6:
            mvwprintw(rollwin, 1, 3, "O   O");
            mvwprintw(rollwin, 2, 3, "O   O");
            mvwprintw(rollwin, 3, 3, "O   O");
            break;
        default:
            break;
    }
    wrefresh(rollwin);
}

int playing_the_game(PLAYERS *players, int *spaces, int ai) { // To be honest I was pretty drunk while coding this
    print_gameboard(spaces, players);                           // so the actual nitty-gritty of the code is between
    WINDOW *turn_vis = newwin(3, 5, 14, 88);                    // my drunk self and God
    box(turn_vis, 0, 0);
    mvprintw(13, 88, "TURN:");
    mvprintw(26, 80, "PRESS ANY KEY TO ROLL");
    mvprintw(40, 75, "Tip: Pressing 'Q' forfeits the game");
    refresh(); // Context clues

    scoreboard(spaces, players, 5, 0);
    scoreboard(spaces, players, 140, 1); // Prints the scoreboards

    int turn = 1, p = 0, roll = rand()%6+1; // Current turn, player on turn and first dice roll
    int c = getch();
    while (c != 'q') { // Q forfeits the game
        if (ai) mvprintw(11, 87, "P: %s", p ? "CPU>" : "<YOU");
        else mvprintw(11, 87, "P: %s", p ? "BLU>" : "<RED"); // What player is doing the current turn
        mvwprintw(turn_vis, 1, 1, "%2.d", turn);
        wrefresh(turn_vis);
        roll_vis(roll); // Prints the dice

        if (players[p].pos == 4 && players[p].in_hell) { // Checks if the player is moving from hell
            if (players[p].hp > 1) {
                players[p].hp -= 2;
                players[p].in_hell = 0;
            } else {
                return p; // Death
            }
        }

        players[p].pos += roll; // Checks if the player passed Start
        if (players[p].pos > 15) {
            players[p].pos = players[p].pos%16;
            players[p].hp += 2;
        }

        if (players[p].pos == 12) { // Checks if the player entered Go to Hell
            hell_popup(players, p, ai);
        } else if (game_board[players[p].pos].property != NULL) { // Checks if the player landed on a demon
            if (spaces[players[p].pos] == 0) { // if the demon is 'unkilled'
                if (demon_popup(spaces, players, players[p].pos, p, ai)) return p;
            } else if (spaces[players[p].pos] != p+1) { // If it isn't the player's territory
                if (player_popup(spaces, players, players[p].pos, p, ai)) return p;
            }
        } else if (game_board[players[p].pos].type == Chainsaw_fuel) players[p].fuel++; // Check if the player landed on Chainsaw fuel

        scoreboard(spaces, players, 5, 0);
        scoreboard(spaces, players, 140, 1);
        print_gameboard(spaces, players); // Refresh with new info
        refresh();

        c = getch();
        if (p) p = 0; else p = 1; // Swap players
        turn++;
        roll = rand()%6+1; // Add new values
    }
    return p;
}

void diff_sel(WINDOW *opt, int diff, char *diff_opt[3]) { // Difficulty highlight
    for (int i = 0; i < 3; i++) {
        if (i == diff) {
            if (diff == i) wattron(opt, COLOR_PAIR(5));
            mvwprintw(opt, (2*i)+1, 2, "> %s", diff_opt[i]);
            if (diff == i) wattroff(opt, COLOR_PAIR(5));
        } else {
            mvwprintw(opt, (2*i)+1, 2, " %s ", diff_opt[i]);
        }
    }
    wrefresh(opt);
}

int difficulty() { // Choosing the difficulty
    mvprintw(10, 72, "CHOOSE DIFFICULTY");
    mvprintw(20, 72, "PRESS 'F' TO START");
    mvprintw(42, 60, "Tip: For best immersion play 'Rip & Tear' by Mick Gordon");
    WINDOW *opt = newwin(7, 35, 12, 70);
    box(opt, 0, 0);
    char *diff_opt[3] = {" :-(  I'M TOO YOUNG TO DIE", " :-|  HURT ME PLENTY", " >:)  ULTRA-VIOLENCE"};
    diff_sel(opt, 0, diff_opt);
    refresh();

    int c = getch(), diff = 0; // Control up and down
    while (c != 'f') {
        if (c == KEY_UP && diff > 0) {
            diff--;
        } else if (c == KEY_DOWN && diff < 2) {
            diff++;
        }
        diff_sel(opt, diff, diff_opt);
        c = getch();
    }
    return diff;
}

void deathscreen(WINDOW *win) { // Death screen art
    mvwprintw(win, 1, 1, "     )    )           (     (        (      ");
    mvwprintw(win, 2, 1, "  ( /( ( /(           )\\ )  )\\ )     )\\ )   ");
    mvwprintw(win, 3, 1, "  )\\()))\\())     (   (()/( (()/( (  (()/(   ");
    mvwprintw(win, 4, 1, " ((_)\\((_)\\      )\\   /(_)) /(_)))\\  /(_))  ");
    mvwprintw(win, 5, 1, "__ ((_) ((_)  _ ((_) (_))_ (_)) ((_)(_))_   ");
    mvwprintw(win, 6, 1, "\\ \\ / // _ \\ | | | |  |   \\|_ _|| __||   \\  ");
    mvwprintw(win, 7, 1, " \\ V /| (_) || |_| |  | |) || | | _| | |) | ");
    mvwprintw(win, 8, 1, "  |_|  \\___/  \\___/   |___/|___||___||___/  ");
}

int main(void) {
    initscr(); // Initializes the screen
    cbreak(); // Disable line buffering
    noecho(); // Disable character echo
    keypad(stdscr, TRUE); // Enables special characters
    curs_set(FALSE); // Disables the cursor
    if (has_colors() == FALSE) { // Quits if the user doesn't have colors installed
        endwin();
        printf("Download necessary libraries to support color\n");
        return 1;
    }

    start_color(); // Initialize colors
    init_pair(COLOR_RED, COLOR_BLACK, COLOR_RED);
    init_pair(COLOR_BLUE, COLOR_BLACK, COLOR_BLUE);
    init_pair(COLOR_GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(COLOR_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    init_pair(7, COLOR_GREEN, COLOR_BLACK);
    init_pair(8, COLOR_RED, COLOR_BLACK);
    init_pair(DOOMGUYCOLOR, COLOR_RED, COLOR_BLACK);
    init_pair(TERRORBILLYCOLOR, COLOR_BLUE, COLOR_BLACK);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_WHITE, COLOR_BLUE);

    while (1) { // Loops each 'game'
        srand(time(NULL)); // Based on irl time generates a new seed for rand()
        int c = main_menu(); // Gets the desired mode
        clear();
        refresh();
        if (c == 2) { // If quit then quit
            break;
        }

        int spaces[NUM_SPACES]; // Spaces maps to each space on the board 0 - unkilled demon/prop=NULL 1 - p1 demon 2 - p2 demon
        memset(spaces, 0, NUM_SPACES * sizeof(int));

        int diff = difficulty(); // Selects difficulty
        clear();
        refresh();

        PLAYERS players[2]; // Player struct object
        for (int i = 0; i < 2; i++) {
            strcpy(players[i].name, names[i]);
            players[i].pos = 0;
            players[i].num_kills = 0;
            players[i].hp = (3 - diff) * 5; // 15/10/5 health
            players[i].fuel = 0;
            players[i].in_hell = 0;
        }
        if (c == 1) players[1].hp = 10; // It wouldn't be hard if the CPU didn't have advantage

        int defeated = playing_the_game(players, spaces, c); // Returns the defeated player
        WINDOW *end_screen = newwin(17, 46, 8, 69);
        box(end_screen, 0, 0);
        refresh();
        deathscreen(end_screen);
        mvwprintw(end_screen, 12, 2, "%s HAS PERISHED", names[defeated]);
        mvwprintw(end_screen, 13, 2, "%s WINS", names[(defeated+1)%2]);
        mvwprintw(end_screen, 15, 2, "PRESS ANY KEY TO CONTINUE");
        wrefresh(end_screen);
        getch();
        clear(); // Prints who won and who lost
    }
    endwin(); // Ends the window
    return 0;
} // Hopefully you liked my little game :D
