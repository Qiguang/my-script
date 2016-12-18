CC = g++
CC_FLAGS = -std=c++11 -g -Wall
EXEC = myscript.exe
SRC = $(wildcard *.cpp) 
OBJ = $(SRC:.cpp=.o)

$(EXEC) : $(OBJ)
	$(CC) $(CC_FLAGS) -o $(EXEC) $(OBJ)

%.o : %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $^

clean :
	rm -f $(EXEC) $(OBJ)
