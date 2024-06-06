#ifndef GRID_H
#define GRID_H
#include <raylib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 1024
#define COLS 1024
// ceil size in pixels
#define CELL_SIZE 16

typedef struct world world;

struct world {
  int rows;
  int columns;
  float cell_size;
  Color line_color;
  bool** actual_map;
  bool** new_map;
};

world* init_world(int rows, int columns, float cell_size, Color color)
{
  world* actual_world = (world*) malloc(sizeof(world));
  bool** actual_map = (bool**) malloc(rows * (sizeof(bool*)));
  bool** new_map = (bool**) malloc(rows * (sizeof(bool*)));

  for (int i = 0; i < rows; i++) {
    actual_map[i] = (bool*) malloc(columns * sizeof(bool*));
    new_map[i] = (bool*) malloc(columns * sizeof(bool*));
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      actual_map[i][j] = 0;
      new_map[i][j] = 0;
    }
  }

  actual_world->columns = columns;
  actual_world->rows = rows;
  actual_world->cell_size = cell_size;
  actual_world->line_color = color;
  actual_world->actual_map = actual_map;
  actual_world->new_map = new_map;
  return actual_world;
}

void render_world(world* g) {
  for (int i = 0; i < g->rows; i++) {
    for (int j = 0; j < g->columns; j++) {
      if (!(g->actual_map[i][j])) {
	DrawRectangle(i*g->cell_size, j*g->cell_size, g->cell_size, g->cell_size, BLACK);
      } else {
	DrawRectangle(i*g->cell_size, j*g->cell_size, g->cell_size, g->cell_size, RAYWHITE);
      }
    }
  }
}

void reset_world(world* g) 
{
  for (int i = 0; i < g->rows; i++) {
    for (int j = 0; j < g->columns; j++) {
	    g->actual_map[i][j] = 0;
    }
  }
}

bool check_neighbours(world* g, int i, int j) 
{
	int count_neighbours = 0;
	for (int k = -1; k <= 1; k++) {
		for (int l = -1; l <= 1; l++) {
			if (k == 0 && l == 0) continue;
			int nx = i + k;
			int ny = j + l;
			if (nx >= 0 && nx < g->rows && ny >= 0 && ny < g->columns) {	
				count_neighbours += g->actual_map[nx][ny];
			}
		}
	}
	bool actual_state = g->actual_map[i][j];
	if (actual_state) {
		if (count_neighbours >= 2 && count_neighbours <= 3) {
			return 1;
		}
		if (count_neighbours > 3) {
			return 0;
		}
		if (count_neighbours < 2) {
			return 0;
		}
	}
	if (count_neighbours == 3) {
		return 1;
	}
	return 0;
}

void new_state(world* g) 
{
	for (int i = 0; i < g->rows; i++) {
		for (int j = 0; j < g->columns; j++) {
			bool state = check_neighbours(g, i, j);	
			if (state) {
				g->new_map[i][j] = 1;
			} else {
				g->new_map[i][j] = 0;
			}
		}
	}
	bool** tmp_map = g->actual_map;
	g->actual_map = g->new_map;
	g->new_map = tmp_map;
}

void edit_world(world* g, Vector2 pos, bool s) 
{
	int x = (int) pos.x / CELL_SIZE;
	int y = (int) pos.y / CELL_SIZE;
	g->actual_map[x][y] = s ? 1 : 0;
}

#endif
