SRC := ../c/src
TEST := ../c/tests
PRIMARY_FILES := parent.o child.o
# for README, testable-files
TESTABLE_FILES := be_yourself.o file_segment.o
# end of snippet
SECONDARY_FILES := stack.o child_data.o params.o parent_params.o child_res.o get_names.o
TRICKY_FILES := fork.o shmem.o
TEST_NAMED_FILES := test-stack.o test-util.o test-arg-parse.o test-get-names.o test-fork.o test-parent.o

NORMAL_FILES := $(PRIMARY_FILES) $(SECONDARY_FILES) $(TRICKY_FILES)
RLR_FILES := main.o $(NORMAL_FILES) $(TESTABLE_FILES)
TEST_FILES := test-main.o $(TEST_NAMED_FILES) $(NORMAL_FILES)
TEST_DEP := main.o test-main.o $(TEST_NAMED_FILES) $(NORMAL_FILES) $(TESTABLE_FILES)

all: rlr

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find ../output -type f -exec rm {} ';'

rlr: $(RLR_FILES)
	gcc $(RLR_FILES) -o rlr

dev: $(RLR_FILES)
	gcc -D DEV $(RLR_FILES) -o rlr

test: $(TEST_DEP)
	gcc -D TEST $(TEST_FILES) -o test
	gcc -D TEST $(RLR_FILES) -o rlr

test-%.o: $(TEST)/test-%.c
	gcc -c $(TEST)/$(basename $(notdir $@)).c -o $@

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(basename $(notdir $@)).c -o $@

