#include <string.h>
#include "lcx_rng.h"

#include "draw.h"
#include "menu.h"

#define DRAW_RESULT_MAX_STR_SIZE 20

static char drawResultStr[DRAW_RESULT_MAX_STR_SIZE];

static nbgl_layout_t* drawLayout = NULL;
static nbgl_layout_t* drawingLayout = NULL;
static draw_types_e currentDrawType;

typedef enum {
  DRAW_BACK_BUTTON_TOKEN = 0,
  DRAW_FLIP_COIN_BUTTON_TOKEN,
  DRAW_NEW_DRAW_BUTTON_TOKEN
} draw_tokens_e;

void fillDrawingLayout(nbgl_layout_t* layout, draw_types_e drawType) {
    // TODO condition text on draw type
    nbgl_layoutCenteredInfo_t info = {
      .text1 = "Hang tight!",
      .text2 = "Your coin is flipping\nin the air...",
      .style = NORMAL_INFO
    };
    nbgl_layoutAddCenteredInfo(layout, &info);

    // FIXME: looks like the spinner ticker messes with the layout ticker,
    // the processing screen is never dismissed after the ticker timeout.
    // nbgl_layoutAddSpinner(layout, "Tossing coin\ninto the air...", false);
}

static void drawingTickerCallback(void) {
  draw_result_t result = {
    .value = cx_rng_u32_range_func(0, 2, cx_rng_u32)
  };

  nbgl_layoutRelease(drawingLayout);
  // FIXME: maybe using `nbgl_screenRedraw()` with proper state handling would be better
  drawDrawPage(currentDrawType, DRAW_STATE_DONE, &result);
}

void drawDrawingPage(draw_types_e drawType) {
    nbgl_screenTickerConfiguration_t tickerConfiguration = {
        .tickerCallback = &drawingTickerCallback,
        .tickerIntervale = 0,
        .tickerValue = 1600
    };

    nbgl_layoutDescription_t layoutDescription = {
        .modal = true,
        .withLeftBorder = true,
        .onActionCallback = NULL,
        .ticker = tickerConfiguration
    };
  
    drawingLayout = nbgl_layoutGet(&layoutDescription);
    fillDrawingLayout(drawingLayout, drawType);
    nbgl_layoutDraw(drawingLayout);
    nbgl_refreshSpecial(FULL_COLOR_PARTIAL_REFRESH);
}

void fillDrawLayout(nbgl_layout_t* layout, draw_types_e drawType, draw_states_e drawState, draw_result_t* result) {
  nbgl_layoutAddProgressIndicator(layout, 0, 0, true, DRAW_BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);

  if (drawState == DRAW_STATE_READY) {
    nbgl_layoutCenteredInfo_t centeredInfo = {
        .text1 = "Heads or tails?",
        .text2 = "Press and hold the coin\nto flip it.",
        .icon = NULL,
        .onTop = true,
        .style = LARGE_CASE_INFO,
        .offsetY = 0
    };
    nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutButton_t flipButtonInfo = {
      .text = "Flip",
      .token = DRAW_FLIP_COIN_BUTTON_TOKEN,
      .style = BLACK_BACKGROUND,
      .fittingContent = true,
      .onBottom = false,
      .tuneId = TUNE_TAP_CASUAL
    };
    nbgl_layoutAddButton(layout, &flipButtonInfo);

  } else if (drawState == DRAW_STATE_DONE) {
    if (result->value == 0) {
      snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "Heads");;
    } else if (result->value == 1) {
      snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "Tails");
    } else {
      // TODO: some kind of crash
      // assert(false);
    }
    // TODO: use this for the dice roll
    // snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "You got: %d", result->value);

    nbgl_layoutCenteredInfo_t centeredInfo = {
        .text1 = drawResultStr,
        .text2 = "Use it wisely.",
        .icon = NULL,
        .onTop = true,
        .style = LARGE_CASE_INFO,
        .offsetY = 0
    };
    nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutAddFooter(layout, "New flip", DRAW_NEW_DRAW_BUTTON_TOKEN, TUNE_TAP_CASUAL);
  } 
}

static void drawLayoutTouchCallback(int token, uint8_t index) {
  if (token == DRAW_BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(drawLayout);
    ui_menu_main();
  } else if (token == DRAW_FLIP_COIN_BUTTON_TOKEN) {
    drawDrawingPage(currentDrawType);
  } else if (token == DRAW_NEW_DRAW_BUTTON_TOKEN) {
    drawDrawPage(currentDrawType, DRAW_STATE_READY, NULL);
  }
}

void drawDrawPage(draw_types_e drawType, draw_states_e drawState, draw_result_t* result) {
    currentDrawType = drawType;

    nbgl_layoutDescription_t layoutDescription = {
        .modal = false,
        .withLeftBorder = true,
        .onActionCallback = &drawLayoutTouchCallback
    };
    drawLayout = nbgl_layoutGet(&layoutDescription);
    fillDrawLayout(drawLayout, drawType, drawState, result);
    nbgl_layoutDraw(drawLayout);
}