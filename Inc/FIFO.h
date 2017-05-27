#ifndef _FIFO_H
#define _FIFO_H

#include "SimCom.h"

#define FIFO_INDEX_TYPE SIMCOM_LENGTH_TYPE

typedef struct {
	char *data;
	FIFO_INDEX_TYPE front;
	FIFO_INDEX_TYPE rear;
	FIFO_INDEX_TYPE max_size;
} fifo;

bool init_fifo(fifo *p, char buf[], FIFO_INDEX_TYPE max_size);
bool in_fifo(fifo *p, char c);
bool out_fifo(fifo *p, char *c);

#endif
