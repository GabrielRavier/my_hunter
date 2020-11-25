/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "../game.h"
#include "change_music.h"
#include "set/current/round.h"
#include "set/current/score.h"
#include "set/mode.h"
#include "center_text_box_text.h"
#include "update/do.h"
#include "update/end_session.h"
#include "get_duck_speed.h"
#include "handle/key.h"
#include "session_duck/set.h"
#include "session_duck/update.h"
#include "../random.h"
#include "../top_score.h"
#include "../text_utils.h"
#include "dog.h"
#include "round_duck/update.h"
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
#include <SFML/System/Clock.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <inttypes.h>
#include <sys/types.h>

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
            dog_update_flying(self->resources.sprites.dog,
                self->state.frames_since_mode_begin);
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
                dog_update_walking(self->resources.sprites.dog);
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
            session_duck_update(&self->state.session.ducks[i], self);
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
                        session_duck_set_state(&self->state.session.ducks[i], self,
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
    sfClock *clock = sfClock_create();
    int64_t elapsed_time = 0;
    static const int64_t one_frame_in_microseconds = 17000;

    MY_ASSERT(clock != NULL);
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
        elapsed_time += sfClock_getElapsedTime(clock).microseconds;
        sfClock_restart(clock);
        game_update(self);
        game_draw(self);
        elapsed_time -= one_frame_in_microseconds;
        while (elapsed_time > 0) {
            my_dputs("Dropped a frame, skipping a frame...\n", STDERR_FILENO);
            ++self->state.frames_since_mode_begin;
            game_update(self);
            game_draw(self);
            elapsed_time -= one_frame_in_microseconds;
        }
        sfRenderWindow_display(self->window);
        ++self->state.frames_since_mode_begin;
    }
    sfClock_destroy(clock);
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
