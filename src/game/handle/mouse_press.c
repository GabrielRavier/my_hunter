/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_handle_mouse_press
*/

#include "mouse_press.h"
#include "../set/mode.h"
#include "../session_duck/set.h"
#include "../session_duck/do_score.h"
#include "my/macros.h"
#include <SFML/Audio/Sound.h>
#include <SFML/Graphics/Rect.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/Graphics/Sprite.h>
#include <SFML/Window/Event.h>

static const int MOCKING_RECT_TOP = 320;
static const uintmax_t FRAME_DOG_MOCKING_CUTSCENE_START_GOING_DOWN = 97;

static bool try_shoot_duck(struct session_duck *duck, struct game *game,
    const sfVector2f real_mouse_coordinates,
    bool got_none_and_there_were_ducks[2])
{
    sfFloatRect tmp_duck_rect;

    if (duck->state == DUCK_STATE_FLYING) {
        duck->draw_shoot_rectangle = true;
        tmp_duck_rect = sfSprite_getGlobalBounds(duck->sprite);
        got_none_and_there_were_ducks[1] = true;
        if (sfFloatRect_contains(&tmp_duck_rect, real_mouse_coordinates.x,
                real_mouse_coordinates.y)) {
            session_duck_set_state(duck, game, DUCK_STATE_FALLING);
            session_duck_do_score(duck, game);
            got_none_and_there_were_ducks[0] = false;
            return (true);
        }
    }
    return (false);
}

static void handle_shooting(struct game *self,
    const sfMouseButtonEvent *mouse_button)
{
    bool got_none_and_there_were_ducks[2] = {true, false};
    const sfVector2f real_mouse_coordinates = sfRenderWindow_mapPixelToCoords(
        self->window, (sfVector2i){mouse_button->x, mouse_button->y}, NULL);
    
    if (self->state.session.shots_left != 0) {
        --self->state.session.shots_left;
        sfSound_play(self->resources.sounds.gun_shoot.sf_sound);
        self->state.session.clear_screen_for_shoot = true;
        for (size_t i = 0; i < MY_ARRAY_SIZE(self->state.session.ducks); ++i)
            if (try_shoot_duck(&self->state.session.ducks[i], self,
                    real_mouse_coordinates, got_none_and_there_were_ducks))
                break;
    }
    if ((self->state.session.shots_left == 0) &&
        got_none_and_there_were_ducks[0] && got_none_and_there_were_ducks[1])
        game_set_mode(self, GAME_MODE_SESSION_FLY_AWAY);
}

static void handle_dog_shooting(struct game *self,
    const sfMouseButtonEvent *mouse_button)
{
    const sfVector2f real_mouse_coordinates = sfRenderWindow_mapPixelToCoords(
        self->window, (sfVector2i){mouse_button->x, mouse_button->y}, NULL);
    sfFloatRect dog_rect = sfSprite_getGlobalBounds(
        self->resources.sprites.dog);

    if (sfFloatRect_contains(&dog_rect, real_mouse_coordinates.x,
        real_mouse_coordinates.y)) {
        sfSound_play(self->resources.sounds.gun_shoot.sf_sound);
        self->state.session.clear_screen_for_shoot = true;
        game_set_mode(self, GAME_MODE_REVENGE);
    }
}

void game_handle_mouse_press(struct game *self,
    const sfMouseButtonEvent *mouse_button)
{
    if (self->state.mode == GAME_MODE_TITLE)
        game_set_mode(self, GAME_MODE_START_ROUND);
    if (self->state.mode == GAME_MODE_SESSION ||
        self->state.mode == GAME_MODE_SESSION_FLY_AWAY)
        handle_shooting(self, mouse_button);
    if (self->state.mode == GAME_MODE_END_SESSION &&
        sfSprite_getTextureRect(self->resources.sprites.dog).top ==
        MOCKING_RECT_TOP && self->state.frames_since_mode_begin <
        (FRAME_DOG_MOCKING_CUTSCENE_START_GOING_DOWN + 20) &&
        self->state.frames_since_mode_begin >
        (FRAME_DOG_MOCKING_CUTSCENE_START_GOING_DOWN - 40))
        handle_dog_shooting(self, mouse_button);
}
