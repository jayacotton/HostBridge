/* this code:  CircularBuffer was lifted enmas from
phillipjohnston at github.

https://github.com/embeddedartistry/embedded-resources/blob/master/examples/c/circular_buffer/circular_buffer.c

*/
  
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "circular.h"

// The definition of our circular buffer structure is hidden from the user
struct circular_buf_t {
	unsigned char * buffer;
	unsigned int head;
	unsigned int tail;
	unsigned int max; //of the buffer
	bool full;
};

//#pragma mark - Private Functions -

static void advance_pointer(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif
	if(cbuf->full)
    {
        cbuf->tail = (cbuf->tail + 1) % cbuf->max;
    }

	cbuf->head = (cbuf->head + 1) % cbuf->max;

	// We mark full because we will advance tail on the next time around
	cbuf->full = (cbuf->head == cbuf->tail);
}

static void retreat_pointer(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif

	cbuf->full = false;
	cbuf->tail = (cbuf->tail + 1) % cbuf->max;
}

//#pragma mark - APIs -

cbuf_handle_t circular_buf_init(unsigned char* buffer, unsigned int size)
{
#ifdef DEBUG
	assert(buffer && size);
#endif

	cbuf_handle_t cbuf = malloc(sizeof(circular_buf_t));
#ifdef DEBUG
	assert(cbuf);
#endif

	cbuf->buffer = buffer;
	cbuf->max = size;
	circular_buf_reset(cbuf);
#ifdef DEBUG
	assert(circular_buf_empty(cbuf));
#endif

	return cbuf;
}

void circular_buf_free(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif
	free(cbuf);
}

void circular_buf_reset(cbuf_handle_t cbuf)
{
#ifdef DEBUG
    assert(cbuf);
#endif

    cbuf->head = 0;
    cbuf->tail = 0;
    cbuf->full = false;
}

unsigned int circular_buf_size(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif

	unsigned int size = cbuf->max;

	if(!cbuf->full)
	{
		if(cbuf->head >= cbuf->tail)
		{
			size = (cbuf->head - cbuf->tail);
		}
		else
		{
			size = (cbuf->max + cbuf->head - cbuf->tail);
		}

	}

	return size;
}

unsigned int circular_buf_capacity(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif

	return cbuf->max;
}

void circular_buf_put(cbuf_handle_t cbuf, unsigned char data)
{
#ifdef DEBUG
	assert(cbuf && cbuf->buffer);
#endif

    cbuf->buffer[cbuf->head] = data;

    advance_pointer(cbuf);
}

int circular_buf_put2(cbuf_handle_t cbuf, unsigned char data)
{
    int r = -1;
#ifdef DEBUG
    assert(cbuf && cbuf->buffer);
#endif

    if(!circular_buf_full(cbuf))
    {
        cbuf->buffer[cbuf->head] = data;
        advance_pointer(cbuf);
        r = 0;
    }

    return r;
}

int circular_buf_get(cbuf_handle_t cbuf, unsigned char * data)
{
#ifdef DEBUG 
    assert(cbuf && data && cbuf->buffer);
#endif
    int r = -1;

    if(!circular_buf_empty(cbuf))
    {
        *data = cbuf->buffer[cbuf->tail];
        retreat_pointer(cbuf);

        r = 0;
    }

    return r;
}

bool circular_buf_empty(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif

    return (!cbuf->full && (cbuf->head == cbuf->tail));
}

bool circular_buf_full(cbuf_handle_t cbuf)
{
#ifdef DEBUG
	assert(cbuf);
#endif

    return cbuf->full;
}
