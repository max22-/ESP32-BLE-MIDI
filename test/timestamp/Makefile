all: timestamp

timestamp: timestamp.c
	gcc timestamp.c -o timestamp

.PHONY: run clean

run: timestamp
	./timestamp

clean:
	rm -f timestamp