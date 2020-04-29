#!/bin/sh

#  
#  Copyright 2020 Rinwasyu
# 
#  This file is part of brainfuck-compiler.
#  
#  Brainfuck-compiler is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#  
#  Brainfuck-compiler is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  

# C Compiler
CC=gcc

output='./output'
cnt_ok=0
cnt_failed=0

echo "----------------------------------"

rm $output/* 2>/dev/null

OK(){
	echo "\e[32;1mOK\e[m"
	cnt_ok=$(($cnt_ok+1))
}

FAILED(){
	echo "\e[2D\e[31;1mFAILED\e[m"
	cnt_failed=$(($cnt_failed+1))
}

echo -n "  C Compiler ($CC)\e[30G"
which $CC >/dev/null
if [ $? -eq 0 ]
then OK
else FAILED; exit
fi

echo -n "  Brainfuck-compiler\e[30G"
$CC -o $output/bfc ../bfc.c
if [ $? -eq 0 ]
then OK
else FAILED; exit
fi

for fname in $(ls *.bf)
do
	echo -n "  $fname\e[30G"
	timeout -sKILL 10 $output/bfc "$fname" > "$output/$(basename $fname .bf).s"
	if [ $? -eq 0 ]
	then
		$CC -o "$output/$(basename $fname .bf).bin" "$output/$(basename $fname .bf).s"
		if [ $? -eq 0 ]
		then
			timeout -sKILL 5 sh -c "cat $(basename $fname .bf).in | $output/$(basename $fname .bf).bin" > $output/$(basename $fname .bf).out 2>/dev/null
			if [ $? -eq 0 ]
			then
				if [ "$(diff $output/$(basename $fname .bf).out $(basename $fname .bf).out)" = "" ]
				then OK
				else FAILED
				fi
			else FAILED
			fi
		else FAILED
		fi
	else FAILED
	fi
done

echo "----------------------------------"
echo "  \e[33;1mTotal\e[m: $(($cnt_ok+$cnt_failed)),  \e[32;1mOK\e[m: $cnt_ok,  \e[31;1mFAILED\e[m: $cnt_failed"
