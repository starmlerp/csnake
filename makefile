TARGET = main

$CC = x86_64-w64-mingw32-gcc

CPPFLAGS = -lcurses

all: $(TARGET) tags

$(TARGET): $(TARGET).c makefile
	$(CC) $(CPPFLAGS) -o $(TARGET) $(TARGET).c

tags: $(TARGET).c $(HEDS)
	ctags $^

.PHONY: cleanup
cleanup:
	rm $(TARGET).o 

.PHONY: run
run: $(TARGET)
	./$^

.PHONY: debug
debug: $(TARGET)
	gdb $(TARGET)
