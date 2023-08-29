
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
#include "nbgl_use_case.h"
#include "nbgl_layout.h"
#include "os_io_seproxyhal.h"
#include "lcx_rng.h"

#include "../globals.h"
#include "menu.h"
#include "draw.h"
#include "shuffle.h"

typedef enum {
  QUIT_APP_BUTTON_TOKEN = 0,
  INFO_BUTTON_TOKEN,
  FLIP_COIN_BUTTON_TOKEN,
  ROLL_DICE_BUTTON_TOKEN,
  SHUFFLE_LIST_BUTTON_TOKEN
} home_tokens_e;

static nbgl_layout_t* homeLayout = NULL;

void app_quit(void) {
  os_sched_exit(-1);
}

void fillHomeLayout(nbgl_layout_t* layout) {
  nbgl_layoutAddTopRightButton(layout, &C_info_i_32px, INFO_BUTTON_TOKEN, TUNE_TAP_CASUAL);

  nbgl_layoutCenteredInfo_t centeredInfo = {
    .text1 = APPNAME,
    .text2 = "Spice up your decisions\nwith true randomness.",
    .icon = NULL,
    .onTop = true,
    .style = LARGE_CASE_INFO,
    .offsetY = 0
  };
  nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

  nbgl_layoutButton_t flipButtonInfo = {
    .text = "Flip a coin",
    .token = FLIP_COIN_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &flipButtonInfo);

  nbgl_layoutButton_t rollButtonInfo = {
    .text = "Roll a dice",
    .token = ROLL_DICE_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &rollButtonInfo);

  nbgl_layoutButton_t shuffleButtonInfo = {
    .text = "Shuffle a list",
    .token = SHUFFLE_LIST_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = false,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &shuffleButtonInfo);

  nbgl_layoutAddFooter(layout, "Quit Alea", QUIT_APP_BUTTON_TOKEN, TUNE_TAP_CASUAL);
}

static const char* const INFO_TYPES[] = {"Version", "Developer"};
static const char* const INFO_CONTENTS[] = {APPVERSION, "Benoit Lucet"};

static bool nav_callback(uint8_t page, nbgl_pageContent_t* content) {
    UNUSED(page);
    content->type = INFOS_LIST;
    content->infosList.nbInfos = 2;
    content->infosList.infoTypes = (const char**) INFO_TYPES;
    content->infosList.infoContents = (const char**) INFO_CONTENTS;
    return true;
}

void ui_menu_about() {
    nbgl_useCaseSettings(
        "About " APPNAME,
        0,
        1,
        true,
        ui_menu_main,
        nav_callback,
        NULL
    );
}

static void homeLayoutTouchCallback(int token, uint8_t index) {
  if (token == QUIT_APP_BUTTON_TOKEN) {
    app_quit();
  } else if (token == INFO_BUTTON_TOKEN) {
    ui_menu_about();
  } else if (token == FLIP_COIN_BUTTON_TOKEN) {
    nbgl_layoutRelease(homeLayout);
    drawDrawPage(DRAW_TYPE_FLIP_COIN, DRAW_STATE_READY, NULL);
  } else if (token == ROLL_DICE_BUTTON_TOKEN) {
    drawDrawPage(DRAW_TYPE_DICE_ROLL, DRAW_STATE_READY, NULL);
  } else if (token == SHUFFLE_LIST_BUTTON_TOKEN) {
    drawShufflePage();
  }
}

void drawHomePage() {
    nbgl_layoutDescription_t layoutDescription = {
      .modal = false,
      .withLeftBorder = true,
      .onActionCallback = &homeLayoutTouchCallback
    };
    homeLayout = nbgl_layoutGet(&layoutDescription);
    fillHomeLayout(homeLayout);
  nbgl_layoutDraw(homeLayout);
}

void ui_menu_main(void) {
  drawHomePage();
}

#endif
