#include <stdio.h>

int main(int argc, char **argv){
	if (argc < 2) return -1;
	
	FILE *fp = fopen(argv[1], "r");
	
	if (fp == NULL) {
		return -1;
	}
	
	printf(".globl main\n");
	printf(".text\n");
	printf("main:\n");
	printf("	pushq	%%rbp\n");
	printf("	movq	%%rsp, %%rbp\n");
	printf("	subq	$10000, %%rsp\n");
	
	printf("	movq	$0, %%rax\n");
	printf("	movq	$0, %%rbx\n");
	
	char c;
	int i = 0;
	while ((c = fgetc(fp)) != -1) {
		if (c == '+') {
			printf("	addq	$1, -10000(%%rbp,%%rbx,8)\n");
		} else if (c == '-') {
			printf("	subq	$1, -10000(%%rbp,%%rbx,8)\n");
		} else if (c == '>') {
			printf("	incq	%%rbx\n");
		} else if (c == '<') {
			printf("	decq	%%rbx\n");
		} else if (c == '.') {
			printf("	movsbl	-10000(%%rbp,%%rbx,8), %%eax\n");
			printf("	movl	%%eax, %%edi\n");
			printf("	call	putchar@PLT\n");
			printf("	movl	$0, %%eax\n");
		} else if (c == ',') {
			printf("	call	getchar@PLT\n");
			printf("	movb	%%al, -10000(%%rbp,%%rbx,8)\n");
			printf("	cmpb	$-1, %%al\n");
			printf("	movq	$0, %%rax\n");
			printf("	je	EXIT\n");
		} else if (c == '[') {
			int cnt = 0, cnt_brackets = 0;
			char c;
			for (int depth = 1; depth > 0 || c != ']'; cnt++) {
				c = getc(fp);
				if (c == '[') {
					cnt_brackets++;
					depth++;
				} else if (c == ']') {
					cnt_brackets++;
					depth--;
				}
			}
			fseek(fp, -cnt, SEEK_CUR);
			printf("	movq	-10000(%%rbp,%%rbx,8), %%rax\n");
			printf("	cmpq	$0, %%rax\n");
			printf("	movq	$0, %%rax\n");
			printf("	je		GOTO_%d\n", i + cnt_brackets);
			printf("GOTO_%d:\n", i);
			i++;
		} else if (c == ']') {
			int cnt = 0, cnt_brackets = 0;
			char c;
			for (int depth = 1; depth > 0 || c != '['; cnt++) {
				fseek(fp, -2, SEEK_CUR);
				c = getc(fp);
				if (c == '[') {
					cnt_brackets++;
					depth--;
				} else if (c == ']') {
					cnt_brackets++;
					depth++;
				}
			}
			fseek(fp, cnt, SEEK_CUR);
			printf("	movq	-10000(%%rbp,%%rbx,8), %%rax\n");
			printf("	cmpq	$0, %%rax\n");
			printf("	movq	$0, %%rax\n");
			printf("	jne		GOTO_%d\n", i - cnt_brackets);
			printf("GOTO_%d:\n", i);
			i++;
		}
	}
	
	printf("EXIT:\n");
	printf("	leaveq\n");
	printf("	retq\n");
	
	return 0;
}