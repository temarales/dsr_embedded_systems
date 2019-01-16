all: clean library main

flash: main
	@$(MAKE) -C main flash

library:
	@$(MAKE) -C lib all

main: library
	@$(MAKE) -C main all

clean:
	@$(MAKE) -C main clean
	@$(MAKE) -C lib clean