BUILD := build
SRC := c/src
TEST := c/tests

all: rlr

clean:
	rm $(BUILD)/*

dev:
	export DEBUG_FLAG="-D DEV"; make all

test: test-main.o test-stack.o test-util.o test-arg-parse.o params.o stack.o
	gcc $(BUILD)/test-*.o $(BUILD)/params.o $(BUILD)/stack.o -o $(BUILD)/test


rlr: main.o stack.o child_data.o parent.o params.o
	gcc $(BUILD)/main.o $(BUILD)/stack.o $(BUILD)/child_data.o $(BUILD)/parent.o $(BUILD)/params.o -o $(BUILD)/rlr


test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $(BUILD)/$@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $(BUILD)/$@
