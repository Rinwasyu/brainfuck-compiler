#include <stdio.h>
#include <elf.h>

void dump_elf64_ehdr(Elf64_Ehdr *ehdr) {
	printf("ehdr->e_ident:     ");
	for (int i = 0; i < EI_NIDENT; i++) {
		printf("%02x", ehdr->e_ident[i]);
	}
	printf("\n");
	printf("ehdr->e_type:      %02x\n", ehdr->e_type);
	printf("ehdr->e_machine:   %02x\n", ehdr->e_machine);
	printf("ehdr->e_version:   %04x\n", ehdr->e_version);
	printf("ehdr->e_entry:     %08lx\n", ehdr->e_entry);
	printf("ehdr->e_phoff:     %08lx\n", ehdr->e_phoff);
	printf("ehdr->e_shoff:     %08lx\n", ehdr->e_shoff);
	printf("ehdr->e_flags:     %04x\n", ehdr->e_flags);
	printf("ehdr->e_ehsize:    %02x\n", ehdr->e_ehsize);
	printf("ehdr->e_phentsize: %02x\n", ehdr->e_phentsize);
	printf("ehdr->e_phnum:     %02x\n", ehdr->e_phnum);
	printf("ehdr->e_shentsize: %02x\n", ehdr->e_shentsize);
	printf("ehdr->e_shnum:     %02x\n", ehdr->e_shnum);
	printf("ehdr->e_shstrndx:  %02x\n", ehdr->e_shstrndx);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		return -1;
	}
	
	FILE *fp = fopen(argv[1], "r");
	
	Elf64_Ehdr ehdr;
	fread(&ehdr, sizeof(ehdr), 1, fp);
	dump_elf64_ehdr(&ehdr);
	
	fclose(fp);
	
	return 0;
}