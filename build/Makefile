SRC := ../c/src
TEST := ../c/tests
PRIMARY_FILES := parent.o child.o
# for README, testable-files
TESTABLE_FILES := be_yourself.o parent_loop.o
# end of snippet
SECONDARY_FILES := params.o parent_params.o get_names.o
TERTIARY_FILES := stack.o child_res.o file_segment.o msg_cycler.o req.o
TRICKY_FILES := fork.o shmem.o
TEST_NAMED_FILES := test-util.o test-stack.o test-get-names.o test-fork.o test-msg-cycler.o test-req.o test-parent.o

NORMAL_FILES := $(PRIMARY_FILES) $(SECONDARY_FILES) $(TERTIARY_FILES) $(TRICKY_FILES) $(TESTABLE_FILES)

RLR_FILES := $(NORMAL_FILES)
DEV_FILES := $(addsuffix .dev.o,$(NORMAL_FILES))
TEST_FILES := $(addsuffix .test.o,$(TEST_NAMED_FILES) $(NORMAL_FILES))

all: rlr

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find ../output -type f -exec rm {} ';'

rlr: $(RLR_FILES) main.o
	gcc $(RLR_FILES) main.o -o rlr

dev: $(DEV_FILES) main.o.dev.o
	gcc $(DEV_FILES) main.o.dev.o -o rlr

test: $(TEST_FILES) test-main.o.test.o main.o.test.o
	gcc $(TEST_FILES) test-main.o.test.o -o test
	gcc $(TEST_FILES) main.o.test.o -o rlr

%.o: $(SRC)/%.c
	gcc -c $(SRC)/$(@:.o=.c) -o $@

%.o.dev.o: $(SRC)/%.c
	gcc -c -D DEV $(SRC)/$(@:.o.dev.o=.c) -o $@

%.o.test.o: $(SRC)/%.c
	gcc -c -D TEST $(SRC)/$(@:.o.test.o=.c) -o $@

test-%.o.test.o: $(TEST)/test-%.c
	gcc -c -D TEST $(TEST)/$(@:.o.test.o=.c) -o $@

