/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements main
*/

#include "game.h"
#include "my/stdio.h"
#include "my/string.h"
#include <unistd.h>

static int usage(const char *program_name)
{
    my_printf("Usage: %s\n"
              "Implements a small Duck Hunt game\n"
              "Most of the controls are done through mouse and the arrow keys\n"
              , program_name);
    return 0;
}

int main(int argc, char **argv)
{
    struct game game;

    if (argc >= 2 && (my_strcmp(argv[1], "-h") == 0))
        return usage(argv[0]);
    if (!game_create(&game)) {
        my_dputs("Failed to initialize game\n", STDERR_FILENO);
        game_destroy(&game);
        return 84;
    }
    game_main_loop(&game);
    game_destroy(&game);
}
