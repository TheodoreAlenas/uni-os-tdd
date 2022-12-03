BUILD := build
SRC := c/src
TEST := c/tests

all: listener worker manager

clean:
	rm $(BUILD)/*

test: $(TEST)/main.c $(TEST)/stack.c
	gcc -c $(TEST)/stack.c -o $(BUILD)/stack.o
	gcc $(TEST)/main.c  -o $(BUILD)/test

listener: $(SRC)/listener.c
	gcc $(SRC)/listener.c -o $(BUILD)/listener

worker: $(SRC)/worker.c
	gcc $(SRC)/worker.c -o $(BUILD)/worker

manager: $(SRC)/manager.c
	gcc $(SRC)/manager.c -o $(BUILD)/manager

