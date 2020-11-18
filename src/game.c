/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game.h"
#include "my/stdio.h"
#include "my/stdlib.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

static void game_draw(struct game *self)
{
    sfRenderWindow_drawSprite(self->window, self->title_background_sprite,
        NULL);
}

static void set_top_score(int top_score)
{
    static const int all_wr = 0666;
    int fd = open("top_score.txt", O_WRONLY | O_CREAT | O_TRUNC, all_wr);

    if (fd < 0)
        return;
    my_dprintf(fd, "%d", top_score);
    close(fd);
}

static int get_top_score(void)
{
    FILE *score_file = fopen("top_score.txt", "r");
    char buffer[100];
    int bytes_read;
    int result;

    if (!score_file)
        return 12000;
    bytes_read = fread(buffer, 1, 99, score_file);
    buffer[bytes_read] = '\0';
    result = my_strtol_base_str(buffer, NULL, "0123456789");
    fclose(score_file);
    return result;
}

bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 240, 32},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);
    if (!self->window)
        return (false);
    self->title_background_texture = sfTexture_createFromFile(
        "assets/title.png", NULL);
    if (!self->title_background_texture)
        return (false);
    self->title_background_sprite = sfSprite_create();
    if (!self->title_background_sprite)
        return (false);
    sfSprite_setTexture(self->title_background_sprite, self->title_background_texture, true);
    self->top_score = get_top_score();
    return (true);
}

void game_main_loop(struct game *self)
{
    sfEvent event;

    while (sfRenderWindow_isOpen(self->window)) {
        while (sfRenderWindow_pollEvent(self->window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(self->window);
        }
        sfRenderWindow_clear(self->window, sfBlack);
        game_draw(self);
        sfRenderWindow_display(self->window);
    }
}

void game_destroy(struct game *self)
{
    set_top_score(self->top_score);
    sfSprite_destroy(self->title_background_sprite);
    sfTexture_destroy(self->title_background_texture);
    sfRenderWindow_destroy(self->window);
}
