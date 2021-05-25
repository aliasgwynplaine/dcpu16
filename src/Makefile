CC=gcc
CFLAGS=-Wall
EMU_OUT=emulator
ASM_OUT=assembler

all: assembler emulator	

debug_assembler:
	$(CC) $(CFLAGS) d16asm.c -D_DEBUG -o $(ASM_OUT)

.PHONY: assembler
assembler:
	$(CC) $(CFLAGS) d16asm.c -o $(ASM_OUT)

debug_emulator:
	$(CC) $(CFLAGS) cpu.c -D_DEBUG -o $(EMU_OUT)

.PHONY: emulator
emulator:
	$(CC) $(CFLAGS) cpu.c -o $(EMU_OUT)

.PHONY: clean
clean:
	rm $(EMU_OUT)
