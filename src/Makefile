CFLAGS = -Wall -O2
LDFLAGS = -lm -lpowercap
POWERCAP_SRC_DIR = powercap/src
POWERCAP_INC_DIR = powercap/inc

all: main bubbleSort selectionSort insertionSort radixSort run

main: main.c rapl.o powercap.o
	gcc $(CFLAGS) -o main main.c rapl.o powercap.o $(LDFLAGS)
	chmod 777 main

bubbleSort: C/bubbleSort.c
	gcc $(CFLAGS) -std=c99 C/bubbleSort.c -o exe/bubbleSort

selectionSort: C/selectionSort.c
	gcc $(CFLAGS) C/selectionSort.c -o exe/selectionSort

insertionSort: C/insertionSort.c
	gcc $(CFLAGS) C/insertionSort.c -o exe/insertionSort

radixSort: C/radixSort.c
	gcc $(CFLAGS) C/radixSort.c -o exe/radixSort

rapl.o: rapl.c rapl.h
	gcc $(CFLAGS) -c rapl.c -lm

powercap.o: $(POWERCAP_SRC_DIR)/powercap.c $(POWERCAP_INC_DIR)/powercap.h
	gcc $(CFLAGS) -c $(POWERCAP_SRC_DIR)/powercap.c -I$(POWERCAP_INC_DIR) $(LDFLAGS)

run:
	sudo bash bash.sh

clean:
	rm -rf main rapl.o powercap.o resultados/*.csv
	rm -rf exe/bubbleSort exe/selectionSort exe/insertionSort exe/radixSort
