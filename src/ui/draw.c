#include <string.h>
#include "os_random.h"
#include "glyphs.h"
#include "nbgl_use_case.h"
#include "os_pic.h"
#include "os_nvm.h"

#include "draw.h"
#include "menu.h"

#define N_storage (*(volatile internalStorage_t *) PIC(&N_storage_real))

typedef struct internalStorage_s {
    uint8_t diceSides;
} internalStorage_t;

const internalStorage_t N_storage_real;

#define TEST_MODE 0
#define TEST_LENGTH 10000

static uint32_t testResult[6];

#define DRAW_RESULT_MAX_STR_SIZE 200

static char drawResultStr[DRAW_RESULT_MAX_STR_SIZE];

static nbgl_layout_t* drawLayout = NULL;
static nbgl_layout_t* drawingLayout = NULL;
static draw_types_e currentDrawType;

typedef enum {
  BACK_BUTTON_TOKEN = 0,
  SETTINGS_BUTTON_TOKEN,
  DRAW_BUTTON_TOKEN,
  NEW_DRAW_BUTTON_TOKEN,
  RADIO_BUTTON_TOKEN
} draw_tokens_e;

enum {
  SETTINGS_RADIO_TOKEN = FIRST_USER_TOKEN
};

void fillDrawingLayout(nbgl_layout_t* layout) {
    // TODO condition text on draw type
    nbgl_layoutCenteredInfo_t info = {
      .text1 = currentDrawType == DRAW_TYPE_FLIP_COIN
        ? "Your coin is flipping\nin the air..."
        : "Your dice is rolling\non the table...",
      .style = NORMAL_INFO
    };
    nbgl_layoutAddCenteredInfo(layout, &info);

    // FIXME: looks like the spinner ticker messes with the layout ticker,
    // the processing screen is never dismissed after the ticker timeout.
    // nbgl_layoutAddSpinner(layout, "Processing...", false);
}

// D4: 1 buffer, mask for 2 bits
// D6: 3 buffers, modulo 6 (default)
// D12: 3 buffers, modulo 12
// D20: 5 buffers, modulo 20
uint32_t drawRandom(draw_types_e drawType) {
  uint32_t result = 0;

  if (drawType == DRAW_TYPE_FLIP_COIN) {
    uint8_t buffer[1];
    cx_get_random_bytes(buffer, 1);
    result = (buffer[0] & 0x01);
  } else {
    uint8_t buffer[3];
    cx_get_random_bytes(buffer, 3);
    for (size_t i = 0; i < 3; i++) {
      result = (result << 8) | buffer[i];
    }
    result = result % 6;
  }
  return result;
}

static void drawingTickerCallback(void) {
  #if TEST_MODE
    memset(testResult, 0, 6*sizeof(testResult[0]));
    for (int i = 0; i < TEST_LENGTH; i++) {
      testResult[drawRandom(DRAW_TYPE_DICE_ROLL)] += 1;
    }
    nbgl_layoutRelease(drawingLayout);
    drawDrawPage(currentDrawType, DRAW_STATE_DONE, NULL);
  #else
    draw_result_t resultStruct = {
      .value = drawRandom(currentDrawType)
    };

    nbgl_layoutRelease(drawingLayout);
    // TODO: maybe using `nbgl_screenRedraw()` with state change (of draw_state_e) would be better
    drawDrawPage(currentDrawType, DRAW_STATE_DONE, &resultStruct);
  #endif
}

void drawDrawingPage() {
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
    fillDrawingLayout(drawingLayout);
    nbgl_layoutDraw(drawingLayout);
    nbgl_refreshSpecial(FULL_COLOR_PARTIAL_REFRESH);
}

void fillDrawLayout(nbgl_layout_t* layout, draw_states_e drawState, draw_result_t* result) {
  nbgl_layoutAddProgressIndicator(layout, 0, 0, true, BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);

  if (currentDrawType == DRAW_TYPE_DICE_ROLL && drawState == DRAW_STATE_READY) {
    nbgl_layoutAddTopRightButton(layout, &C_wheel32px, SETTINGS_BUTTON_TOKEN, TUNE_TAP_CASUAL);
  }
  
  if (drawState == DRAW_STATE_READY) {
    nbgl_layoutCenteredInfo_t centeredInfo = {
        .text1 = currentDrawType == DRAW_TYPE_FLIP_COIN
          ? "Heads or tails?"
          : "Six-sided dice",
        .text2 = currentDrawType == DRAW_TYPE_FLIP_COIN
          ? "Press and hold the coin\nto flip it."
          : "Press and hold the dice\nto roll it.",
        .icon = NULL,
        .onTop = true,
        .style = LARGE_CASE_INFO,
        .offsetY = 50
    };
    nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutButton_t flipButtonInfo = {
      .text = currentDrawType == DRAW_TYPE_FLIP_COIN
        ? "Flip"
        : "Roll",
      .token = DRAW_BUTTON_TOKEN,
      .style = BLACK_BACKGROUND,
      .fittingContent = true,
      .onBottom = false,
      .tuneId = TUNE_TAP_CASUAL
    };
    nbgl_layoutAddButton(layout, &flipButtonInfo);

  } else if (drawState == DRAW_STATE_DONE) {
    if (currentDrawType == DRAW_TYPE_FLIP_COIN) {
      if (result->value == 0) {
        snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "Heads");
      } else if (result->value == 1) {
        snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "Tails");
      } else {
        // TODO: some kind of crash?
        // assert(false);
      }
    } else if (currentDrawType == DRAW_TYPE_DICE_ROLL) {
      # if TEST_MODE
        snprintf(
          drawResultStr,
          DRAW_RESULT_MAX_STR_SIZE,
          "#of 1's: %d\n#of 2's: %d\n#of 3's: %d\n#of 4's: %d\n#of 5's: %d\n#of 6's: %d",
          testResult[0],
          testResult[1],
          testResult[2],
          testResult[3],
          testResult[4],
          testResult[5]
        );
      #else
        snprintf(drawResultStr, DRAW_RESULT_MAX_STR_SIZE, "You got: %d", result->value + 1);
      #endif
    }

    nbgl_layoutCenteredInfo_t centeredInfo = {
        .text1 = drawResultStr,
        .text2 = "Use it wisely.",
        .icon = NULL,
        .onTop = true,
        .style = LARGE_CASE_INFO,
        .offsetY = 0
    };
    nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutAddFooter(
      layout,
      currentDrawType == DRAW_TYPE_FLIP_COIN ? "New flip" : "New roll",
      NEW_DRAW_BUTTON_TOKEN,
      TUNE_TAP_CASUAL
    );
  } 
}

static bool navCallback(uint8_t page, nbgl_pageContent_t* content) {
    // TODO: assert page always is 0?
    const char* diceTypes[4] = {"Four-sided dice", "Six-sided dice", "Twelve-sided dice", "Twenty-sided dice"};
    nbgl_layoutRadioChoice_t radioLayout = {
        .nbChoices = 4,
        .initChoice = 1,
        .token = SETTINGS_RADIO_TOKEN,
        .names = diceTypes
    };
    content->type = CHOICES_LIST;
    content->choicesList = (nbgl_layoutRadioChoice_t) radioLayout;

    return false;
}

static void controlsCallback(int token, uint8_t index) {
    // TODO: needs condition on token?
    uint8_t diceSides = 0;
    switch (index) {
        case 0:
            diceSides = 4;
            break;
        case 1:
            diceSides = 6;
            break;
        case 2:
            diceSides = 12;
            break;
        case 3:
            diceSides = 20;
            break;
    }
    nvm_write((void*) &N_storage.diceSides, (void*) &diceSides, sizeof(uint8_t));
}

void drawDefaultDiceRollPage(void) {
  drawDrawPage(DRAW_TYPE_DICE_ROLL, DRAW_STATE_READY, NULL);
}

void ui_diceroll_settings(void) {
  nbgl_useCaseSettings(
    "Dice type",
    0,
    1,
    true,
    drawDefaultDiceRollPage,
    navCallback,
    controlsCallback
  );
}

static void drawLayoutTouchCallback(int token, uint8_t index) {
  if (token == BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(drawLayout);
    ui_menu_main();
  } else if (token == SETTINGS_BUTTON_TOKEN) {
    ui_diceroll_settings();
  } else if (token == DRAW_BUTTON_TOKEN) {
    drawDrawingPage();
  } else if (token == NEW_DRAW_BUTTON_TOKEN) {
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
    fillDrawLayout(drawLayout, drawState, result);
    nbgl_layoutDraw(drawLayout);
}