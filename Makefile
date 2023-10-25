all: $(TARGET)
.PHONY: all

.PHONY: init
init: compile_commands.json

.PHONY: clean
clean:
	rm -f compile_commands.json

compile_commands.json:
	sh init.sh
