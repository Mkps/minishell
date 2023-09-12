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
bash -c "$1" > ./tmp_b
./mshell -c "$1" > ./tmp_m
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
printf "Output matches bash:	${color}[%s]\n${NC}" $result
rm ./tmp_b ./tmp_m
