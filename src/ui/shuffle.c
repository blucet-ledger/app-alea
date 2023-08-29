#include "nbgl_layout.h"
#include "glyphs.h"

#include "shuffle.h"

#define MAX_PAIRS 6

typedef enum {
  BACK_BUTTON_TOKEN = 0
} shuffle_tokens_e;

static nbgl_layout_t* shuffleLayout = NULL;
// static nbgl_layoutTagValue_t pairs[MAX_PAIRS];

void fillShuffleLayout(nbgl_layout_t* layout) {
    nbgl_layoutAddProgressIndicator(layout, 0, 0, true, BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);

    // nbgl_layoutCenteredInfo_t centeredInfo = {
    //     .text1 = "Shuffle dat list",
    //     .onTop = true,
    //     .style = LARGE_CASE_INFO,
    //     .offsetY = 0
    // };
    // nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutTagValue_t pairs[MAX_PAIRS];

    pairs[0].item = "1.";
    pairs[0].value = "Harry Potter";
    pairs[0].valueIcon = &C_edit_pen_32px;
    pairs[1].item = "2.";
    pairs[1].value = "Hermione Granger";
    pairs[1].valueIcon = &C_edit_pen_32px;
    pairs[2].item = "3.";
    pairs[2].value = "Lord Voldemort";
    pairs[2].valueIcon = &C_edit_pen_32px;

    pairs[3].item = "1.";
    pairs[3].value = "Harry Potter";
    pairs[3].valueIcon = &C_edit_pen_32px;
    pairs[4].item = "2.";
    pairs[4].value = "Hermione Granger";
    pairs[4].valueIcon = &C_edit_pen_32px;
    pairs[5].item = "3.";
    pairs[5].value = "Lord Voldemort";
    pairs[5].valueIcon = &C_edit_pen_32px;

    nbgl_layoutTagValueList_t contentDesc = {
        .pairs = (nbgl_layoutTagValue_t*) pairs,
        .startIndex = 0,
        .nbPairs = MAX_PAIRS,
        .nbMaxLinesForValue = 1,
        .token = 0, // TODO handle later
        .wrapping = false
    };
    nbgl_layoutAddTagValueList(layout, &contentDesc);
}

static void shuffleLayoutTouchCallback(int token, uint8_t index) {
  if (token == BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(shuffleLayout);
    ui_menu_main();
  }
}

void drawShufflePage(void) {
    nbgl_layoutDescription_t layoutDescription = {
        .modal = false,
        .withLeftBorder = true,
        .onActionCallback = &shuffleLayoutTouchCallback
    };
    shuffleLayout = nbgl_layoutGet(&layoutDescription);
    fillShuffleLayout(shuffleLayout);
    nbgl_layoutDraw(shuffleLayout);
}
