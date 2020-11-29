/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Declares game_state
*/

#pragma once

#include "scores.h"
#include "session_state.h"
#include "round_state.h"
#include "revenge_state.h"
#include "round.h"
#include "title_state.h"
#include "text_box.h"
#include <SFML/Audio/Types.h>
#include <stdint.h>

// This is the state of the game
struct game_state {
    struct game_scores scores;
    struct game_session_state session;
    struct game_round_state round;
    struct game_round current_round;
    struct game_title_state title;
    struct game_text_box text_box;
    struct game_revenge_state revenge;
    uintmax_t frames_since_mode_begin;
    enum game_mode {
        GAME_MODE_NONE,
        GAME_MODE_TITLE,
        GAME_MODE_START_ROUND,
        GAME_MODE_SESSION,
        GAME_MODE_SESSION_FLY_AWAY,
        GAME_MODE_END_SESSION,
        GAME_MODE_REVENGE,
        GAME_MODE_END_ROUND
    } mode;
    sfMusic *current_music;
};
