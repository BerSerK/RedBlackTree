CC=g++-mp-5
CFLAGS=-g -c
OBJS=StreamStatic.so main.so
EXES=run

all:$(EXES)

%.so:%.cpp
	$(CC) $< -o $@ $(CFLAGS)
run:$(OBJS)
	$(CC)	$(OBJS) -o $@ -lm
clean:
	rm run
	rm *.so
