#include <u.h>
#include <libc.h>

#include "prismriver.h"
#include "p-internal.h"

void
destroywavegen(Wavegen *gen)
{
	gen->destroy(gen);
}

Buffer*
buffermap(Buffer *buf, Wavegen *gen, double freq, ulong *pc, int d, ulong s, ulong sz)
{
	if(buf == nil)
		buf = createbuffer(s + sz);

	for(ulong c = s; c < sz; c++)
		buf->data[c] = gen->fn(gen, freq, c + (pc == nil ? 0 : *pc));

	if(d)
		destroywavegen(gen);

	if(pc != nil)
		*pc += sz;

	return buf;
}

/** Simple Waveforms **/

typedef struct
{
	Wavegen;
	double (*wavefn)(double);
	double amp, phase;
} Waveprops;

Stuple
waveformfn(Wavegen *w, double freq, ulong t)
{
	Waveprops *p = (Waveprops*)w;
	double v	= p->amp * p->wavefn(s2d(t) * freq + p->phase);
	sample val	= truncate(v);
	return (Stuple)
	{
		val, val
	};
}

Wavegen*
waveform(double (*fn)(double), double amp, double φ)
{
	Waveprops *p = emallocz(sizeof(Waveprops));
	p->fn		= waveformfn;
	p->destroy 	= (void(*)(Wavegen*))free;
	p->wavefn	= fn;
	p->amp		= amp;
	p->phase	= φ;
	return p;
}

double
sine(double num)
{
	return sin(2 * π * num);
}

double
saw(double num)
{
	return 2 * (num - (int)num) - 1;
}

double
pulse(double num, double threshold)
{
	return saw(num) < threshold ? -1.0 : 1.0;
}

double
square(double num)
{
	return pulse(num, 0);
}

double
triangle(double num)
{
	return 2 * fabs(saw(num)) - 1;
}

/** PCM-backed wave "generators" **/

typedef struct
{
	Wavegen;
	Buffer *buf;
} Pcmprops;

Stuple
pcmfn(Wavegen *w, double freq, ulong t)
{
	(void)freq;
	Pcmprops *p = (Pcmprops*)w;
	if(t < p->buf->size)
		return p->buf->data[t];
	return (Stuple) { 0, 0 };
}

void
pcmdestroy(Wavegen *w)
{
	Pcmprops *p = (Pcmprops*)w;
	destroybuffer(p->buf);
	free(p);
}

Wavegen*
pcm(Buffer *buf)
{
	Pcmprops *p = emallocz(sizeof(*p));
	p->fn		= pcmfn;
	p->destroy	= pcmdestroy;
	p->buf		= buf;
	return p;
}
