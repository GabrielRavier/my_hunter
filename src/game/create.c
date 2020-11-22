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

static bool make_textures(struct game_textures *self)
{
    if (!make_texture(&self->title, "assets/title.png"))
        return (false);
    if (!make_texture(&self->cursor, "assets/cursor.png"))
        return (false);
    if (!make_texture(&self->background, "assets/backgrounds.png"))
        return (false);
    if (!make_texture(&self->ducks, "assets/ducks.png"))
        return (false);
    if (!make_texture(&self->dog, "assets/dog.png"))
        return (false);
    return (true);
}

static bool make_sprites(struct game_sprites *self,
    const struct game_textures *textures)
{
    if (!make_sprite(&self->title_background, textures->title,
        false))
        return (false);
    if (!make_sprite(&self->title_cursor, textures->cursor, true))
        return (false);
    if (!make_sprite(&self->gameplay_background,
        textures->background, false))
        return (false);
    if (!make_sprite(&self->fly_away, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->fly_away,
        (sfIntRect){54, 129, 127 - 54, 146 - 129});
    sfSprite_setPosition(self->fly_away, (sfVector2f){96, 56});
    if (!make_sprite(&self->text_box, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->text_box,
        (sfIntRect){184, 127, 233 - 184, 160 - 127});
    sfSprite_setPosition(self->text_box, (sfVector2f){104, 48});
    if (!make_sprite(&self->dog, textures->dog, false))
        return (false);
    sfSprite_setTextureRect(self->dog, (sfIntRect){0, 0, 0, 0});
    if (!make_sprite(&self->perfect_box, textures->ducks, false))
        return (false);
    sfSprite_setTextureRect(self->perfect_box,
        (sfIntRect){239, 127, 312 - 239, 160 - 127});
    return (true);
}

static bool make_session_duck(struct session_duck *self, struct game *game)
{
    if (!make_sprite(&self->sprite,
        game->resources.textures.ducks, false))
        return (false);
    sfSprite_setTextureRect(self->sprite,
        (sfIntRect){0, 0, 0, 0});
    self->state = DUCK_STATE_INACTIVE;
    self->draw_shoot_rectangle = false;
    self->frames_since_state_change = 0;
    if (!make_nes_text(&self->score_text,
        game->resources.nes_font))
        return (false);
    sfText_setColor(self->score_text,
        sfColor_fromRGB(252, 252, 252));
    sfText_setPosition(self->score_text,
        (sfVector2f){1000, 1000});
    return (true);
}

static bool make_scores(struct game_scores *self, sfFont *nes_font)
{
    self->top_as_int = get_top_score();
    if (!make_nes_text(&self->top_as_text, nes_font))
        return (false);
    text_set_printf(self->top_as_text, "%" PRId32, self->top_as_int);
    sfText_setColor(self->top_as_text, sfColor_fromRGB(76, 220, 72));
    sfText_setPosition(self->top_as_text, (sfVector2f){191 -
        sfText_getGlobalBounds(self->top_as_text).width, 183});
    if (!make_nes_text(&self->current_as_text, nes_font))
        return (false);
    return (true);
}

static bool make_sounds(struct game_sounds *self)
{
    if (!make_sound(&self->duck, "assets/duck.wav"))
        return (false);
    sfSound_setLoop(self->duck.sf_sound, true);
    if (!make_sound(&self->flying, "assets/flying.wav"))
        return (false);
    sfSound_setLoop(self->flying.sf_sound, true);
    if (!make_sound(&self->gun_shoot, "assets/gun_shoot.wav"))
        return (false);
    if (!make_sound(&self->duck_falling, "assets/duck_falling.wav"))
        return (false);
    if (!make_sound(&self->duck_thud, "assets/duck_thud.wav"))
        return (false);
    if (!make_sound(&self->gottem, "assets/gottem.wav"))
        return (false);
    if (!make_sound(&self->dog_mocking, "assets/dog_mocking.wav"))
        return (false);
    if (!make_sound(&self->round_ducks_move,
        "assets/round_ducks_move.wav"))
        return (false);
    if (!make_sound(&self->end_round_success,
        "assets/end_round_success.wav"))
        return (false);
    if (!make_sound(&self->game_over, "assets/game_over.wav"))
        return (false);
    if (!make_sound(&self->game_over_dog, "assets/game_over_dog.wav"))
        return (false);
    if (!make_sound(&self->perfect, "assets/perfect.wav"))
        return (false);
    return (true);
}

// We can't disable font anti-aliasing. Oh well, I guess the game will only be
// pretty on NES resolution, then ¯\_(ツ)_/¯
bool game_create(struct game *self)
{
    self->window = sfRenderWindow_create(
        (sfVideoMode){256, 224, sfVideoMode_getDesktopMode().bitsPerPixel},
        "Duck Hunt but it's done with CSFML", sfDefaultStyle, NULL);
    if (self->window == NULL)
        return (false);
    sfRenderWindow_setSize(self->window, (sfVector2u){256 * 4, 224 * 4});
    sfRenderWindow_setFramerateLimit(self->window, 60);
    sfRenderWindow_setVerticalSyncEnabled(self->window, true);
    if (!make_textures(&self->resources.textures))
        return (false);
    if (!make_sprites(&self->resources.sprites, &self->resources.textures))
        return (false);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.round.ducks); ++i)
        if (!make_sprite(&self->state.round.ducks[i].sprite,
            self->resources.textures.background, false))
            return (false);
    self->resources.nes_font = sfFont_createFromFile("assets/nes_font.ttf");
    if (self->resources.nes_font == NULL)
        return (false);
    for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
        if (!make_session_duck(&self->state.session.ducks[i], self))
            return (false);
    self->state.session.clear_screen_for_shoot = false;
    if (!make_scores(&self->state.scores, self->resources.nes_font))
        return (false);
    if (!make_nes_text(&self->state.text_box.text,
        self->resources.nes_font))
        return (false);
    if (!make_nes_text(&self->state.current_round.as_text,
        self->resources.nes_font))
        return (false);
    self->state.current_music = NULL;
    if (!make_sounds(&self->resources.sounds))
        return (false);
    self->state.mode = GAME_MODE_NONE;
    return (true);
}
