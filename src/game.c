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
#include "my/macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/types.h>

static int rand_at_most(int max)
{
    unsigned num_bins = (unsigned)max + 1;
    unsigned num_rand = (unsigned)RAND_MAX + 1;
    unsigned bin_size = num_rand / num_bins;
    unsigned defect = num_rand % num_bins;
    int x;

    MY_ASSERT(max < RAND_MAX);
    do {
        x = rand();
    } while (num_rand - defect <= (unsigned)x);
    return (x / bin_size);
}

static int random_int_between(int min, int max)
{
    const int range = max - min;

    MY_ASSERT(min <= max);
    return (rand_at_most(range) + min);
}

static float random_float_between(float min, float max)
{
    float scale = rand() / (float)RAND_MAX;

    MY_ASSERT(min <= max);
    return min + scale * (max - min);
}

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
    self->ducks_texture = sfTexture_createFromFile(
        "assets/ducks.png", NULL);
    if (!self->ducks_texture)
        return (false);
    self->dog_texture = sfTexture_createFromFile(
        "assets/dog.png", NULL);
    if (!self->dog_texture)
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
        self->cursor_texture, true);
    self->gameplay_background_sprite = sfSprite_create();
    if (!self->gameplay_background_sprite)
        return (false);
    sfSprite_setTexture(self->gameplay_background_sprite,
        self->background_texture, false);
    self->text_box_sprite = sfSprite_create();
    if (!self->text_box_sprite)
        return (false);
    sfSprite_setTexture(self->text_box_sprite,
        self->ducks_texture, false);
    sfSprite_setTextureRect(self->text_box_sprite,
         (sfIntRect){184, 127, 233 - 184, 160 - 127});
    sfSprite_setPosition(self->text_box_sprite, (sfVector2f){104, 48});
    self->dog_sprite = sfSprite_create();
    if (!self->dog_sprite)
        return (false);
    sfSprite_setTexture(self->dog_sprite, self->dog_texture, false);
    sfSprite_setTextureRect(self->dog_sprite, (sfIntRect){0, 0, 0, 0});
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i) {
        self->ducks[i].sprite = sfSprite_create();
        if (!self->ducks[i].sprite)
            return (false);
        sfSprite_setTexture(self->ducks[i].sprite, self->ducks_texture, false);
        sfSprite_setTextureRect(self->ducks[i].sprite, (sfIntRect){0, 0, 0, 0});
        self->ducks[i].is_active = false;
    }
    self->nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (!self->nes_font)
        return (false);
    self->top_score = get_top_score();
    self->top_score_text = make_nes_text(self->nes_font);
    text_set_printf(self->top_score_text, "%" PRId32, self->top_score);
    sfText_setColor(self->top_score_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_score_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_score_text).width, 183});
    self->current_score_text = NULL;
    self->text_box_text = NULL;
    self->current_round_text = NULL;
    self->current_music = NULL;
    self->duck_sound_buffer = sfSoundBuffer_createFromFile("assets/duck.wav");
    self->duck_sound = sfSound_create();
    if (!self->duck_sound)
        return (false);
    sfSound_setBuffer(self->duck_sound, self->duck_sound_buffer);
    sfSound_setLoop(self->duck_sound, true);
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

static void game_center_text_box_text(struct game *self)
{
    sfText_setPosition(self->text_box_text, (sfVector2f){109, 64 -
        (int)(sfText_getGlobalBounds(self->text_box_text).height / 2)});
}

static void game_change_music(struct game *self, const char *music_name)
{
    sfMusic_destroy(self->current_music);
    self->current_music = sfMusic_createFromFile(music_name);
    MY_ASSERT(self->current_music);
    sfMusic_play(self->current_music);
}

static void game_set_mode(struct game *self, enum game_mode mode)
{
    self->frames_since_mode_begin = 0;
    if (self->mode == GAME_MODE_TITLE) {
        self->selected_game = 0;
        game_set_current_round(self, 1);
        game_set_current_score(self, 0);
        sfSprite_setTextureRect(self->gameplay_background_sprite,
            (sfIntRect){(512 * (self->selected_game == 2)), 8, 255, 224});
    }
    if (self->mode == GAME_MODE_ROUND) {
        sfSound_stop(self->duck_sound);
    }
    self->mode = mode;
    if (self->mode == GAME_MODE_TITLE) {
        game_change_music(self, "assets/title.ogg");
        self->should_draw_text_box = false;
    }
    if (self->mode == GAME_MODE_START_ROUND) {
        game_change_music(self, "assets/start_round.ogg");
        self->should_draw_text_box = true;
        sfText_destroy(self->text_box_text);
        self->text_box_text = make_nes_text(self->nes_font);
        text_set_printf(self->text_box_text, "ROUND\n\n %2d",
            self->current_round);
        sfText_setColor(self->text_box_text, sfColor_fromRGB(252, 252, 252));
        game_center_text_box_text(self);
        sfSprite_setPosition(self->dog_sprite, (sfVector2f){2, 136});
    }
    if (self->mode == GAME_MODE_ROUND) {
        self->should_draw_text_box = false;
        sfMusic_destroy(self->current_music);
        self->current_music = NULL;
        self->ducks[0].is_active = true;
        self->ducks[1].is_active = (self->selected_game == 1);
        sfSound_play(self->duck_sound);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
            if (self->ducks[i].is_active) {
                sfSprite_setPosition(self->ducks[i].sprite, (sfVector2f){
                    random_int_between(0, 256 - (33 - 6)), 184 - (119 - 89)});
                self->ducks[i].speed = 1.5;
                // Probably has to be corrected, imo rn it's complete shite
                self->ducks[i].angle = atan2f(120 - sfSprite_getPosition(
                    self->ducks[i].sprite).y, random_int_between(25, 230)
                    - sfSprite_getPosition(self->ducks[i].sprite).x);
                self->ducks[i].color = random_int_between(0, 2);
            }
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

static void game_advance_dog(struct game *self)
{
    sfVector2f dog_position = sfSprite_getPosition(self->dog_sprite);
    sfIntRect dog_rect = sfSprite_getTextureRect(self->dog_sprite);

    dog_position.x += 2;
    if (dog_rect.top != 13)
        dog_rect = (sfIntRect){-1, 13, 56, 56 - 13};
    dog_rect.left += 56;
    if (dog_rect.left == ((56 * 4) - 1))
        dog_rect.left = -1;
    sfSprite_setPosition(self->dog_sprite, dog_position);
    sfSprite_setTextureRect(self->dog_sprite, dog_rect);
}

static void do_flying_dog_movement(struct game *self)
{
    static const int table_x[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1};
    static const int table_y[51] = {3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -1, 0, -1, -1, -2,
        -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    int index = self->frames_since_mode_begin - 368;
    sfSprite_setPosition(self->dog_sprite, (sfVector2f){
        sfSprite_getPosition(self->dog_sprite).x +
            (index < 42 ? table_x[index] : 0),
        sfSprite_getPosition(self->dog_sprite).y -
            (index < 51 ? table_y[index] : 0)});
}

static void duck_update(struct duck *self, struct game *game)
{
    int which_sprite = ((game->frames_since_mode_begin % (3 + 3 + 5)) >= 3) +
        ((game->frames_since_mode_begin % (3 + 3 + 5)) >= (3 + 3));
    static const sfIntRect rects[6] = {
        {4, 2, 35 - 4, 33 - 2},
        {41, 2, 70 - 41, 33 - 2},
        {72, 2, 98 - 72, 35 - 2},
        {109, 8, 143 - 109, 37 - 8},
        {147, 8, 181 - 147, 38 - 8},
        {184, 8, 218 - 184, 40 - 8},
    };
    sfIntRect final_rect;

    (void)game;
    sfSprite_setPosition(self->sprite, (sfVector2f){
        sfSprite_getPosition(self->sprite).x + (self->speed * cosf(self->angle)),
        sfSprite_getPosition(self->sprite).y + (self->speed * sinf(self->angle))});
    if (fabsf(sinf(self->angle)) < fabsf(cosf(self->angle)))
        final_rect = rects[which_sprite + 3];
    else
        final_rect = rects[which_sprite];
    final_rect.top += (self->color * 42);
    final_rect.left -= (self->color == 2) * 2;
    if (cosf(self->angle) < 0)
        final_rect = (sfIntRect){final_rect.left + final_rect.width,
            final_rect.top, -final_rect.width, final_rect.height};
    sfSprite_setTextureRect(self->sprite, final_rect);
    if (sfSprite_getGlobalBounds(self->sprite).left < 0) {
        if (cosf(self->angle) < 0) {
            self->angle = self->angle + M_PI;
            if (random_int_between(0, 5) != 0)
                self->angle = -self->angle;
        }
        duck_update(self, game);
    }
    if ((sfSprite_getGlobalBounds(self->sprite).left +
        sfSprite_getGlobalBounds(self->sprite).width) > 256) {
        if (cosf(self->angle) > 0) {
            self->angle = self->angle + M_PI;
            if (random_int_between(0, 5) != 0)
                self->angle = -self->angle;
        }
        duck_update(self, game);
    }
    if (sfSprite_getGlobalBounds(self->sprite).top < 0) {
        if (sinf(self->angle) < 0)
            self->angle = -self->angle;
        duck_update(self, game);
    }
    if ((sfSprite_getGlobalBounds(self->sprite).top +
        sfSprite_getGlobalBounds(self->sprite).height) > 160) {
        if (sinf(self->angle) > 0)
            self->angle = -self->angle;
        duck_update(self, game);
    }
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
    if (self->mode == GAME_MODE_START_ROUND) {
        if (self->frames_since_mode_begin > 468)
            game_set_mode(self, GAME_MODE_ROUND);
        else if (self->frames_since_mode_begin > 368) {
            if (self->frames_since_mode_begin == (368 + 18))
                sfSprite_setTextureRect(self->dog_sprite,
                    (sfIntRect){40, 185, 37, 169 - 121});
            do_flying_dog_movement(self);
        }
        else if (self->frames_since_mode_begin == 368) {
            sfSprite_setTextureRect(self->dog_sprite,
                (sfIntRect){0, 185, 37, 169 - 121});
            sfSprite_setPosition(self->dog_sprite, (sfVector2f){
                sfSprite_getPosition(self->dog_sprite).x, 119});
            game_change_music(self, "assets/bark.ogg");
        }
        else if ((self->frames_since_mode_begin % 7) == 0) {
            if (self->frames_since_mode_begin > 343) {
                sfSprite_setTextureRect(self->dog_sprite,
                    (sfIntRect){0, 121, 53, 169 - 121});
                sfSprite_setPosition(self->dog_sprite, (sfVector2f){
                    sfSprite_getPosition(self->dog_sprite).x, 131});
            }
            else if (!(self->frames_since_mode_begin > 120 &&
                self->frames_since_mode_begin < 170) &&
                !(self->frames_since_mode_begin > 285 &&
                self->frames_since_mode_begin < 335))
                game_advance_dog(self);
            else {
                sfSprite_setTextureRect(self->dog_sprite, (sfIntRect){
                    (56 * ((self->frames_since_mode_begin % 14) == 0)) - 1,
                    69, 56, 112 - 69});
            }
        }
        if (self->frames_since_mode_begin > 130)
            self->should_draw_text_box = false;
    }
    if (self->mode == GAME_MODE_ROUND) {
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
            if (self->ducks[i].is_active)
                duck_update(&self->ducks[i], self);
    }
}

static void game_draw(struct game *self)
{
    sfRectangleShape *black_rectangle;

    sfRenderWindow_clear(self->window, sfBlack);
    if (self->mode == GAME_MODE_TITLE) {
        sfRenderWindow_drawSprite(self->window, self->title_background_sprite,
                                  NULL);
        if (self->frames_since_mode_begin >= 1) {
            sfRenderWindow_drawText(self->window, self->top_score_text, NULL);
            sfRenderWindow_drawSprite(self->window, self->title_cursor_sprite,
                                      NULL);
        }
    }
    if (self->mode == GAME_MODE_START_ROUND || self->mode == GAME_MODE_ROUND) {
        sfRenderWindow_clear(self->window, sfColor_fromRGB(60, 188, 252));
        if (self->mode == GAME_MODE_ROUND)
            for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
                if (self->ducks[i].is_active)
                    sfRenderWindow_drawSprite(self->window,
                        self->ducks[i].sprite, NULL);
        if (self->mode == GAME_MODE_START_ROUND)
            if (self->frames_since_mode_begin > (368 + 18))
                sfRenderWindow_drawSprite(self->window, self->dog_sprite, NULL);
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
        if (self->mode == GAME_MODE_START_ROUND)
            if (self->frames_since_mode_begin <= (368 + 18))
                sfRenderWindow_drawSprite(self->window, self->dog_sprite, NULL);
    }
    if (self->should_draw_text_box) {
        sfRenderWindow_drawSprite(self->window, self->text_box_sprite, NULL);
        sfRenderWindow_drawText(self->window, self->text_box_text, NULL);
    }
}

void game_main_loop(struct game *self)
{
    sfEvent event;

    game_set_mode(self, GAME_MODE_TITLE);
    game_set_mode(self, GAME_MODE_START_ROUND);
    game_set_mode(self, GAME_MODE_ROUND);
    while (sfRenderWindow_isOpen(self->window)) {
        while (sfRenderWindow_pollEvent(self->window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(self->window);
            if (event.type == sfEvtMouseButtonPressed)
                game_handle_mouse_press(self, &event.mouseButton);
            if (event.type == sfEvtKeyPressed)
                game_handle_key(self, event.key.code);
        }
        game_update(self);
        game_draw(self);
        sfRenderWindow_display(self->window);
        ++self->frames_since_mode_begin;
    }
}

void game_destroy(struct game *self)
{
    sfSound_destroy(self->duck_sound);
    sfSoundBuffer_destroy(self->duck_sound_buffer);
    sfMusic_destroy(self->current_music);
    sfText_destroy(self->text_box_text);
    sfText_destroy(self->current_round_text);
    sfText_destroy(self->current_score_text);
    sfText_destroy(self->top_score_text);
    set_top_score(self->top_score);
    sfFont_destroy(self->nes_font);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        sfSprite_destroy(self->ducks[i].sprite);
    sfSprite_destroy(self->dog_sprite);
    sfSprite_destroy(self->text_box_sprite);
    sfSprite_destroy(self->gameplay_background_sprite);
    sfSprite_destroy(self->title_cursor_sprite);
    sfSprite_destroy(self->title_background_sprite);
    sfTexture_destroy(self->dog_texture);
    sfTexture_destroy(self->ducks_texture);
    sfTexture_destroy(self->background_texture);
    sfTexture_destroy(self->cursor_texture);
    sfTexture_destroy(self->title_texture);
    sfRenderWindow_destroy(self->window);
}
