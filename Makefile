
all clean:
	$(MAKE) --no-print-directory -C build $@ DEBUG=1

release:
	$(MAKE) --no-print-directory -C build $@ DEBUG=0
