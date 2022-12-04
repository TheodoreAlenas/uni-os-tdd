BUILD := build
SRC := c/src
TEST := c/tests

all: listener worker manager

clean:
	rm $(BUILD)/*

dev:
	export DEBUG_FLAG="-D DEV"; make all

test: test-main.o test-stack.o test-util.o stack.o
	gcc $(BUILD)/test-main.o $(BUILD)/test-stack.o $(BUILD)/test-util.o $(BUILD)/stack.o -o $(BUILD)/test


listener: listener.o
	gcc $(BUILD)/listener.o -o $(BUILD)/listener

worker: worker.o
	gcc $(BUILD)/worker.o -o $(BUILD)/worker

manager: manager.o stack.o child_data.o parent.o util_for_main.o
	gcc $(BUILD)/manager.o $(BUILD)/stack.o $(BUILD)/child_data.o $(BUILD)/parent.o $(BUILD)/util_for_main.o -o $(BUILD)/rlr


test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $(BUILD)/$@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $(BUILD)/$@
