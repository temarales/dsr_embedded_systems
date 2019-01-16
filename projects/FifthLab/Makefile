all: clean library main

flash_main: main
	@$(MAKE) -C example flash

library:
	@$(MAKE) -C lib all

main: library
	@$(MAKE) -C main all

clean:
	@$(MAKE) -C main clean
	@$(MAKE) -C lib clean