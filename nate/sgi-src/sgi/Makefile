###############################################################################
#
# Makefile
#
###############################################################################

HEADERS  = 
SOURCES  = area.c maiden.c multiview.c nii.c null.c qix.c rotate.c \
           starfield.c strip.c texture.c voronoi.c
DEPENDS  = $(SOURCES:.c=.d)
OBJECTS  = $(SOURCES:.c=.o)
PROGRAMS = $(SOURCES:.c=)

###############################################################################

ifdef DEBUG
OPTFLAGS = -g
else
OPTFLAGS = -O3
endif

CC      = gcc
CFLAGS  = -Wall $(OPTFLAGS)
LDFLAGS = -lglut -lglu -lgl

UNAME = $(shell uname)
ifeq ($(UNAME), Darwin)  # Mac OS X
# OpenGL and GLUT are frameworks, override LDFLAGS above
LDFLAGS = -framework OpenGL -framework GLUT
# Universal binary support
CFLAGS  += -arch ppc -arch i386
endif
ifeq ($(UNAME), CYGWIN_NT-5.1)  # Cygwin
# OpenGL and GLUT libs are named differently, override LDFLAGS above
LDFLAGS = -lglut32 -lglu32 -lopengl32
EXE = .exe
endif

###############################################################################

all: $(PROGRAMS)

$(PROGRAMS):
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

define PROGRAM_template
$(1): $(addsuffix .o,$(1))
endef
$(foreach t,$(PROGRAMS),$(eval $(call PROGRAM_template,$(t))))

clean:
	$(RM) $(OBJECTS) $(DEPENDS)
	$(RM) $(PROGRAMS:=$(EXE))

.PHONY: all clean

###############################################################################

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.d: %.c
	$(CC) -MM $< -o $@

###############################################################################

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

###############################################################################
