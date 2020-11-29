/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game_update_title
*/

#include "title.h"
#include "../change_music.h"
#include <SFML/Graphics/Sprite.h>

static const uintmax_t FRAMES_PER_TITLE_MUSIC_PLAY = 2800;
static const int TITLE_CURSOR_X = 48;
static const int TITLE_CURSOR_BASE_Y = 129;
static const int TITLE_CURSOR_Y_PER_GAME = 16;

// The music resets every 2800 frames or so
void game_update_title(struct game *self)
{
    if (!(self->state.frames_since_mode_begin % FRAMES_PER_TITLE_MUSIC_PLAY))
        game_change_music(&self->state.current_music, "assets/title.ogg");
    sfSprite_setPosition(self->resources.sprites.title_cursor,
        (sfVector2f){TITLE_CURSOR_X, TITLE_CURSOR_BASE_Y +
            (self->state.title.selected_game * TITLE_CURSOR_Y_PER_GAME)});
}
