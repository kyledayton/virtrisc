ALL:
	riscv64-unknown-elf-as -march=rv32i -mabi=ilp32 rom.S
	riscv64-unknown-elf-ld -m elf32lriscv -o rom.elf -T rom.ld a.out
	riscv64-unknown-elf-objcopy -O binary rom.elf rom.bin
	rm a.out rom.elf