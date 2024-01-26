// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
// qmk compile -kb kinesis/kint41 -km dvorak
// teensy_loader_cli -w -v --mcu=TEENSY41 .build/kinesis_kint41_dvorak.hex
// https://getreuer.info/posts/keyboards/macros/index.html
// https://github.com/qmk/qmk_firmware/blob/master/docs/keycodes.md
// https://github.com/qmk/qmk_firmware/blob/master/docs/feature_layers.md#switching-and-toggling-layers
//https://docs.qmk.fm/#/feature_send_string

#include QMK_KEYBOARD_H

enum layer_names {
    _DVORAK,
    _KEYPAD,
    _COMMAND,
    _DVORAK_MAC_MODE,
};
enum custom_keycodes {
  UPDIR = SAFE_RANGE,
  TMXCPY,
  TMXPST,
};
#define DVORAK DF(_DVORAK)
#define KEYPAD TG(_KEYPAD)
#define CMD OSL(_COMMAND)

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  switch (keycode) {
    case UPDIR:  // Types ../ to go up a directory on the shell.
      if (record->event.pressed) {
        SEND_STRING("../");
      }
      return false;
    case TMXCPY:  // Tmux Copy Command
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("k") "[");
      }
      return false;
    case TMXPST:  // Tmux Paste Command
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("k") "]");
      }
      return false;
  }
  return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_DVORAK] = LAYOUT(
        // left hand
        KC_ESC,        KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5, KC_F6, KC_F7, KC_F8,        KC_F9,  KC_F10,   KC_F11,   KC_F12,   KC_PSCR, KC_SCRL, KC_PAUS, KEYPAD, QK_BOOT,
        KC_EQL,        KC_1,    KC_2,    KC_3,    KC_4,   KC_5,        KC_6,   KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,
        KC_TAB,        KC_QUOT, KC_COMM, KC_DOT,  KC_P,   KC_Y,        KC_F,   KC_G,     KC_C,     KC_R,     KC_L,    KC_SLSH,
        CMD,           KC_A,    KC_O,    KC_E,    KC_U,   KC_I,        KC_D,   KC_H,     KC_T,     KC_N,     KC_S,    KC_BSLS,
        KC_LSFT,       KC_SCLN, KC_Q,    KC_J,    KC_K,   KC_X,        KC_B,   KC_M,     KC_W,     KC_V,     KC_Z,    KC_RSFT,
                       KC_GRV,  KC_INS, KC_LEFT, KC_RIGHT,                          KC_UP,  KC_DOWN, KC_LBRC, KC_RBRC,

                            KC_LCTL, KC_LALT,        KC_RALT,    KC_RCTL,
                                     KC_HOME,        KC_PGUP,
                   KC_BSPC, KC_DEL,  KC_END,        KC_PGDN, KC_ENT, KC_SPC,
                   KEYPAD,CMD,KC_ESC
    ),
[_KEYPAD] = LAYOUT (
        _______,_______,_______,_______,_______,_______,_______,_______,_______,        _______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,        _______,KC_NUM, KC_PEQL,KC_PSLS,KC_PAST,_______,
        _______,_______,KC_MUTE,KC_VOLD,KC_VOLU,_______,        _______,KC_7,  KC_8,  KC_9,  KC_PMNS,_______,
        _______,KC_MSTP,KC_MPRV,KC_MPLY,KC_MNXT,KC_MSEL,        _______,KC_4,  KC_5,  KC_6,  KC_PPLS,_______,
        _______,_______,_______,_______,_______,_______,        _______,KC_1,  KC_2,  KC_3,  KC_PENT,_______,
        _______,_______,_______,_______,                KC_LPRN,KC_RPRN,KC_PDOT,KC_PENT,
        _______,_______,        _______,_______,
        _______,        _______,
        _______,_______,_______,        _______,KC_PENT,KC_0,
        _______,_______,_______
        ),

[_COMMAND] = LAYOUT (
        _______,_______,_______,_______,_______,_______,_______,_______,_______,        _______,_______,_______,_______,_______,_______,_______,_______,_______,
        _______,_______,_______,_______,_______,_______,        _______,KC_NUM, KC_PEQL,KC_PSLS,KC_PAST,_______,
        _______,_______,KC_MUTE,KC_VOLD,KC_VOLU,_______,        _______,KC_7,  KC_8,  KC_9,  KC_PMNS,_______,
        _______,KC_MSTP,KC_MPRV,KC_MPLY,KC_MNXT,KC_MSEL,        _______,KC_4,  KC_5,  KC_6,  KC_PPLS,_______,
        _______,_______,_______,_______,_______,_______,        _______,KC_1,  KC_2,  KC_3,  KC_PENT,_______,
        _______,_______,_______,_______,                TMXCPY,TMXPST,KC_MS_BTN3,UPDIR,
        _______,_______,        _______,_______,
        _______,        _______,
        _______,_______,_______,        _______,KC_PENT,KC_0,
        _______,_______,_______
        ),

[_DVORAK_MAC_MODE] = LAYOUT(
        // left hand
        KC_CAPS,  KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5, KC_F6, KC_F7, KC_F8,        KC_F9,  KC_F10,   KC_F11,   KC_F12,   KC_PSCR, KC_SCRL, KC_PAUS, KC_NUM, KC_PWR,
        KC_EQL,   KC_1,    KC_2,    KC_3,    KC_4,   KC_5,        KC_6,   KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,
        KC_TAB,   KC_QUOT, KC_COMM, KC_DOT,  KC_P,   KC_Y,        KC_F,   KC_G,     KC_C,     KC_R,     KC_L,    KC_SLSH,
        KC_ESC,   KC_A,    KC_O,    KC_E,    KC_U,   KC_I,        KC_D,   KC_H,     KC_T,     KC_N,     KC_S,    KC_BSLS,
        KC_LSFT,  KC_SCLN, KC_Q,    KC_J,    KC_K,   KC_X,        KC_B,   KC_M,     KC_W,     KC_V,     KC_Z,    KC_RSFT,
                  KC_GRV,  KC_INS,  KC_LEFT, KC_RIGHT,                          KC_UP,  KC_DOWN, KC_LBRC,    KC_RBRC,

                            KC_LGUI, KC_LALT,        KC_RCTL,    KC_RGUI,
                                     KC_HOME,        KC_PGUP,
                   KC_BSPC, KC_DEL,  KC_END,        KC_PGDN, KC_ENT, KC_SPC,
                   KC_ESC,KC_ESC,KC_ESC
    )
};
