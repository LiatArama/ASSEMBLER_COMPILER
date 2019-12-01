all: assembler

OBJECTS :=  db_handler.o main.o first_read.o second_read.o utility.o  

-include $(OBJECTS:.o=.d)

%.c:%.h

%.o: %.c
	@echo Compiling : $@
	@gcc -Wall -ansi -pedantic $< -c -o $@
	@gcc -Wall -ansi -pedantic $< -MM -MF $(*F).d

assembler: $(OBJECTS)
	@echo "linking $@ -> $^"
	@gcc -Wall -ansi -pedantic -o $@ $^

clean:
	@rm -f $(OBJECTS)
	@rm -f $(OBJECTS:.o=.d)

