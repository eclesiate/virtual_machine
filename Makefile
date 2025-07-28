CFLAGS=-Wall -Wextra -std=c11 -pedantic -fsanitize=address -Wswitch-enum
LIBS=

david-vm: david-vm.c
	$(CC) $(CFLAGS) -o david-vm david-vm.c $(LIBS)