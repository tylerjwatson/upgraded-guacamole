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

#include "talloc/talloc.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct binary_reader_context {
	char *file_path;
	FILE *fp;
};

int
binary_reader_new(TALLOC_CTX *parent_context, const char *file_path, struct binary_reader_context **out_context);

size_t
binary_reader_pos(struct binary_reader_context *context);

int
binary_reader_skip(struct binary_reader_context *context, size_t num_bytes);

int
binary_reader_open(struct binary_reader_context *context);

int
binary_reader_read_boolean(struct binary_reader_context *context, bool *out_value);

int
binary_reader_read_byte(struct binary_reader_context *context, uint8_t *out_value);

int
binary_reader_read_decimal(struct binary_reader_context *context, long double *out_value);

int
binary_reader_read_double(struct binary_reader_context *context, double *out_value);

int
binary_reader_read_int16(struct binary_reader_context *context, int16_t *out_value);

int
binary_reader_read_int32(struct binary_reader_context *context, int32_t *out_value);

int
binary_reader_read_int64(struct binary_reader_context *context, int64_t *out_value);

int
binary_reader_read_single(struct binary_reader_context *context, float *out_value);

int
binary_reader_read_string(struct binary_reader_context *context, char **out_value);

int
binary_reader_read_string_buffer(uint8_t *buf, int pos, int *out_len, char **out_value);

int
binary_reader_read_uint16(struct binary_reader_context *context, uint16_t *out_value);

int
binary_reader_read_uint32(struct binary_reader_context *context, uint32_t *out_value);

int
binary_reader_read_uint64(struct binary_reader_context *context, uint64_t *out_value);

int
binary_reader_close(struct binary_reader_context *context);

int
binary_reader_read_7bit_int(const uint8_t *buf, int *pos, int *out_value);

#ifdef __cplusplus
}
#endif
