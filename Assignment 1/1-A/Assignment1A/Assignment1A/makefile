COMP = g++
CFLAGS = -std=c++17
MATH = -lm
DEBUG = -g
EXEC = Assignment1.exe
INPUT_FILE = *.cpp

make:
	$(COMP) $(CFLAGS) -o $(EXEC) $(INPUT_FILE) $(MATH)

debug:
	$(COMP) $(CFLAGS) -o $(EXEC) $(INPUT_FILE) $(MATH) $(DEBUG)

clean:
	rm $(EXEC)
