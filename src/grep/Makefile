TARGET = s21_grep
FLAGS = -Wall -Werror -Wextra

$(TARGET) : main.o utils.o
	gcc $(FLAGS) main.o utils.o -o $(TARGET)

main.o : main.c
	gcc  $(FLAGS) -c main.c -o main.o

utils.o : utils.c
	gcc  $(FLAGS) -c utils.c -o utils.o

rebuild: clean build

build: $(TARGET)

clean :
	rm $(TARGET) *.o

test:
	./grep_tests.sh