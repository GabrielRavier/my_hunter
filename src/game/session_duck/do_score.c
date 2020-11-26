/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements session_duck_do_score
*/

#include "do_score.h"
#include "../set/current/score.h"
#include "../../text_utils.h"

static const int ROUND_SECOND_SCORES = 6;
static const int ROUND_THIRD_SCORES = 11;

void session_duck_do_score(struct session_duck *self, struct game *game)
{
    static const int score_table[3][3] =
    {
        {500, 1500, 1000},
        {800, 2400, 1600},
        {1000, 3000, 2000}
    };
    int added_score = score_table[0][self->color];

    if (game->state.current_round.as_int >= ROUND_SECOND_SCORES)
        added_score = score_table[1][self->color];
    if (game->state.current_round.as_int >= ROUND_THIRD_SCORES)
        added_score = score_table[2][self->color];
    game_set_current_score(&game->state.scores,
        game->state.scores.current_as_int + added_score);
    text_set_printf(self->score_text, "%d", added_score);
}
