#include "crkbd.h"
#include "bootloader.h"
#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "keymap_jp.h"       // qmk_firmware-master/quantum/keymap_extras/keymap_jp.h 日本語キーボード設定用
#include <sendstring_jis.h>  // macro sendstring for jis keyboard マクロ文字列送信時に日本語キーボード設定での文字化け回避
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#include "LUFA/Drivers/Peripheral/TWI.h"
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif

#include "../lib/mode_icon_reader.c"
#include "layer_state_reader.c"
#include "../lib/host_led_state_reader.c"
#include "../lib/logo_reader.c"
#include "../lib/keylogger.c"
#include "../lib/timelogger.c"

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum layer_number {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
  RGBRST,
  MCR1,                // マクロ1
  MCR2,                // マクロ2
  MCR3,                // マクロ3
  MCR4,                // マクロ4
  MCR5,                // マクロ5
  WN_SCLN              // タップでJISの「:」  シフトでJISの「;」 (Windows)
};

enum macro_keycodes {
  KC_SAMPLEMACRO,
};


#define _____ KC_TRNS
#define XXXXX KC_NO
#define LRST  RGBRST
#define LTOG  RGB_TOG
#define LHUI  RGB_HUI
#define LHUD  RGB_HUD
#define LSAI  RGB_SAI
#define LSAD  RGB_SAD
#define LVAI  RGB_VAI
#define LVAD  RGB_VAD
#define LSMOD RGB_SMOD
#define LRMOD RGB_RMOD
#define LOWER LT(_LOWER, KC_MHEN)   // タップで無変換     ホールドでLower
#define RAISE LT(_RAISE, KC_HENK)   // タップで変換       ホールドでRaise
#define GAME TO(_GAME)              // _GAME
#define ADJUST TO(_ADJUST)          // _ADJUST
#define ALTAB ALT_T(KC_TAB)         // タップでTab    ホールドでALT
#define ESCWN LGUI_T(KC_ESC)        // タップでEsc    ホールドで左Win     (Windows)
#define LSLP MT(MOD_LSFT, KC_RBRC)  // タップで[   全角時「   ホールドで左Shift
#define RSRP MT(MOD_RSFT, KC_NUHS)  // タップで]   全角時 」  ホールドで右Shift
#define ALPS LALT(KC_PSCR)          // Alt + PrintScreen
#define WN_CAPS S(KC_CAPS)          // Caps Lock                                  (Windows)
#define CTL_Z LCTL(KC_Z)            // Ctr + Z
#define CTL_X LCTL(KC_X)            // Ctr + X
#define CTL_C LCTL(KC_C)            // Ctr + C
#define CTL_V LCTL(KC_V)            // Ctr + V
#define CTL_Y LCTL(KC_Y)            // Ctr + Y
#define CTL_1 LCTL(KC_1)            // Ctr + 1


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |Win/Es|     Q|     W|     E|     R|     T|             |     Y|     U|     I|     O|     P|     -|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  Ctrl|     A|     S|     D|     F|     G|             |     H|     J|     K|     L|     ;|     '|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Sft/[|     Z|     X|     C|     V|     B|             |     N|     M|     ,|     .|     /| Sft/]|
   * `------------------------+------+------+------.     ,------+------+------+---+------+------+------'
   *                          | Bksp |Lower |Enter |     |Space |Raise |Alt/Tb|
   *                          `--------------------'     `--------------------'
   */
  [_QWERTY] = LAYOUT( \
       ESCWN,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS, \
     KC_LCTL,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,    WN_SCLN, JP_QUOT, \
        LSLP,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSRP, \
                                     KC_BSPC, LOWER,   KC_ENT,   KC_SPC,   RAISE,   ALTAB \
  ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |   F12|    F1|    F2|    F3|    F4|    F5|             |     7|     8|     9|     ,|     =|  Bspc|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  Ctrl|    F6|    F7|    F8|    F9|   F10|             |     4|     5|     6|     +|     *|   Tab|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift| Ctl+Z| Ctl+X| Ctl+C| Ctl+V|   F11|             |     1|     2|     3|     -|     /| Enter|
   * `------------------------+------+------+------.     ,------+------+------+---+------+------+------'
   *                          |   Del| Lower|  Caps|     |     0| Raise|     .|
   *                          `--------------------'     `--------------------'
   */
  [_LOWER] = LAYOUT( \
       KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,           KC_7,    KC_8,    KC_9,    KC_PCMM, JP_EQL,  KC_BSPC, \
        _____,  KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,           KC_4,    KC_5,    KC_6,    KC_PPLS, KC_PAST,  KC_TAB, \
      KC_LSFT,  CTL_Z,   CTL_X,   CTL_C,   CTL_V,  KC_F11,           KC_1,    KC_2,    KC_3,    KC_PMNS, KC_PSLS,  KC_ENT, \
                                      KC_DEL,  _____, WN_CAPS,   KC_0,  _____, KC_PDOT \
  ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |     ~|     !|     @|     #|     $|     %|             |  Home|   App|   Ins|  Calc|PrtScr|     _|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |  Ctrl|     ^|     &|     *|     \|     ||             |  Left|  Down|    Up| Right|     ;|     "|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|     `|Vol.Dn|Vol.Up|     {|     }|             |   End|  Mute|     (|     )|     ?| Enter|
   * `------------------------+------+------+------.     ,------+------+------+---+------+------+------'
   *                          |   Del| Lower| Enter|     | Space| Raise|PageDn|
   *                          `--------------------'     `--------------------'
   */
  [_RAISE] = LAYOUT( \
      JP_TILD, KC_EXLM,   JP_AT, KC_HASH, KC_DLR,  KC_PERC,            KC_HOME,  KC_APP,  KC_INS, KC_CALC,    ALPS, JP_UNDS, \
        _____, JP_CIRC, JP_AMPR, JP_ASTR, JP_YEN,  JP_PIPE,            KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, KC_SCLN,  JP_DQT, \
      KC_LSFT,  JP_GRV, KC_VOLD, KC_VOLU, JP_LCBR, JP_RCBR,            KC_END,  KC_MUTE, JP_LPRN, JP_RPRN, KC_QUES,  KC_ENT, \
                                      KC_DEL,    _____,  _____,   _____,   _____,   KC_PGDN \
  ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |RESET | MCR1 | MCR2 | MCR3 | MCR4 | MCR5 |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |RGB ON| MODE |PLAIN | HUE+ | SAT+ | VAL+ |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |RGBRST| RMD  |SNAKE | HUE- | SAT- | VAL- |             |  Play|  Mute|Vol.Dn|Vol.Up|  Prev|  Next|
   * `------------------------+------+------+------.     ,------+------+------+---+------+------+------'
   *                          |      |      |      |     |      |      |      |
   *                          `--------------------'     `--------------------'
   */
  [_ADJUST] = LAYOUT( \
      RESET, MCR1,     MCR2, MCR3, MCR4, MCR5,             XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, \
      LTOG, LSMOD,  RGB_M_P, LHUI, LSAI, LVAI,             XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, XXXXX, \
      LRST, LRMOD, RGB_M_SN, LHUD, LSAD, LVAD,             KC_MPLY, KC_MUTE, KC_VOLD, KC_VOLU, KC_MPRV, KC_MNXT, \
                                XXXXX, _____, XXXXX,   XXXXX,  _____, XXXXX \
  )
};


// RGB Underglow使用時のレイヤー毎のカラー切り替え
uint32_t layer_state_set_keymap (uint32_t state) {
  return state;
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      rgblight_enable();
      rgblight_sethsv_turquoise();  //デフォルトのRGBカラー
//      RGB_current_mode = rgblight_config.mode;
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        TWI_Init(TWI_BIT_PRESCALE_1, TWI_BITLENGTH_FROM_FREQ(1, 800000));
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}

//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
   iota_gfx_task();
}

void matrix_render_user(struct CharacterMatrix *matrix) {
  if (is_master) {
    matrix_write_ln(matrix, read_layer_state());
    matrix_write_ln(matrix, read_keylog());
    matrix_write_ln(matrix, read_keylogs());
    //matrix_write_ln(matrix, read_mode_icon(keymap_config.swap_lalt_lgui));
    //matrix_write_ln(matrix, read_host_led_state());
    //matrix_write_ln(matrix, read_timelog());
  } else {
    matrix_write(matrix, read_logo());
  }
}

void matrix_update(struct CharacterMatrix *dest, const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;
  matrix_clear(&matrix);
  matrix_render_user(&matrix);
  matrix_update(&display, &matrix);
}

uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _RAISE, _LOWER, _ADJUST);
#ifdef RGBLIGHT_ENABLE
    switch (biton32(state)) {
    case _RAISE:
      rgblight_sethsv_goldenrod();
      break;
    case _LOWER:
      rgblight_sethsv_coral();
      break;
    case _ADJUST:
      rgblight_sethsv_red();
      break;
    default: //  for any other layers, or the default layer
      rgblight_sethsv_cyan();
      break;
    }
#endif
return state;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
    set_timelog();
  }

  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
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
        SEND_STRING("12345679890"); // 送信文字列
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
        SEND_STRING("hoge@hogehoge.hoge"); // 送信文字列
      }
      return false;
      break;
    case MCR4:
      if (record->event.pressed) {
        SEND_STRING("=\"\""SS_TAP(X_LEFT)); // 送信文字列
      }
      return false;
      break;
    case MCR5:
      if (record->event.pressed) {
        SEND_STRING("<>"SS_TAP(X_LEFT)); // 送信文字列
      }
      return false;
      break;
  }
  return true;
}

#endif
