# makefile made by zhanglintc

Objects = affix.o snake.o

opts = -std=c++11 -g

make: $(Objects)
ifeq ($(shell uname), Darwin)
	g++ $(opts) -o snake $(Objects) -lncurses
else
	g++ $(opts) -o snake $(Objects) -lncursesw
endif

affix.o: affix.cpp snake.h
	g++ $(opts) -c affix.cpp

snake.o: snake.cpp snake.h
	g++ $(opts) -c snake.cpp

# phony commands
.PHONY:
clean:
	@rm -f *.o
	@rm -f snake
	@echo "rm *.o: OK"
	@echo "rm snake: OK"

run:
	@./snake
