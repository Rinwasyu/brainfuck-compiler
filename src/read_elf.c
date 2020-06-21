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
	printf("\tehdr->e_ident:     ");
	for (int i = 0; i < EI_NIDENT; i++) {
		printf("%02x", ehdr->e_ident[i]);
	}
	printf("\n");
	printf("\tehdr->e_type:      %04x\n", ehdr->e_type);
	printf("\tehdr->e_machine:   %04x\n", ehdr->e_machine);
	printf("\tehdr->e_version:   %08x\n", ehdr->e_version);
	printf("\tehdr->e_entry:     %016lx\n", ehdr->e_entry);
	printf("\tehdr->e_phoff:     %016lx\n", ehdr->e_phoff);
	printf("\tehdr->e_shoff:     %016lx\n", ehdr->e_shoff);
	printf("\tehdr->e_flags:     %08x\n", ehdr->e_flags);
	printf("\tehdr->e_ehsize:    %04x\n", ehdr->e_ehsize);
	printf("\tehdr->e_phentsize: %04x\n", ehdr->e_phentsize);
	printf("\tehdr->e_phnum:     %04x\n", ehdr->e_phnum);
	printf("\tehdr->e_shentsize: %04x\n", ehdr->e_shentsize);
	printf("\tehdr->e_shnum:     %04x\n", ehdr->e_shnum);
	printf("\tehdr->e_shstrndx:  %04x\n", ehdr->e_shstrndx);
}

void dump_elf64_phdr(Elf64_Phdr *phdr) {
	printf("\n");
	printf("\tphdr->p_type:      %04x %s\n", phdr->p_type,
			phdr->p_type == PT_NULL ? "(PT_NULL)" :
			phdr->p_type == PT_LOAD ? "(PT_LOAD)" :
			phdr->p_type == PT_DYNAMIC ? "(PT_DYNAMIC)" :
			phdr->p_type == PT_INTERP ? "(PT_INTERP)" :
			phdr->p_type == PT_NOTE ? "(PT_NOTE)" :
			phdr->p_type == PT_SHLIB ? "(PT_SHLIB)" :
			phdr->p_type == PT_PHDR ? "(PT_PHDR)" :
			phdr->p_type == PT_LOPROC ? "(PT_LOPROC)" :
			phdr->p_type == PT_HIPROC ? "(PT_HIPROC)" : "(Unknown)"
		);
	printf("\tphdr->p_flags:     %04x\n", phdr->p_flags);
	printf("\tphdr->p_offset:    %016lx\n", phdr->p_offset);
	printf("\tphdr->p_vaddr:     %016lx\n", phdr->p_vaddr);
	printf("\tphdr->p_paddr:     %016lx\n", phdr->p_paddr);
	printf("\tphdr->p_filesz:    %016lx\n", phdr->p_filesz);
	printf("\tphdr->p_memsz:     %016lx\n", phdr->p_memsz);
	printf("\tphdr->p_align:     %016lx\n", phdr->p_align);
}

void dump_elf64_shdr(Elf64_Shdr *shdr) {
	printf("\n");
	printf("\tshdr->sh_name:     %04x\n", shdr->sh_name);
	printf("\tshdr->sh_type:     %04x %s\n", shdr->sh_type, 
			shdr->sh_type == SHT_NULL ? "(SHT_NULL)" :
			shdr->sh_type == SHT_PROGBITS ? "(SHT_PROGBITS)" :
			shdr->sh_type == SHT_SYMTAB ? "(SHT_SYMTAB)" :
			shdr->sh_type == SHT_STRTAB ? "(SHT_STRTAB)" :
			shdr->sh_type == SHT_RELA ? "(SHT_RELA)" :
			shdr->sh_type == SHT_HASH ? "(SHT_HASH)" :
			shdr->sh_type == SHT_DYNAMIC ? "(SHT_DYNAMIC)" :
			shdr->sh_type == SHT_NOTE ? "(SHT_NOTE)" :
			shdr->sh_type == SHT_NOBITS ? "(SHT_NOBITS)" :
			shdr->sh_type == SHT_REL ? "(SHT_REL)" :
			shdr->sh_type == SHT_SHLIB ? "(SHT_SHLIB)" :
			shdr->sh_type == SHT_DYNSYM ? "(SHT_DYNSYM)" :
			shdr->sh_type == SHT_LOPROC ? "(SHT_LOPROC)" :
			shdr->sh_type == SHT_HIPROC ? "(SHT_HIPROC)" :
			shdr->sh_type == SHT_LOUSER ? "(SHT_LOUSER)" :
			shdr->sh_type == SHT_HIUSER ? "(SHT_HIUSER)" : ""
		);
	printf("\tshdr->sh_flags:    %016lx\n", shdr->sh_flags);
	printf("\tshdr->sh_addr:     %016lx\n", shdr->sh_addr);
	printf("\tshdr->sh_offset:   %016lx\n", shdr->sh_offset);
	printf("\tshdr->sh_size:     %016lx\n", shdr->sh_size);
	printf("\tshdr->sh_link:     %08x\n", shdr->sh_link);
	printf("\tshdr->sh_info:     %08x\n", shdr->sh_info);
	printf("\tshdr->sh_addralign:%016lx\n", shdr->sh_addralign);
	printf("\tshdr->sh_entsize:  %016lx\n", shdr->sh_entsize);
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
	printf("\n******************** Elf Header ********************\n");
	dump_elf64_ehdr(&ehdr);
	
	// if program header table exists
	if (ehdr.e_phoff != 0) {
		printf("\n******************** Program Headers ********************\n");
		fseek(fp, ehdr.e_phoff, SEEK_SET);
		for (int i = 0; i < ehdr.e_phnum; i++) {
			fread(&phdr, sizeof(phdr), 1, fp);
			dump_elf64_phdr(&phdr);
		}
	}
	
	// if section header table exists
	if (ehdr.e_shoff != 0) {
		printf("\n******************** Section Headers ********************\n");
		fseek(fp, ehdr.e_shoff, SEEK_SET);
		for (int i = 0; i < ehdr.e_shnum; i++) {
			fread(&shdr, sizeof(shdr), 1, fp);
			dump_elf64_shdr(&shdr);
		}
	}
	
	fclose(fp);
	
	return 0;
}