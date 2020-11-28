#pragma lib "libprismriver.a"

typedef double sample;

static const ulong SAMPLE_MAX	= 1;
static const ulong SAMPLE_MIN	= -1;
static const ulong SAMPLE_RATE	= 44100;

typedef struct
{
	sample l, r;
} Stuple;

/** PCM API **/

typedef struct
{
	ulong size;
	Stuple *data;
} Buffer;

Buffer*	createbuffer(ulong size);
Buffer* resizebuffer(Buffer *buf, ulong newsize);
void	destroybuffer(Buffer *buf);

void play(Buffer *buf, int fd);

/** Wave generators **/

typedef struct Wavegen
{
	Stuple	(*fn)(struct Wavegen*, double, ulong);
	void	(*destroy)(struct Wavegen*);
} Wavegen;

void destroywavegen(Wavegen *gen);

Buffer*	buffermap(Buffer *buf, Wavegen *gen, double freq, ulong *clock, int d, ulong s, ulong sz);

Wavegen* waveform(double (*fn)(double), double amp, double phase);
Wavegen* pcm(Buffer *buf);

Wavegen* unison(Wavegen *gen, uint voices, double pitchvar, double phasevar);

double sine(double);
double saw(double);
double square(double);
double triangle(double);

/** Utilities **/

double pitch(uint note, int octave);

ulong	d2s(double seconds);
double	s2d(ulong samples);
