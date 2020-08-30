#include <u.h>
#include <libc.h>

#include "prismriver.h"
#include "p-internal.h"

enum
{
	C,
	Db, // C#
	D,
	Eb, // D#
	E,
	F,
	Gb, // F#
	G,
	Ab, // G#
	A,
	Bb, // A#
	B
};

const double equaltemperament[] = 
{
	[C]  = 16.35,
	[Db] = 17.32,
	[D]  = 18.35,
	[Eb] = 19.45,
	[E]  = 20.60,
	[F]  = 21.83,
	[Gb] = 23.12,
	[G]  = 24.50,
	[Ab] = 25.96,
	[A]  = 27.50,
	[Bb] = 29.14,
	[B]  = 30.87
};

double 
pitch(uint n, int o)
{
	return equaltemperament[n % 12] * pow(2, o + n/12);
}

ulong /* Duration (seconds) to samples */
d2s(double d)
{
	return d * SAMPLE_RATE;
}

double /* Samples to duration (seconds) */
s2d(ulong s)
{
	return (double)s / SAMPLE_RATE;
}
