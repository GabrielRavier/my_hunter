/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares internal create functions
*/

#pragma once

#include "../textures.h"
#include "../sprites.h"
#include "../session_duck.h"
#include "../sounds.h"
#include "../scores.h"
#include "../state.h"
#include "../../game.h"
#include <SFML/Graphics/Types.h>
#include <stdbool.h>

bool make_nes_text(sfText **text, sfFont *nes_font);
bool make_textures(struct game_textures *self);
bool make_sprite(sfSprite **sprite, sfTexture *texture, bool reset_rect);
bool make_sprites(struct game_sprites *self,
    const struct game_textures *textures);
bool make_session_duck(struct session_duck *self, struct game *game);
bool make_sounds(struct game_sounds *self);
bool make_scores(struct game_scores *self, sfFont *nes_font);
bool make_game_state(struct game_state *self, struct game *game);
bool make_window(sfRenderWindow **window, bool is_original_resolution);

