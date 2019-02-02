// WindowsでJIS配列として認識しているときに、オシャレなUS配列用キーキャップを使うためのキーマップです。
// @leopard_gecko さんがPlanck用に作成されたキーマップをかなり参考にしています。

#include QMK_KEYBOARD_H
#include "keymap_jp.h"       // qmk_firmware-master/quantum/keymap_extras/keymap_jp.h 日本語キーボード設定用
#include <sendstring_jis.h>  // macro sendstring for jis keyboard マクロ文字列送信時に日本語キーボード設定での文字化け回避
#ifdef PROTOCOL_LUFA
  #include "lufa.h"
  #include "split_util.h"
#endif

extern keymap_config_t keymap_config;

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _HDBX,
    _LOWER,
    _RAISE,
    _GAME,
    _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE, // QWERTY配列
  HDBX,                // オリジナル配列
  RGBRST,              // RGB LEDのリセット用
  DYNAMIC_MACRO_RANGE, // ダイナミックマクロ
  MCR1,                // マクロ1
  MCR2,                // マクロ2
  MCR3,                // マクロ3
  MCR4,                // マクロ4
  MCR5,                // マクロ5
  WN_SCLN              // タップでJISの「:」  シフトでJISの「;」 (Windows)
};

// Use Dynamic macro
#include "dynamic_macro.h"

// Fillers to make layering more clear
#define LOWER   LT(_LOWER, KC_MHEN)    // タップで無変換     ホールドでLower
#define RAISE   LT(_RAISE, KC_HENK)    // タップで変換       ホールドでRaise
#define GUI_ESC GUI_T(KC_ESC)          // タップでESC        ホールドでGUI
#define SFT_BS  SFT_T(KC_BSPC)         // タップでBackSpace  ホールドでSHIFT
#define CTL_ENT CTL_T(KC_ENT)          // タップでEnter      ホールドでCTRL
#define CTL_TAB CTL_T(KC_TAB)          // タップでTAB        ホールドでCTRL
#define SFT_DEL SFT_T(KC_DEL)          // タップでDELETE     ホールドでSHIFT
#define ALT_LBR ALT_T(JP_LBRC)         // タップで[          ホールドでALT
#define SFT_RBR SFT_T(JP_RBRC)         // タップで]          ホールドでSHIFT
#define ALT_GRV ALT_T(JP_GRV)          // タップで`          ホールドでALT
#define WN_CAPS S(KC_CAPS)             // Caps Lock                    (Windows)
#define CTRL_Z  LCTL(KC_Z)             // Ctrl + Z
#define CTRL_X  LCTL(KC_X)             // Ctrl + X
#define CTRL_C  LCTL(KC_C)             // Ctrl + C
#define CTRL_V  LCTL(KC_V)             // Ctrl + V
#define CTRL_F  LCTL(KC_F)             // Ctrl + F
#define CTRL_Y  LCTL(KC_Y)             // Ctrl + Y
#define CTRL_Y  LCTL(KC_Y)             // Ctrl + Y
#define CTRL_1  LCTL(KC_1)             // Ctrl + 1
#define MPLAY1  DYN_MACRO_PLAY1        // ダイナミックマクロ1 
#define MPLAY2  DYN_MACRO_PLAY2        // ダイナミックマクロ2
#define MREC1   DYN_REC_START1         // ダイナミックマクロ1記録開始
#define MREC2   DYN_REC_START2         // ダイナミックマクロ2記録開始
#define MRSTOP  DYN_REC_STOP           // ダイナミックマクロ記録終了

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY // WindowsでJIS配列時のデフォルトキーマップ
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |Win/Esc|   Q   |   W   |   E   |   R   |   T   |   =   |   |   \   |   Y   |   U   |   I   |   O   |   P   |   '   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |   ~   |   A   |   S   |   D   |   F   |   G   |   <   |   |   >   |   H   |   J   |   K   |   L   |   :   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Alt/[ |   Z   |   X   |   C   |   V   |   B   |   (   |   |   )   |   N   |   M   |   ,   |   .   |   /   |Shift/]|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Game  |Ctrl+C |Ctrl+V |  Del  |Sft/Bsp|Low/Mhe|Ctl/Ent|   | Space |Rai/Hen|Ctl/Tab| Left  | Right |PrntScr| Ctrl  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_QWERTY] = LAYOUT( \
  GUI_ESC,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    JP_EQL,    JP_YEN,  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_QUOT, \
  JP_TILD,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LT,     KC_GT,   KC_H,    KC_J,    KC_K,    KC_L,    WN_SCLN, KC_MINS, \
  ALT_LBR,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    JP_LPRN,   JP_RPRN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, SFT_RBR, \
  TG(_GAME), CTRL_C,  CTRL_V,  KC_DEL,  SFT_BS,  LOWER,   CTL_ENT,   KC_SPC,  RAISE,   CTL_TAB, KC_LEFT, KC_RGHT, KC_PSCR, KC_LCTL \
),

/* HDBX // もともとはEUCALYN配列だったらしいキーマップ
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |Win/Esc|   Q   |   W   |   E   |   ,   |   .   |   =   |   |   \   |   Y   |   D   |   P   |   F   |   :   |   '   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |   ~   |   A   |   I   |   O   |   U   |   G   |   <   |   |   >   |   M   |   N   |   T   |   R   |   S   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Alt/[ |   Z   |   X   |   C   |   V   |   B   |   (   |   |   )   |   H   |   J   |   K   |   L   |   /   |Shift/]|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Game  |Ctrl+C |Ctrl+V |  Del  |Sft/Bsp|Low/Mhe|Ctl/Ent|   | Space |Rai/Hen|Ctl/Tab| Left  | Right |PrntScr| Ctrl  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_HDBX] = LAYOUT( \
  GUI_ESC,   KC_Q,    KC_W,    KC_E,    KC_COMM, KC_DOT,  JP_EQL,    JP_YEN,  KC_Y,    KC_D,    KC_P,    KC_F,    WN_SCLN, JP_QUOT, \
  JP_TILD,   KC_A,    KC_I,    KC_O,    KC_U,    KC_G,    KC_LT,     KC_GT,   KC_M,    KC_N,    KC_T,    KC_R,    KC_S,    KC_MINS, \
  ALT_LBR,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    JP_LPRN,   JP_RPRN, KC_H,    KC_J,    KC_K,    KC_L,    KC_SLSH, SFT_RBR, \
  TG(_GAME), CTRL_C,  CTRL_V,  KC_DEL,  SFT_BS,  LOWER,   CTL_ENT,   KC_SPC,  RAISE,   CTL_TAB, KC_LEFT, KC_RGHT, KC_PSCR, KC_LCTL  \
),

/* LOWER // 数字入力用レイヤー
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  F1   |  F2   |  F3   |  F4   |  F5   |  F6   |       |   |   F2  |   7   |   8   |   9   |   .   |   =   | Bspc  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |  F7   |  F8   |  F9   |  F10  |  F11  |  F12  |       |   |   Up  |   4   |   5   |   6   |   +   |   *   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |Ctrl+Y |Ctrl+Z |Ctrl+X |Ctrl+C |Ctrl+V |Ctrl+F |Ctrl+1 |   |  Down |   1   |   2   |   3   |   \   |   /   | Enter |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |       |       |       |       |Sft/Del| Lower | Ctrl  |   |  Caps | Raise |   0   | Home  |  End  | PgDn  | PgUp  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_LOWER] = LAYOUT( \
  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   _______,    KC_F2,   KC_7,    KC_8,   KC_9,    KC_DOT,  JP_EQL,  KC_BSPC, \
  KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______,    KC_UP,   KC_4,    KC_5,   KC_6,    KC_PPLS, KC_PAST, KC_TAB,  \
  CTRL_Y,  CTRL_Z , CTRL_X , CTRL_C , CTRL_V , CTRL_F,  CTRL_1,     KC_DOWN, KC_1,    KC_2,   KC_3,    KC_PMNS, KC_PSLS, KC_ENT,  \
  _______, _______, _______, _______, SFT_DEL, _______, KC_LCTL,    WN_CAPS, _______, KC_0,   KC_HOME, KC_END,  KC_PGDN, KC_PGUP  \
),

/* RAISE // 記号入力用レイヤー
 * ,-------------------------------------------------------.   ,------------------------------------------------------.
 * |  Esc  |   !   |   @   |   #   |   $   |   %   |   +   |   |   `   | Home  |  App  |  Ins  | Calc  |PrntScr|   _   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |   `   |   ^   |   &   |   *   |   \   |   |   |   [   |   |   ]   | Left  | Down  |   Up  | Right |   ;   |   "   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |  Alt  | Play  | Vol.- | Vol.+ | Prev  | Next  |   {   |   |   }   |  End  | Mute  |   <   |   >   |   ?   | Shift |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |       |       |       |       |Sft/Del| Lower | Ctrl  |   |  Caps | Raise | Ctrl  | Home  |  End  | PgDn  | PgUp  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_RAISE] = LAYOUT( \
  _______, KC_EXLM, JP_AT,   KC_HASH, KC_DLR,  KC_PERC, JP_PLUS,    JP_GRV,  KC_HOME, KC_APP,  KC_INS,  KC_CALC, KC_PSCR, JP_UNDS, \
  _______, JP_CIRC, JP_AMPR, JP_ASTR, JP_YEN,  JP_PIPE, KC_RBRC,    KC_NUHS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN, JP_DQT,  \
  _______, KC_MPLY, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT, JP_LCBR,    JP_RCBR, KC_END,  KC_MUTE, KC_LT,   KC_GT,   KC_QUES, KC_RSFT, \
  _______, _______, _______, _______, KC_DEL,  _______, _______,    _______, _______, _______, KC_HOME, KC_END,  KC_PGDN, KC_PGUP  \
),

/* GAME // 固定使用レイヤー
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  Esc  |XXXXXXX|   Up  |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|   |   F2  |   7   |   8   |   9   |   .   |   =   | Bspc  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Ctrl  | Left  | Down  | Right |XXXXXXX|XXXXXXX|XXXXXXX|   |   Up  |   4   |   5   |   6   |   +   |   *   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |  Alt  |Ctrl+Z |Ctrl+X |Ctrl+C |Ctrl+V |Ctrl+F |Ctrl+1 |   |  Down |   1   |   2   |   3   |   \   |   /   | Enter |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |       |  Win  |  Alt  |  Del  | Shift |XXXXXXX| Ctrl  |   |  Esc  |   ,   |   0   | Left  | Down  |   Up  | Right |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_GAME] = LAYOUT( \
  KC_ESC,   XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_F2,   KC_P7,   KC_P8,   KC_P9,   KC_DOT,  JP_EQL,  KC_BSPC, \
  KC_LCTL,  KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,   KC_UP,   KC_P4,   KC_P5,   KC_P6,   KC_PPLS, KC_PAST, KC_PMNS, \
  KC_LALT,  CTRL_Z , CTRL_X , CTRL_C , CTRL_V , CTRL_F,  CTRL_1,    KC_DOWN, KC_P1,   KC_P2,   KC_P3,   JP_YEN,  KC_PSLS, KC_ENT, \
  _______,  KC_LGUI, KC_LALT, KC_DEL,  KC_LSFT, XXXXXXX, KC_LCTL,   KC_ESC,  KC_COMM, KC_P0,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* ADJUST // 設定用レイヤー (LOWER+RAISE)
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * | RESET | MCR1  | MCR2  | MCR3  | MCR4  | MCR5  |XXXXXXX|   |XXXXXXX|PLAY_M1|PLAY_M2|REC_M1 |REC_M2 |STP_REC|XXXXXXX|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |RGB_TOG| MODE  | PLAIN | HUE+  | SAT+  | VAL+  |       |   |XXXXXXX|XXXXXXX|QWERTY | HDBX  |XXXXXXX|XXXXXXX|XXXXXXX|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |RGB_RST| RMODE | SNAKE | HUE-  | SAT-  | VAL-  |       |   |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|       |XXXXXXX|   |XXXXXXX|       |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_ADJUST] = LAYOUT( \
  RESET,   MCR1,    MCR2,    MCR3,    MCR4,    MCR5,    XXXXXXX,   XXXXXXX, MPLAY1,  MPLAY2,  MREC1,   MREC2,   MRSTOP,  XXXXXXX, \
  RGB_TOG, RGB_MOD, RGB_M_P, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX,   XXXXXXX, XXXXXXX, QWERTY,  HDBX,    XXXXXXX, XXXXXXX, XXXXXXX, \
  RGBRST, RGB_RMOD, RGB_M_SN,RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,   XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX \
)

};

// RGB Underglow使用時のレイヤー毎のカラー切り替え
uint32_t layer_state_set_keymap (uint32_t state) {
  return state;
}

void matrix_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  rgblight_enable();
  rgblight_sethsv_pink();
#endif
}

uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef RGBLIGHT_ENABLE
    switch (biton32(state)) {
    case _RAISE:
      rgblight_sethsv_chartreuse();
      break;
    case _LOWER:
      rgblight_sethsv_turquoise();
      break;
    case _ADJUST:
      rgblight_sethsv_red();
      break;
    case _GAME:
      rgblight_sethsv_cyan();
      break;
    default: //  for any other layers, or the default layer
      rgblight_sethsv_pink();
      break;
    }
#endif
return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_record_dynamic_macro(keycode, record)) {
      return false;
  }
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case HDBX:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_HDBX);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
//          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      break;
    static bool     lshift = false;
    case WN_SCLN: // コロン「;:」
      if (record->event.pressed) {
        lshift = keyboard_report->mods & MOD_BIT(KC_LSFT);
        if (lshift) {
          unregister_code(KC_LSFT);
          register_code(JP_SCLN);
          unregister_code(JP_SCLN);
        } else {
          register_code(JP_COLN);
          unregister_code(JP_COLN);
        }
      }
      return false;
      break;
    case MCR1:
      if (record->event.pressed) {
        SEND_STRING("hoge"); // 送信文字列
      }
      return false;
      break;
    case MCR2:
      if (record->event.pressed) {
        SEND_STRING("hogehoge"SS_TAP(X_ENTER)); // 送信文字列
      }
      return false;
      break;
    case MCR3:
      if (record->event.pressed) {
                SEND_STRING("hoge@hoge.hoge"); // 送信文字列
      }
      return false;
      break;
  }
  return true;
}
