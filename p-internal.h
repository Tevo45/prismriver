
static const double π = 3.1415926;

static void*
emallocz(ulong s)
{
	void *p = mallocz(s, 1);
	if(p == nil)
		sysfatal("mallocz: %r");
	return p;
}

static void*
erealloc(void *p, ulong s)
{
	p = realloc(p, s);
	if(p == nil)
		sysfatal("realloc: %r");
	return p;
}

static sample
truncate(double val)
{
	if(val > 1.0)
		val = 1.0;
	else if(val < -1.0)
		val = -1.0;
	/* FIXME this is off by 1 when val == -1 */
	return (sample)(val * SAMPLE_MAX);
}
