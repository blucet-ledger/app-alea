#ifdef HAVE_NBGL

#include "os.h"
#include "ox_rng.h"
// #include "glyphs.h"
#include "nbgl_layout.h"
#include "../types_alea.h"

// #include "../globals.h"
#include "menu.h"
#include "coinflip.h"

static nbgl_layout_t *layout;
static nbgl_layout_t *layout_drawing;

static draw_state_e drawState = DRAW_READY;

enum {
    BACK_BUTTON_TOKEN = 0,
    FLIP_BUTTON_TOKEN
};

// TODO: maybe better to use predefined functions such as cx_rng_u32 or cx_rng_u32_range_func
bool flipCoin() {
    uint8_t buf[1];
    cx_trng_get_random_data(buf, 1);
    return buf[0] & 0x01;
}

static void layoutTouchCallback(int token, uint8_t index) {
  if (token == BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(layout);
    ui_menu_main();
  } else if (token == FLIP_BUTTON_TOKEN) {
    ui_coin_flip_drawing();
  }
}

static void tickerCallback() {
  ui_coin_flip();
  nbgl_layoutRelease(layout_drawing);
  nbgl_refresh();
}

void ui_coin_flip(void) {
  nbgl_layoutDescription_t layoutDescription = {
    .modal = false,
    .withLeftBorder = true,
    .onActionCallback = &layoutTouchCallback
  };
  layout = nbgl_layoutGet(&layoutDescription);
  
    nbgl_layoutAddProgressIndicator(
        layout,
        0,
        0,
        true,
        BACK_BUTTON_TOKEN,
        TUNE_TAP_CASUAL
    );

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
    .token = FLIP_BUTTON_TOKEN,
    .style = BLACK_BACKGROUND,
    .fittingContent = true,
    .onBottom = false,
    .tuneId = TUNE_TAP_CASUAL
  };
  nbgl_layoutAddButton(layout, &flipButtonInfo);

    nbgl_layoutDraw(layout);
    nbgl_refresh();
}

void ui_coin_flip_drawing(void) {
    static nbgl_screenTickerConfiguration_t drawing_ticker = {
        .tickerCallback = &tickerCallback,
        .tickerValue = 1600,
        .tickerIntervale = 0
    };

    nbgl_layoutDescription_t layoutDescription = {
        .modal = true,
        .withLeftBorder = true,
        .onActionCallback = &layoutTouchCallback,
        .ticker = drawing_ticker
    };

    layout_drawing = nbgl_layoutGet(&layoutDescription);

    nbgl_layoutAddSpinner(layout_drawing, "Coin's flipping\nin the air...", false);

    nbgl_layoutDraw(layout_drawing);
    nbgl_refreshSpecial(FULL_COLOR_PARTIAL_REFRESH);
}

void ui_coin_flip_done(void) {
    nbgl_layoutDraw(layout);
    nbgl_refresh();
}

#endif
