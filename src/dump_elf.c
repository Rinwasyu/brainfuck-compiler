/*
 * Copyright 2020 Rinwasyu
 * 
 * This file is part of brainfuck-compiler.
 * 
 * Brainfuck-compiler is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Brainfuck-compiler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include <stdio.h>
#include <elf.h>

void dump_elf64_ehdr(Elf64_Ehdr *ehdr) {
	printf("ehdr->e_ident:     ");
	for (int i = 0; i < EI_NIDENT; i++) {
		printf("%02x", ehdr->e_ident[i]);
	}
	printf("\n");
	printf("ehdr->e_type:      %04x\n", ehdr->e_type);
	printf("ehdr->e_machine:   %04x\n", ehdr->e_machine);
	printf("ehdr->e_version:   %08x\n", ehdr->e_version);
	printf("ehdr->e_entry:     %016lx\n", ehdr->e_entry);
	printf("ehdr->e_phoff:     %016lx\n", ehdr->e_phoff);
	printf("ehdr->e_shoff:     %016lx\n", ehdr->e_shoff);
	printf("ehdr->e_flags:     %08x\n", ehdr->e_flags);
	printf("ehdr->e_ehsize:    %04x\n", ehdr->e_ehsize);
	printf("ehdr->e_phentsize: %04x\n", ehdr->e_phentsize);
	printf("ehdr->e_phnum:     %04x\n", ehdr->e_phnum);
	printf("ehdr->e_shentsize: %04x\n", ehdr->e_shentsize);
	printf("ehdr->e_shnum:     %04x\n", ehdr->e_shnum);
	printf("ehdr->e_shstrndx:  %04x\n", ehdr->e_shstrndx);
}

void dump_elf64_phdr(Elf64_Phdr *phdr) {
	printf("\n");
	printf("phdr->p_type:      %04x\n", phdr->p_type);
	printf("phdr->p_flags:     %04x\n", phdr->p_flags);
	printf("phdr->p_offset:    %016lx\n", phdr->p_offset);
	printf("phdr->p_vaddr:     %016lx\n", phdr->p_vaddr);
	printf("phdr->p_paddr:     %016lx\n", phdr->p_paddr);
	printf("phdr->p_filesz:    %016lx\n", phdr->p_filesz);
	printf("phdr->p_memsz:     %016lx\n", phdr->p_memsz);
	printf("phdr->p_align:     %016lx\n", phdr->p_align);
}

void dump_elf64_shdr(Elf64_Shdr *shdr) {
	printf("\n");
	printf("shdr->sh_name:     %04x\n", shdr->sh_name);
	printf("shdr->sh_type:     %04x\n", shdr->sh_type);
	printf("shdr->sh_flags:    %016lx\n", shdr->sh_flags);
	printf("shdr->sh_addr:     %016lx\n", shdr->sh_addr);
	printf("shdr->sh_offset:   %016lx\n", shdr->sh_offset);
	printf("shdr->sh_size:     %016lx\n", shdr->sh_size);
	printf("shdr->sh_link:     %08x\n", shdr->sh_link);
	printf("shdr->sh_info:     %08x\n", shdr->sh_info);
	printf("shdr->sh_addralign:%016lx\n", shdr->sh_addralign);
	printf("shdr->sh_entsize:  %016lx\n", shdr->sh_entsize);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		return -1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	//FILE *fp = fopen("dump_elf", "r");
	
	Elf64_Ehdr ehdr;
	Elf64_Phdr phdr;
	Elf64_Shdr shdr;
	
	fread(&ehdr, sizeof(ehdr), 1, fp);
	dump_elf64_ehdr(&ehdr);
	
	// if program header table exists
	if (ehdr.e_phoff != 0) {
		fread(&phdr, sizeof(phdr), 1, fp);
		dump_elf64_phdr(&phdr);
	}
	
	// if section header table exists
	if (ehdr.e_shoff != 0) {
		fread(&shdr, sizeof(shdr), 1, fp);
		dump_elf64_shdr(&shdr);
	}
	
	fclose(fp);
	
	return 0;
}