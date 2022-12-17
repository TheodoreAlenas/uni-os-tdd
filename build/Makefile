SRC := ../c
TMP := /tmp/alenas-rlr-makefile
FLAGS :=

PARENT_O := $(foreach f, $(wildcard $(SRC)/parent/*.c), parent-$(notdir $(f:.c=.o)))
CHILD_O := $(foreach f, $(wildcard $(SRC)/child/*.c), child-$(notdir $(f:.c=.o)))
BOTH_O := $(foreach f, $(wildcard $(SRC)/both/*.c), both-$(notdir $(f:.c=.o)))
BEFORE_O := $(foreach f, $(wildcard $(SRC)/before/*.c), before-$(notdir $(f:.c=.o)))
TEST_O := $(foreach f, $(wildcard $(SRC)/test/*.c), test-$(notdir $(f:.c=.o)))

RLR_MAIN_O := $(PARENT_O) $(CHILD_O) $(BOTH_O) $(BEFORE_O)
TEST_MAIN_O := $(RLR_MAIN_O) $(TEST_O)

all: rlr
	:

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find ../output -type f -exec rm {} ';'

rlr: $(RLR_MAIN_O) main.o
	gcc $(RLR_MAIN_O) main.o -o rlr

dev: FLAGS = -DDEV
dev: $(RLR_MAIN_O) main.o
	gcc $(RLR_MAIN_O) main.o -o rlr

test: FLAGS = -DTEST
test: $(TEST_MAIN_O) test_main.o main.o
	gcc $(TEST_MAIN_O) test_main.o -o test
	gcc $(RLR_MAIN_O) main.o -o rlr

%.o:
	gcc $(FLAGS) -c $(SRC)/$(subst -,/,$(@:.o=.c)) -o $@

