BUILD := build
SRC := c/src
TEST := c/tests

all: rlr

clean:
	rm $(BUILD)/*

dev:
	export DEBUG_FLAG="-D DEV"; make all

test:
	export DEBUG_FLAG="-D TEST"; make test-phase-2

test-phase-2: test-named test-related
	gcc $(BUILD)/test-*.o $(BUILD)/params.o $(BUILD)/parent_params.o $(BUILD)/stack.o -o $(BUILD)/test

test-named: test-main.o test-stack.o test-util.o test-arg-parse.o

test-related: params.o parent_params.o stack.o


rlr: main.o stack.o child_data.o parent.o child.o params.o parent_params.o fork.o shmem.o
	sh/link-files


test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $(BUILD)/$@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $(BUILD)/$@
