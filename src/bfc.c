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
	int cnt_goto = 0;
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
			int cnt_chars = 0, cnt_brackets = 0;
			char c;
			for (int depth = 1; depth > 0 || c != ']'; cnt_chars++) {
				c = getc(fp);
				if (c == '[') {
					cnt_brackets++;
					depth++;
				} else if (c == ']') {
					cnt_brackets++;
					depth--;
				}
			}
			fseek(fp, -cnt_chars, SEEK_CUR);
			printf("	movq	-10000(%%rbp,%%rbx,8), %%rax\n");
			printf("	cmpq	$0, %%rax\n");
			printf("	movq	$0, %%rax\n");
			printf("	je		GOTO_%d\n", cnt_goto + cnt_brackets);
			printf("GOTO_%d:\n", cnt_goto);
			cnt_goto++;
		} else if (c == ']') {
			int cnt_chars = 0, cnt_brackets = 0;
			char c;
			for (int depth = 1; depth > 0 || c != '['; cnt_chars++) {
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
			fseek(fp, cnt_chars, SEEK_CUR);
			printf("	movq	-10000(%%rbp,%%rbx,8), %%rax\n");
			printf("	cmpq	$0, %%rax\n");
			printf("	movq	$0, %%rax\n");
			printf("	jne		GOTO_%d\n", cnt_goto - cnt_brackets);
			printf("GOTO_%d:\n", cnt_goto);
			cnt_goto++;
		}
	}
	
	printf("EXIT:\n");
	printf("	leaveq\n");
	printf("	retq\n");
	
	fclose(fp);
	
	return 0;
}