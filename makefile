CFLAGS =  -c -Wall

all:        work1.elf

work1.elf:  main.o
	gcc $< -o $@

%.o:        %.c
	gcc $(CFLAGS) $< -o $@

debug:		work1.dbg.elf

work1.dbg.elf:	main.dbg.o
	gcc -g $< -o $@

%.dbg.o:	%.c
	gcc $(CFLAGS) -g $< -o $@

clear:
	rm -rf *.o *.elf
