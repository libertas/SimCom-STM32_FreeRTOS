#include "FIFO.h"

bool init_fifo(fifo *p, char buf[], FIFO_INDEX_TYPE max_size)
{
	p->data = buf;
	p->front = p->rear = 0;
	p->max_size = max_size;
	return true;
}

bool in_fifo(fifo *p, char c)
{
	p->data[p->rear] = c;
	p->rear = (p->rear + 1) % p->max_size;

	return true;
}

bool out_fifo(fifo *p, char *c)
{
	if(p->front != p->rear) {
		*c = p->data[p->front];
		p->front = (p->front + 1) % p->max_size;
		return true;
	}

	return false;
}
