
MF=	Makefile

CC=	g++

CFLAGS=-I$(HOME)/local_software/include
LFLAGS=-L$(HOME)/local_software/lib -lquickfix
PREPROC=

EXE=	prog
SRC=	Application.cpp \
		Market.cpp \
		ordermatch.cpp

OBJ=	$(SRC:.cpp=.o)

INC=	IDGenerator.h \
		Order.h \
		OrderMatcher.h \


.SUFFIXES:
.SUFFIXES:	.cpp .o

all:	$(EXE)

.cpp.o:
	$(CC) $(CFLAGS) $(PREPROC) -c $<

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS) 

$(OBJ):	$(SRC) $(INC) $(MF)

clean:
	rm -f *.o $(EXE) core

relink:
	rm -f $(EXE)
	$(MAKE)

run:
	./prog

