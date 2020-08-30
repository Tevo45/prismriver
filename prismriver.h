#pragma lib "libprismriver.a"

typedef short sample;

static const ulong SAMPLE_MAX	= 32767;
static const ulong SAMPLE_MIN	= -32768;
static const ulong SAMPLE_RATE	= 44100;

typedef struct
{
	sample l, r;
} STuple;

/** PCM API **/

typedef struct
{
	ulong size;
	STuple *data;
} Buffer;

Buffer*	createbuffer(ulong size);
Buffer* resizebuffer(Buffer *buf, ulong newsize);
void	destroybuffer(Buffer *buf);

void play(Buffer *buf, int fd);

/** Wave generators **/

typedef struct Wavegen
{
	STuple	(*fn)(struct Wavegen*, ulong);
	void	(*destroy)(struct Wavegen*);
} Wavegen;

void destroywavegen(Wavegen *gen);

Buffer*	buffermap(Buffer *buf, Wavegen *gen, int d, ulong s, ulong sz);

Wavegen* waveform(double (*fn)(double), double amp, double freq, double φ);

double sine(double);
double saw(double);
double square(double);
double triangle(double);

/** Utilities **/

double pitch(uint note, int octave);

ulong	d2s(double seconds);
double	s2d(ulong samples);
