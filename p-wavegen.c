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
buffermap(Buffer *buf, Wavegen *gen, int d, ulong s, ulong sz)
{
	if(buf == nil)
		buf = createbuffer(s + sz);

	for(ulong c = s; c < sz; c++)
		buf->data[c] = gen->fn(gen, c);

	if(d)
		destroywavegen(gen);

	return buf;
}

/** Simple Waveforms **/

typedef struct
{
	Wavegen;
	double (*wavefn)(double);
	double amp, freq, phase;
} Waveprops;

STuple
waveformfn(Wavegen *w, ulong t)
{
	Waveprops *p = (Waveprops*)w;
	double v	= p->amp * p->wavefn(s2d(t) * p->freq + p->phase);
	sample val	= truncate(v);
	return (STuple)
	{
		val, val
	};
}

Wavegen*
waveform(double (*fn)(double), double amp, double freq, double φ)
{
	Waveprops *p = emallocz(sizeof(Waveprops));
	p->fn		= waveformfn;
	p->destroy 	= (void(*)(Wavegen*))free;
	p->wavefn	= fn;
	p->amp		= amp;
	p->freq		= freq;
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
