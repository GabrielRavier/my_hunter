/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game.h"
#include "random.h"
#include "top_score.h"
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

static void text_set_printf(sfText *text, const char *format, ...)
{
    char *formatted_string;
    va_list arguments;

    va_start(arguments, format);
    MY_ASSERT(my_vasprintf(&formatted_string, format, arguments) >= 0);
    va_end(arguments);
    sfText_setString(text, formatted_string);
    free(formatted_string);
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
        (sfVideoMode){256, 224, sfVideoMode_getDesktopMode().bitsPerPixel},
        "Duck Hunt but it's done with CSFML", sfResize | sfClose, NULL);
    if (!self->window)
        return (false);
    sfRenderWindow_setFramerateLimit(self->window, 60);
    sfRenderWindow_setVerticalSyncEnabled(self->window, true);
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
    self->fly_away_sprite = sfSprite_create();
    if (!self->fly_away_sprite)
        return (false);
    sfSprite_setTexture(self->fly_away_sprite,
        self->ducks_texture, false);
    sfSprite_setTextureRect(self->fly_away_sprite,
        (sfIntRect){54, 129, 127 - 54, 146 - 129});
    sfSprite_setPosition(self->fly_away_sprite, (sfVector2f){96, 56});
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
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i) {
        self->round_ducks[i].sprite = sfSprite_create();
        if (!self->round_ducks[i].sprite)
            return (false);
        sfSprite_setTexture(self->round_ducks[i].sprite,
            self->background_texture, false);
    }
    self->nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (!self->nes_font)
        return (false);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i) {
        self->ducks[i].sprite = sfSprite_create();
        if (!self->ducks[i].sprite)
            return (false);
        sfSprite_setTexture(self->ducks[i].sprite, self->ducks_texture, false);
        sfSprite_setTextureRect(self->ducks[i].sprite, (sfIntRect){0, 0, 0, 0});
        self->ducks[i].state = DUCK_STATE_INACTIVE;
        self->ducks[i].draw_shoot_rectangle = false;
        self->ducks[i].frames_since_state_change = 0;
        self->ducks[i].score_text = make_nes_text(self->nes_font);
        sfText_setColor(self->ducks[i].score_text,
            sfColor_fromRGB(252, 252, 252));
        sfText_setPosition(self->ducks[i].score_text, (sfVector2f){1000, 1000});
    }
    self->clear_screen_for_shoot = false;
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
    self->flying_sound_buffer =
        sfSoundBuffer_createFromFile("assets/flying.wav");
    self->flying_sound = sfSound_create();
    if (!self->flying_sound)
        return (false);
    sfSound_setBuffer(self->flying_sound, self->flying_sound_buffer);
    sfSound_setLoop(self->flying_sound, true);
    self->gun_shoot_sound_buffer =
        sfSoundBuffer_createFromFile("assets/gun_shoot.wav");
    self->gun_shoot_sound = sfSound_create();
    if (!self->gun_shoot_sound)
        return (false);
    sfSound_setBuffer(self->gun_shoot_sound, self->gun_shoot_sound_buffer);
    self->duck_falling_sound_buffer =
        sfSoundBuffer_createFromFile("assets/duck_falling.wav");
    self->duck_falling_sound = sfSound_create();
    if (!self->duck_falling_sound)
        return (false);
    sfSound_setBuffer(self->duck_falling_sound,
        self->duck_falling_sound_buffer);
    self->duck_thud_sound_buffer =
        sfSoundBuffer_createFromFile("assets/duck_thud.wav");
    self->duck_thud_sound = sfSound_create();
    if (!self->duck_thud_sound)
        return (false);
    sfSound_setBuffer(self->duck_thud_sound,
        self->duck_thud_sound_buffer);
    self->gottem_sound_buffer =
        sfSoundBuffer_createFromFile("assets/gottem.wav");
    self->gottem_sound = sfSound_create();
    if (!self->gottem_sound)
        return (false);
    sfSound_setBuffer(self->gottem_sound,
        self->gottem_sound_buffer);
    self->dog_mocking_sound_buffer =
        sfSoundBuffer_createFromFile("assets/dog_mocking.wav");
    self->dog_mocking_sound = sfSound_create();
    if (!self->dog_mocking_sound)
        return (false);
    sfSound_setBuffer(self->dog_mocking_sound,
        self->dog_mocking_sound_buffer);
    self->round_ducks_move_sound_buffer =
        sfSoundBuffer_createFromFile("assets/round_ducks_move.wav");
    self->round_ducks_move_sound = sfSound_create();
    if (!self->round_ducks_move_sound)
        return (false);
    sfSound_setBuffer(self->round_ducks_move_sound,
        self->round_ducks_move_sound_buffer);
    self->end_round_success_sound_buffer =
        sfSoundBuffer_createFromFile("assets/end_round_success.wav");
    self->end_round_success_sound = sfSound_create();
    if (!self->end_round_success_sound)
        return (false);
    sfSound_setBuffer(self->end_round_success_sound,
        self->end_round_success_sound_buffer);
    self->game_over_sound_buffer =
        sfSoundBuffer_createFromFile("assets/game_over.wav");
    self->game_over_sound = sfSound_create();
    if (!self->game_over_sound)
        return (false);
    sfSound_setBuffer(self->game_over_sound,
        self->game_over_sound_buffer);
    self->game_over_dog_sound_buffer =
        sfSoundBuffer_createFromFile("assets/game_over_dog.wav");
    self->game_over_dog_sound = sfSound_create();
    if (!self->game_over_dog_sound)
        return (false);
    sfSound_setBuffer(self->game_over_dog_sound,
        self->game_over_dog_sound_buffer);
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
    if (self->current_score > self->top_score)
        set_top_score(self->current_score);
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

static void game_update_do_sounds_stop_if_no_ducks_flying(struct game *self)
{
    bool should_stop = true;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        if (self->ducks[i].state == DUCK_STATE_FLYING) {
            should_stop = false;
            break;
        }
    if (should_stop) {
        sfSound_stop(self->flying_sound);
        sfSound_stop(self->duck_sound);
    }
}

static float game_get_duck_speed(struct game *self, struct duck *duck)
{
    float result = 1.5f;

    if (self->current_round == 2)
        result *= 1.07f;
    if (self->current_round == 3)
        result *= 1.2f;
    if (self->current_round == 4)
        result *= 1.5f;
    if (self->current_round == 5)
        result *= 1.65f;
    if (self->current_round == 6)
        result *= 2.f;
    if (self->current_round == 7)
        result *= 2.3f;
    if (self->current_round == 8)
        result *= 2.5f;
    if (self->current_round == 9)
        result *= 3.f;
    if (self->current_round == 10)
        result *= 3.5f;
    if (self->current_round >= 11)
        result *= 6.f;
    if (duck->color == 2)
        result *= 1.25f;
    if (duck->color == 1)
        result *= 1.5f;
    return result;
}

static void game_set_mode(struct game *self, enum game_mode mode)
{
    self->frames_since_mode_begin = 0;
    if (self->mode == GAME_MODE_TITLE) {
        game_set_current_round(self, 0);
        game_set_current_score(self, 0);
        sfSprite_setTextureRect(self->gameplay_background_sprite,
            (sfIntRect){(512 * (self->selected_game == 2)), 8, 255, 224});
    }
    if (self->mode == GAME_MODE_SESSION ||
        self->mode == GAME_MODE_SESSION_FLY_AWAY) {
        if (mode != GAME_MODE_SESSION_FLY_AWAY) {
            sfSound_stop(self->duck_sound);
            sfSound_stop(self->flying_sound);
        }
    }
    if (self->mode == GAME_MODE_END_SESSION) {
        self->current_round_ducks_index += self->selected_game + 1;
        if (self->current_round_ducks_index >= MY_ARRAY_SIZE(self->round_ducks))
            mode = GAME_MODE_END_ROUND;
    }
    self->mode = mode;
    if (self->mode == GAME_MODE_TITLE) {
        game_change_music(self, "assets/title.ogg");
        self->should_draw_text_box = false;
        self->selected_game = 0;
    }
    if (self->mode == GAME_MODE_START_ROUND) {
        game_change_music(self, "assets/start_round.ogg");
        game_set_current_round(self, self->current_round + 1);
        self->should_draw_text_box = true;
        sfText_destroy(self->text_box_text);
        self->text_box_text = make_nes_text(self->nes_font);
        text_set_printf(self->text_box_text, "ROUND\n\n %2d",
            self->current_round);
        sfText_setColor(self->text_box_text, sfColor_fromRGB(252, 252, 252));
        game_center_text_box_text(self);
        sfSprite_setPosition(self->dog_sprite, (sfVector2f){2, 136});
        sfSprite_setTextureRect(self->dog_sprite, (sfIntRect){0, 0, 0, 0});
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i)  {
            sfSprite_setTextureRect(self->round_ducks[i].sprite,
                (sfIntRect){0, 0, 0, 0});
            self->round_ducks[i].state = ROUND_DUCK_STATE_LIVES;
        }
        self->current_round_ducks_index = 0;
        self->shots_left = 3;
    }
    if (self->mode == GAME_MODE_SESSION) {
        self->should_draw_text_box = false;
        sfMusic_destroy(self->current_music);
        self->current_music = NULL;
        self->ducks[0].state = DUCK_STATE_FLYING;
        self->round_ducks[self->current_round_ducks_index].state =
            ROUND_DUCK_STATE_FLYING;
        if (self->selected_game == 1) {
            self->round_ducks[self->current_round_ducks_index + 1].state =
                ROUND_DUCK_STATE_FLYING;
            self->ducks[1].state = DUCK_STATE_FLYING;
        }
        else
            self->ducks[1].state = DUCK_STATE_INACTIVE;
        sfSound_play(self->duck_sound);
        sfSound_play(self->flying_sound);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
            if (self->ducks[i].state != DUCK_STATE_INACTIVE) {
                sfSprite_setPosition(self->ducks[i].sprite, (sfVector2f){
                    random_int_between(0, 256 - (33 - 6)), 184 - (119 - 89)});
                self->ducks[i].color = random_int_between(0, 2);
                self->ducks[i].speed = game_get_duck_speed(self, &self->ducks[i]);
                self->ducks[i].angle = atan2f(120 - sfSprite_getPosition(
                    self->ducks[i].sprite).y, random_int_between(25, 230)
                    - sfSprite_getPosition(self->ducks[i].sprite).x);
            }
        self->shots_left = 3;
    }
    if (self->mode == GAME_MODE_END_ROUND) {
        self->frame_finished_sorting = -1;
        sfSprite_setPosition(self->dog_sprite, (sfVector2f){1000, 1000});
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

static void duck_set_state(struct duck *self, struct game *game,
    enum duck_state state)
{
    self->state = state;
    self->frames_since_state_change = 0;
    if (self->state == DUCK_STATE_FALLING)
        game->round_ducks[(self - game->ducks) +
            game->current_round_ducks_index].state = ROUND_DUCK_STATE_DEAD;
    if (self->state == DUCK_STATE_DEAD) {
        sfText_setPosition(self->score_text, (sfVector2f){1000, 1000});
        sfSound_play(game->duck_thud_sound);
        sfSound_stop(game->duck_falling_sound);
        game->last_duck_fall_x_position = sfSprite_getPosition(self->sprite).x;
    }
    if (self->state == DUCK_STATE_INACTIVE)
        game->round_ducks[(self - game->ducks) +
            game->current_round_ducks_index].state = ROUND_DUCK_STATE_LIVES;
}

static void duck_set_text_position(struct duck *self)
{
    sfFloatRect text_bounds = sfText_getGlobalBounds(self->score_text);
    sfFloatRect duck_bounds = sfSprite_getGlobalBounds(self->sprite);

    sfText_setPosition(self->score_text, (sfVector2f){
        (int)(duck_bounds.left +
             ((duck_bounds.width - text_bounds.width) / 2)),
        (int)(duck_bounds.top +
             ((duck_bounds.height - text_bounds.height) / 2))});
}

static void duck_update(struct duck *self, struct game *game)
{
    int which_sprite = ((game->frames_since_mode_begin % (3 + 3 + 5)) >= 3) +
        ((game->frames_since_mode_begin % (3 + 3 + 5)) >= (3 + 3));
    static const sfIntRect rects[3][8] = {
        {
            {4, 2, 35 - 4, 33 - 2},
            {41, 2, 71 - 41, 33 - 2},
            {72, 2, 98 - 72, 35 - 2},
            {109, 8, 143 - 109, 37 - 8},
            {147, 8, 181 - 147, 38 - 8},
            {184, 8, 218 - 184, 40 - 8},
            {226, 12, 257 - 226, 41 - 12},
            {267, 10, 285 - 267, 41 - 10},
        },
        {
            {8, 45, 35 - 8, 76 - 45},
            {39, 45, 71 - 39, 76 - 45},
            {73, 45, 98 - 73, 78 - 45},
            {107, 48, 141 - 107, 77 - 48},
            {145, 48, 179 - 145, 78 - 48},
            {182, 48, 216 - 182, 80 - 48},
            {224, 52, 255 - 224, 81 - 52},
            {264, 51, 282 - 264, 82 - 51},
        },
        {
            {6, 88, 33 - 6, 119 - 88},
            {37, 88, 69 - 37, 119 - 88},
            {71, 88, 96 - 71, 121 - 88},
            {105, 91, 139 - 105, 120 - 91},
            {143, 91, 177 - 143, 121 - 91},
            {180, 91, 214 - 180, 123 - 91},
            {222, 95, 253 - 222, 124 - 95},
            {263, 93, 281 - 263, 124 - 93},
        }
    };
    sfIntRect final_rect;
    sfVector2f current_position = sfSprite_getPosition(self->sprite);
    sfFloatRect self_bounds;

    ++self->frames_since_state_change;
    if (self->state == DUCK_STATE_FLYING) {
        if (game->shots_left == 0)
            self->angle = -M_PI / 2;
        sfSprite_setPosition(self->sprite, (sfVector2f){
            current_position.x + (self->speed * cosf(self->angle)),
            current_position.y + (self->speed * sinf(self->angle))});
        if (fabsf(sinf(self->angle)) < fabsf(cosf(self->angle)))
            final_rect = rects[self->color][which_sprite + 3];
        else
            final_rect = rects[self->color][which_sprite];
        if (cosf(self->angle) < 0)
            final_rect = (sfIntRect){final_rect.left + final_rect.width,
                final_rect.top, -final_rect.width, final_rect.height};
        sfSprite_setTextureRect(self->sprite, final_rect);
        self_bounds = sfSprite_getGlobalBounds(self->sprite);
        if (game->shots_left == 0) {
            if (((self_bounds.left + self_bounds.width) < 0) ||
               (self_bounds.left > 256) ||
               ((self_bounds.top + self_bounds.height) < 0))
            duck_set_state(self, game, DUCK_STATE_INACTIVE);
        } else {
            if (self_bounds.left < 0) {
                if (cosf(self->angle) < 0) {
                    self->angle = self->angle + M_PI +
                        random_float_between(-0.25f, 0.25f);
                    if (random_int_between(0, 5) != 0)
                        self->angle = -self->angle;
                    duck_update(self, game);
                }
            }
            if ((self_bounds.left + self_bounds.width) > 256) {
                if (cosf(self->angle) > 0) {
                    self->angle = self->angle + M_PI +
                        random_float_between(-0.25f, 0.25f);
                    if (random_int_between(0, 5) != 0)
                        self->angle = -self->angle;
                    duck_update(self, game);
                }
            }
            if (self_bounds.top < 0) {
                if (sinf(self->angle) < 0) {
                    self->angle = -self->angle +
                        random_float_between(-0.25f, 0.25f);
                    duck_update(self, game);
                }
            }
        }
        if ((self_bounds.top + self_bounds.height) > 160) {
            if (sinf(self->angle) > 0) {
                self->angle = -self->angle +
                        random_float_between(-0.25f, 0.25f);
                duck_update(self, game);
            }
        }
    }
    if (self->state == DUCK_STATE_FALLING) {
        if (self->frames_since_state_change < 22) {
            final_rect = rects[self->color][6];
            if (cosf(self->angle) < 0)
                final_rect = (sfIntRect){final_rect.left + final_rect.width,
                    final_rect.top, -final_rect.width, final_rect.height};
            sfSprite_setTextureRect(self->sprite,
                final_rect);
        } else if (self->frames_since_state_change == 22) {
            sfSound_play(game->duck_falling_sound);
            duck_set_text_position(self);
        } else {
            if (self->frames_since_state_change > (22 + 45))
                sfText_setPosition(self->score_text, (sfVector2f){1000, 1000});
            if ((self->frames_since_state_change % 2) == 0)
                ++current_position.x;
            else
                --current_position.x;
            current_position.y += 2;
            sfSprite_setPosition(self->sprite, current_position);
            if (current_position.y > 184 - (119 - 89))
                duck_set_state(self, game, DUCK_STATE_DEAD);
            final_rect = rects[self->color][7];
            if ((self->frames_since_state_change % 10) < 5)
                final_rect = (sfIntRect){final_rect.left + final_rect.width,
                    final_rect.top, -final_rect.width, final_rect.height};
            sfSprite_setTextureRect(self->sprite,
                final_rect);
        }
    }
}

static void round_duck_update(struct round_duck *self, struct game *game,
    size_t i)
{
    sfIntRect rect_living = {96, 209, 7, 7};
    sfIntRect rect_dead = {96, 241, 7, 7};
    sfIntRect black_rect = {96 - 7, 209, 7, 7};
    sfIntRect final_rect;

    sfSprite_setPosition(self->sprite,
        (sfVector2f){96 + (i * 8), 201});
    if (self->state == ROUND_DUCK_STATE_DEAD)
        final_rect = rect_dead;
    else if (self->state == ROUND_DUCK_STATE_LIVES ||
        ((self->state == ROUND_DUCK_STATE_FLYING) &&
        ((game->frames_since_mode_begin % 32) < 16)))
        final_rect = rect_living;
    else
        final_rect = black_rect;
    if (self->state == ROUND_DUCK_STATE_END_ROUND_FLASHING) {
        if ((game->frames_since_mode_begin % 32) < 16)
            final_rect = rect_living;
        else
            final_rect = rect_dead;
    }
    sfSprite_setTextureRect(self->sprite, final_rect);
}

static void game_update_do_change_to_end_session_if_all_ducks_dead(
    struct game *self)
{
    bool should_change = true;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        if (self->ducks[i].state != DUCK_STATE_DEAD &&
            self->ducks[i].state != DUCK_STATE_INACTIVE) {
            should_change = false;
            break;
        }
    if (should_change)
        game_set_mode(self, GAME_MODE_END_SESSION);
}

static void game_update_end_session(struct game *self)
{
    int ducks_dead = 0;
    sfVector2f dog_position = sfSprite_getPosition(self->dog_sprite);

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        ducks_dead += (self->ducks[i].state == DUCK_STATE_DEAD);
    if (self->frames_since_mode_begin == 50) {
        if (self->last_duck_fall_x_position < 44)
            self->last_duck_fall_x_position = 44;
        if (self->last_duck_fall_x_position > 132)
            self->last_duck_fall_x_position = 132;
        sfSprite_setPosition(self->dog_sprite, (sfVector2f){ducks_dead != 0 ?
          self->last_duck_fall_x_position : 112, 161});
        if (ducks_dead == 1)
            sfSprite_setTextureRect(self->dog_sprite,
                (sfIntRect){10 - 22, 264, 44 - (10 - 22), 304 - 264});
        if (ducks_dead >= 2)
            sfSprite_setTextureRect(self->dog_sprite,
            (sfIntRect){52, 264, 108 - 52, 304 - 264});
        if (ducks_dead > 0)
            sfSound_play(self->gottem_sound);
        else
            sfSound_play(self->dog_mocking_sound);
    }
    if (self->frames_since_mode_begin > 50) {
        if (ducks_dead == 0)
            sfSprite_setTextureRect(self->dog_sprite,
                (sfIntRect){2 + (((self->frames_since_mode_begin % 10) < 5) *
                    (34 - 2)), 320, 31 - 2, 360 - 320});
        if (self->frames_since_mode_begin < 77)
            dog_position.y -= 2;
        if (self->frames_since_mode_begin > 97 &&
            self->frames_since_mode_begin < 120)
            dog_position.y += 2;
        sfSprite_setPosition(self->dog_sprite, dog_position);
    }
    if (self->frames_since_mode_begin > 150)
        game_set_mode(self, GAME_MODE_SESSION);
}

static void game_update_end_round(struct game *self)
{
    bool is_finished_sorting = true;
    size_t killed_ducks;
    size_t rounds_threshold_required[19] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7,
        8, 8, 9, 9, 9, 9, 9};

    for (size_t i = 0; i < (MY_ARRAY_SIZE(self->round_ducks) - 1); ++i)
        if (self->round_ducks[i].state == ROUND_DUCK_STATE_LIVES &&
            self->round_ducks[i + 1].state == ROUND_DUCK_STATE_DEAD) {
            is_finished_sorting = false;
            break;
        }
    if (!is_finished_sorting) {
        if (((self->frames_since_mode_begin % 17) == 0)) {
            sfSound_play(self->round_ducks_move_sound);
            for (size_t i = 0; i < (MY_ARRAY_SIZE(self->round_ducks) - 1); ++i)
                if (self->round_ducks[i].state == ROUND_DUCK_STATE_LIVES &&
                    self->round_ducks[i + 1].state == ROUND_DUCK_STATE_DEAD) {
                    self->round_ducks[i].state = ROUND_DUCK_STATE_DEAD;
                    self->round_ducks[i + 1].state = ROUND_DUCK_STATE_LIVES;
                }
        }
    } else {
        if (self->frame_finished_sorting == -1)
            self->frame_finished_sorting = self->frames_since_mode_begin;
        for (killed_ducks = 0; killed_ducks < MY_ARRAY_SIZE(self->round_ducks);
            ++killed_ducks)
            if ((self->round_ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_DEAD) &&
                (self->round_ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_END_ROUND_FLASHING))
                break;
        if (killed_ducks < (self->current_round < 20 ?
            rounds_threshold_required[self->current_round - 1] : 10)) {
            if (((uintmax_t)self->frame_finished_sorting + 16) ==
                self->frames_since_mode_begin) {
                sfSound_play(self->game_over_sound);
                self->should_draw_text_box = true;
                text_set_printf(self->text_box_text, "GAME\n\n OVER");
            }
            if (((uintmax_t)self->frame_finished_sorting + 140) ==
                self->frames_since_mode_begin) {
                sfSound_play(self->game_over_dog_sound);
                sfSprite_setPosition(self->dog_sprite, (sfVector2f){112, 161});
            }
            if ((((uintmax_t)self->frame_finished_sorting + 140) <
                self->frames_since_mode_begin) &&
                (((uintmax_t)self->frame_finished_sorting + 167) >
                self->frames_since_mode_begin)) {
                sfSprite_setPosition(self->dog_sprite, (sfVector2f){
                    sfSprite_getPosition(self->dog_sprite).x,
                    sfSprite_getPosition(self->dog_sprite).y - 2});
            }
            if (((uintmax_t)self->frame_finished_sorting + 370) ==
                self->frames_since_mode_begin)
                game_set_mode(self, GAME_MODE_TITLE);
            sfSprite_setTextureRect(self->dog_sprite,
                (sfIntRect){2 + (((self->frames_since_mode_begin % 17) < 12) *
                    (34 - 2)), 320, 31 - 2, 360 - 320});
        } else {
            if (((uintmax_t)self->frame_finished_sorting + 16) ==
                self->frames_since_mode_begin) {
                sfSound_play(self->end_round_success_sound);
                for (size_t i = 0; i < killed_ducks; ++i)
                    self->round_ducks[i].state =
                        ROUND_DUCK_STATE_END_ROUND_FLASHING;
            }
            if (((uintmax_t)self->frame_finished_sorting + 280) ==
                self->frames_since_mode_begin)
                game_set_mode(self, GAME_MODE_START_ROUND);
        }
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
            game_set_mode(self, GAME_MODE_SESSION);
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
    if (self->mode == GAME_MODE_SESSION && self->frames_since_mode_begin > 260)
        game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
    if (self->mode == GAME_MODE_SESSION ||
        self->mode == GAME_MODE_SESSION_FLY_AWAY) {
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
            duck_update(&self->ducks[i], self);
        game_update_do_sounds_stop_if_no_ducks_flying(self);
        game_update_do_change_to_end_session_if_all_ducks_dead(self);
    }
    if (self->mode == GAME_MODE_SESSION ||
        self->mode == GAME_MODE_START_ROUND ||
        self->mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->mode == GAME_MODE_END_SESSION ||
        self->mode == GAME_MODE_END_ROUND)
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i)
            round_duck_update(&self->round_ducks[i], self, i);
    if (self->mode == GAME_MODE_END_SESSION)
        game_update_end_session(self);
    if (self->mode == GAME_MODE_END_ROUND)
        game_update_end_round(self);
}

static void game_draw_shoot_frame(struct game *self)
{
    sfRectangleShape *duck_white_rectangle = sfRectangleShape_create();
    sfFloatRect duck_bounds;

    MY_ASSERT(duck_white_rectangle);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i) {
        if (self->ducks[i].draw_shoot_rectangle) {
            self->ducks[i].draw_shoot_rectangle = false;
            duck_bounds = sfSprite_getGlobalBounds(
                self->ducks[i].sprite);
            sfRenderWindow_clear(self->window, sfBlack);
            sfRectangleShape_setFillColor(duck_white_rectangle, sfWhite);
            sfRectangleShape_setPosition(duck_white_rectangle,
                (sfVector2f){duck_bounds.left, duck_bounds.top});
            sfRectangleShape_setSize(duck_white_rectangle,
                (sfVector2f){duck_bounds.width, duck_bounds.height});
            sfRenderWindow_drawRectangleShape(self->window,
                duck_white_rectangle, NULL);
        }
    }
    sfRectangleShape_destroy(duck_white_rectangle);
}

static void game_draw_background_color(struct game *self)
{
    sfColor background_color;

    if (self->mode == GAME_MODE_SESSION_FLY_AWAY)
        background_color = sfColor_fromRGB(252, 188, 176);
    else
        background_color = sfColor_fromRGB(60, 188, 252);
    sfRenderWindow_clear(self->window, background_color);
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
    if (self->mode == GAME_MODE_START_ROUND ||
        self->mode == GAME_MODE_SESSION ||
        self->mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->mode == GAME_MODE_END_SESSION ||
        self->mode == GAME_MODE_END_ROUND) {
        game_draw_background_color(self);
        if (self->mode == GAME_MODE_SESSION ||
            self->mode == GAME_MODE_SESSION_FLY_AWAY)
            for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
                if (self->ducks[i].state != DUCK_STATE_INACTIVE)
                    sfRenderWindow_drawSprite(self->window,
                        self->ducks[i].sprite, NULL);
        if (self->mode == GAME_MODE_START_ROUND ||
            self->mode == GAME_MODE_END_SESSION ||
            self->mode == GAME_MODE_END_ROUND)
            if (!(self->mode == GAME_MODE_START_ROUND) ||
                self->frames_since_mode_begin > (368 + 18))
                sfRenderWindow_drawSprite(self->window, self->dog_sprite, NULL);
        sfRenderWindow_drawSprite(self->window,
          self->gameplay_background_sprite, NULL);
        black_rectangle = sfRectangleShape_create();
        MY_ASSERT(black_rectangle);
        sfRectangleShape_setFillColor(black_rectangle, sfBlack);
        sfRectangleShape_setPosition(black_rectangle, (sfVector2f){
            sfText_getPosition(self->current_round_text).x,
            sfText_getPosition(self->current_round_text).y + 1});
        sfRectangleShape_setSize(black_rectangle, (sfVector2f){
            sfText_getGlobalBounds(self->current_round_text).width + 2,
            sfText_getGlobalBounds(self->current_round_text).height + 1});
        sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
        sfRectangleShape_setPosition(black_rectangle, (sfVector2f){46, 200});
        sfRectangleShape_setSize(black_rectangle,
            (sfVector2f){-((3 - self->shots_left) * 8), 8});
        sfRenderWindow_drawRectangleShape(self->window, black_rectangle, NULL);
        if (self->mode == GAME_MODE_SESSION ||
            self->mode == GAME_MODE_SESSION_FLY_AWAY)
            if (self->shots_left == 0) {
                if ((self->frames_since_mode_begin % 16) < 8) {
                    sfRectangleShape_setPosition(black_rectangle,
                        (sfVector2f){24, 208});
                    sfRectangleShape_setSize(black_rectangle,
                        (sfVector2f){24, 8});
                    sfRenderWindow_drawRectangleShape(self->window,
                        black_rectangle, NULL);
                }
            }
        sfRectangleShape_destroy(black_rectangle);
        sfRenderWindow_drawText(self->window, self->current_round_text, NULL);
        sfRenderWindow_drawText(self->window, self->current_score_text, NULL);
        if (self->mode == GAME_MODE_START_ROUND)
            if (self->frames_since_mode_begin <= (368 + 18))
                sfRenderWindow_drawSprite(self->window, self->dog_sprite, NULL);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i)
            sfRenderWindow_drawSprite(self->window,
                self->round_ducks[i].sprite, NULL);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
            sfRenderWindow_drawText(self->window, self->ducks[i].score_text,
                NULL);
    }
    if (self->mode == GAME_MODE_SESSION_FLY_AWAY)
        sfRenderWindow_drawSprite(self->window, self->fly_away_sprite, NULL);
    if (self->should_draw_text_box) {
        sfRenderWindow_drawSprite(self->window, self->text_box_sprite, NULL);
        sfRenderWindow_drawText(self->window, self->text_box_text, NULL);
    }
    if (self->clear_screen_for_shoot) {
        self->clear_screen_for_shoot = false;
        sfRenderWindow_clear(self->window, sfBlack);
    }
    else
        game_draw_shoot_frame(self);
}

static void game_do_score_from_color(struct game *self, struct duck *duck)
{
    static const int score_table[3][3] =
    {
        {500, 1500, 1000},
        {800, 2400, 1600},
        {1000, 3000, 2000}
    };
    int added_score = score_table[0][duck->color];

    if (self->current_round >= 6)
        added_score = score_table[1][duck->color];
    if (self->current_round >= 11)
        added_score = score_table[2][duck->color];
    game_set_current_score(self, self->current_score + added_score);
    text_set_printf(duck->score_text, "%d", added_score);
}

static void game_handle_mouse_press(struct game *self,
     const sfMouseButtonEvent *mouse_button)
{
    sfFloatRect tmp_duck_rect;
    sfVector2f real_mouse_coordinates = sfRenderWindow_mapPixelToCoords(
        self->window, (sfVector2i){mouse_button->x, mouse_button->y}, NULL);
    bool got_none = true;
    bool there_were_ducks = false;

    (void)mouse_button;
    if (self->mode == GAME_MODE_TITLE)
        game_set_mode(self, GAME_MODE_START_ROUND);
    if (self->mode == GAME_MODE_SESSION ||
        self->mode == GAME_MODE_SESSION_FLY_AWAY) {
        if (self->shots_left != 0) {
            --self->shots_left;
            sfSound_play(self->gun_shoot_sound);
            self->clear_screen_for_shoot = true;
            for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
                if (self->ducks[i].state == DUCK_STATE_FLYING) {
                    self->ducks[i].draw_shoot_rectangle = true;
                    tmp_duck_rect = sfSprite_getGlobalBounds(
                        self->ducks[i].sprite);
                    there_were_ducks = true;
                    if (sfFloatRect_contains(&tmp_duck_rect,
                        real_mouse_coordinates.x, real_mouse_coordinates.y)) {
                        duck_set_state(&self->ducks[i], self,
                            DUCK_STATE_FALLING);
                        game_do_score_from_color(self, &self->ducks[i]);
                        got_none = false;
                        break;
                    }
                }
        }
        if ((self->shots_left == 0) && got_none && there_were_ducks) {
            game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
        }
    }
}

void game_main_loop(struct game *self)
{
    sfEvent event;

    game_set_mode(self, GAME_MODE_TITLE);
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
    sfSound_destroy(self->game_over_dog_sound);
    sfSoundBuffer_destroy(self->game_over_dog_sound_buffer);
    sfSound_destroy(self->game_over_sound);
    sfSoundBuffer_destroy(self->game_over_sound_buffer);
    sfSound_destroy(self->end_round_success_sound);
    sfSoundBuffer_destroy(self->end_round_success_sound_buffer);
    sfSound_destroy(self->round_ducks_move_sound);
    sfSoundBuffer_destroy(self->round_ducks_move_sound_buffer);
    sfSound_destroy(self->dog_mocking_sound);
    sfSoundBuffer_destroy(self->dog_mocking_sound_buffer);
    sfSound_destroy(self->gottem_sound);
    sfSoundBuffer_destroy(self->gottem_sound_buffer);
    sfSound_destroy(self->duck_thud_sound);
    sfSoundBuffer_destroy(self->duck_thud_sound_buffer);
    sfSound_destroy(self->duck_falling_sound);
    sfSoundBuffer_destroy(self->duck_falling_sound_buffer);
    sfSound_destroy(self->gun_shoot_sound);
    sfSoundBuffer_destroy(self->gun_shoot_sound_buffer);
    sfSound_destroy(self->flying_sound);
    sfSoundBuffer_destroy(self->flying_sound_buffer);
    sfSound_destroy(self->duck_sound);
    sfSoundBuffer_destroy(self->duck_sound_buffer);
    sfMusic_destroy(self->current_music);
    sfText_destroy(self->text_box_text);
    sfText_destroy(self->current_round_text);
    sfText_destroy(self->current_score_text);
    sfText_destroy(self->top_score_text);
    set_top_score(self->top_score);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i) {
        sfText_destroy(self->ducks[i].score_text);
        sfSprite_destroy(self->ducks[i].sprite);
    }
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i)
        sfSprite_destroy(self->round_ducks[i].sprite);
    sfFont_destroy(self->nes_font);
    sfSprite_destroy(self->fly_away_sprite);
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
