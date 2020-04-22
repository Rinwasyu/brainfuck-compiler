# brainfuck-compiler
A brainfuck compiler that outputs an assembly code.

## Usage
build
```console
$ cd src
$ gcc -o bfc bfc.c
```
compile (brainfuck -> assembly)
```console
$ ./bfc hello.bf > hello.s
```
assemble & link (assembly -> binary)
```console
$ gcc hello.s -o hello.bin
```
execute
```console
$ ./hello.bin
Hello World!
$
```

## License
This project is licensed under the GPLv3.