CC := g++
CFLAGS := -lm -g -std=c++11
FONTES := $(wildcard *.cpp)
OBJS := $(FONTES:.c=.o)
EXEC := gerador
VALGRIND := valgrind #--leak-check=full #--show-leak-kinds=all

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	@rm -rf *.o $(EXEC)

run:$(EXEC)
	./$(EXEC)

runv: $(EXEC)
	$(VALGRIND) ./$(EXEC) 