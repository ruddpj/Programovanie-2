#ifndef Z4_MONOPOLY_H
#define Z4_MONOPOLY_H

#include <stdio.h>

// Makra
#define MAX_NAME 100        // max. dlzka nazvu (bez znaku '\0')
#define NUM_PROPERTIES 16   // pocet nehnutelnosti na hracom plane
#define NUM_SPACES 24       // pocet poli na hracom plane
#define MAX_PLAYERS 4       // max. pocet hracov
#define NUM_SPACE_TYPES 6   // pocet typov poli na hracom plane
#define NUM_COLORS 8        // pocet farieb nehnutelnosti

// Definicie vlastnych datovych typov

// Typy poli na hracom plane
typedef enum _space_type {
    Property,       // nehnutelnost
    Start,          // startovacie pole
    Free_parking,   // specialne pole bez akcie
    In_jail,        // specialne pole, predstavuje vazenie
    Go_to_jail,     // specialne pole, ktore presunie hraca do vazenia
    Jail_pass       // specialne pole s priepustkou
} SPACE_TYPE;

// Farba nehnutelnosti
typedef enum _color {
    Brown,
    Skyblue,
    Purple,
    Orange,
    Red,
    Yellow,
    Green,
    Blue
} COLOR;

// Struktura pre nehnutelnost
typedef struct _property {
    char name[MAX_NAME + 1];    // nazov
    int price;                  // cena nehnutelnosti/vyska najomneho
    COLOR color;                // farba
} PROPERTY;

// Struktura pre pole na hracom plane
typedef struct _space {
    SPACE_TYPE type;        // typ pola na hracom plane
    PROPERTY *property;     // pointer na nehnutelnost ak je pole
                            // typu Property, inak NULL
} SPACE;

// Struktura pre hraca
typedef struct _player {
    int number;             // cislo hraca (1, 2, 3 alebo 4)
    int space_number;       // cislo pola, na ktorom stoji (pole Start ma cislo 1)
    int cash;               // hotovost hraca
    int num_jail_pass;      // pocet nazbieranych priepustiek z vazenia
    int is_in_jail;         // info, ci je hrac vo vazeni: 0 - nie je; 1 - je
    PROPERTY *owned_properties[NUM_PROPERTIES]; // zoznam kupenych nehnutelnosti
    int num_properties;                         // pocet kupenych nehnutelnosti
} PLAYER;

// Deklaracie premennych
extern const char *space_types[NUM_SPACE_TYPES];    // nazvy typov poli na hracom plane
extern const char *property_colors[NUM_COLORS];     // nazvy farieb nehnutelnosti
extern PROPERTY properties[NUM_PROPERTIES];         // zoznam vsetkych nehnutelnosti
extern SPACE game_board[NUM_SPACES];                // hraci plan s 24 poliami

#endif //Z4_MONOPOLY_H
