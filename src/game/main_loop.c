/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements game
*/

#include "main_loop.h"
#include "update.h"
#include "draw.h"
#include "handle/key.h"
#include "handle/mouse_press.h"
#include "set/mode.h"
#include "my/assert.h"
#include "my/stdio.h"
#include <SFML/System/Types.h>
#include <SFML/Graphics/RenderWindow.h>
#include <SFML/System/Clock.h>
#include <SFML/Window/Event.h>
#include <unistd.h>

static void do_events(struct game *self)
{
    sfEvent event;

    while (sfRenderWindow_pollEvent(self->window, &event)) {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(self->window);
        if (event.type == sfEvtMouseButtonPressed)
            game_handle_mouse_press(self, &event.mouseButton);
        if (event.type == sfEvtKeyPressed)
            game_handle_key(self, event.key.code);
    }
}

static void do_updates_until_all_dropped_frames_done(struct game *self,
    sfClock *clock, int64_t *elapsed_time)
{
    static const int64_t one_frame_in_microseconds = 17000;

    *elapsed_time += sfClock_getElapsedTime(clock).microseconds;
    sfClock_restart(clock);
    game_update(self);
    game_draw(self);
    *elapsed_time -= one_frame_in_microseconds;
    while (*elapsed_time > 0) {
        my_dputs("Dropped a frame, skipping a frame...\n", STDERR_FILENO);
        ++self->state.frames_since_mode_begin;
        game_update(self);
        game_draw(self);
        *elapsed_time -= one_frame_in_microseconds;
    }
}

void game_main_loop(struct game *self)
{
    sfClock *clock = sfClock_create();
    int64_t elapsed_time = 0;

    MY_ASSERT(clock != NULL);
    game_set_mode(self, GAME_MODE_TITLE);
    while (sfRenderWindow_isOpen(self->window)) {
        do_events(self);
        do_updates_until_all_dropped_frames_done(self, clock, &elapsed_time);
        sfRenderWindow_display(self->window);
        ++self->state.frames_since_mode_begin;
    }
    sfClock_destroy(clock);
}
