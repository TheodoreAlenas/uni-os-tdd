SRC := ../c/src
TEST := ../c/tests
TEST_FILES := test-main.o test-stack.o test-util.o test-arg-parse.o
TEST_RELATED := params.o parent_params.o stack.o
PRIMARY_FILES := main.o parent.o child.o
SECONDARY_FILES := stack.o child_data.o params.o parent_params.o
TRICKY_FILES := fork.o shmem.o
RLR_FILES := $(PRIMARY_FILES) $(SECONDARY_FILES) $(TRICKY_FILES)

all: rlr

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'

dev:
	export DEBUG_FLAG="-D DEV"; make all

test:
	export DEBUG_FLAG="-D TEST"; make test-phase-2

test-phase-2: $(TEST_FILES) $(TEST_RELATED)
	gcc $(TEST_FILES) $(TEST_RELATED) -o test

rlr: $(RLR_FILES)
	gcc $(RLR_FILES) -o rlr

test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $@

