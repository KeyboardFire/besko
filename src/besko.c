/*
 * besko - a roguelike game
 * Copyright (C) 2017  Keyboard Fire <andy@keyboardfire.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "besko.h"

/*
 * load all the data in the assets directory into memory
 */
int load_assets(struct besko *besko) {
    SDL_Surface *tmp;
    tmp = IMG_Load("assets/img/player.png");
    if (tmp) {
        besko->img.player = SDL_ConvertSurface(tmp, besko->surf->format, 0);
        SDL_FreeSurface(tmp);
        if (!besko->img.player) {
            fprintf(stderr, "could not optimize player image\n(SDL error: %s)\n",
                    SDL_GetError());
            return 1;
        }
        SDL_SetColorKey(besko->img.player, SDL_TRUE,
                SDL_MapRGB(besko->surf->format, 0xff, 0x00, 0xff));
    } else {
        fprintf(stderr, "could not load player image\n(SDL/IMG error: %s)\n",
                IMG_GetError());
        return 1;
    }
    return 0;
}

#define FAIL(msg) do { \
    besko_destroy(besko); \
    fprintf(stderr, msg "\n(SDL error: %s)\n", SDL_GetError()); \
    return NULL; \
} while (0)

struct besko *besko_init() {
    // main game struct - basically all information is kept in here
    struct besko *besko = malloc(sizeof *besko);

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        FAIL("failed to initialize SDL");
    }

    // create game window
    besko->win = SDL_CreateWindow("besko",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);
    if (!besko->win) {
        FAIL("window could not be created");
    }

    // get surface
    besko->surf = SDL_GetWindowSurface(besko->win);

    // load assets
    if (load_assets(besko)) FAIL("failed to load assets");

    // temporary
    SDL_FillRect(besko->surf, NULL, SDL_MapRGB(besko->surf->format, 0, 0xff, 0));
    SDL_BlitSurface(besko->img.player, NULL, besko->surf, NULL);
    SDL_UpdateWindowSurface(besko->win);

    return besko;
}

void besko_main_loop(struct besko *besko) {
    SDL_Event e;
    for (;;) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                return;
            default:
                // unknown event, ignore it
                break;
            }
        }
    }
}

void besko_destroy(struct besko *besko) {
    SDL_FreeSurface(besko->img.player);
    SDL_DestroyWindow(besko->win);
    SDL_Quit();
    free(besko);
}
