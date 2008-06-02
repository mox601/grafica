CFLAGS=
INCDIR=
LINKLIBS=-lglut -lGLU -lGL -lm

OBJECTS=camera clipping lighting subdivision vectors model


all:$(OBJECTS)
	@echo "all done"
	
#.c.o:
#	$(CC)  -w -o $@  -c $(CFLAGS) $(INCDIR) $<

camera: camera.o vectors.o model.o
	$(CC) -o $@ $^ $(LINKLIBS)

clipping: clipping.o
	$(CC) -o $@ $^ $(LINKLIBS)

lighting: lighting.o
	$(CC) -o $@ $^ $(LINKLIBS)

subdivision: subdivision.o
	$(CC) -o $@ $^ $(LINKLIBS)


#librerie incluse con i .h

vectors.o: vectors.c
	$(CC) -c -o $@ $<

model.o: model.c vectors.o
	$(CC) -c -o $@ $<


clean:
	rm -f *.o $(OBJECTS) *~
	@echo "clean done"


