CC = gcc
FILES = anagram.c
OUT_EXE = anagram

build: $(FILES)
	$(CC) -o $(OUT_EXE) $(FILES)
