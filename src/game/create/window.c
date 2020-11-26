/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_window
*/

#include "internal.h"
#include <SFML/Graphics/RenderWindow.h>

static const int NES_SCREEN_WIDTH = 256;
static const int NES_SCREEN_HEIGHT = 224;
static const int NES_FRAMERATE = 60;

bool make_window(sfRenderWindow **window, bool is_original_resolution)
{
    *window = sfRenderWindow_create(
        (sfVideoMode){NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT,
            sfVideoMode_getDesktopMode().bitsPerPixel},
        "Duck Hunt but it's done with CSFML", sfDefaultStyle, NULL);
    if (*window == NULL)
        return (false);
    if (!is_original_resolution)
        sfRenderWindow_setSize(*window, (sfVector2u){NES_SCREEN_WIDTH * 4,
            NES_SCREEN_HEIGHT * 4});
    sfRenderWindow_setFramerateLimit(*window, NES_FRAMERATE);
    sfRenderWindow_setVerticalSyncEnabled(*window, true);
    return (true);
}
