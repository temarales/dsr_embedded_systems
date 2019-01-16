all: clean library main

flash_main: main
	@$(MAKE) -C example flash

example: library
	@$(MAKE) -C main all

library:
	@$(MAKE) -C library all

clean:
	@$(MAKE) -C main clean
	@$(MAKE) -C library clean