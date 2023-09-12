# Performs a simple diff between the bash inline output and the mshell inline output
RED="\033[31m"
GREEN="\033[32m"
NC="\033[0m"
# make
# exit_code=$?
# if [ $exit_code -eq 0 ]
# then
# 	result="OK"
# 	color=$GREEN
# else
# 	result="KO"
# 	color=$RED
# fi
# printf "The program compiles:	${color}[%s]\n${NC}" $result
if ! [[ -v 1 ]];
then 
	echo "Use with ./mtests.sh [Cmd to be tested]"
	exit 1;
fi
echo > ./tmp_b
echo > ./tmp_m
echo > ./err_b
echo > ./err_m
bash -c "$1" > ./tmp_b 2> ./err_b
exit_code_b=$?
./mshell -c "$1" > ./tmp_m 2> ./err_m
exit_code_m=$?
diff ./tmp_b ./tmp_m
exit_code=$?
if [ $exit_code -eq 0 ]
then
	result="OK"
	color=$GREEN
else
	result="KO"
	color=$RED
fi
printf "Standard output matches bash:	${color}[%s]\n${NC}" $result
diff ./err_b ./err_m
exit_code=$?
if [ $exit_code -eq 0 ]
then
	result="OK"
	color=$GREEN
else
	result="KO"
	color=$RED
fi
printf "Error output matches bash:	${color}[%s]\n${NC}" $result
if [ $exit_code_b -eq $exit_code_m ]
then
	result="OK"
	color=$GREEN
else
	result="KO"
	color=$RED
fi
printf "Exit code matches bash:		${color}[%s]\n${NC}" $result
rm ./tmp_b ./tmp_m ./err_b ./err_m