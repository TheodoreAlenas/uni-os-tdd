BUILD := build
SRC := c/src
TEST := c/tests

all: listener worker manager

clean:
	rm $(BUILD)/*

test: test-main.o test-stack.o test-util.o stack.o
	gcc $(BUILD)/test-main.o $(BUILD)/test-stack.o $(BUILD)/test-util.o $(BUILD)/stack.o -o $(BUILD)/test


listener: listener.o
	gcc $(BUILD)/listener.o -o $(BUILD)/listener

worker: worker.o
	gcc $(BUILD)/worker.o -o $(BUILD)/worker

manager: manager.o stack.o
	gcc $(BUILD)/manager.o $(BUILD)/stack.o -o $(BUILD)/manager


listener.o: $(SRC)/listener.c
	gcc -c $(SRC)/listener.c -o $(BUILD)/listener.o

worker.o: $(SRC)/worker.c
	gcc -c $(SRC)/worker.c -o $(BUILD)/worker.o

manager.o: $(SRC)/manager.c
	gcc -c $(SRC)/manager.c -o $(BUILD)/manager.o

stack.o: $(SRC)/stack.c
	gcc -c $(SRC)/stack.c -o $(BUILD)/stack.o


test-main.o: $(TEST)/test-main.c
	gcc -c $(TEST)/test-main.c -o $(BUILD)/test-main.o

test-util.o: $(TEST)/test-util.c
	gcc -c $(TEST)/test-util.c -o $(BUILD)/test-util.o

test-stack.o: $(TEST)/test-stack.c
	gcc -c $(TEST)/test-stack.c -o $(BUILD)/test-stack.o



