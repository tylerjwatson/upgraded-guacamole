/*
* upgraded-guacamole - A Terraria server written in C for POSIX operating systems
* Copyright (C) 2016  Tyler Watson <tyler@tw.id.au>
*
* This file is part of upgraded-guacamole.
*
* upgraded-guacamole is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.

* upgraded-guacamole is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with upgraded-guacamole.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "player_info.h"
#include "../game.h"
#include "../binary_reader.h"
#include "../util.h"

int player_info_new(TALLOC_CTX *ctx, const struct player *player, struct packet **out_packet)
{
	int ret = -1;
	TALLOC_CTX *temp_context;
	struct packet *packet;

	temp_context = talloc_new(NULL);
	if (temp_context == NULL) {
		_ERROR("%s: out of memory allocating temp context for packet %d\n", __FUNCTION__, PACKET_TYPE_PLAYER_INFO);
		ret = -ENOMEM;
		goto out;
	}

	packet = talloc(temp_context, struct packet);
	if (packet == NULL) {
		_ERROR("%s: out of memory allocating packet %d\n", __FUNCTION__, PACKET_TYPE_PLAYER_INFO);
		ret = -ENOMEM;
		goto out;
	}

	/*
	 * Packet has no payload.
	 */

	packet->type = PACKET_TYPE_PLAYER_INFO;
	packet->len = PACKET_HEADER_SIZE;
	packet->data = NULL;
	packet->player = (struct player *)player;

	*out_packet = (struct packet *)talloc_steal(ctx, packet);

	ret = 0;
out:
	talloc_free(temp_context);

	return ret;
}

int player_info_read(struct packet *packet, const uv_buf_t *buf)
{
	int ret = -1, pos = 0, name_len = 0;
	TALLOC_CTX *temp_context;
	struct player_info *player_info;

	char *name;

	temp_context = talloc_new(NULL);
	if (temp_context == NULL) {
		_ERROR("%s: out of memory allocating temp context for player info.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	player_info = talloc_zero(temp_context, struct player_info);
	if (player_info == NULL) {
		_ERROR("%s: out of memory allocating player info.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	player_info->id = buf->base[pos++];
	player_info->skin_variant = buf->base[pos++];
	player_info->hair = buf->base[pos++];

	binary_reader_read_string_buffer(buf->base, pos, &name_len, &name);

	player_info->name = talloc_size(player_info, name_len + 1);
	memcpy(player_info->name, name, name_len);

	pos += name_len;

	player_info->hair_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->skin_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->eye_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->shirt_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->under_shirt_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->pants_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);
	player_info->shoe_colour = *(struct colour *)buf->base[pos];
	pos += sizeof(struct colour);

	player_info->difficulty = buf->base[pos];

	packet->data = (void *)talloc_steal(packet, player_info);

	ret = 0;
out:
	talloc_free(temp_context);

	return ret;
}