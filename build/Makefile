SRC := ../c/src
TEST := ../c/tests
PRIMARY_FILES := parent.o child.o
TESTABLE_FILES := be_yourself.o
SECONDARY_FILES := stack.o child_data.o params.o parent_params.o child_res.o get_names.o
TRICKY_FILES := fork.o shmem.o
TEST_NAMED_FILES := test-stack.o test-util.o test-arg-parse.o test-parent.o
NORMAL_FILES := $(PRIMARY_FILES) $(SECONDARY_FILES) $(TESTABLE_FILES) $(TRICKY_FILES)
RLR_FILES := main.o $(NORMAL_FILES)
TEST_FILES := test-main.o $(TEST_NAMED_FILES) $(TESTABLE_FILES) $(NORMAL_FILES)

all: rlr

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find ../output -type f -exec rm {} ';'

dev:
	export DEBUG_FLAG="-D DEV"; make all

test:
	export DEBUG_FLAG="-D TEST"; make test-phase-2

test-phase-2: $(TEST_FILES)
	gcc $(TEST_FILES) -o test

rlr: $(RLR_FILES)
	gcc $(RLR_FILES) -o rlr

test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $@

