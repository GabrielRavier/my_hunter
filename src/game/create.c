/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_create
*/

#include "create.h"
#include "../text_utils.h"
#include "../top_score.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Audio/SoundBuffer.h>
#include <SFML/Graphics/Font.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Text.h>
#include <SFML/Graphics/Texture.h>
#include <SFML/System/Vector2.h>
#include "my/macros.h"
#include <inttypes.h>

static bool make_texture(sfTexture **texture, const char *filename)
{
    *texture = sfTexture_createFromFile(filename, NULL);
    return (*texture != NULL);
}

static bool make_sprite(sfSprite **sprite, sfTexture *texture, bool reset_rect)
{
    *sprite = sfSprite_create();
    if (*sprite == NULL)
        return (false);
    sfSprite_setTexture(*sprite, texture, reset_rect);
    return (true);
}

static bool make_sound(struct sound_with_buffer *sound, const char *filename)
{
    sound->buffer = sfSoundBuffer_createFromFile(filename);
    if (sound->buffer == NULL)
        return (false);
    sound->sf_sound = sfSound_create();
    if (sound->sf_sound == NULL)
        return (false);
    sfSound_setBuffer(sound->sf_sound, sound->buffer);
    return (true);
}

static bool make_nes_text(sfText **text, sfFont *nes_font)
{
    static const int NES_CHARACTER_SIZE = 8;

    *text = sfText_create();
    if (*text == NULL)
        return (false);
    sfText_setFont(*text, nes_font);
    sfText_setCharacterSize(*text, NES_CHARACTER_SIZE);
    return (true);
}

// We can't disable font anti-aliasing. Oh well, I guess the game will only be
// pretty on NES resolution, then ¯\_(ツ)_/¯
bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 224, sfVideoMode_getDesktopMode().bitsPerPixel},
        "Duck Hunt but it's done with CSFML", sfDefaultStyle, NULL);
    if (!self->window)
        return (false);
    sfRenderWindow_setSize(self->window, (sfVector2u){256 * 4, 224 * 4});
    sfRenderWindow_setFramerateLimit(self->window, 60);
    sfRenderWindow_setVerticalSyncEnabled(self->window, true);
    if (!make_texture(&self->title_texture, "assets/title.png"))
        return (false);
    if (!make_texture(&self->cursor_texture, "assets/cursor.png"))
        return (false);
    if (!make_texture(&self->background_texture, "assets/backgrounds.png"))
        return (false);
    if (!make_texture(&self->ducks_texture, "assets/ducks.png"))
        return (false);
    if (!make_texture(&self->dog_texture, "assets/dog.png"))
        return (false);
    if (!make_sprite(&self->title_background_sprite, self->title_texture,
        false))
        return (false);
    if (!make_sprite(&self->title_cursor_sprite, self->cursor_texture, true))
        return (false);
    if (!make_sprite(&self->gameplay_background_sprite,
        self->background_texture, false))
        return (false);
    if (!make_sprite(&self->fly_away_sprite, self->ducks_texture, false))
        return (false);
    sfSprite_setTextureRect(self->fly_away_sprite,
        (sfIntRect){54, 129, 127 - 54, 146 - 129});
    sfSprite_setPosition(self->fly_away_sprite, (sfVector2f){96, 56});
    if (!make_sprite(&self->text_box_sprite, self->ducks_texture, false))
        return (false);
    sfSprite_setTextureRect(self->text_box_sprite,
         (sfIntRect){184, 127, 233 - 184, 160 - 127});
    sfSprite_setPosition(self->text_box_sprite, (sfVector2f){104, 48});
    if (!make_sprite(&self->dog_sprite, self->dog_texture, false))
        return (false);
    sfSprite_setTextureRect(self->dog_sprite, (sfIntRect){0, 0, 0, 0});
    if (!make_sprite(&self->perfect_box_sprite, self->ducks_texture, false))
        return (false);
    sfSprite_setTextureRect(self->perfect_box_sprite,
        (sfIntRect){239, 127, 312 - 239, 160 - 127});
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->round_ducks); ++i)
        if (!make_sprite(&self->round_ducks[i].sprite, self->background_texture,
            false))
            return (false);
    self->nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (!self->nes_font)
        return (false);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->ducks); ++i) {
        if (!make_sprite(&self->ducks[i].sprite, self->ducks_texture, false))
            return (false);
        sfSprite_setTextureRect(self->ducks[i].sprite, (sfIntRect){0, 0, 0, 0});
        self->ducks[i].state = DUCK_STATE_INACTIVE;
        self->ducks[i].draw_shoot_rectangle = false;
        self->ducks[i].frames_since_state_change = 0;
        if (!make_nes_text(&self->ducks[i].score_text, self->nes_font))
            return (false);
        sfText_setColor(self->ducks[i].score_text,
            sfColor_fromRGB(252, 252, 252));
        sfText_setPosition(self->ducks[i].score_text, (sfVector2f){1000, 1000});
    }
    self->clear_screen_for_shoot = false;
    self->top_score = get_top_score();
    if (!make_nes_text(&self->top_score_text, self->nes_font))
        return (false);
    text_set_printf(self->top_score_text, "%" PRId32, self->top_score);
    sfText_setColor(self->top_score_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_score_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_score_text).width, 183});
    if (!make_nes_text(&self->current_score_text, self->nes_font))
        return (false);
    if (!make_nes_text(&self->text_box_text, self->nes_font))
        return (false);
    if (!make_nes_text(&self->current_round_text, self->nes_font))
        return (false);
    self->current_music = NULL;
    if (!make_sound(&self->duck_sound, "assets/duck.wav"))
        return (false);
    sfSound_setLoop(self->duck_sound.sf_sound, true);
    if (!make_sound(&self->flying_sound, "assets/flying.wav"))
        return (false);
    sfSound_setLoop(self->flying_sound.sf_sound, true);
    if (!make_sound(&self->gun_shoot_sound, "assets/gun_shoot.wav"))
        return (false);
    if (!make_sound(&self->duck_falling_sound, "assets/duck_falling.wav"))
        return (false);
    if (!make_sound(&self->duck_thud_sound, "assets/duck_thud.wav"))
        return (false);
    if (!make_sound(&self->gottem_sound, "assets/gottem.wav"))
        return (false);
    if (!make_sound(&self->dog_mocking_sound, "assets/dog_mocking.wav"))
        return (false);
    if (!make_sound(&self->round_ducks_move_sound,
        "assets/round_ducks_move.wav"))
        return (false);
    if (!make_sound(&self->end_round_success_sound,
        "assets/end_round_success.wav"))
        return (false);
    if (!make_sound(&self->game_over_sound, "assets/game_over.wav"))
        return (false);
    if (!make_sound(&self->game_over_dog_sound, "assets/game_over_dog.wav"))
        return (false);
    if (!make_sound(&self->perfect_sound, "assets/perfect.wav"))
        return (false);
    self->mode = GAME_MODE_NONE;
    return (true);
}
