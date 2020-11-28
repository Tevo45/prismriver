#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <prismriver.h>

void
usage(void)
{
	print("Usage: %s [-o <output>] [-b <bufsize>]\n", argv0);
	exits("usage");
}

//const char map[] = "zsxdcvgbhnjm";
const char map[] = "qazwsxedcrfvtgbyhnujmik,ol.p;/[']";

int indexof(char *str, char c)
{
	for(int i = 0; str[i] != '\0'; i++)
		if(str[i] == c)
			return i;
	return -1;
}

void
main(int argc, char** argv)
{
	int out = -2, cont = 1, bufsize = 4096;
	Wavegen *w;
	ulong clock = 0;
	Rune e;

	ARGBEGIN {
	case 'o':
		out = create(EARGF(usage()), OWRITE, 0644);
		break;
	case 'b':
		bufsize = atoi(EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND;

	if(out == -2)
		out = open("/dev/audio", OWRITE);

	if(out == -1)
		sysfatal("open: %r");

	initdraw(nil, nil, "Prismsynth");
	einit(Ekeyboard);

	w = unison(waveform(sine, 1.0, 0), 7, 0.5, 0.3);

	while(cont)
		switch(e = ekbd())
		{
		case 0x1B:	/* Esc */
			cont = 0;
			break;
		default:
			{
				Buffer *buf;
				int i = indexof(map, (char)e);
				if(i < 0)
					continue;
				buf = buffermap(nil, w, pitch(i, 4), &clock, 0, 0, bufsize);
				play(buf, out);
				destroybuffer(buf);
			}
		}

	destroywavegen(w);
	close(out);
	exits(0);
}

void
eresized(int new)
{
	if(new)
		getwindow(display, Refnone);
}