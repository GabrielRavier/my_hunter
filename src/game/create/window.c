/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements make_window
*/

#include "internal.h"
#include <SFML/Graphics/RenderWindow.h>

bool make_window(sfRenderWindow **window, bool is_original_resolution)
{
    *window = sfRenderWindow_create(
        (sfVideoMode){256, 224, sfVideoMode_getDesktopMode().bitsPerPixel},
        "Duck Hunt but it's done with CSFML", sfDefaultStyle, NULL);
    if (*window == NULL)
        return (false);
    if (!is_original_resolution)
        sfRenderWindow_setSize(*window, (sfVector2u){256 * 4, 224 * 4});
    sfRenderWindow_setFramerateLimit(*window, 60);
    sfRenderWindow_setVerticalSyncEnabled(*window, true);
    return (true);
}
