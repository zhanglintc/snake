# makefile made by zhanglintc

Objects = affix.o snake.o

make: $(Objects)
	g++ -lncurses -o snake $(Objects)

affix.o: affix.cpp snake.h
	g++ -c affix.cpp

snake.o: snake.cpp snake.h
	g++ -c snake.cpp

# phony commands
.PHONY:
clean:
	@rm -f *.o
	@rm -f snake
	@echo "rm *.o: OK"
	@echo "rm snake: OK"

run:
	@./snake
