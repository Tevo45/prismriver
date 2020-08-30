</$objtype/mkfile

LIB=/$objtype/lib/libprismriver.a

OFILES=\
	p-pcm.$O	\
	p-utils.$O	\
	p-wavegen.$O

HFILES=\
	/sys/include/prismriver.h

/sys/include/%.h: %.h
	cp $stem.h /sys/include/$stem.h

</sys/src/cmd/mksyslib
