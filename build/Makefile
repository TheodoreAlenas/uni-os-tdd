SRC := ../c
TMP := /tmp/alenas-rlr-makefile

PARENT_O := $(foreach f, $(wildcard $(SRC)/parent/*.c), parent-$(notdir $(f:.c=.o)))
CHILD_O := $(foreach f, $(wildcard $(SRC)/child/*.c), child-$(notdir $(f:.c=.o)))
BOTH_O := $(foreach f, $(wildcard $(SRC)/both/*.c), both-$(notdir $(f:.c=.o)))
BEFORE_O := $(foreach f, $(wildcard $(SRC)/before/*.c), before-$(notdir $(f:.c=.o)))
TEST_O := $(foreach f, $(wildcard $(SRC)/tests/*.c), tests-$(notdir $(f:.c=.o)))

RLR_MAIN_O := $(PARENT_O) $(CHILD_O) $(BOTH_O) $(BEFORE_O)

all: rlr
	:

clean:
	find . -type f '!' -name 'Makefile' -exec rm {} ';'
	find output -type f -exec rm {} ';'

rlr: $(RLR_MAIN_O) main.o
	gcc -lrt -lpthread $(RLR_MAIN_O) main.o -o rlr -lrt -lpthread

# for README, make-dev
dev: FLAGS = -DDEV
dev: $(RLR_MAIN_O) main.o
	gcc $(FLAGS) $(RLR_MAIN_O) main.o -o rlr -lrt -lpthread
# end of snippet

test: FLAGS = -DTEST
test: $(TEST_O) $(RLR_MAIN_O) test.o
	gcc $(FLAGS) $(TEST_O) $(RLR_MAIN_O) test.o -o test -lrt -lpthread

%.o:
	gcc $(FLAGS) -c $(SRC)/$(subst -,/,$(@:.o=.c)) -o $@ -lrt -lpthread

