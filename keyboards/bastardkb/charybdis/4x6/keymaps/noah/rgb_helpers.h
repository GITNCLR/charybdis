#pragma once

#ifdef RGB_MATRIX_ENABLE

// We assume QMK_KEYBOARD_H (or quantum.h) has already been included
// in the file that includes this header, so we have:
// - rgb_matrix_set_color
// - RGB_MATRIX_LED_COUNT
// - is_keyboard_left()
// - rgb_t, hsv_t, hsv_to_rgb, etc.

// ------------------------------------------------------------
// Side awareness helpers
// ------------------------------------------------------------

static inline bool led_is_left(uint8_t index) {
    // 0–28 → left half
    return index < 29;
}

static inline bool led_is_right(uint8_t index) {
    // 29–55 → right half (56/57 are the simulated LEDs)
    return (index >= 29 && index <= 55);
}

// This tells us which physical half we are running on at runtime.
static inline bool this_is_left_half(void) {
    return is_keyboard_left();
}

// ------------------------------------------------------------
// Core LED helpers
// ------------------------------------------------------------

static inline void set_led_rgb(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= RGB_MATRIX_LED_COUNT) {
        return;
    }

    // Filter: only act on LEDs belonging to this half
    if (this_is_left_half()) {
        if (!led_is_left(index)) {
            return;
        }
    } else {
        if (!led_is_right(index)) {
            return;
        }
    }

    rgb_matrix_set_color(index, r, g, b);
}

// Set a single LED to one color
static inline void set_led_color(uint8_t index, rgb_t color) {
    set_led_rgb(index, color.r, color.g, color.b);
}

// Set a list of arbitrary LEDs (e.g. {7, 9, 1}) to one color
static inline void set_led_group(const uint8_t *indices, uint8_t count, rgb_t color) {
    for (uint8_t i = 0; i < count; i++) {
        set_led_color(indices[i], color);
    }
}

// Fill a range of LEDs [from, to) with one color
static inline void fill_led_range(uint8_t from, uint8_t to, rgb_t color) {
    if (from >= RGB_MATRIX_LED_COUNT) return;
    if (to > RGB_MATRIX_LED_COUNT) to = RGB_MATRIX_LED_COUNT;

    for (uint8_t i = from; i < to; i++) {
        set_led_color(i, color);
    }
}

// Convenience wrappers for whole halves
static inline void set_left_side(rgb_t color) {
    for (uint8_t i = 0; i < 29; i++) {
        set_led_color(i, color);
    }
}

static inline void set_right_side(rgb_t color) {
    for (uint8_t i = 29; i <= 55; i++) {
        set_led_color(i, color);
    }
}

static inline void set_both_sides(rgb_t color) {
    set_left_side(color);
    set_right_side(color);
}

#endif // RGB_MATRIX_ENABLE