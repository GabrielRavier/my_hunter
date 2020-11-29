##
## EPITECH PROJECT, 2020
## myhunter
## File description:
## Makefile for this project
##

# We use `override` to enable setting part of CFLAGS on the command line
# This makes the compiler generate dependency files, which will solve any header-related dependency problems we could have had
override CFLAGS += -MMD -MP -MF $@.d

# We need to be able to include the libmy include files
override CFLAGS += -Iinclude

# We need to be able to link to libmy, and link to it
override LDFLAGS += -Llib $(CFLAGS)

.PHONY: all clean fclean re libmy tests_run

.PREVIOUS: obj/%.o

BINARY_NAME := my_hunter

all: $(BINARY_NAME)

# Sources for this project
SOURCE_FILES := main game/create/create game/create/game_state game/create/nes_text game/create/scores game/create/session_duck game/create/sounds game/create/sprite game/create/sprites game/create/textures game/create/window game/draw/draw game/draw/gameplay/gameplay game/draw/gameplay/background_color game/draw/gameplay/black_rectangles game/draw/shooting/shooting game/draw/shooting/duck_frame game/draw/text_box game/draw/title game/handle/key game/handle/mouse_press game/round_duck/update game/session_duck/do_score game/session_duck/set game/session_duck/update/update game/session_duck/update/falling game/session_duck/update/flying game/set/current/round game/set/current/score game/set/mode/mode game/set/mode/exit game/set/mode/init/init game/set/mode/init/end_round game/set/mode/init/session game/set/mode/init/revenge game/set/mode/init/start_round game/set/mode/init/title game/update/do game/update/end_round/end_round game/update/end_round/after_sorting game/update/end_round/before_sorting game/update/end_round/game_over game/update/end_round/won game/update/update game/update/revenge game/update/start_round game/update/title game/update/end_session game/change_music game/center_text_box_text game/destroy game/dog game/get_duck_speed game/main_loop random top_score text_utils

OBJECT_FILES := $(addprefix obj/, $(addsuffix .o, $(SOURCE_FILES)))

$(BINARY_NAME): libmy $(OBJECT_FILES)
	$(CC) $(LDFLAGS) -o $@ $(OBJECT_FILES) -lmy -lcsfml-graphics -lcsfml-audio -lcsfml-window -lcsfml-system -lm

obj/%.o: src/%.c libmy
	@mkdir --parents obj/game/create
	@mkdir --parents obj/game/draw/gameplay
	@mkdir --parents obj/game/draw/shooting
	@mkdir --parents obj/game/handle
	@mkdir --parents obj/game/round_duck
	@mkdir --parents obj/game/session_duck/update
	@mkdir --parents obj/game/set/current
	@mkdir --parents obj/game/set/mode/init
	@mkdir --parents obj/game/update/end_round
	$(CC) -c $< -o $@ $(CFLAGS)

# Just build the entire libmy when we need these headers
libmy:
	$(MAKE) --directory=lib/my

# Include dependencies for the object files
include $(wildcard obj/*.d)

# Remove all object files
clean:
	rm --recursive --force obj
	$(MAKE) --directory=lib/my clean

# Remove all object, binary and other produced files
fclean: clean
	rm --force $(BINARY_NAME)
	$(MAKE) --directory=lib/my fclean

# "Remakes" the project. This rule is shit and won't work properly with parallel make, but it's not like I'm using this target, and neither is Marvin using parallel Make (afaik)
re: clean all
	$(MAKE) --directory=lib/my re

tests_run:
	$(MAKE) --directory=lib/my tests_binary
	rm -rf obj/
	$(MAKE) CFLAGS="$(CFLAGS) --coverage"
	./lib/my/tests_binary
