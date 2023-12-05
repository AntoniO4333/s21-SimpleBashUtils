TARGET = s21_cat
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
	./tests_Linux/cat_tests.sh