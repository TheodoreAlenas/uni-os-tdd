BUILD=build
SRC=c/src

all:
	make compile

clean:
	rm ${BUILD}/*

compile: listener worker manager

comp-test: ${SRC}/test.c
	gcc ${SRC}/test.c -o ${BUILD}/test

listener: ${SRC}/listener.c
	gcc ${SRC}/listener.c -o ${BUILD}/listener

worker: ${SRC}/worker.c
	gcc ${SRC}/worker.c -o ${BUILD}/worker

manager: ${SRC}/manager.c
	gcc ${SRC}/manager.c -o ${BUILD}/manager

