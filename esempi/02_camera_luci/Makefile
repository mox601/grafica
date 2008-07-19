CFLAGS=
INCDIR=
#linux#LINKLIBS=-lglut -lGLU -lGL -lm

LINKLIBS=-framework OpenGL -framework Glut -lm


OBJECTS= camera clipping lighting subdivision vectors model lights mesh2d ply particles

all: $(OBJECTS)
	@echo "all done"
	
#.c.o:
#	$(CC)  -w -o $@  -c $(CFLAGS) $(INCDIR) $<

camera: camera.o vectors.o model.o lights.o mesh2d.o ply.o simpleviewer.o particles.o
	$(CC) -o $@ $^ $(LINKLIBS)

#clipping: clipping.o
#	$(CC) -o $@ $^ $(LINKLIBS)

#lighting: lighting.o
#	$(CC) -o $@ $^ $(LINKLIBS)
	
simpleviewer: simpleviewer.o
	$(CC) -o $@ $^ $(LINKLIBS)
	
mesh2d: mesh2d.o ply
	$(CC) -o $@ $^ $(LINKLIBS)
	
ply: ply.o
	$(CC) -o $@ $^ $(LINKLIBS)
	
particles: particles.o
	$(CC) -o $@ $^ $(LINKLIBS)
	
subdivision: subdivision.o
	$(CC) -o $@ $^ $(LINKLIBS)


#librerie incluse con i .h

vectors.o: vectors.c
	$(CC) -c -o $@ $<

lights.o: lights.c
	$(CC) -c -o $@ $<

model.o: model.c vectors.o lights.o
	$(CC) -c -o $@ $<


clean:
	rm -f *.o $(OBJECTS) *~
	@echo "clean done"
	
	
