/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements main
*/

#include "game.h"
#include "game/create.h"
#include "game/destroy.h"
#include "game/main_loop.h"
#include "my/stdio.h"
#include "my/string.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static const int ERROR_RETURN_VALUE = 84;

static int usage(const char *program_name)
{
    my_printf("Usage: %s\n\n"
        "    --original-resolution starts the game in 256*224, the "
        "original resolution of the NES (Recommanded)\n"
        "    -h                    displays this message and exits\n\n"
        "Implements a small Duck Hunt game based on the NES Duck Hunt.\n"
        "Pressing C and V is equivalent to pressing Start and Select.\n"
        "Clicking with the mouse is equivalent to shooting with the "
        "zapper, wherether you're currently pointing at.\n", program_name);
    return (0);
}

int main(int argc, char **argv)
{
    struct game game;

    srand(time(NULL));
    if (argc >= 2 && (my_strcmp(argv[1], "-h") == 0))
        return (usage(argv[0]));
    if (!game_create(&game, argc >= 2 && (my_strncmp(argv[1], "--o", 3)
        == 0))) {
        my_dputs("Failed to initialize game\n", STDERR_FILENO);
        return (ERROR_RETURN_VALUE);
    }
    game_main_loop(&game);
    game_destroy(&game);
}
