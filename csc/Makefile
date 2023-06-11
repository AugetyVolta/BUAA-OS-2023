make_all:code/main.c code/fibo.c include/fibo.h
	gcc -I include include/fibo.h -c code/main.c 
	mv main.o code/
	gcc -I include include/fibo.h -c code/fibo.c 
	mv fibo.o code/
	gcc -o fibo code/main.o code/fibo.o
add_include:./include/fibo.h
	cp ./include/fibo.h ./code
clean: code/main.o code/fibo.o include/fibo.h.*
	rm include/fibo.h.*
	rm ./code/*.o
