
/*****************************************************************************
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#ifdef HAVE_NBGL

#include "os.h"
#include "glyphs.h"
#include "nbgl_layout.h"
#include "os_io_seproxyhal.h"
#include "lcx_rng.h"

#include "../globals.h"
#include "menu.h"
#include "coinflip.h"

typedef enum {
  HOME_QUIT_APP_BUTTON_TOKEN = 0,
  HOME_FLIP_COIN_BUTTON_TOKEN,
  HOME_ROLL_DICE_BUTTON_TOKEN,
  HOME_SHUFFLE_LIST_BUTTON_TOKEN
} home_tokens_e;

typedef enum {
  DRAW_BACK_BUTTON_TOKEN = 0,
  DRAW_FLIP_COIN_BUTTON_TOKEN
} draw_tokens_e;

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

static nbgl_layout_t* homeLayout;
static nbgl_layout_t* drawLayout;
static nbgl_layout_t* drawingLayout;

void app_quit(void) {
  os_sched_exit(-1);
}

void fillHomeLayout(nbgl_layout_t* layout) {
  nbgl_layoutCenteredInfo_t centeredInfo = {
    .text1 = APPNAME,
    .text2 = "Spice up your decisions\nwith true randomness.",
    .icon = NULL,
    .onTop = true,
    .style = LARGE_CASE_INFO,
    .offsetY = 40
  };
  nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

  nbgl_layoutButton_t flipButtonInfo = {
    .text = "Flip a coin",
    .token = HOME_FLIP_COIN_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false
  };
  nbgl_layoutAddButton(layout, &flipButtonInfo);

  nbgl_layoutButton_t rollButtonInfo = {
    .text = "Roll a dice",
    .token = HOME_ROLL_DICE_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false
  };
  nbgl_layoutAddButton(layout, &rollButtonInfo);

  nbgl_layoutButton_t shuffleButtonInfo = {
    .text = "Shuffle a list",
    .token = HOME_SHUFFLE_LIST_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false
  };
  nbgl_layoutAddButton(layout, &shuffleButtonInfo);

  nbgl_layoutAddFooter(layout, "Quit Alea", HOME_QUIT_APP_BUTTON_TOKEN, TUNE_TAP_CASUAL);
}

void fillDrawLayout(nbgl_layout_t* layout, draw_types_e drawType) {
  nbgl_layoutAddProgressIndicator(layout, 0, 0, true, DRAW_BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);

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
}

void fillDrawingLayout(nbgl_layout_t* layout, draw_types_e drawType) {
  nbgl_layoutAddSpinner(layout, "Tossing coin\ninto the air...", false);
}

static void homeLayoutTouchCallback(int token, uint8_t index) {
  if (token == HOME_QUIT_APP_BUTTON_TOKEN) {
    app_quit();
  } else if (token == HOME_FLIP_COIN_BUTTON_TOKEN) {
    fillDrawLayout(drawLayout, DRAW_TYPE_FLIP_COIN);
    nbgl_layoutDraw(drawLayout);
    nbgl_refresh();
  }
}

static void drawLayoutTouchCallback(int token, uint8_t index) {
  if (token == DRAW_BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(drawLayout);
    fillHomeLayout(homeLayout);
    nbgl_layoutDraw(homeLayout);
  } else if (token == DRAW_FLIP_COIN_BUTTON_TOKEN) {
    // TODO
  }
}

static void drawingTickerCallback(void) {
  // TODO
}

void ui_menu_main(void) {
  // cx_rng_u32_range_func(1, 2, cx_rng_u32);

  nbgl_layoutDescription_t homeLayoutDescription = {
    .modal = false,
    .withLeftBorder = true,
    .onActionCallback = &homeLayoutTouchCallback
  };
  homeLayout = nbgl_layoutGet(&homeLayoutDescription);

  nbgl_layoutDescription_t drawLayoutDescription = {
    .modal = false,
    .withLeftBorder = true,
    .onActionCallback = &drawLayoutTouchCallback
  };
  drawLayout = nbgl_layoutGet(&drawLayoutDescription);

  nbgl_screenTickerConfiguration_t tickerConfiguration = {
    .tickerValue = 1600,
    .tickerIntervale = 0,
    .tickerCallback = &drawingTickerCallback
  };

  nbgl_layoutDescription_t drawingLayoutDescription = {
    .modal = true,
    .withLeftBorder = true,
    .onActionCallback = NULL,
    .ticker = tickerConfiguration
  };
  // drawingLayout = nbgl_layoutGet(&drawingLayoutDescription);


  fillHomeLayout(homeLayout);
  nbgl_layoutDraw(homeLayout);
}

#endif
