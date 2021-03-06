/*
 * paper-tiger - A Terraria server written in C for POSIX operating systems
 * Copyright (C) 2016  Tyler Watson <tyler@tw.id.au>
 *
 * This file is part of paper-tiger.
 *
 * paper-tiger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * paper-tiger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paper-tiger.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "talloc/talloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define binary_writer_write_value(dest, src) binary_writer_write_internal(dest, &src, sizeof(src));

static inline int
binary_writer_write_internal(void *dest, const void *src, int n)
{
	if (dest != NULL) {
		memcpy(dest, src, n);
	}

	return n;
}

int
binary_writer_7bit_len(int value);

void
binary_writer_write_7bit_int(uint8_t *buf, int value, int *pos);

int
binary_writer_write_string(void *dest, const char *src);

#ifdef __cplusplus
}
#endif
