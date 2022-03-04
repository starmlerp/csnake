TARGET = main

CPPFLAGS = -lcurses

all: $(TARGET) tags

$(TARGET): $(TARGET).c
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(TARGET).c

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
