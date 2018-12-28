// WindowsでJIS配列として認識しているときに、US配列として使うためのキーマップ
// @leopard_gecko さんがPlanck用に作成されたキーマップをかなり参考にしています。

#include QMK_KEYBOARD_H
#include "keymap_jp.h"       // qmk_firmware-master/quantum/keymap_extras/keymap_jp.h 日本語キーボード設定用
#include <sendstring_jis.h>  // macro sendstring for jis keyboard マクロ文字列送信時に日本語キーボード設定での文字化け回避

extern keymap_config_t keymap_config;

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _EUCALYN,
    _LOWER,
    _RAISE,
    _GAME,
    _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE, // QWERTY配列
  EUCALYN,             // オリジナル配列
  RGBRST,
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
#define LOWER LT(_LOWER, KC_MHEN)    // タップで無変換     ホールドでLower
#define RAISE LT(_RAISE, KC_HENK)    // タップで変換       ホールドでRaise
#define KC_LSLP MT(MOD_LSFT, KC_RBRC)  // タップで[   全角時「   ホールドで左Shift
#define KC_RSRP MT(MOD_RSFT, KC_NUHS)  // タップで]   全角時 」  ホールドで右Shift
#define CTL_ZH  CTL_T(KC_ZKHK)         // タップで半角/全角  ホールドで左Control     (Windows)
#define WN_CAPS S(KC_CAPS)             // Caps Lock                                  (Windows)
#define CTRL_Z LCTL(KC_Z)              // Ctr + Z
#define CTRL_X LCTL(KC_X)              // Ctr + X
#define CTRL_C LCTL(KC_C)              // Ctr + C
#define CTRL_V LCTL(KC_V)              // Ctr + V
#define CTRL_Y LCTL(KC_Y)              // Ctr + Y
#define CTRL_1 LCTL(KC_1)              // Ctr + 1

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY // WindowsでJIS配列時のデフォルトキーマップ
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  Esc  |   Q   |   W   |   E   |   R   |   T   |   =   |   |   ~   |   Y   |   U   |   I   |   O   |   P   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |ZH/Ctrl|   A   |   S   |   D   |   F   |   G   |   <   |   |   >   |   H   |   J   |   K   |   L   |   :   |   '   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |Shift/[|   Z   |   X   |   C   |   V   |   B   |   (   |   |   )   |   N   |   M   |   ,   |   .   |   /   |Shift/]|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |TG(Gam)|  GUI  |  Alt  |   \   | Bspc  |Lwr/Mhn| Enter |   | Space |Rse/Hen|  Tab  | Left  | Right |  Del  | Ctrl  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_QWERTY] = LAYOUT( \
  KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    JP_EQL,    JP_TILD, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
  CTL_ZH,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LT,     KC_GT,   KC_H,    KC_J,    KC_K,    KC_L,    WN_SCLN, JP_QUOT, \
  KC_LSLP,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    JP_LPRN,   JP_RPRN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSRP, \
  TG(_GAME), KC_LGUI, KC_LALT, KC_JYEN, KC_BSPC, LOWER,   KC_ENT,    KC_SPC,  RAISE,   KC_TAB,  KC_LEFT, KC_RGHT, KC_DEL,  KC_RCTL \
),

/* EUCALYN // もともとはEUCALYN配列だったらしいキーマップ
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  Esc  |   Q   |   W   |   E   |   ,   |   .   |   =   |   |   ~   |   Y   |   D   |   P   |   F   |   :   |   -   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |ZH/Ctrl|   A   |   I   |   O   |   U   |   G   |   <   |   |   >   |   M   |   N   |   T   |   R   |   S   |   '   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |Shift/[|   Z   |   X   |   C   |   V   |   B   |   (   |   |   )   |   H   |   J   |   K   |   L   |   /   |Shift/]|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |TG(Gam)|  GUI  |  Alt  |   \   | Bspc  |Lwr/Mhn| Enter |   | Space |Rse/Hen|  Tab  | Left  | Right |  Del  | Ctrl  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_EUCALYN] = LAYOUT( \
  KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_COMM, KC_DOT,  JP_EQL,    JP_TILD, KC_Y,    KC_D,    KC_P,    KC_F,    WN_SCLN, KC_MINS, \
  CTL_ZH,    KC_A,    KC_I,    KC_O,    KC_U,    KC_G,    KC_LT,     KC_GT,   KC_M,    KC_N,    KC_T,    KC_R,    KC_S,    JP_QUOT, \
  KC_LSLP,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    JP_LPRN,   JP_RPRN, KC_H,    KC_J,    KC_K,    KC_L,    KC_SLSH, KC_RSRP, \
  TG(_GAME), KC_LGUI, KC_LALT, KC_JYEN, KC_BSPC, LOWER,   KC_ENT,    KC_SPC,  RAISE,   KC_TAB,  KC_LEFT, KC_RGHT, KC_DEL,  KC_RCTL \
),

/* LOWER // 数字入力用レイヤー
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  Esc  |  F1   |  F2   |  F3   |  F4   |  F5   |  F6   |   |  Home |   7   |   8   |   9   |   .   |   =   | Bspc  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Ctrl  |  F7   |  F8   |  F9   |  F10  |  F11  |  F12  |   |  F2   |   4   |   5   |   6   |   +   |   *   |  Tab  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Shift |Ctrl+Z |Ctrl+X |Ctrl+C |Ctrl+V |Ctrl+Y |Ctrl+1 |   |  End  |   1   |   2   |   3   |   -   |   /   | Enter |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |TG(Gam)|  GUI  |  Alt  | Ctrl  |  Del  | Lower | Caps  |   |  Esc  | Raise |   0   | Left  | Down  |   Up  | Right |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_LOWER] = LAYOUT( \
  _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,      KC_HOME, KC_7,    KC_8,   KC_9,    KC_DOT,  JP_EQL,  KC_BSPC, \
  _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,     KC_F2,   KC_4,    KC_5,   KC_6,    KC_PPLS, KC_PAST, KC_TAB,  \
  _______, CTRL_Z , CTRL_X , CTRL_C , CTRL_V , CTRL_Y,  CTRL_1,     KC_END,  KC_1,    KC_2,   KC_3,    KC_PMNS, KC_PSLS, KC_ENT, \
  _______, _______, _______, _______, KC_DEL,  _______, WN_CAPS,    KC_ESC,  _______, KC_0,   _______, _______, _______, _______ \
),

/* RAISE // 記号入力用レイヤー
 * ,-------------------------------------------------------.   ,------------------------------------------------------.
 * |  Esc  |   !   |   @   |   #   |   $   |   %   |   +   |   |   `   | Home  |  App  |  Ins  | Calc  |PrntScr|   _   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Ctrl  |   ^   |   &   |   *   |   \   |   |   |   [   |   |   ]   | Left  | Down  |   Up  | Right |   ;   |   "   |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Shift | Play  |Vol.Dn |Vol.Up | Prev  | Next  |   {   |   |   }   |  End  | Mute  |   <   |   >   |   ?   | Shift |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |TG(Gam)|  GUI  |  Alt  | Ctrl  |  Del  | Lower | Enter |   | Space | Raise |  Tab  | Home  | PgDn  | PgUp  |  End  |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_RAISE] = LAYOUT( \
  _______, KC_EXLM, JP_AT,   KC_HASH, KC_DLR,  KC_PERC, JP_PLUS,    JP_GRV,  KC_HOME, KC_APP,  KC_INS,  KC_CALC, KC_PSCR, JP_UNDS,  \
  _______, JP_CIRC, JP_AMPR, JP_ASTR, JP_YEN,  JP_PIPE, KC_RBRC,    KC_NUHS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCLN, JP_DQT,  \
  _______, KC_MPLY, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT, JP_LCBR,    JP_RCBR, KC_END,  KC_MUTE, KC_LT,   KC_GT,   KC_QUES, _______, \
  _______, _______, _______, _______, KC_DEL,  _______, _______,    _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END \
),

/* GAME // 左ゲーム用レイヤー、右NumPad
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * |  Esc  |XXXXXXX|   Up  |XXXXXXX|   R   |   T   |  F2   |   |  Home |   7   |   8   |   9   |   .   |   =   | Bspc  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Ctrl  | Left  | Down  | Right |   F   |   G   |  F1   |   |  F2   |   4   |   5   |   6   |   -   |   /   |  Tab  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * | Shift |Ctrl+Z |Ctrl+X |Ctrl+C |Ctrl+V |Ctrl+Y |Ctrl+1 |   |  End  |   1   |   2   |   3   |   +   |   *   | Enter |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |       |  GUI  |  Alt  | Ctrl  | Bspc  | Lower | Enter |   |  Esc  | Raise |   0   | Left  | Down  |   Up  | Right |
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_GAME] = LAYOUT( \
  KC_ESC,   XXXXXXX, KC_UP,   XXXXXXX, KC_R,    KC_T,    _______,   KC_HOME, KC_P7,   KC_P8,   KC_P9,   KC_DOT,  JP_EQL,  KC_BSPC, \
  KC_LCTL,  KC_LEFT, KC_DOWN, KC_RGHT, KC_F,    KC_G,    KC_F1,     KC_F2,   KC_P4,   KC_P5,   KC_P6,   KC_PMNS, KC_PSLS, KC_TAB,  \
  KC_LSFT,  CTRL_Z , CTRL_X , CTRL_C , CTRL_V , CTRL_Y,  CTRL_1,    KC_END,  KC_P1,   KC_P2,   KC_P3,   KC_PPLS, KC_PAST, KC_ENT, \
  _______,  KC_LGUI, KC_LALT, _______, KC_BSPC, _______, KC_ENT,    KC_ESC,  _______, KC_P0,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT \
),

/* ADJUST // 設定用レイヤー (LOWER+RAISE)
 * ,-------------------------------------------------------.   ,-------------------------------------------------------.
 * | RESET | MCR1  | MCR2  | MCR3  | MCR4  | MCR5  |XXXXXXX|   |XXXXXXX|PLAY_M1|PLAY_M2|REC_M1 |REC_M2 |STP_REC| BSPC  |
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |RGB_TOG|       |       |       |       |       |       |   |XXXXXXX|XXXXXXX|QWERTY |EUCALYN|XXXXXXX|XXXXXXX|XXXXXXX|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |RGB_RST|       |       |       |       |       |       |   |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
 * |-------+-------+-------+-------+-------+-------+-------|   |-------+-------+-------+-------+-------+-------+-------|
 * |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|       |XXXXXXX|   |XXXXXXX|       |XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|XXXXXXX|
 * `-------------------------------------------------------'   `-------------------------------------------------------'
 */
[_ADJUST] = LAYOUT( \
  RESET,    MCR1,    MCR2,    MCR3,    MCR4,    MCR5,    XXXXXXX,   XXXXXXX, DYN_MACRO_PLAY1, DYN_MACRO_PLAY2, DYN_REC_START1, DYN_REC_START2, DYN_REC_STOP, KC_BSPC, \
  RGB_TOG,  RGB_MOD, RGB_M_P, RGB_M_B, RGB_HUI, RGB_SAI, RGB_VAI,   XXXXXXX, XXXXXXX, QWERTY,  EUCALYN, XXXXXXX, XXXXXXX, XXXXXXX, \
  RGBRST,   RGB_M_R, RGB_M_SN,RGB_M_G, RGB_HUD, RGB_SAD, RGB_VAD,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX,   XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX \
),

};

// RGB Underglow使用時のレイヤー毎のカラー切り替え
uint32_t layer_state_set_keymap (uint32_t state) {
  return state;
}

void matrix_init_user(void) {
#ifdef RGBLIGHT_ENABLE
  rgblight_enable();
  rgblight_sethsv_goldenrod();
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
      rgblight_sethsv_cyan();
      break;
    case _ADJUST:
      rgblight_sethsv_red();
      break;
    case _GAME:
      rgblight_sethsv_pink();
      break;
    default: //  for any other layers, or the default layer
      rgblight_sethsv_yellow();
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
    case EUCALYN:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_EUCALYN);
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
