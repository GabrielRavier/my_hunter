/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements main
*/

#include <SFML/Audio.h>
#include "my/stdio.h"
#include "my/string.h"

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
    if (argc >= 2 && (my_strcmp(argv[1], "-h") == 0))
        return usage(argv[0]);
}
