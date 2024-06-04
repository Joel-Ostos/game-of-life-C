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
  bool** map;
};

world* init_world(int rows, int columns, float cell_size, Color color)
{
  world* g = (world*) malloc(sizeof(world));
  bool** m = (bool**) malloc(rows * (sizeof(bool*)));
  for (int i = 0; i < rows; i++) {
    m[i] = (bool*) malloc(columns * sizeof(bool*));
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) {
      m[i][j] = 0;
    }
  }
  g->columns = columns;
  g->rows = rows;
  g->cell_size = cell_size;
  g->line_color = color;
  g->map = m;
  return g;
}

void render_world(world* g) {
  for (int i = 0; i < g->rows; i++) {
    for (int j = 0; j < g->columns; j++) {
      if (!(g->map[i][j])) {
	DrawRectangle(i*g->cell_size, j*g->cell_size, g->cell_size, g->cell_size, BLACK);
      } else {
	DrawRectangle(i*g->cell_size, j*g->cell_size, g->cell_size, g->cell_size, WHITE);
      }
    }
  }
}

void reset_world(world* g) 
{
  for (int i = 0; i < g->rows; i++) {
    for (int j = 0; j < g->columns; j++) {
	    g->map[i][j] = 0;
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
				count_neighbours += g->map[nx][ny];
			}
		}
	}
	bool actual_state = g->map[i][j];
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
	} else {
		if (count_neighbours == 3) {
			return 1;
		}
	}
}

void new_state(world* g) 
{
	bool** m = (bool**) malloc(g->rows * (sizeof(bool*)));
	for (int i = 0; i < g->rows; i++) {
		m[i] = (bool*) malloc(g->columns * sizeof(bool*));
	}
	for (int i = 0; i < g->rows; i++) {
		for (int j = 0; j < g->columns; j++) {
			bool state = check_neighbours(g, i, j);	
			if (state) {
				m[i][j] = 1;
			} else {
				m[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < g->rows; i++) {
		free(g->map[i]);
	}
	free(g->map);
	g->map = m;
}

void edit_world(world* g, Vector2 pos, bool s) 
{
	int x = (int) pos.x / CELL_SIZE;
	int y = (int) pos.y / CELL_SIZE;
	g->map[x][y] = s ? 1 : 0;
}

#endif
