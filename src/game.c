/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "game/game.h"
#include "random.h"
#include "top_score.h"
#include "text_utils.h"
#include "my/stdio.h"
#include "my/stdlib.h"
#include "my/assert.h"
#include "my/macros.h"
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/RectangleShape.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Texture.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Audio/Music.h>
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundBuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/types.h>

static void game_set_current_round(struct game_round *self, int new_round)
{
    self->as_int = new_round;
    if (self->as_int > 99)
        self->as_int = 1;
    text_set_printf(self->as_text, "%" PRId32, self->as_int);
    sfText_setColor(self->as_text, sfColor_fromRGB(128, 208, 16));
    sfText_setPosition(self->as_text, (sfVector2f){40, 183});
}

static void game_set_current_score(struct game_scores *self, int new_score)
{
    self->current_as_int = new_score;
    if (self->current_as_int > 999999)
        self->current_as_int = 0;
    if (self->current_as_int > self->top_as_int)
        set_top_score(self->current_as_int);
    text_set_printf(self->current_as_text, "%06" PRId32, self->current_as_int);
    sfText_setColor(self->current_as_text, sfColor_fromRGB(252, 252, 252));
    sfText_setPosition(self->current_as_text, (sfVector2f){192, 199});
}

static void game_center_text_box_text(sfText *self)
{
    sfText_setPosition(self, (sfVector2f){109, 64 -
        (int)(sfText_getGlobalBounds(self).height / 2)});
}

static void game_change_music(sfMusic **self, const char *music_name)
{
    sfMusic_destroy(*self);
    *self = sfMusic_createFromFile(music_name);
    MY_ASSERT(*self != NULL);
    sfMusic_play(*self);
}

static void game_update_do_sounds_stop_if_no_ducks_flying(struct game *self)
{
    bool should_stop = true;

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (self->state.session.ducks[i].state == DUCK_STATE_FLYING) {
            should_stop = false;
            break;
        }
    if (should_stop) {
        sfSound_stop(self->resources.sounds.flying.sf_sound);
        sfSound_stop(self->resources.sounds.duck.sf_sound);
    }
}

static float game_get_duck_speed(struct game_state *self,
    struct session_duck *duck)
{
    float result = 1.5f;

    if (self->current_round.as_int == 2)
        result *= 1.07f;
    if (self->current_round.as_int == 3)
        result *= 1.2f;
    if (self->current_round.as_int == 4)
        result *= 1.5f;
    if (self->current_round.as_int == 5)
        result *= 1.65f;
    if (self->current_round.as_int == 6)
        result *= 2.f;
    if (self->current_round.as_int == 7)
        result *= 2.3f;
    if (self->current_round.as_int == 8)
        result *= 2.5f;
    if (self->current_round.as_int == 9)
        result *= 3.f;
    if (self->current_round.as_int == 10)
        result *= 3.5f;
    if (self->current_round.as_int >= 11)
        result *= 6.f;
    if (duck->color == 2)
        result *= 1.25f;
    if (duck->color == 1)
        result *= 1.5f;
    return result;
}

static void game_set_mode(struct game *self, enum game_mode mode)
{
    self->state.frames_since_mode_begin = 0;
    if (self->state.mode == GAME_MODE_TITLE) {
        game_set_current_round(&self->state.current_round, 0);
        game_set_current_score(&self->state.scores, 0);
        sfSprite_setTextureRect(self->resources.sprites.gameplay_background,
            (sfIntRect){(512 * (self->state.title.selected_game == 2)), 8, 255,
            224});
    }
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
        if (mode != GAME_MODE_SESSION_FLY_AWAY) {
            sfSound_stop(self->resources.sounds.duck.sf_sound);
            sfSound_stop(self->resources.sounds.flying.sf_sound);
        }
    }
    if (self->state.mode == GAME_MODE_END_SESSION) {
        self->state.round.current_ducks_index +=
            self->state.title.selected_game + 1;
        if (self->state.round.current_ducks_index >=
            MY_ARRAY_SIZE(self->state.round.ducks))
            mode = GAME_MODE_END_ROUND;
    }
    self->state.mode = mode;
    if (self->state.mode == GAME_MODE_TITLE) {
        game_change_music(&self->state.current_music, "assets/title.ogg");
        self->state.text_box.should_draw = false;
        self->state.title.selected_game = 0;
    }
    if (self->state.mode == GAME_MODE_START_ROUND) {
        game_change_music(&self->state.current_music, "assets/start_round.ogg");
        game_set_current_round(&self->state.current_round,
            self->state.current_round.as_int + 1);
        self->state.text_box.should_draw = true;
        text_set_printf(self->state.text_box.text, "ROUND\n\n %2d",
            self->state.current_round.as_int);
        sfText_setColor(self->state.text_box.text,
            sfColor_fromRGB(252, 252, 252));
        game_center_text_box_text(self->state.text_box.text);
        sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){2, 136});
        sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){0, 0, 0, 0});
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i) {
            sfSprite_setTextureRect(self->state.round.ducks[i].sprite,
                (sfIntRect){0, 0, 0, 0});
            self->state.round.ducks[i].state = ROUND_DUCK_STATE_LIVES;
        }
        self->state.round.current_ducks_index = 0;
    }
    if (self->state.mode == GAME_MODE_SESSION) {
        self->state.text_box.should_draw = false;
        sfMusic_destroy(self->state.current_music);
        self->state.current_music = NULL;
        self->state.session.ducks[0].state = DUCK_STATE_FLYING;
        self->state.round.ducks[self->state.round.current_ducks_index].state =
            ROUND_DUCK_STATE_FLYING;
        if (self->state.title.selected_game == 1) {
            self->state.round.ducks[self->state.round.current_ducks_index + 1]
                .state = ROUND_DUCK_STATE_FLYING;
            self->state.session.ducks[1].state = DUCK_STATE_FLYING;
        }
        else
            self->state.session.ducks[1].state = DUCK_STATE_INACTIVE;
        sfSound_play(self->resources.sounds.duck.sf_sound);
        sfSound_play(self->resources.sounds.flying.sf_sound);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
            if (self->state.session.ducks[i].state != DUCK_STATE_INACTIVE) {
                sfSprite_setPosition(self->state.session.ducks[i].sprite,
                    (sfVector2f){random_int_between(0, 256 - (33 - 6)),
                        184 - (119 - 89)});
                self->state.session.ducks[i].color = random_int_between(0, 2);
                self->state.session.ducks[i].speed =
                    game_get_duck_speed(&self->state,
                        &self->state.session.ducks[i]);
                self->state.session.ducks[i].angle =
                    atan2f(120 - sfSprite_getPosition(
                    self->state.session.ducks[i].sprite).y,
                    random_int_between(25, 230) - sfSprite_getPosition(
                    self->state.session.ducks[i].sprite).x);
            }
        self->state.session.shots_left = 3;
    }
    if (self->state.mode == GAME_MODE_END_ROUND) {
        self->state.round.frame_finished_sorting = -1;
        sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){1000, 1000});
        sfSprite_setPosition(self->resources.sprites.perfect_box,
            (sfVector2f){1000, 1000});
    }
}

static void game_handle_key(struct game *self, sfKeyCode key_code)
{
    if (self->state.mode == GAME_MODE_TITLE) {
        if (key_code == sfKeyV)
            if (++self->state.title.selected_game > 1)
                self->state.title.selected_game = 0;
        if (key_code == sfKeyC)
            game_set_mode(self, GAME_MODE_START_ROUND);
    }
}

static void game_advance_dog(sfSprite *self)
{
    sfVector2f dog_position = sfSprite_getPosition(self);
    sfIntRect dog_rect = sfSprite_getTextureRect(self);

    dog_position.x += 2;
    if (dog_rect.top != 13)
        dog_rect = (sfIntRect){-1, 13, 56, 56 - 13};
    dog_rect.left += 56;
    if (dog_rect.left == ((56 * 4) - 1))
        dog_rect.left = -1;
    sfSprite_setPosition(self, dog_position);
    sfSprite_setTextureRect(self, dog_rect);
}

static void do_flying_dog_movement(struct game *self)
{
    static const int table_x[42] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1};
    static const int table_y[51] = {3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, -1, 0, -1, -1, -2,
        -2, -2, -2, -2, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    int index = self->state.frames_since_mode_begin - 368;

    sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
        sfSprite_getPosition(self->resources.sprites.dog).x +
            (index < 42 ? table_x[index] : 0),
        sfSprite_getPosition(self->resources.sprites.dog).y -
            (index < 51 ? table_y[index] : 0)});
}

static void duck_set_state(struct session_duck *self, struct game *game,
    enum duck_state state)
{
    self->state = state;
    self->frames_since_state_change = 0;
    if (self->state == DUCK_STATE_FALLING)
        game->state.round.ducks[(self - game->state.session.ducks) +
            game->state.round.current_ducks_index].state =
                ROUND_DUCK_STATE_DEAD;
    if (self->state == DUCK_STATE_DEAD) {
        sfText_setPosition(self->score_text, (sfVector2f){1000, 1000});
        sfSound_play(game->resources.sounds.duck_thud.sf_sound);
        sfSound_stop(game->resources.sounds.duck_falling.sf_sound);
        game->state.session.last_duck_fall_x_position =
            sfSprite_getPosition(self->sprite).x;
    }
    if (self->state == DUCK_STATE_INACTIVE)
        game->state.round.ducks[(self - game->state.session.ducks) +
            game->state.round.current_ducks_index].state = ROUND_DUCK_STATE_LIVES;
}

static void duck_set_text_position(struct session_duck *self)
{
    sfFloatRect text_bounds = sfText_getGlobalBounds(self->score_text);
    sfFloatRect duck_bounds = sfSprite_getGlobalBounds(self->sprite);

    sfText_setPosition(self->score_text, (sfVector2f){
        (int)(duck_bounds.left +
             ((duck_bounds.width - text_bounds.width) / 2)),
        (int)(duck_bounds.top +
             ((duck_bounds.height - text_bounds.height) / 2))});
}

static void duck_update(struct session_duck *self, struct game *game)
{
    int which_sprite = ((game->state.frames_since_mode_begin % (3 + 3 + 5)) >=
        3) + ((game->state.frames_since_mode_begin % (3 + 3 + 5)) >= (3 + 3));
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
        if (game->state.session.shots_left == 0)
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
        if (game->state.session.shots_left == 0 ||
            game->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
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
            sfSound_play(game->resources.sounds.duck_falling.sf_sound);
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

static void round_duck_update(struct round_duck *self,
    struct game_state *game_state, size_t i)
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
        ((game_state->frames_since_mode_begin % 32) < 16)))
        final_rect = rect_living;
    else
        final_rect = black_rect;
    if (self->state == ROUND_DUCK_STATE_END_ROUND_FLASHING) {
        if ((game_state->frames_since_mode_begin % 32) < 16)
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

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (self->state.session.ducks[i].state != DUCK_STATE_DEAD &&
            self->state.session.ducks[i].state != DUCK_STATE_INACTIVE) {
            should_change = false;
            break;
        }
    if (should_change)
        game_set_mode(self, GAME_MODE_END_SESSION);
}

static void game_update_end_session(struct game *self)
{
    int ducks_dead = 0;
    sfVector2f dog_position = sfSprite_getPosition(self->resources.sprites.dog);

    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        ducks_dead += (self->state.session.ducks[i].state == DUCK_STATE_DEAD);
    if (self->state.frames_since_mode_begin == 50) {
        if (self->state.session.last_duck_fall_x_position < 44)
            self->state.session.last_duck_fall_x_position = 44;
        if (self->state.session.last_duck_fall_x_position > 132)
            self->state.session.last_duck_fall_x_position = 132;
        sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
          ducks_dead != 0 ? self->state.session.last_duck_fall_x_position : 112,
          161});
        if (ducks_dead == 1)
            sfSprite_setTextureRect(self->resources.sprites.dog,
                (sfIntRect){10 - 22, 264, 44 - (10 - 22), 304 - 264});
        if (ducks_dead >= 2)
            sfSprite_setTextureRect(self->resources.sprites.dog,
            (sfIntRect){52, 264, 108 - 52, 304 - 264});
        if (ducks_dead > 0)
            sfSound_play(self->resources.sounds.gottem.sf_sound);
        else
            sfSound_play(self->resources.sounds.dog_mocking.sf_sound);
    }
    if (self->state.frames_since_mode_begin > 50) {
        if (ducks_dead == 0)
            sfSprite_setTextureRect(self->resources.sprites.dog,
                (sfIntRect){2 + (((self->state.frames_since_mode_begin % 10)
                    < 5) * (34 - 2)), 320, 31 - 2, 360 - 320});
        if (self->state.frames_since_mode_begin < 77)
            dog_position.y -= 2;
        if (self->state.frames_since_mode_begin > 97 &&
            self->state.frames_since_mode_begin < 120)
            dog_position.y += 2;
        sfSprite_setPosition(self->resources.sprites.dog, dog_position);
    }
    if (self->state.frames_since_mode_begin > 150)
        game_set_mode(self, GAME_MODE_SESSION);
}

static void game_update_end_round(struct game *self)
{
    bool is_finished_sorting = true;
    size_t killed_ducks;
    size_t rounds_threshold_required[19] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7,
        8, 8, 9, 9, 9, 9, 9};

    for (size_t i = 0; i < (MY_ARRAY_SIZE(self->state.round.ducks) - 1); ++i)
        if (self->state.round.ducks[i].state == ROUND_DUCK_STATE_LIVES &&
            self->state.round.ducks[i + 1].state == ROUND_DUCK_STATE_DEAD) {
            is_finished_sorting = false;
            break;
        }
    if (!is_finished_sorting) {
        if (((self->state.frames_since_mode_begin % 17) == 0)) {
            sfSound_play(self->resources.sounds.round_ducks_move.sf_sound);
            for (size_t i = 0; i < (MY_ARRAY_SIZE(self->state.round.ducks) - 1);
                ++i)
                if (self->state.round.ducks[i].state == ROUND_DUCK_STATE_LIVES
                    && self->state.round.ducks[i + 1].state ==
                    ROUND_DUCK_STATE_DEAD) {
                    self->state.round.ducks[i].state = ROUND_DUCK_STATE_DEAD;
                    self->state.round.ducks[i + 1].state = ROUND_DUCK_STATE_LIVES;
                }
        }
    } else {
        if (self->state.round.frame_finished_sorting == -1)
            self->state.round.frame_finished_sorting =
                self->state.frames_since_mode_begin;
        for (killed_ducks = 0; killed_ducks <
            MY_ARRAY_SIZE(self->state.round.ducks); ++killed_ducks)
            if ((self->state.round.ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_DEAD) &&
                (self->state.round.ducks[killed_ducks].state !=
                ROUND_DUCK_STATE_END_ROUND_FLASHING))
                break;
        if (killed_ducks < (self->state.current_round.as_int < 20 ?
            rounds_threshold_required[self->state.current_round.as_int - 1] :
            10)) {
            if (((uintmax_t)self->state.round.frame_finished_sorting + 16) ==
                self->state.frames_since_mode_begin) {
                sfSound_play(self->resources.sounds.game_over.sf_sound);
                self->state.text_box.should_draw = true;
                text_set_printf(self->state.text_box.text, "GAME\n\n OVER");
            }
            if (((uintmax_t)self->state.round.frame_finished_sorting + 140) ==
                self->state.frames_since_mode_begin) {
                sfSound_play(self->resources.sounds.game_over_dog.sf_sound);
                sfSprite_setPosition(self->resources.sprites.dog,
                    (sfVector2f){112, 161});
            }
            if ((((uintmax_t)self->state.round.frame_finished_sorting + 140) <
                self->state.frames_since_mode_begin) &&
                (((uintmax_t)self->state.round.frame_finished_sorting + 167) >
                self->state.frames_since_mode_begin)) {
                sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
                    sfSprite_getPosition(self->resources.sprites.dog).x,
                    sfSprite_getPosition(self->resources.sprites.dog).y - 2});
            }
            if (((uintmax_t)self->state.round.frame_finished_sorting + 370) ==
                self->state.frames_since_mode_begin)
                game_set_mode(self, GAME_MODE_TITLE);
            sfSprite_setTextureRect(self->resources.sprites.dog,
                (sfIntRect){2 + (((self->state.frames_since_mode_begin % 17) <
                    12) * (34 - 2)), 320, 31 - 2, 360 - 320});
        } else {
            if (((uintmax_t)self->state.round.frame_finished_sorting + 16) ==
                self->state.frames_since_mode_begin) {
                sfSound_play(self->resources.sounds.end_round_success.sf_sound);
                for (size_t i = 0; i < killed_ducks; ++i)
                    self->state.round.ducks[i].state =
                        ROUND_DUCK_STATE_END_ROUND_FLASHING;
            }
            if (((uintmax_t)self->state.round.frame_finished_sorting + 280) ==
                self->state.frames_since_mode_begin) {
                if (killed_ducks < MY_ARRAY_SIZE(self->state.round.ducks))
                    game_set_mode(self, GAME_MODE_START_ROUND);
                else {
                    game_set_current_score(&self->state.scores,
                        self->state.scores.current_as_int + 10000);
                    sfSound_play(self->resources.sounds.perfect.sf_sound);
                    sfSprite_setPosition(self->resources.sprites.perfect_box,
                        (sfVector2f){96, 48});
                    for (size_t i = 0; i < killed_ducks; ++i)
                        self->state.round.ducks[i].state =
                            ROUND_DUCK_STATE_DEAD;
                }
            }
            if (((uintmax_t)self->state.round.frame_finished_sorting + 450) ==
                self->state.frames_since_mode_begin)
                game_set_mode(self, GAME_MODE_START_ROUND);
        }
    }
}

static bool are_there_flying_ducks(const struct game_session_state *self)
{
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i)
        if (self->ducks[i].state == DUCK_STATE_FLYING)
            return (true);
    return (false);
}

// The music resets every 2800 frames or so
static void game_update(struct game *self)
{
    if (self->state.mode == GAME_MODE_TITLE) {
        if ((self->state.frames_since_mode_begin % 2800) == 0) {
            game_change_music(&self->state.current_music, "assets/title.ogg");
        }
        sfSprite_setPosition(self->resources.sprites.title_cursor,
            (sfVector2f){48, 129 + (self->state.title.selected_game * 16)});
    }
    if (self->state.mode == GAME_MODE_START_ROUND) {
        if (self->state.frames_since_mode_begin > 468)
            game_set_mode(self, GAME_MODE_SESSION);
        else if (self->state.frames_since_mode_begin > 368) {
            if (self->state.frames_since_mode_begin == (368 + 18))
                sfSprite_setTextureRect(self->resources.sprites.dog,
                    (sfIntRect){40, 185, 37, 169 - 121});
            do_flying_dog_movement(self);
        }
        else if (self->state.frames_since_mode_begin == 368) {
            sfSprite_setTextureRect(self->resources.sprites.dog,
                (sfIntRect){0, 185, 37, 169 - 121});
            sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
                sfSprite_getPosition(self->resources.sprites.dog).x, 119});
            game_change_music(&self->state.current_music, "assets/bark.ogg");
        }
        else if ((self->state.frames_since_mode_begin % 7) == 0) {
            if (self->state.frames_since_mode_begin > 343) {
                sfSprite_setTextureRect(self->resources.sprites.dog,
                    (sfIntRect){0, 121, 53, 169 - 121});
                sfSprite_setPosition(self->resources.sprites.dog, (sfVector2f){
                    sfSprite_getPosition(self->resources.sprites.dog).x, 131});
            }
            else if (!(self->state.frames_since_mode_begin > 120 &&
                self->state.frames_since_mode_begin < 170) &&
                !(self->state.frames_since_mode_begin > 285 &&
                self->state.frames_since_mode_begin < 335))
                game_advance_dog(self->resources.sprites.dog);
            else {
                if ((self->state.frames_since_mode_begin % 14) == 0)
                    sfSprite_setTextureRect(self->resources.sprites.dog,
                        (sfIntRect){55, 69, 109 - 55, 112 - 69});
                else
                    sfSprite_setTextureRect(self->resources.sprites.dog,
                        (sfIntRect){-1, 69, 56, 112 - 69});
            }
        }
        if (self->state.frames_since_mode_begin > 130)
            self->state.text_box.should_draw = false;
    }
    if (self->state.mode == GAME_MODE_SESSION &&
        self->state.frames_since_mode_begin > 260 &&
        are_there_flying_ducks(&self->state.session))
        game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
            duck_update(&self->state.session.ducks[i], self);
        game_update_do_sounds_stop_if_no_ducks_flying(self);
        game_update_do_change_to_end_session_if_all_ducks_dead(self);
    }
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_START_ROUND ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->state.mode == GAME_MODE_END_SESSION ||
        self->state.mode == GAME_MODE_END_ROUND)
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
            round_duck_update(&self->state.round.ducks[i], &self->state, i);
    if (self->state.mode == GAME_MODE_END_SESSION)
        game_update_end_session(self);
    if (self->state.mode == GAME_MODE_END_ROUND)
        game_update_end_round(self);
}

static void game_draw_shoot_frame(struct game *self)
{
    sfRectangleShape *duck_white_rectangle = sfRectangleShape_create();
    sfFloatRect duck_bounds;

    MY_ASSERT(duck_white_rectangle);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i) {
        if (self->state.session.ducks[i].draw_shoot_rectangle) {
            self->state.session.ducks[i].draw_shoot_rectangle = false;
            duck_bounds = sfSprite_getGlobalBounds(
                self->state.session.ducks[i].sprite);
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

    if (self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        background_color = sfColor_fromRGB(252, 188, 176);
    else
        background_color = sfColor_fromRGB(60, 188, 252);
    sfRenderWindow_clear(self->window, background_color);
}

static void game_draw(struct game *self)
{
    sfRectangleShape *black_rectangle;

    sfRenderWindow_clear(self->window, sfBlack);
    if (self->state.mode == GAME_MODE_TITLE) {
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.title_background, NULL);
        if (self->state.frames_since_mode_begin >= 1) {
            sfRenderWindow_drawText(self->window,
                self->state.scores.top_as_text, NULL);
            sfRenderWindow_drawSprite(self->window,
                self->resources.sprites.title_cursor, NULL);
        }
    }
    if (self->state.mode == GAME_MODE_START_ROUND ||
        self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY ||
        self->state.mode == GAME_MODE_END_SESSION ||
        self->state.mode == GAME_MODE_END_ROUND) {
        game_draw_background_color(self);
        if (self->state.mode == GAME_MODE_SESSION ||
            self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
            for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks);
                ++i)
                if (self->state.session.ducks[i].state != DUCK_STATE_INACTIVE)
                    sfRenderWindow_drawSprite(self->window,
                        self->state.session.ducks[i].sprite, NULL);
        if (self->state.mode == GAME_MODE_START_ROUND ||
            self->state.mode == GAME_MODE_END_SESSION ||
            self->state.mode == GAME_MODE_END_ROUND)
            if (!(self->state.mode == GAME_MODE_START_ROUND) ||
                self->state.frames_since_mode_begin > (368 + 18))
                sfRenderWindow_drawSprite(self->window,
                    self->resources.sprites.dog, NULL);
        sfRenderWindow_drawSprite(self->window,
          self->resources.sprites.gameplay_background, NULL);
        black_rectangle = sfRectangleShape_create();
        MY_ASSERT(black_rectangle);
        sfRectangleShape_setFillColor(black_rectangle, sfBlack);
        sfRectangleShape_setPosition(black_rectangle, (sfVector2f){
            sfText_getPosition(self->state.current_round.as_text).x,
            sfText_getPosition(self->state.current_round.as_text).y + 1});
        sfRectangleShape_setSize(black_rectangle, (sfVector2f){
            sfText_getGlobalBounds(self->state.current_round.as_text).width + 2,
            sfText_getGlobalBounds(self->state.current_round.as_text).height +
            1});
        sfRenderWindow_drawRectangleShape(self->window, black_rectangle,
            NULL);
        if (self->state.mode != GAME_MODE_START_ROUND) {
            sfRectangleShape_setPosition(black_rectangle, (sfVector2f){46,
                200});
            sfRectangleShape_setSize(black_rectangle,
                (sfVector2f){-((3 - self->state.session.shots_left) * 8), 8});
            sfRenderWindow_drawRectangleShape(self->window, black_rectangle,
                NULL);
        }
        if (self->state.mode == GAME_MODE_SESSION ||
            self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
            if (self->state.session.shots_left == 0) {
                if ((self->state.frames_since_mode_begin % 16) < 8) {
                    sfRectangleShape_setPosition(black_rectangle,
                        (sfVector2f){24, 208});
                    sfRectangleShape_setSize(black_rectangle,
                        (sfVector2f){24, 8});
                    sfRenderWindow_drawRectangleShape(self->window,
                        black_rectangle, NULL);
                }
            }
        sfRectangleShape_destroy(black_rectangle);
        sfRenderWindow_drawText(self->window, self->state.current_round.as_text,
            NULL);
        sfRenderWindow_drawText(self->window,
            self->state.scores.current_as_text, NULL);
        if (self->state.mode == GAME_MODE_START_ROUND)
            if (self->state.frames_since_mode_begin <= (368 + 18))
                sfRenderWindow_drawSprite(self->window,
                    self->resources.sprites.dog, NULL);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
            sfRenderWindow_drawSprite(self->window,
                self->state.round.ducks[i].sprite, NULL);
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
            sfRenderWindow_drawText(self->window,
                self->state.session.ducks[i].score_text, NULL);
    }
    if (self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.fly_away, NULL);
    if (self->state.text_box.should_draw) {
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.text_box, NULL);
        sfRenderWindow_drawText(self->window, self->state.text_box.text, NULL);
    }
    if (self->state.mode == GAME_MODE_END_ROUND)
        sfRenderWindow_drawSprite(self->window,
            self->resources.sprites.perfect_box, NULL);
    if (self->state.session.clear_screen_for_shoot) {
        self->state.session.clear_screen_for_shoot = false;
        sfRenderWindow_clear(self->window, sfBlack);
    }
    else
        game_draw_shoot_frame(self);
}

static void game_do_shot_duck_score(struct game *self, struct session_duck *duck)
{
    static const int score_table[3][3] =
    {
        {500, 1500, 1000},
        {800, 2400, 1600},
        {1000, 3000, 2000}
    };
    int added_score = score_table[0][duck->color];

    if (self->state.current_round.as_int >= 6)
        added_score = score_table[1][duck->color];
    if (self->state.current_round.as_int >= 11)
        added_score = score_table[2][duck->color];
    game_set_current_score(&self->state.scores,
        self->state.scores.current_as_int + added_score);
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
    if (self->state.mode == GAME_MODE_TITLE)
        game_set_mode(self, GAME_MODE_START_ROUND);
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY) {
        if (self->state.session.shots_left != 0) {
            --self->state.session.shots_left;
            sfSound_play(self->resources.sounds.gun_shoot.sf_sound);
            self->state.session.clear_screen_for_shoot = true;
            for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks);
                ++i)
                if (self->state.session.ducks[i].state == DUCK_STATE_FLYING) {
                    self->state.session.ducks[i].draw_shoot_rectangle = true;
                    tmp_duck_rect = sfSprite_getGlobalBounds(
                        self->state.session.ducks[i].sprite);
                    there_were_ducks = true;
                    if (sfFloatRect_contains(&tmp_duck_rect,
                        real_mouse_coordinates.x, real_mouse_coordinates.y)) {
                        duck_set_state(&self->state.session.ducks[i], self,
                            DUCK_STATE_FALLING);
                        game_do_shot_duck_score(self,
                            &self->state.session.ducks[i]);
                        got_none = false;
                        break;
                    }
                }
        }
        if ((self->state.session.shots_left == 0) && got_none &&
            there_were_ducks)
            game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
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
        ++self->state.frames_since_mode_begin;
    }
}

static void destroy_sound_with_buffer(const struct sound_with_buffer *self)
{
    sfSound_destroy(self->sf_sound);
    sfSoundBuffer_destroy(self->buffer);
}

static void game_destroy_sounds(struct game_sounds *self)
{
    destroy_sound_with_buffer(&self->perfect);
    destroy_sound_with_buffer(&self->game_over_dog);
    destroy_sound_with_buffer(&self->game_over);
    destroy_sound_with_buffer(&self->end_round_success);
    destroy_sound_with_buffer(&self->round_ducks_move);
    destroy_sound_with_buffer(&self->dog_mocking);
    destroy_sound_with_buffer(&self->gottem);
    destroy_sound_with_buffer(&self->duck_thud);
    destroy_sound_with_buffer(&self->duck_falling);
    destroy_sound_with_buffer(&self->gun_shoot);
    destroy_sound_with_buffer(&self->flying);
    destroy_sound_with_buffer(&self->duck);
}

static void game_destroy_resources(struct game_resources *self)
{
    game_destroy_sounds(&self->sounds);
    sfSprite_destroy(self->sprites.perfect_box);
    sfSprite_destroy(self->sprites.fly_away);
    sfSprite_destroy(self->sprites.dog);
    sfSprite_destroy(self->sprites.text_box);
    sfSprite_destroy(self->sprites.gameplay_background);
    sfSprite_destroy(self->sprites.title_cursor);
    sfSprite_destroy(self->sprites.title_background);
    sfTexture_destroy(self->textures.dog);
    sfTexture_destroy(self->textures.ducks);
    sfTexture_destroy(self->textures.background);
    sfTexture_destroy(self->textures.cursor);
    sfTexture_destroy(self->textures.title);
    sfFont_destroy(self->nes_font);
}

void game_destroy(struct game *self)
{
    sfMusic_destroy(self->state.current_music);
    sfText_destroy(self->state.text_box.text);
    sfText_destroy(self->state.current_round.as_text);
    sfText_destroy(self->state.scores.current_as_text);
    sfText_destroy(self->state.scores.top_as_text);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i) {
        sfText_destroy(self->state.session.ducks[i].score_text);
        sfSprite_destroy(self->state.session.ducks[i].sprite);
    }
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
        sfSprite_destroy(self->state.round.ducks[i].sprite);
    game_destroy_resources(&self->resources);
    sfRenderWindow_destroy(self->window);
}
