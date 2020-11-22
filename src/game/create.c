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
    self->perfect_box_sprite = sfSprite_create();
    if (!self->perfect_box_sprite)
        return (false);
    sfSprite_setTexture(self->perfect_box_sprite, self->ducks_texture, false);
    sfSprite_setTextureRect(self->perfect_box_sprite,
        (sfIntRect){239, 127, 312 - 239, 160 - 127});
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
    self->perfect_sound_buffer =
        sfSoundBuffer_createFromFile("assets/perfect.wav");
    self->perfect_sound = sfSound_create();
    if (!self->perfect_sound)
        return (false);
    sfSound_setBuffer(self->perfect_sound,
        self->perfect_sound_buffer);
    self->mode = GAME_MODE_NONE;
    return (true);
}
