SRC := ../c/src
TEST := ../c/tests
PRIMARY_FILES := parent.o child.o
# for README, testable-files
TESTABLE_FILES := be_yourself.o
# end of snippet
SECONDARY_FILES := params.o parent_params.o get_names.o
TERTIARY_FILES := stack.o child_res.o file_segment.o msg_cycler.o req.o
TRICKY_FILES := fork.o shmem.o
TEST_NAMED_FILES := test-util.o test-stack.o test-get-names.o test-fork.o test-msg-cycler.o test-req.o

NORMAL_FILES := $(PRIMARY_FILES) $(SECONDARY_FILES) $(TERTIARY_FILES) $(TRICKY_FILES)
RLR_FILES := main.o $(NORMAL_FILES) $(TESTABLE_FILES)
TEST_FILES := test-main.o $(TEST_NAMED_FILES) $(NORMAL_FILES)
TEST_DEP := main.o test-main.o $(TEST_NAMED_FILES) $(NORMAL_FILES) $(TESTABLE_FILES)
DEV_FILES := $(addsuffix .dev.o,$(RLR_FILES))

all: rlr

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find ../output -type f -exec rm {} ';'

rlr: $(RLR_FILES)
	gcc $(RLR_FILES) -o rlr

dev: $(DEV_FILES)
	gcc $(DEV_FILES) -o rlr

test: $(TEST_DEP)
	gcc $(TEST_FILES) -o test
	gcc $(RLR_FILES) -o rlr

%.o: $(SRC)/%.c
	gcc -c $$DEBUG_FLAG $(SRC)/$(@:.o=.c) -o $@

%.o.dev.o: $(SRC)/%.c
	gcc -c -D DEV $(SRC)/$(@:.o.dev.o=.c) -o $@

test-%.o: $(TEST)/test-%.c
	gcc -c -D TEST $(TEST)/$(@:.o=.c) -o $@

