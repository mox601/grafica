
#usa il compilatore gcc
CC = gcc

#ottimizza livello 2 ed includi simboli debugging
CFLAGS = -O2 -g 

#dove trovare gli header (es -I<dir>)
INCDIR=

#dove trovare le librerie (es -L<dir)
INCLIB= 

#librerie da linkare 
#LIBS = -lglut -lGLU -lGL
LIBS=-framework OpenGL -framework Glut -lm




#gli obj file da compilare
OBJS=mesh2d.o ply.o simpleviewer.o trackball.o

#regola di compilazione degli obj a partire dai sorgenti *.c
#specifica anche dipendenze implicite, es mesh2d.o dipende da mesh2d.c
#ogni volta che mesh2d.c cambia e' necessario ricompilare mesh2d.o
.c.o:
	$(CC) -w -o $@  $(CFLAGS) $(INCDIR) -c  $<

#regola base per il makefile. Crea il binario simpleviewer
all: simpleviewer

#simpleviewer si ottiene attraverso il linking degli obj file. Dipende dalla compilazione degli obj.
simpleviewer : $(OBJS)
	     $(CC) -o simpleviewer  $(OBJS)  $(INCLIB) $(LIBS)

#un "make clean" cancella obj e binari
clean : 
	rm -Rf simpleviewer *.o   

#dependencies (es mesh2d.o dipende da ply.h cioe' ogni volta che ply.h cambia e' necessario ricompilare mesh2d.o)
mesh2d.o: ply.h
ply.o: ply.h
simpleviewer.o: simpleviewer.h trackball.h
trackball.o: trackball.h
