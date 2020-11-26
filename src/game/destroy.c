/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_destroy
*/

#include "destroy.h"
#include "my/macros.h"
#include <SFML/Audio/Music.h>
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundBuffer.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Texture.h>

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
