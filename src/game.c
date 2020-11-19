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
#include <stdarg.h>
#include <inttypes.h>
#include <sys/types.h>

static void set_top_score(int top_score)
{
    static const int all_rw = 0666;
    int fd = open("top_score.txt", O_WRONLY | O_CREAT | O_TRUNC, all_rw);

    if (fd < 0) {
        my_dputs("Failed to open top_score.txt\n", STDERR_FILENO);
        return;
    }
    my_dprintf(fd, "%d", top_score);
    close(fd);
}

static int32_t get_top_score(void)
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

static void text_set_printf(sfText *text, const char *format, ...)
{
    char *val_as_string;
    va_list arguments;

    va_start(arguments, format);
    MY_ASSERT(my_vasprintf(&val_as_string, format, arguments) >= 0);
    va_end(arguments);
    sfText_setString(text, val_as_string);
    free(val_as_string);
}

static sfText *make_nes_text(sfFont *nes_font)
{
    sfText *result = sfText_create();

    MY_ASSERT(result);
    sfText_setFont(result, nes_font);
    sfText_setCharacterSize(result, 8);
    return result;
}

// We can't disable font anti-aliasing. Oh well, I guess the game will only be
// pretty on NES resolution, then ¯\_(ツ)_/¯
bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 224, 32},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);
    if (!self->window)
        return (false);
    sfRenderWindow_setFramerateLimit(self->window, 60);
    self->title_texture = sfTexture_createFromFile(
        "assets/title.png", NULL);
    if (!self->title_texture)
        return (false);
    self->cursor_texture = sfTexture_createFromFile(
        "assets/cursor.png", NULL);
    if (!self->cursor_texture)
        return (false);
    self->background_texture = sfTexture_createFromFile(
        "assets/backgrounds.png", NULL);
    if (!self->background_texture)
        return (false);
    self->title_background_sprite = sfSprite_create();
    if (!self->title_background_sprite)
        return (false);
    sfSprite_setTexture(self->title_background_sprite,
        self->title_texture, true);
    self->title_cursor_sprite = sfSprite_create();
    if (!self->title_cursor_sprite)
        return (false);
    sfSprite_setTexture(self->title_cursor_sprite,
        self->cursor_texture, false);
    self->gameplay_background_sprite = sfSprite_create();
    if (!self->title_cursor_sprite)
        return (false);
    sfSprite_setTexture(self->gameplay_background_sprite,
        self->background_texture, true);
    self->nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (!self->nes_font)
        return (false);
    self->top_score = get_top_score();
    self->top_score_text = make_nes_text(self->nes_font);
    self->current_score_text = NULL;
    text_set_printf(self->top_score_text, "%" PRId32, self->top_score);
    sfText_setColor(self->top_score_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_score_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_score_text).width, 183});
    self->current_round_text = NULL;
    self->current_music = NULL;
    self->mode = GAME_MODE_NONE;
    return (true);
}

static void game_set_current_round(struct game *self, int new_round)
{
    self->current_round = new_round;
    if (self->current_round > 99)
        self->current_round = 1;
    sfText_destroy(self->current_round_text);
    self->current_round_text = make_nes_text(self->nes_font);
    text_set_printf(self->current_round_text, "%" PRId32, self->current_round);
    sfText_setColor(self->current_round_text, sfColor_fromRGB(128, 208, 16));
    sfText_setPosition(self->current_round_text, (sfVector2f){40, 183});
}

static void game_set_current_score(struct game *self, int new_score)
{
    self->current_score = new_score;
    if (self->current_score > 999999)
        self->current_score = 0;
    sfText_destroy(self->current_score_text);
    self->current_score_text = make_nes_text(self->nes_font);
    text_set_printf(self->current_score_text, "%06" PRId32, self->current_score);
    sfText_setColor(self->current_score_text, sfColor_fromRGB(252, 252, 252));
    sfText_setPosition(self->current_score_text, (sfVector2f){192, 199});
}

static void game_set_mode(struct game *self, enum game_mode mode)
{
    if (self->mode == GAME_MODE_TITLE) {
        sfMusic_destroy(self->current_music);
        self->current_music = NULL;
        game_set_current_round(self, 1);
        game_set_current_score(self, 0);
        sfSprite_setTextureRect(self->gameplay_background_sprite,
            (sfIntRect){(512 * (self->selected_game == 2)), 8, 255, 224});
    }
    self->mode = mode;
    if (self->mode == GAME_MODE_TITLE) {
        self->current_music = sfMusic_createFromFile("assets/title.ogg");
        MY_ASSERT(self->current_music);
        sfMusic_play(self->current_music);
    }
    if (self->mode == GAME_MODE_START_ROUND) {
        self->current_music = sfMusic_createFromFile("assets/start_round.ogg");
        MY_ASSERT(self->current_music);
        sfMusic_play(self->current_music);
    }
}

static void game_handle_key(struct game *self, sfKeyCode key_code)
{
    if (self->mode == GAME_MODE_TITLE) {
        if (key_code == sfKeyV)
            if (++self->selected_game > 2)
                self->selected_game = 0;
        if (key_code == sfKeyC)
            game_set_mode(self, GAME_MODE_START_ROUND);
    }
}

static void game_handle_mouse_press(struct game *self,
     const sfMouseButtonEvent *mouse_button)
{
    (void)mouse_button;
    if (self->mode == GAME_MODE_TITLE)
        game_set_mode(self, GAME_MODE_START_ROUND);
}

// The music resets every 2800 frames or so
static void game_update(struct game *self)
{
    if (self->mode == GAME_MODE_TITLE) {
        if ((self->frames_since_mode_begin % 2800) == 0) {
            sfMusic_destroy(self->current_music);
            self->current_music = sfMusic_createFromFile("assets/title.ogg");
            sfMusic_play(self->current_music);
        }
        sfSprite_setPosition(self->title_cursor_sprite, (sfVector2f){48, 129 +
            (self->selected_game * 16)});
    }
}

static void game_draw(struct game *self)
{
    sfRectangleShape *black_rectangle;

    if (self->mode == GAME_MODE_TITLE) {
        sfRenderWindow_drawSprite(self->window, self->title_background_sprite,
                                  NULL);
        if (self->frames_since_mode_begin >= 1) {
            sfRenderWindow_drawText(self->window, self->top_score_text, NULL);
            sfRenderWindow_drawSprite(self->window, self->title_cursor_sprite,
                                      NULL);
        }
    }
    if (self->mode == GAME_MODE_START_ROUND) {
        sfRenderWindow_drawSprite(self->window,
            self->gameplay_background_sprite, NULL);
        black_rectangle = sfRectangleShape_create();
        MY_ASSERT(black_rectangle);
        sfRectangleShape_setFillColor(black_rectangle, sfBlack);
        sfRectangleShape_setPosition(black_rectangle, (sfVector2f){
            sfText_getPosition(self->current_round_text).x,
            sfText_getPosition(self->current_round_text).y + 1,});
        sfRectangleShape_setSize(black_rectangle, (sfVector2f){
            sfText_getGlobalBounds(self->current_round_text).width + 2,
            sfText_getGlobalBounds(self->current_round_text).height + 1});
        sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
        sfRectangleShape_destroy(black_rectangle);
        sfRenderWindow_drawText(self->window, self->current_round_text, NULL);
        sfRenderWindow_drawText(self->window, self->current_score_text, NULL);
    }
}

void game_main_loop(struct game *self)
{
    sfEvent event;

    game_set_mode(self, GAME_MODE_TITLE);
    self->mode = GAME_MODE_TITLE;
    self->selected_game = 0;
    self->frames_since_mode_begin = 0;
    while (sfRenderWindow_isOpen(self->window)) {
        while (sfRenderWindow_pollEvent(self->window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(self->window);
            if (event.type == sfEvtMouseButtonPressed)
                game_handle_mouse_press(self, &event.mouseButton);
            if (event.type == sfEvtKeyPressed)
                game_handle_key(self, event.key.code);
        }
        sfRenderWindow_clear(self->window, sfBlack);
        game_update(self);
        game_draw(self);
        sfRenderWindow_display(self->window);
        ++self->frames_since_mode_begin;
    }
}

void game_destroy(struct game *self)
{
    sfMusic_destroy(self->current_music);
    sfText_destroy(self->current_round_text);
    sfText_destroy(self->current_score_text);
    sfText_destroy(self->top_score_text);
    set_top_score(self->top_score);
    sfFont_destroy(self->nes_font);
    sfSprite_destroy(self->gameplay_background_sprite);
    sfSprite_destroy(self->title_cursor_sprite);
    sfSprite_destroy(self->title_background_sprite);
    sfTexture_destroy(self->background_texture);
    sfTexture_destroy(self->cursor_texture);
    sfTexture_destroy(self->title_texture);
    sfRenderWindow_destroy(self->window);
}
