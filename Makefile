BUILD=build
SRC=c/src
TEST=c/tests/main.c

all:
	make compile

clean:
	rm ${BUILD}/*

compile: listener worker manager

test: ${TEST}
	gcc ${TEST} -o ${BUILD}/test

listener: ${SRC}/listener.c
	gcc ${SRC}/listener.c -o ${BUILD}/listener

worker: ${SRC}/worker.c
	gcc ${SRC}/worker.c -o ${BUILD}/worker

manager: ${SRC}/manager.c
	gcc ${SRC}/manager.c -o ${BUILD}/manager

