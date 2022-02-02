CC=g++
CFLAGS = -w -Isrc/

SRC_DIR := src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)


kompilator: src/lexer.yy.c src/parser.tab.c $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $^
	
src/lexer.yy.c: src/lexer.l parser.tab.h
	flex -o "src/lexer.yy.c" "src/lexer.l"

src/parser.tab.c parser.tab.h: src/parser.y
	bison -d -o "src/parser.tab.c" "src/parser.y"

