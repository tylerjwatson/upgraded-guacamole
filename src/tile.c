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

#include <signal.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <WinSock2.h>
#include <direct.h>
#include "fcntl.h"
#include "windows-mmap.h"
#else
#include <sys/mman.h>
#include <unistd.h>
#include <libgen.h>
#endif

#include "binary_writer.h"
#include "game.h"
#include "tile.h"
#include "util.h"
#include "world.h"

int
tile_heap_new(TALLOC_CTX *context, const uint32_t size_x, const uint32_t size_y, struct tile ***out_tiles)
{
	TALLOC_CTX *temp_context;
	int ret;
	struct tile **tile_heap;

	if ((temp_context = talloc_new(NULL)) == NULL) {
		_ERROR("%s: Could not allocate talloc context for tile heap.", __FUNCTION__);
		ret = -1;
		goto out;
	}

	if ((tile_heap = talloc_zero_array(temp_context, struct tile *, size_x)) == NULL) {
		_ERROR("%s: Could not allocate X tile heap of size %d\n", __FUNCTION__, size_x);
		ret = -1;
		goto out;
	}

	for (unsigned column = 0; column < size_x; column++) {
		if ((tile_heap[column] = talloc_zero_array(tile_heap, struct tile, size_y)) == NULL) {
			_ERROR("%s: Could not allocate Y tile heap of size %d\n", __FUNCTION__, size_y);
			ret = -1;
			goto out;
		}
	}

	*out_tiles = talloc_steal(context, tile_heap);
	ret = 0;
out:
	talloc_free(temp_context);
	return ret;
}

static int
__make_tile_directory(int world_id)
{
	int len = snprintf(NULL, 0, PT_WORLD_PATH, world_id);
	char pt_base[65535];

	snprintf(pt_base, len + 1, PT_WORLD_PATH, world_id);

	int result = mkdir(pt_base);

	/*
	 * If the file exists already, that's cool
	 */
	if (result == -1 && errno == EEXIST) {
		return 0;
	}

	return result;
}

int
tile_container_init(TALLOC_CTX *context, struct tile_container *container, struct world *world)
{
	int fd = 0;
	int ret = -1;
	int tiles_path_len = snprintf(NULL, 0, PT_TILES_PATH, world->worldID);
	size_t tile_size = sizeof(struct tile) * world->max_tiles_x * world->max_tiles_y;
	char tiles_path[65535];

	tiles_path_len = snprintf(tiles_path, tiles_path_len + 1, PT_TILES_PATH, world->worldID);

	container->mmap_file_name = talloc_strdup(context, tiles_path);

	if (__make_tile_directory(world->worldID) == -1) {
		_ERROR("%s: mkdir for %s failed: %s\n", __FUNCTION__, tiles_path, strerror(errno));
		ret = -1;
		goto error;
	}

	fd = open(container->mmap_file_name, O_RDWR | O_CREAT, 0600);
	if (fd == -1) {
		_ERROR("%s: cannot open tile map %s: %s\n", __FUNCTION__, tiles_path, strerror(errno));
		ret = -1;
		goto error;
	}

	if (lseek(fd, tile_size, SEEK_SET) < 0) {
		_ERROR("%s: cannot seek %s to %zu bytes: %s\n", __FUNCTION__, container->mmap_file_name, tile_size,
			   strerror(errno));
		ret = -1;
		goto error;
	}

	write(fd, "", 1);

	if ((container->tile_memory = (struct tile *)mmap(NULL, tile_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) ==
		MAP_FAILED) {
		return -1;
	}

	container->mmap_size = tile_size;
	container->mmap_fd = fd;

	ret = 0;
	return ret;

error:
	if (fd > 0) {
		close(fd);
	}

	return ret;
}

void
tile_container_destroy(struct tile_container *container)
{
#ifndef _WIN32
	msync(container->tile_memory, container->mmap_size, MS_SYNC);
#endif
	munmap(container->mmap_file_name, container->mmap_size);
	close(container->mmap_fd);
}

bool
tile_active(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_ACTIVE) == S_TILE_HEADER_ACTIVE;
}

void
tile_set_active(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 5);
	} else {
		BIT_CLEAR(tile->s_tile_header, 5);
	}
}

uint8_t
tile_colour(const struct tile *tile)
{
	return (uint8_t)(tile->s_tile_header & 31);
}

void
tile_set_colour(struct tile *tile, uint8_t colour)
{
	tile->s_tile_header &= (65504 | (colour > 30 ? 30 : colour));
}

uint8_t
tile_wall_colour(const struct tile *tile)
{
	return (uint8_t)(tile->b_tile_header & 31);
}

void
tile_set_wall_colour(struct tile *tile, uint8_t colour)
{
	tile->s_tile_header &= (224 | (colour > 30 ? 30 : colour));
}

bool
tile_honey(const struct tile *tile)
{
	return (tile->b_tile_header & B_TILE_HEADER_HONEY) == B_TILE_HEADER_HONEY;
}

void
tile_set_honey(struct tile *tile, bool honey)
{
	if (honey) {
		tile->b_tile_header &= (159 | B_TILE_HEADER_HONEY);
	} else {
		tile->b_tile_header &= 191;
	}
}

bool
tile_lava(const struct tile *tile)
{
	return (tile->b_tile_header & 32) == 32;
}

void
tile_set_lava(struct tile *tile, bool lava)
{
	if (lava) {
		tile->b_tile_header &= (159 | B_TILE_HEADER_HONEY);
	} else {
		tile->b_tile_header &= 223;
	}
}

bool
tile_half_brick(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_HALFBRICK) == S_TILE_HEADER_HALFBRICK;
}

void
tile_set_half_brick(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 10);
	} else {
		BIT_CLEAR(tile->s_tile_header, 10);
	}
}

void
tile_set_wire(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 7);
	} else {
		BIT_CLEAR(tile->s_tile_header, 7);
	}
}

void
tile_set_wire_2(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 8);
	} else {
		BIT_CLEAR(tile->s_tile_header, 8);
	}
}

void
tile_set_wire_3(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 9);
	} else {
		BIT_CLEAR(tile->s_tile_header, 9);
	}
}

void
tile_set_wire_4(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->b_tile_header, 7);
	} else {
		BIT_CLEAR(tile->b_tile_header, 7);
	}
}

bool
tile_actuator(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_ACTUATOR) == S_TILE_ACTUATOR;
}

void
tile_set_actuator(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 11);
	} else {
		BIT_CLEAR(tile->s_tile_header, 11);
	}
}

bool
tile_inactive(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_INACTIVE) == S_TILE_HEADER_INACTIVE;
}

void
tile_set_inactive(struct tile *tile, bool val)
{
	if (val) {
		BIT_SET(tile->s_tile_header, 6);
	} else {
		BIT_CLEAR(tile->s_tile_header, 6);
	}
}

uint8_t
tile_slope(const struct tile *tile)
{
	return (uint8_t)((tile->s_tile_header & 28672) >> 12);
}

bool
tile_wire(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_WIRE) == S_TILE_HEADER_WIRE;
}

bool
tile_wire2(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_WIRE_2) == S_TILE_HEADER_WIRE_2;
}

bool
tile_wire3(const struct tile *tile)
{
	return (tile->s_tile_header & S_TILE_HEADER_WIRE_3) == S_TILE_HEADER_WIRE_3;
}

bool
tile_wire4(const struct tile *tile)
{
	return (tile->b_tile_header & B_TILE_HEADER_WIRE_4) == B_TILE_HEADER_WIRE_4;
}

void
tile_copy(const struct tile *src, struct tile *dest)
{
	memcpy(dest, src, sizeof(struct tile));
}

int
tile_cmp(const struct tile *src, const struct tile *dest)
{
	if (src == NULL || dest == NULL) {
		return false;
	}
	return memcmp(src, dest, sizeof(*src));
}

int
tile_pack_completely(const struct world *world, const struct tile *tile, uint8_t *buffer)
{
	uint8_t tile_data[13];
	uint8_t tile_headers[3];
	int tile_len = 0, buffer_pos = 0;

	memset(tile_data, 0, sizeof(tile_data));
	memset(tile_headers, 0, sizeof(tile_headers));

	if ((tile_len = tile_pack(world->game, tile, tile_data, &tile_headers[0], &tile_headers[1], &tile_headers[2])) <
		0) {
		_ERROR("%s: error packing tile to buffer.\n", __FUNCTION__);
		return -1;
	}

	buffer_pos += binary_writer_write_value(buffer + buffer_pos, tile_headers[0]);
	if ((tile_headers[0] & 1) == 1) {
		buffer_pos += binary_writer_write_value(buffer + buffer_pos, tile_headers[1]);

		if ((tile_headers[1] & 1) == 1) {
			buffer_pos += binary_writer_write_value(buffer + buffer_pos, tile_headers[2]);
		}
	}

	memcpy(&buffer[buffer_pos], tile_data, tile_len);

	return buffer_pos + tile_len;
}

int
tile_pack(const ptGame *game, const struct tile *tile, uint8_t *dest, uint8_t *tile_flags_1, uint8_t *tile_flags_2,
		  uint8_t *tile_flags_3)
{
	unsigned pos = 0;

	uint8_t colour, wall_colour;
	int slope = 0;

	*tile_flags_1 = *tile_flags_2 = *tile_flags_3 = 0;

	if (tile_active(tile) == true) {
		uint8_t lsb = (uint8_t)tile->type;
		*tile_flags_1 |= 2;

		pos += binary_writer_write_value(dest + pos, lsb);

		if (tile->type > 255) {
			uint8_t msb = (uint8_t)(tile->type >> 8);
			*tile_flags_1 |= 32;

			pos += binary_writer_write_value(dest + pos, msb);
		}

		if (tile->type < 0 || tile->type > sizeof(game->tileFrameImportant)) {
			printf("%s: tile type %d is out of range of %ld", __FUNCTION__, tile->type, sizeof(game->tileFrameImportant));
			raise(SIGINT);
		}

		if (game->tileFrameImportant[tile->type]) {
			pos += binary_writer_write_value(dest + pos, tile->frame_x);
			pos += binary_writer_write_value(dest + pos, tile->frame_y);
		}

		if ((colour = tile_colour(tile)) != 0) {
			*tile_flags_3 |= 8;
			pos += binary_writer_write_value(dest + pos, colour);
		}
	}

	if (tile->wall != 0) {
		*tile_flags_1 |= 4;

		pos += binary_writer_write_value(dest + pos, tile->wall);

		if ((wall_colour = tile_wall_colour(tile)) != 0) {
			*tile_flags_3 |= 16;
			pos += binary_writer_write_value(dest + pos, wall_colour);
		}
	}

	if (tile->liquid != 0) {
		if (tile_lava(tile)) {
			*tile_flags_1 |= 16;
		} else if (tile_honey(tile)) {
			*tile_flags_1 |= 24;
		} else {
			*tile_flags_1 |= 8;
		}

		pos += binary_writer_write_value(dest + pos, tile->liquid);
	}

	if (tile_wire(tile)) {
		*tile_flags_2 |= 2;
	}

	if (tile_wire2(tile)) {
		*tile_flags_2 |= 4;
	}

	if (tile_wire3(tile)) {
		*tile_flags_2 |= 8;
	}

	if (tile_wire4(tile)) {
		*tile_flags_3 |= 32;
	}

	if (tile_half_brick(tile)) {
		slope = 16;
	} else if ((slope = tile_slope(tile)) != 0) {
		slope = (slope + 1) << 4;
	}

	*tile_flags_2 |= slope;

	if (tile_actuator(tile)) {
		*tile_flags_3 |= 2;
	}

	if (tile_inactive(tile)) {
		*tile_flags_3 |= 4;
	}

	if (*tile_flags_3 != 0) {
		*tile_flags_2 |= 1;
	}

	if (*tile_flags_2 != 0) {
		*tile_flags_1 |= 1;
	}

	return pos;
}
