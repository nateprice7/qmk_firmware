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
#define DVORAK DF(_DVORAK)
#define KEYPAD TG(_KEYPAD)
#define CMD OSL(_COMMAND)

enum custom_keycodes {
  UPDIR = SAFE_RANGE,
  TMXCPY,
  TMXPST,
  VIMSV,
};
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
    case VIMSV:  // Vim Save Command
      if (record->event.pressed) {
        SEND_STRING(SS_TAP(X_ESC)":w"SS_TAP(X_ENT));
      }
      return false;
        //SEND_STRING(SS_TAP(KC_ESC) ":w" SS_TAP(KC_ENT));
  }
  return true;
}

////Tap Dance Declarations
///https://github.com/qmk/qmk_firmware/blob/master/docs/feature_tap_dance.md
enum {
    U_TMUX,
    TD_H_ESC,
    TD_CMD_MAC,
    TD_CMD_PC
};

static inline uint16_t RSFT_IF_CAPSWORD(uint16_t keycode) {
    return unlikely(is_caps_word_on())
        ? RSFT(keycode)
        : keycode;
}

void u_tmux(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        SEND_STRING(SS_LCTL("k"));
        //SEND_STRING("Safety dance!");
        //tap_code16(C(KC_K));
        reset_tap_dance(state);
    } else {
        register_code16(RSFT_IF_CAPSWORD(KC_U));
        //register_code(KC_U);
    }
}
void u_tmux_reset(tap_dance_state_t *state, void *user_data)
{
    if (state->count < 2) {
        unregister_code(KC_U);
    }
}
void h_esc(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        //SEND_STRING(KC_ESC);
        tap_code16(KC_ESC);
        //SEND_STRING("Safety dance!");
        //tap_code16(C(KC_K));
        reset_tap_dance(state);
    } else {
        register_code16(RSFT_IF_CAPSWORD(KC_H));
        //register_code(KC_U);
    }
}
void h_esc_reset(tap_dance_state_t *state, void *user_data)
{
    if (state->count < 2) {
        unregister_code(KC_H);
    }
}
void h_mac(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        //SEND_STRING(KC_ESC);
        if (layer_state_is(_DVORAK_MAC_MODE)) {
            //if already set, then switch it off
            layer_off(_DVORAK_MAC_MODE);
        } else {
            //if not already set, then switch the layer on
            layer_on(_DVORAK_MAC_MODE);
        }
        //tap_code16(C(KC_K));
        reset_tap_dance(state);
    } else {
        register_code16(RSFT_IF_CAPSWORD(KC_RCTL));
        //register_code(KC_U);
    }
}
void h_mac_reset(tap_dance_state_t *state, void *user_data)
{
    if (state->count < 2) {
        unregister_code(KC_RCTL);
    }
}
void h_pc(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 2) {
        //SEND_STRING(KC_ESC);
        if (layer_state_is(_DVORAK_MAC_MODE)) {
            //if already set, then switch it off
            layer_off(_DVORAK_MAC_MODE);
        } else {
            //if not already set, then switch the layer on
            layer_on(_DVORAK_MAC_MODE);
        }
        //tap_code16(C(KC_K));
        reset_tap_dance(state);
    } else {
        register_code16(RSFT_IF_CAPSWORD(KC_RGUI));
        //register_code(KC_U);
    }
}
void h_pc_reset(tap_dance_state_t *state, void *user_data)
{
    if (state->count < 2) {
        unregister_code(KC_RGUI);
    }
}

// Associate our tap dance key with its functionality
tap_dance_action_t tap_dance_actions[] = {
    //[U_LAYR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, ql_finished, ql_reset),
    [U_TMUX] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, u_tmux, u_tmux_reset),
    //[U_TMUX] = ACTION_TAP_DANCE_FN(u_tmux),
    [TD_H_ESC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, h_esc, h_esc_reset),
    //[TD_H_ESC] = ACTION_TAP_DANCE_DOUBLE(RSFT_IF_CAPSWORD(KC_H), KC_ESC),
    [TD_CMD_MAC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, h_mac, h_mac_reset),
    [TD_CMD_PC] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, h_pc, h_pc_reset),
};


// Set a custom tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 175;
        default:
            return TAPPING_TERM;
    }
}
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        //case KC_MINS: // CHANGE - do not shift -_ (see below)
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            send_keyboard_report();
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_MINS: // CHANGE - default is -_ is shifted.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        // Tapdance keys are handled manually.
        case TD(U_TMUX):
        case TD(TD_H_ESC):
        case TD(TD_CMD_MAC):
        case TD(TD_CMD_PC):
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_DVORAK] = LAYOUT(
        // left hand
        KC_ESC,        KC_F1,   KC_F2,   KC_F3,   KC_F4,  KC_F5, KC_F6, KC_F7, KC_F8,        KC_F9,  KC_F10,   KC_F11,   KC_F12,   KC_PSCR, KC_SCRL, KC_PAUS, KEYPAD, QK_BOOT,
        KC_EQL,        KC_1,    KC_2,    KC_3,    KC_4,   KC_5,        KC_6,   KC_7,     KC_8,     KC_9,     KC_0,    KC_MINS,
        KC_TAB,        KC_QUOT, KC_COMM, KC_DOT,  KC_P,   KC_Y,        KC_F,   KC_G,     KC_C,     KC_R,     KC_L,    KC_SLSH,
        CMD,           KC_A,    KC_O,    KC_E,    TD(U_TMUX),   KC_I,        KC_D,   TD(TD_H_ESC),     KC_T,     KC_N,     KC_S,    KC_BSLS,
        KC_LSFT,       KC_SCLN, KC_Q,    KC_J,    KC_K,   KC_X,        KC_B,   KC_M,     KC_W,     KC_V,     KC_Z,    KC_RSFT,
                       KC_GRV,  KC_INS, KC_LEFT, KC_RIGHT,                          KC_UP,  KC_DOWN, KC_LBRC, KC_RBRC,

                            KC_LCTL, KC_LALT,        KC_RALT,    TD(TD_CMD_MAC),
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
        _______,KC_MSTP,KC_MPRV,KC_MPLY,KC_MNXT,KC_MSEL,        _______,VIMSV,  KC_5,  KC_6,  KC_PPLS,_______,
        _______,_______,_______,_______,_______,_______,        _______,KC_1,  KC_2,  KC_3,  KC_PENT,_______,
        _______,_______,_______,_______,                TMXCPY,TMXPST,KC_MS_BTN3,UPDIR,
        _______,_______,        _______,_______,
        _______,        _______,
        _______,_______,_______,        _______,KC_PENT,KC_0,
        _______,_______,_______
        ),

[_DVORAK_MAC_MODE] = LAYOUT(
        // left hand
        _______,  _______,  _______,   _______,   _______,  _______, _______, _______, _______,        _______,  _______,   _______,   _______,   _______, _______, _______, _______, _______,
        _______,   _______,    _______,    _______,    _______,   _______,        _______,   _______,     _______,     _______,     _______,    _______,
        _______,   _______, _______, _______,  _______,   _______,        _______,   _______,     _______,     _______,     _______,    _______,
        _______,   _______,    _______,    _______,    _______,   _______,        _______,   _______,     _______,     _______,     _______,    _______,
        _______,  _______, _______,    _______,    _______,   _______,        _______,   _______,     _______,     _______,     _______,    _______,
                  _______,  _______,  _______, _______,                          _______,  _______, _______,    _______,

                            KC_LGUI, KC_RCTL,        KC_RALT,    TD(TD_CMD_PC),
                                     _______,        _______,
                   _______, _______,  _______,        _______, _______, _______,
                   _______,_______,_______
    )
};
