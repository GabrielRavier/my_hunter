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
#include "update.h"
#include "update/do.h"
#include "update/end_session.h"
#include "update/end_round.h"
#include "get_duck_speed.h"
#include "handle/key.h"
#include "session_duck/set.h"
#include "session_duck/update.h"
#include "../random.h"
#include "../top_score.h"
#include "../text_utils.h"
#include "dog.h"
#include "round_duck/update.h"
#include "draw.h"
#include "session_duck/do_score.h"
#include "handle/mouse_press.h"
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
