#include "monopoly.h"

// nazvy typov poli na hracom plane
const char *space_types[NUM_SPACE_TYPES] = {
        "PROPERTY",
        "START",
        "FREE PARKING",
        "IN JAIL",
        "GO TO JAIL",
        "JAIL PASS"
};

// nazvy farieb nehnutelnosti
const char *property_colors[NUM_COLORS] = {
        "Brown",
        "Skyblue",
        "Purple",
        "Orange",
        "Red",
        "Yellow",
        "Green",
        "Blue"
};

// zoznam nehnutelnosti na hracom plane
PROPERTY properties[NUM_PROPERTIES] = {
        {.name = "FOOD TRUCK", .price = 1, .color = Brown},
        {.name = "PIZZA RESTAURANT", .price = 1, .color = Brown},
        {.name = "DOUGHNUT SHOP", .price = 1, .color = Skyblue},
        {.name = "ICE CREAM SHOP", .price = 1, .color = Skyblue},
        {.name = "MUSEUM", .price = 2, .color = Purple},
        {.name = "LIBRARY", .price = 2, .color = Purple},
        {.name = "THE PARK", .price = 2, .color = Orange},
        {.name = "THE BEACH", .price = 2, .color = Orange},
        {.name = "POST OFFICE", .price = 3, .color = Red},
        {.name = "TRAIN STATION", .price = 3, .color = Red},
        {.name = "COMMUNITY GARDEN", .price = 3, .color = Yellow},
        {.name = "PET RESCUE", .price = 3, .color = Yellow},
        {.name = "AQUARIUM", .price = 4, .color = Green},
        {.name = "THE ZOO", .price = 4, .color = Green},
        {.name = "WATER PARK", .price = 5, .color = Blue},
        {.name = "AMUSEMENT PARK", .price = 5, .color = Blue}
};

// inicializacia hracieho pola
SPACE game_board[NUM_SPACES] = {
        {.type = Start, .property = NULL},
        {.type = Property, .property = &properties[0]},
        {.type = Property, .property = &properties[1]},
        {.type = Jail_pass, .property = NULL},
        {.type = Property, .property =  &properties[2]},
        {.type = Property, .property =  &properties[3]},
        {.type = In_jail, .property =  NULL},
        {.type = Property, .property =  &properties[4]},
        {.type = Property, .property =  &properties[5]},
        {.type = Jail_pass, .property =  NULL},
        {.type = Property, .property =  &properties[6]},
        {.type = Property, .property =  &properties[7]},
        {.type = Free_parking, .property =  NULL},
        {.type = Property, .property =  &properties[8]},
        {.type = Property, .property =  &properties[9]},
        {.type = Jail_pass, .property =  NULL},
        {.type = Property, .property =  &properties[10]},
        {.type = Property, .property =  &properties[11]},
        {.type = Go_to_jail, .property =  NULL},
        {.type = Property, .property =  &properties[12]},
        {.type = Property, .property =  &properties[13]},
        {.type = Jail_pass, .property =  NULL},
        {.type = Property, .property =  &properties[14]},
        {.type = Property, .property =  &properties[15]}
};
