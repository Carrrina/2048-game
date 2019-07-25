build: 2048
2048: 2048.c
	gcc -Wall 2048.c -o 2048 -lcurses -lm

.PHONY:

run: 2048
	./2048
clean:
	rm -f 2048
empty:
	> users.txt
	> scores.txt
	> dates.txt
	> last_game.txt

check:
	valgrind --tool=memcheck --leak-check=full --leak-check=full --show-leak-kinds=all ./2048
