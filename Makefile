PREFIX ?= /data/data/hilled.pwnterm/files/usr

all:
	@echo 'Installing...'
	@cp -rf install/* $(PREFIX)/
	@echo 'Done...'

.FORCE: all
.PHONY: all
