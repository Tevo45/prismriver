#include <u.h>
#include <libc.h>

#include "prismriver.h"
#include "p-internal.h"

Buffer*
createbuffer(ulong s)
{
	/* This could be a big contiguous area instead if you really need to
	 * save a malloc invocation
	 */
	Buffer *b = emallocz(sizeof(Buffer));
	b->size = s;
	b->data = emallocz(s * sizeof(Stuple));
	return b;
}

Buffer*
resizebuffer(Buffer *b, ulong ns)
{
	b->data = erealloc(b->data, ns * sizeof(Stuple));
	b->size = ns;
	return b;
}

void
destroybuffer(Buffer *b)
{
	free(b->data);
	free(b);
}

void
play(Buffer *b, int fd)
{
	write(fd, b->data, b->size * sizeof(Stuple));
}
