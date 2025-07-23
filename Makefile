CFLAGS=-Wall -Wextra -std=c11 -pedantic
LIBS=

david-vm: david-vm.c
	$(CC) $(CFLAGS) -o david-vm david-vm.c $(LIBS)