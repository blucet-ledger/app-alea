
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

#include "../globals.h"
#include "menu.h"
#include "coinflip.h"

void app_quit(void) {
    // exit app here
    os_sched_exit(-1);
}

enum {
  QUIT_APP_BUTTON_TOKEN = 0,
  FLIP_COIN_BUTTON_TOKEN,
  ROLL_DICE_BUTTON_TOKEN,
  SHUFFLE_LIST_BUTTON_TOKEN
};

static nbgl_layout_t *layout;

// bool rollDice() {
//   uint8_t buf[3];
//   cx_trng_get_random_data(buf, 3);
//   uint32_t roll = 0;
//   for (int i = 0; i < 3; i++) {
//     roll = (roll << 8) | buf[i];
//   }
//   return (roll % 6) + 1;
// }

static void layoutTouchCallback(int token, uint8_t index) {
  if (token == QUIT_APP_BUTTON_TOKEN) {
    app_quit();
  } else if (token == FLIP_COIN_BUTTON_TOKEN) {
    // change some UI and call nbgl_refresh()
    ui_coin_flip();
  } else if (token == ROLL_DICE_BUTTON_TOKEN) {
    // change some UI and call nbgl_refresh()
  } else if (token == SHUFFLE_LIST_BUTTON_TOKEN) {
    // change some UI and call nbgl_refresh()
  }
}

void ui_menu_main(void) {
  nbgl_layoutDescription_t layoutDescription = {
    .modal = false,
    .withLeftBorder = true,
    .onActionCallback = &layoutTouchCallback
  };
  layout = nbgl_layoutGet(&layoutDescription);
  
  nbgl_layoutCenteredInfo_t centeredInfo = {
    .text1 = "Alea",
    .text2 = "Spice up your decisions\nwith true randomness.",
    .icon = NULL,
    .onTop = true,
    .style = LARGE_CASE_INFO,
    .offsetY = 50
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
    .style = WHITE_BACKGROUND,
    .fittingContent = false,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &rollButtonInfo);

  nbgl_layoutButton_t shuffleButtonInfo = {
    .text = "Shuffle a list",
    .token = SHUFFLE_LIST_BUTTON_TOKEN,
    .style = WHITE_BACKGROUND,
    .fittingContent = false,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &shuffleButtonInfo);

  nbgl_layoutAddFooter(
    layout,
    "Quit Alea",
    QUIT_APP_BUTTON_TOKEN,
    TUNE_TAP_CASUAL
  );

  nbgl_layoutDraw(layout);

// #define SETTINGS_BUTTON_DISABLED (false)
//     nbgl_useCaseHome(APPNAME,
//                      &C_app_boilerplate_64px,
//                      "Spice up your decisions\nwith true randomness.",
//                      SETTINGS_BUTTON_DISABLED,
//                      ui_menu_about,
//                      app_quit);
}

// 'About' menu

// static const char* const INFO_TYPES[] = {"Version", "Developer"};
// static const char* const INFO_CONTENTS[] = {APPVERSION, "Benoit Lucet"};

// static bool nav_callback(uint8_t page, nbgl_pageContent_t* content) {
//     UNUSED(page);
//     content->type = INFOS_LIST;
//     content->infosList.nbInfos = 2;
//     content->infosList.infoTypes = (const char**) INFO_TYPES;
//     content->infosList.infoContents = (const char**) INFO_CONTENTS;
//     return true;
// }

// void ui_menu_about() {
// #define TOTAL_PAGE_NB        (1)
// #define INIT_PAGE_INDEX      (0)
// #define DISABLE_SUB_SETTINGS (false)
//     nbgl_useCaseSettings(APPNAME,
//                          INIT_PAGE_INDEX,
//                          TOTAL_PAGE_NB,
//                          DISABLE_SUB_SETTINGS,
//                          ui_menu_main,
//                          nav_callback,
//                          NULL);
// }

#endif
