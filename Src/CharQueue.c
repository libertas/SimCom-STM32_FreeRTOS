#include "CharQueue.h"

bool init_char_queue(char_queue *p, char buf[], CHARQUEUE_INDEX_TYPE max_size)
{
	p->data = buf;
	p->front = p->rear = max_size - 1;
	p->count = 0;
	p->max_size = max_size;
	return true;
}

bool in_char_queue(char_queue *p, char c)
{
	if(p->count == p->max_size)
		return false;

	p->rear = (p->rear + 1) % p->max_size;
	p->data[p->rear] = c;
	p->count++;

	return true;
}

bool out_char_queue(char_queue *p, char *c)
{
	if(0 == p->count)
		return false;

	p->front = (p->front + 1) % p->max_size;
	*c = p->data[p->front];
	p->count--;

	return true;
}
