/*
The rules are simple:
  1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
  2. Any live cell with two or three live neighbors lives on to the next generation.
  3. Any live cell with more than three live neighbors dies, as if by overpopulation.
  4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/
#include <raylib.h>
#include <assert.h>
#include "grid.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024


int main(void) 
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of life");

  world* w = init_world(ROWS, COLS, CELL_SIZE, GRAY);
  assert(w != NULL);

  SetTargetFPS(10);
  float elapsed_time = 0;
  float thicks = 1.0f;
  bool play = 0, edit = 0;

  while (!WindowShouldClose()) {
    float delta_t = GetFrameTime();
    elapsed_time += delta_t;

    BeginDrawing();

    ClearBackground(WHITE);


    if (IsKeyPressed(KEY_P)) {
	    play = play ? 0 : 1;
    }

    if (!play) {
	    Vector2 pos = GetMousePosition();
	    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		    edit_world(w, pos, 1);
	    } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		    edit_world(w, pos, 0);
	    }
    }

    if (play) {
	    if (elapsed_time >= thicks) {
		    elapsed_time = 0;
		    new_state(w);
	    }
	    if (IsKeyPressed(KEY_SPACE)) {
		    if (thicks > 0) thicks -= 0.1f;
		    else thicks = 0;
	    }
    }

    if (IsKeyPressed(KEY_R)) {
	    reset_world(w);	
    }

    render_world(w);
    EndDrawing();
  }
}
