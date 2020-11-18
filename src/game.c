/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game.h"
#include "my/stdio.h"
#include "my/stdlib.h"
#include "my/assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

static void set_top_score(int top_score)
{
    static const int all_rw = 0666;
    int fd = open("top_score.txt", O_WRONLY | O_CREAT | O_TRUNC, all_rw);

    if (fd < 0)
        return;
    my_dprintf(fd, "%d", top_score);
    close(fd);
}

static int get_top_score(void)
{
    FILE *score_file = fopen("top_score.txt", "r");
    char buffer[7];
    int bytes_read;
    int result;

    if (!score_file)
        return 12000;
    bytes_read = fread(buffer, 1, 6, score_file);
    buffer[bytes_read] = '\0';
    result = my_strtol_base_str(buffer, NULL, "0123456789");
    fclose(score_file);
    return result;
}

static void text_set_int(sfText *text, int val)
{
    char *val_as_string;
    MY_ASSERT(my_asprintf(&val_as_string, "%d", val) >= 0);
    sfText_setString(text, val_as_string);
    free(val_as_string);
}

// We can't disable font anti-aliasing. Oh well, I guess the game will only be
// pretty on NES resolution, then ¯\_(ツ)_/¯
bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 240, 32},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);
    if (!self->window)
        return (false);
    sfRenderWindow_setFramerateLimit(self->window, 60);
    self->nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (!self->nes_font)
        return (false);
    self->title_background_texture = sfTexture_createFromFile(
        "assets/title.png", NULL);
    if (!self->title_background_texture)
        return (false);
    self->title_background_sprite = sfSprite_create();
    if (!self->title_background_sprite)
        return (false);
    sfSprite_setTexture(self->title_background_sprite,
        self->title_background_texture, true);
    self->top_score = get_top_score();
    self->top_score_text = sfText_create();
    sfText_setFont(self->top_score_text, self->nes_font);
    sfText_setCharacterSize(self->top_score_text, 8);
    text_set_int(self->top_score_text, self->top_score);
    sfText_setColor(self->top_score_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_score_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_score_text).width, 183});
    self->title_music = sfMusic_createFromFile("assets/title.ogg");
    if (!self->title_music)
        return (false);
    sfMusic_play(self->title_music);
    self->frames = 0;
    return (true);
}

// The music resets every 2800 frames or so
static void game_update(struct game *self)
{
    if ((self->frames % 2800) == 0) {
        sfMusic_destroy(self->title_music);
        self->title_music = sfMusic_createFromFile("assets/title.ogg");
        sfMusic_play(self->title_music);
    }
}

static void game_draw(struct game *self)
{
    sfRenderWindow_drawSprite(self->window, self->title_background_sprite,
        NULL);
    sfRenderWindow_drawText(self->window, self->top_score_text, NULL);
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
        ++self->frames;
        game_update(self);
        game_draw(self);
        sfRenderWindow_display(self->window);
    }
}

void game_destroy(struct game *self)
{
    sfMusic_destroy(self->title_music);
    sfText_destroy(self->top_score_text);
    set_top_score(self->top_score);
    sfSprite_destroy(self->title_background_sprite);
    sfTexture_destroy(self->title_background_texture);
    sfFont_destroy(self->nes_font);
    sfRenderWindow_destroy(self->window);
}
