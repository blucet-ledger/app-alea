#pragma once

#include "nbgl_layout.h"

#include "menu.h"

typedef enum {
  DRAW_STATE_READY,
  DRAW_STATE_DRAWING,
  DRAW_STATE_DONE
} draw_states_e;

typedef enum {
  DRAW_TYPE_FLIP_COIN,
  DRAW_TYPE_DICE_ROLL,
  DRAW_TYPE_SHUFFLE_LIST
} draw_types_e;

typedef struct draw_result_s {
  uint32_t value;
} draw_result_t;

void drawDrawPage(draw_types_e drawType, draw_states_e drawState, draw_result_t* result);