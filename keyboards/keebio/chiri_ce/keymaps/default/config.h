/* SPDX-License-Identifier: GPL-2.0-or-later */
#pragma once

// Since the outer columns can be broken off, redefine the bootmagic keys as second column from outside
#define BOOTMAGIC_ROW 0
#define BOOTMAGIC_COLUMN 1

#undef BOOTMAGIC_COLUMN_RIGHT
#undef BOOTMAGIC_ROW_RIGHT
#define BOOTMAGIC_ROW_RIGHT 4
#define BOOTMAGIC_COLUMN_RIGHT 1

#define TAPPING_TERM 200
#define AUTO_SHIFT_TIMEOUT 200
#define AUTO_SHIFT_NO_SETUP
#define NO_AUTO_SHIFT_SPECIAL
#define TAPPING_CHORDAL_HOLD
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
