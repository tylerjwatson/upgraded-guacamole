/*
* paper-tiger - A Terraria server written in C for POSIX operating systems
* Copyright (C) 2016  Tyler Watson <tyler@tw.id.au>
* Copyright (C) 2016  DeathCradle <rt.luke.s@gmail.com>
*
* This file is part of paper-tiger.
*
* paper-tiger is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 2 of the License, or
* (at your option) any later version.

* paper-tiger is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with paper-tiger.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>

#include "packets/disconnect.h"

#include "packet.h"
#include "server.h"
#include "binary_reader.h"
#include "binary_writer.h"
#include "util.h"

int disconnect_new(TALLOC_CTX *ctx, const struct player *player, const char * reason, struct packet **out_packet)
{
	int ret = -1;
	TALLOC_CTX *temp_context;
	struct packet *packet;
	struct disconnect *disconnect;

	temp_context = talloc_new(NULL);
	if (temp_context == NULL) {
		_ERROR("%s: out of memory allocating temp context for packet %d\n", __FUNCTION__, PACKET_TYPE_DISCONNECT);
		ret = -ENOMEM;
		goto out;
	}

	packet = talloc(temp_context, struct packet);
	if (packet == NULL) {
		_ERROR("%s: out of memory allocating packet %d\n", __FUNCTION__, PACKET_TYPE_DISCONNECT);
		ret = -ENOMEM;
		goto out;
	}

	disconnect = talloc(temp_context, struct disconnect);
	if (disconnect == NULL) {
		_ERROR("%s: out of memory allocating disconnect.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	packet->type = PACKET_TYPE_DISCONNECT;
	packet->len = PACKET_HEADER_SIZE + (uint16_t)strlen(reason) + 1;
	packet->data = NULL;

	disconnect->reason = talloc_strdup(disconnect, reason);
	packet->data = (void *)talloc_steal(packet, disconnect);

	*out_packet = (struct packet *)talloc_steal(ctx, packet);

	ret = 0;
out:
	talloc_free(temp_context);

	return ret;
}

int disconnect_read(struct packet *packet)
{
	TALLOC_CTX *temp_context;
	int ret = -1, pos = 0, str_len;
	struct disconnect *disconnect;
	char *reason;

	temp_context = talloc_new(NULL);
	if (temp_context == NULL) {
		_ERROR("%s: out of memory allocating temp context for disconnect.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	disconnect = talloc_zero(temp_context, struct disconnect);
	if (disconnect == NULL) {
		_ERROR("%s: out of memory allocating disconnect.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	binary_reader_read_7bit_int(packet->data_buffer, &pos, &str_len);

	reason = talloc_size(temp_context, str_len + 1);
	if (reason == NULL) {
		_ERROR("%s: out of memory copying reason to packet.\n", __FUNCTION__);
		ret = -ENOMEM;
		goto out;
	}

	memcpy(reason, packet->data_buffer + pos, str_len);

	reason[str_len] = '\0';

	disconnect->reason = talloc_steal(disconnect, reason);
	packet->data = (void *)talloc_steal(packet, disconnect);

	ret = 0;
out:
	talloc_free(temp_context);

	return ret;
}

int disconnect_handle(struct player *player, struct packet *packet)
{
	struct disconnect *disconnect = (struct disconnect *)packet->data;

	(void)disconnect;

	return 0;
}

int disconnect_write(const ptGame *game, struct packet *packet)
{
	struct disconnect *disconnect = (struct disconnect *)(packet->data);

	int packet_len = binary_writer_write_string(packet->data_buffer, disconnect->reason);

	return packet_len;
}
