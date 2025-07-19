#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "timer.h"

// Named HSV tuples for readability
#define BASE        0, 0
#define WHITE       0, 0
#define RED         0, 255
#define ORANGE     11, 255
#define YELLOW     43, 255
#define GREEN      85, 255
#define CYAN      128, 255
#define BLUE      170, 255
#define PURPLE    191, 255
#define NAVY      240, 255

static uint8_t brightness = 0;
static uint16_t brightness_timer = 0;

/**
 * custom key codes
 */
enum custom_keycodes {
    MACRO_CTRL_ALT_O = SAFE_RANGE,
    MACRO_CTRL_ALT_M,
};

/**
 * KEYMAP
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_TAB,     KC_Q,         KC_W,           KC_F,         KC_P,      KC_B,                                          KC_J,          KC_L,          KC_U,            KC_Y,           KC_QUOT,         KC_TRNS,
        KC_ESC,     KC_A,         KC_R,           KC_S,         KC_T,      KC_G,                                          KC_M,          KC_N,          KC_E,            KC_I,           KC_O,            KC_TRNS,
        KC_LCTL,    LCTL_T(KC_Z), LALT_T(KC_X),   LGUI_T(KC_C), KC_D,      KC_V,           MO(5),          MO(7),         KC_K,          KC_H,          RGUI_T(KC_COMM), RALT_T(KC_DOT), RCTL_T(KC_SLSH), KC_ENT,
                                                                KC_ESC,    LSFT_T(KC_TAB), LT(4, KC_BSPC), LT(4,KC_SPC),  LT(6,KC_ENT),  KC_LALT
    ),
    [1] = LAYOUT(
        KC_TRNS,    KC_I,         KC_Q,           KC_W,         KC_E,      KC_R,                                          KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_LSFT,      KC_A,           KC_S,         KC_D,      KC_F,                                          KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_M,         KC_Z,           KC_X,         KC_C,      KC_V,           MO(5),          MO(7),         KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
                                                                KC_TRNS,   MO(4),          KC_SPC,         KC_TRNS,       KC_TRNS,       KC_TRNS
    ),
    [2] = LAYOUT(
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,                                       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,                                       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,        KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
                                                                KC_TRNS,   KC_TRNS,        KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS
    ),
    [3] = LAYOUT(
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,                                       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,                                       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,   KC_TRNS,        KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
                                                                KC_TRNS,   KC_TRNS,        KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS
    ),
    [4] = LAYOUT(
        KC_TRNS,    KC_TAB,       KC_BSPC,        KC_UP,        KC_DELETE, KC_LBRC,                                       KC_RBRC,       KC_GRV,        LSFT(KC_EQL),    LSFT(KC_BSLS),  LSFT(KC_QUOTE),  KC_TRNS,
        KC_TRNS,    KC_LSFT,      KC_LEFT,        KC_DOWN,      KC_RIGHT,  LSFT(KC_9),                                    LSFT(KC_0),    KC_MINS,       KC_EQL,          KC_BSLS,        KC_SCLN,         KC_TRNS,
        KC_TRNS,    KC_LGUI,      KC_TRNS,        KC_HOME,      KC_END,    LSFT(KC_LBRC),  KC_TRNS,        KC_TRNS,       LSFT(KC_RBRC), LSFT(KC_MINS), LSFT(KC_COMM),   LSFT(KC_DOT),   LSFT(KC_SLSH),   KC_TRNS,
                                                                KC_TRNS,   KC_TRNS,        KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS
    ),
    [5] = LAYOUT(
        KC_TRNS,    LGUI(KC_S),   KC_VOLD,        KC_MUTE,      KC_VOLU,    KC_BRIU,                                      RGB_VAI,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_MPRV,        KC_MPLY,      KC_MNXT,    KC_BRID,                                      RGB_VAD,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
                                                                QK_BOOT,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_TRNS,       QK_BOOT
    ),
    [6] = LAYOUT(
        KC_TRNS,    KC_F1,        KC_F2,          KC_F3,        KC_F4,      KC_F5,                                        KC_F6,         KC_F7,         KC_F8,           KC_F9,          KC_F10,          KC_TRNS,
        KC_TRNS,    KC_1,         KC_2,           KC_3,         KC_4,       KC_5,                                         KC_6,          KC_7,          KC_8,            KC_9,           KC_0,            KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_F11,        KC_F12,        KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
                                                                KC_TRNS,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS
    ),
    [7] = LAYOUT(
        KC_TRNS,    TO(0),        TO(1),          KC_TRNS,      KC_TRNS,    KC_TRNS,                                      KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,    KC_TRNS,                                      KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,         KC_TRNS,
        KC_TRNS,    KC_TRNS,      KC_TRNS,        KC_TRNS,      KC_TRNS,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_TRNS,       KC_TRNS,       KC_TRNS,         KC_TRNS,        KC_TRNS,          KC_TRNS,
                                                                QK_BOOT,    KC_TRNS,       KC_TRNS,        KC_TRNS,       KC_TRNS,       QK_BOOT
    ),
};

/**
 * Tapping Term
 * Maximum time between key press and release to be considered a tap
 */
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_RGUI, KC_COMM):
        case MT(MOD_RALT, KC_DOT):
        case MT(MOD_RCTL, KC_SLSH):
            return TAPPING_TERM + 50;
        case MT(MOD_LSFT, KC_TAB):
        case MT(MOD_LALT, KC_X):
        case MT(MOD_LGUI, KC_C):
        case MT(MOD_LCTL, KC_Z):
            return TAPPING_TERM + 100;
        default:
            return TAPPING_TERM;
    }
}

/**
 * Quick Tap Term
 * Time to treat a tap-hold key as a tap if a second key is pressed quickly after it
 * Enables tap-repeat and prevents accidental holds
 */
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LSFT, KC_TAB):
        case LT(4, KC_SPC):
        case LT(4, KC_BSPC):
            return 200;
        case MT(MOD_LALT, KC_X):
        case MT(MOD_LGUI, KC_C):
        case MT(MOD_LCTL, KC_Z):
            return 300;
        default:
            return 0;
    }
}

/**
 * Get hold on other key press
 * If a key is held when another is pressed, treat it as a hold, even if it's within the tapping term
 */
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LSFT, KC_TAB):
            return true;
        default:
            return false;
    }
}

/**
 * Permissive Hold
 * Favor hold when another key is pressed during the tapping term
 */
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LSFT, KC_TAB):
        case LT(4, KC_SPC):
        case LT(4, KC_BSPC):
            return true;
        default:
            return false;
    }
}

/**
 * Retro Tapping
 * Wait until a key is released to determine if it should be a tap or hold action
 */
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MT(MOD_LCTL, KC_Z):
        case MT(MOD_LALT, KC_X):
        case MT(MOD_LGUI, KC_C):
        case MT(MOD_RGUI, KC_COMM):
        case MT(MOD_RALT, KC_DOT):
        case MT(MOD_RCTL, KC_SLSH):
            return true;
        default:
            return false;
    }
}

/**
 * Auto Shift
 * Selectively enable autoshift for specific layers & keys
 */
bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    uint8_t layer = get_highest_layer(layer_state);
    switch(layer) {
        case 4:
            switch (keycode) {
                case KC_GRV:
                case KC_MINS:
                case KC_EQL:
                case KC_BSLS:
                case KC_SCLN:
                    return true;
            }
            break;
        case 6:
            switch (keycode) {
                case KC_1:
                case KC_2:
                case KC_3:
                case KC_4:
                case KC_5:
                case KC_6:
                case KC_7:
                case KC_8:
                case KC_9:
                case KC_0:
                    return true;
            }
            break;
    }
    return false;
}

/**
 * RGB SETTINGS
 */
void keyboard_post_init_user(void) {
    rgb_matrix_enable();
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
    // rgb_matrix_sethsv(32, 128, 128);
    eeconfig_read_rgb_matrix(&rgb_matrix_config);   // ✅ correct for latest QMK
    brightness = rgb_matrix_get_val();
    rgb_matrix_sethsv(WHITE, brightness);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    brightness = rgb_matrix_get_val();
    rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);

    switch (layer) {
        case 0:
            rgb_matrix_sethsv(WHITE, brightness);
            break;
        case 1:
            rgb_matrix_sethsv(RED, brightness);
            break;
        case 4:
            rgb_matrix_sethsv(BLUE, brightness);
            break;
        case 6:
            rgb_matrix_sethsv(PURPLE, brightness);
            break;
    }

    return state;
}

/**
 * Per-key handling
 */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case RGB_VAI:
            case RGB_VAD:
                brightness = rgb_matrix_get_val();
                brightness_timer = timer_read();  // debounce
                break;
        }
    }
}

void matrix_scan_user(void) {
    if (brightness_timer && timer_elapsed(brightness_timer) > 1000) {
        eeconfig_update_rgb_matrix(&rgb_matrix_config);  // writes current config from internal state
        brightness_timer = 0;
    }
}
