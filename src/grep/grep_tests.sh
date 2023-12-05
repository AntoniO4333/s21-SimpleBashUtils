#!/bin/bash

#Резальтаты сохраняются в 2 видах:
#1)Все кейсы в файлах global_s21.log и global_grep.log
#2)Кейсы только с ошибками в файлах bad_samples_s21.log и bad_samples_grep.log
 
SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
NUMBER=0

flags=(
"v"
"c"
"l"
"n"
"h"
"o"
)

tests=(
"s test_files/test_0_grep.txt FLAGS"
"for utils.c utils.h Makefile FLAGS"
"for utils.c FLAGS"
"-e for -e ^int utils.c utils.h Makefile FLAGS"
"-e for -e ^int utils.c FLAGS"
"-e regex -e ^print utils.c FLAGS -f test_files/test_ptrn_grep.txt"
"-e while -e void utils.c Makefile FLAGS -f test_files/test_ptrn_grep.txt"
"-e intel -e int FLAGS test_files/test_7_grep.txt"
"-e int -e intel FLAGS test_files/test_7_grep.txt"
)

manual=(
"-n for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-n for test_files/test_1_grep.txt"
"-n -e ^\} test_files/test_1_grep.txt"
"-c -e / test_files/test_1_grep.txt"
"-ce ^int test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e ^int test_files/test_1_grep.txt"
"-nivh = test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-e"
"-ie INT test_files/test_5_grep.txt"
"-echar test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-ne = -e out test_files/test_5_grep.txt"
"-iv int test_files/test_5_grep.txt"
"-in int test_files/test_5_grep.txt"
"-c -l aboba test_files/test_1_grep.txt test_files/test_5_grep.txt"
"-v test_files/test_1_grep.txt -e ank"
"-noe ) test_files/test_5_grep.txt"
"-l for test_files/test_1_grep.txt test_files/test_2_grep.txt"
"-o -e int test_files/test_4_grep.txt"
"-e = -e out test_files/test_5_grep.txt"
"-noe ing -e as -e the -e not -e is test_files/test_6_grep.txt"
"-e ing -e as -e the -e not -e is test_files/test_6_grep.txt"
#"-c -e . test_files/test_1_grep.txt -e '.'"
"-l for no_file.txt test_files/test_2_grep.txt"
"-e int -si no_file.txt utils.c no_file2.txt utils.h"
"-si utils.c -f no_pattern.txt"
)

rm global_s21.log global_grep.log
rm bad_samples_s21.log bad_samples_grep.log

run_test()
{   
    t=$(echo "$@" | sed "s/FLAGS/$var/")
    (( NUMBER++ ))
    echo ================================================TEST#$NUMBER >> global_s21.log
    ./s21_grep $t 2>/dev/null | tee -a global_s21.log > test_s21_grep.log
    echo ================================================TEST#$NUMBER >> global_grep.log
    grep $t 2>/dev/null | tee -a global_grep.log > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
#    if [ "$DIFF_RES" == "Файлы test_s21_grep.log и test_sys_grep.log идентичны" ]
    then
      (( SUCCESS++ ))
      echo "$FAIL/$SUCCESS/$COUNTER success grep $t"
      echo --------------------------------------------------- >> global_s21.log
      echo --------------------------------------------------- >> global_grep.log
      echo Command:\"./s21_grep $t\" >> global_s21.log
      echo Command:\"grep $t\" >> global_grep.log
      echo SUCCESS >> global_s21.log
      echo SUCCESS >> global_grep.log
      echo "$FAIL/$SUCCESS/$COUNTER success grep $t" >> global_s21.log
      echo "$FAIL/$SUCCESS/$COUNTER success grep $t" >> global_grep.log
    else
      (( FAIL++ ))
      echo "$FAIL/$SUCCESS/$COUNTER fail grep $t"
      echo ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^#$NUMBER=FAIL >> global_s21.log
      echo ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^#$NUMBER=FAIL >> global_grep.log
      echo Command:\"./s21_grep $t\" >> global_s21.log
      echo Command:\"grep $t\" >> global_grep.log
      echo "$FAIL/$SUCCESS/$COUNTER fail grep $t" >> global_s21.log
      echo "$FAIL/$SUCCESS/$COUNTER fail grep $t" >> global_grep.log
#Сохраняем кейсы с ошибками в отдельные файлы для удобства
      echo vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvTEST#$NUMBER >> bad_samples_s21.log
      echo vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvTEST#$NUMBER >> bad_samples_grep.log 
      ./s21_grep $t | tee -a bad_samples_s21.log > test_s21_grep.log
      grep $t | tee -a bad_samples_grep.log > test_sys_grep.log
      echo ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^#$NUMBER=FAIL^^^^^Command:\"./s21_grep $t\" >> bad_samples_s21.log
      echo ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^#$NUMBER=FAIL^^^^^Command:\"grep $t\" >> bad_samples_grep.log
    fi
    rm test_s21_grep.log test_sys_grep.log
}

echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"

for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "1 PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        run_test "$i"
    done
done
printf "\n"
echo "======================="
echo "2 PARAMETERS"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                run_test "$i"
            done
        fi
    done
done
printf "\n"
echo "======================="
echo "3 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "======================="
echo "4 PARAMETERS"
echo "======================="
printf "\n"
for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            for var4 in "${flags[@]}"
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="-$var1 -$var2 -$var3 -$var4"
                        run_test "$i"
                    done
                fi
            done
        done
    done
done
printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "DOUBLE PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                run_test "$i"
            done
        fi
    done
done

printf "\n"
echo "#######################"
echo "AUTOTESTS"
echo "#######################"
printf "\n"
echo "======================="
echo "TRIPLE PARAMETER"
echo "======================="
printf "\n"

for var1 in "${flags[@]}"
do
    for var2 in "${flags[@]}"
    do
        for var3 in "${flags[@]}"
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    run_test "$i"
                done
            fi
        done
    done
done
printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
##############################
