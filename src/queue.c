/* queue.c
 *
 * Copyright (C) 2007 Tillmann Werner <tillmann.werner@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"


qelem *queue_ins(queue *q, void *data, ssize_t max_size) {
	qelem* tmp = NULL;

	if (!q || !data) return(NULL);

	/* need to cut off last queue element? */
	if (q->size >= max_size) tmp = queue_cuttail(q);

	/* prepend new element */
	queue_prepend(q, data);

	return(tmp);
}


void *queue_unlink(queue *q, qelem *e) {
	void *data;

	if (!q || !e) return(NULL);

	if (e == q->head) {
		e = queue_cuthead(q);
	} else if (e == q->tail) {
		e = queue_cuttail(q);
	} else {
		e->prev->next = e->next;
		e->next->prev = e->prev;
		if (!q->size--) q->head = q->tail = NULL;
	}

	data = e->data;
	free(e);

	return(data);
}


queue *queue_new(void) {
	queue *q = calloc(1, sizeof(queue));

	return(q);
}


void queue_free(queue *q, void(*cbfn)(void *data)) {
	qelem *cur;

	if (!q) return;

	while (q->head) {
		cur = q->head;
		q->head = q->head->next;
		if (cbfn) cbfn(cur->data);
		free(cur);
	}

	free(q);

	return;
}
