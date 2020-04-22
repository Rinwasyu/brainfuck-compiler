#!/bin/sh

output='./output'
cnt_ok=0
cnt_failed=0

echo "----------------------------------"

rm $output/*

OK(){
	echo "\e[32;1mOK\e[m"
	cnt_ok=$(($cnt_ok+1))
}

FAILED(){
	echo "\e[2D\e[31;1mFAILED\e[m"
	cnt_failed=$(($cnt_failed+1))
}

echo -n "  Brainfuck-compiler\e[30G"
gcc -o $output/bfc ../bfc.c
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
		gcc -o "$output/$(basename $fname .bf).bin" "$output/$(basename $fname .bf).s"
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
