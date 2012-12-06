
all: 
	$(MAKE) -C exception
	$(MAKE) -C utils
	ar rcs libvp.a exception/*.o utils/*.o
	
clean:
	$(MAKE) -C exception clean
	$(MAKE) -C utils clean
	rm -f *.a