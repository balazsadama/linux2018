CC = gcc
CFLAGS = -Wall

all: kl sz

kl: L11_07_baam0146_kl.o
	$(CC) L11_07_baam0146_kl.o -o kl $(CFLAGS)

sz: L11_07_baam0146_sz.o
	$(CC) L11_07_baam0146_sz.c -o sz

L11_07_baam0146_sz.o: L11_07_baam0146_sz.c header.h
	$(CC) L11_07_baam0146_sz.c -c $(CFLAGS)

L11_07_baam0146_kl.o: L11_07_baam0146_kl.c header.h
	$(CC) L11_07_baam0146_kl.c -c $(CFLAGS)

.PHONY: clean

clean:
	rm *.o kl sz
