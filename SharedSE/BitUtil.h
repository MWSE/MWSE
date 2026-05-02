#pragma once

#define BIT_SET_ON(d, p) d |= (1 << p)
#define BIT_SET_OFF(d, p) d &= ~(1 << p)
#define BIT_SET(d, p, v) v ? BIT_SET_ON(d, p) : BIT_SET_OFF(d, p)
#define BIT_TEST(d, p) (((d >> p) & 1) == 1)

#define BITMASK_SET_ON(d, m) d |= m
#define BITMASK_SET_OFF(d, m) d &= ~m
#define BITMASK_SET(d, m, v) v ? BITMASK_SET_ON(d, m) : BITMASK_SET_OFF(d, m)
#define BITMASK_TEST(d, m) ((d & m) == m)
