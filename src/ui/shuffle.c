#include "string.h"
#include "lcx_rng.h"
#include "nbgl_layout.h"
#include "glyphs.h"

#include "shuffle.h"

#define MAX_ITEMS_IN_LIST 5

typedef enum {
  BACK_BUTTON_TOKEN = 0,
  SHUFFLE_BUTTON_TOKEN,
  LIST_CALLBACK_TOKEN,
  KBD_BACK_BUTTON_TOKEN,
  KBD_CONFIRM_BUTTON_TOKEN,
  KBD_TEXT_TOKEN
} shuffle_tokens_e;

static nbgl_layout_t* shuffleLayout = NULL;

// TODO: provide a default list, and store the rest in user memory
static char* list[MAX_ITEMS_IN_LIST] = { NULL };
static uint8_t nbElementsInList = MAX_ITEMS_IN_LIST;

// Keyboard-related global variables
static char textToEnter[20];
static nbgl_layout_t* keyboardLayout;
static int textIndex;

// Implements Fisher-Yates shuffle
void shuffleList(void) {
  for (size_t i = nbElementsInList-1; i > 0; i--) {
    uint32_t j = cx_rng_u32_range_func(0, i+1, cx_rng_u32);
    char* temp = list[i];
    list[i] = list[j];
    list[j] = temp;
  }
}

static void keyboardLayoutTouchCallback(int token, uint8_t index) {
  if (token == KBD_BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(keyboardLayout);
    nbgl_screenRedraw();
    // app_main();
  }
  else if (token == KBD_CONFIRM_BUTTON_TOKEN) {
    // FIXME: do something with entered text
    nbgl_layoutRelease(keyboardLayout);
    nbgl_screenRedraw();
  }
  else if (token == KBD_TEXT_TOKEN) {
    // do something with entered text
  }
}

static void keyboardCallback(char touchedKey) {
  if (touchedKey != BACKSPACE_KEY) {
    int textLen = strlen(textToEnter);
    textToEnter[textLen] = touchedKey;
    textToEnter[textLen+1] = '\0';
  }
  else {
    int textLen = strlen(textToEnter);
    if (textLen == 0)
      return;
    textToEnter[textLen-1] = '\0';
  }
  nbgl_layoutUpdateEnteredText(keyboardLayout, textIndex, false, 0, textToEnter, false);
}

void fillShuffleLayout(nbgl_layout_t* layout) {
    nbgl_layoutAddProgressIndicator(layout, 0, 0, true, BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);

    nbgl_layoutCenteredInfo_t centeredInfo = {
        .text2 = "Tap the pen to edit an item.\nLeave name blank to remove.",
        .onTop = true,
        .style = LARGE_CASE_INFO,
    };
    nbgl_layoutAddCenteredInfo(layout, &centeredInfo);

    nbgl_layoutTagValue_t pairs[MAX_ITEMS_IN_LIST];

    pairs[0].item = "";
    pairs[0].value = "Harry Potter";
    pairs[0].valueIcon = &C_edit_pen_32px;
    pairs[1].item = "";
    pairs[1].value = "Hermione Granger";
    pairs[1].valueIcon = &C_edit_pen_32px;
    pairs[2].item = "";
    pairs[2].value = "Lord Voldemort";
    pairs[2].valueIcon = &C_edit_pen_32px;
    pairs[3].item = "";
    pairs[3].value = "Hagrid";
    pairs[3].valueIcon = &C_edit_pen_32px;
    pairs[4].item = "";
    pairs[4].value = "Ron Weasly";
    pairs[4].valueIcon = &C_edit_pen_32px;

    nbgl_layoutTagValueList_t contentDesc = {
        .pairs = (nbgl_layoutTagValue_t*) pairs,
        .startIndex = 0,
        .nbPairs = MAX_ITEMS_IN_LIST,
        .nbMaxLinesForValue = 1,
        .token = LIST_CALLBACK_TOKEN,
        .wrapping = false,
        .smallCaseForValue = true
    };
    nbgl_layoutAddTagValueList(layout, &contentDesc);

    nbgl_layoutButton_t shuffleButtonInfo = {
      .text = "Shuffle",
      .token = SHUFFLE_BUTTON_TOKEN,
      .style = BLACK_BACKGROUND,
      .fittingContent = true,
      .onBottom = true,
      .tuneId = TUNE_TAP_CASUAL
    };
    nbgl_layoutAddButton(layout, &shuffleButtonInfo);
}

void drawKeyboard(uint8_t index) {
  nbgl_layoutDescription_t layoutDescription = {
    .modal = true,
    .onActionCallback = &keyboardLayoutTouchCallback
  };

  nbgl_layoutKbd_t kbdInfo = {
    .callback = keyboardCallback,
    .keyMask = 0, // no inactive key
    .lettersOnly = false,
    .mode = MODE_LETTERS,
    .casing = LOWER_CASE
  };

  nbgl_layoutCenteredInfo_t centeredInfo = {
    .text2 = "Edit item name:", // to use as "header"
    .style = LARGE_CASE_INFO,
    .offsetY = 0,
    .onTop = true
  };

  keyboardLayout = nbgl_layoutGet(&layoutDescription);
    nbgl_layoutAddProgressIndicator(keyboardLayout, 0, 0, true, KBD_BACK_BUTTON_TOKEN, TUNE_TAP_CASUAL);
    nbgl_layoutAddCenteredInfo(keyboardLayout, &centeredInfo);
    nbgl_layoutAddKeyboard(keyboardLayout, &kbdInfo);
    nbgl_layoutAddConfirmationButton(keyboardLayout, "Confirm", KBD_CONFIRM_BUTTON_TOKEN, TUNE_TAP_CASUAL);
    
    strcpy(textToEnter, list[index]);
    textIndex = nbgl_layoutAddEnteredText(keyboardLayout,
                                          false,       // not numbered
                                          0,           // not used
                                          textToEnter, // text to display
                                          false,       // not grayed-out
                                          32,          // vertical margin from the button
                                          KBD_TEXT_TOKEN); // token used when this area is touched
 
    // draw layout
    nbgl_layoutDraw(keyboardLayout);
}

static void shuffleLayoutTouchCallback(int token, uint8_t index) {
  if (token == BACK_BUTTON_TOKEN) {
    nbgl_layoutRelease(shuffleLayout);
    ui_menu_main();
  } else if (token == SHUFFLE_BUTTON_TOKEN) {
    // TODO
  } else if (token == LIST_CALLBACK_TOKEN) {
    drawKeyboard(index);
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
