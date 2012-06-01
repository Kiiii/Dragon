IDIR =.
CC=gcc
CFLAGS=-I$(IDIR) -Wall -g

SRCDIR=.
ODIR=obj
LIBS=-lallegro -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_image -lallegro_font -lallegro_main -lallegro_ttf -lallegro_primitives

_OBJ = main.o menu.o play.o lists.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUTDIR = bin

$(ODIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUTDIR)/DRAGON: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(OUTPUTDIR)/DRAGON

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(OUTPUTDIR)/*
