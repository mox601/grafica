OBJS=ambiente.o collision.o esterno.o input.o interno.o light.o main.o oggetti.o player.o sistemilineari.o texture.o utils.o
CFLAGS=-g  
	
tesina: $(OBJS) 
	$(CC) $(CFLAGS) -lGLU -lGL -lglut $^ -o $@ -lm 

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -f *.o *~

delete: clean
	rm -f tesina

