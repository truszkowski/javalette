#-- @author Piotr Truszkowski - pt209223 

CC        := gcc
CFLAGS    := -ggdb -Wall -Wextra
LIBS      := -lm -lfl
LEX       := flex
LEXFLAGS  := 
YAC       := bison
YACFLAGS  := 
OBJ       := config.o generate.o analyze.o program.o expr_resolv.o context.o names.o funtbl.o vartbl.o new.o delete.o print.o

#-- bison + flex + obiekty

all: prepare clean tags $(OBJ)
	$(YAC) -d javalette.y
	$(LEX) -t javalette.l > javalette.c
	$(CC)  $(CFLAGS) -c javalette.tab.c
	$(CC)  -ggdb -c javalette.c
	$(CC)  $(CFLAGS) -o javalette javalette.tab.o javalette.o $(OBJ) $(LIBS)
	@echo "-: Gotowe!"

#-- Kompilacja plikow *.c

$(OBJ) : %.o : %.c
	$(CC) $(CFLAGS) -c $<

tags : 
	ctags *.{c,h} 

clean:
	rm -rf javalette.tab.* javalette.output javalette.c javalette *.o tags core core.*

prepare:
	@echo "sprawdzam czy sa w systemie bison, flex i nasm..."
	@which bison || ( echo "brak 'bison'a w systemie" ; exit 1 )
	@which flex  || ( echo "brak 'flex'a w systemie" ; exit 1 )
	@which nasm  || ( echo "brak 'nasm'a w systemie" ; exit 1 )
