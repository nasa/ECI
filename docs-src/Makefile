# A simple makefile to execute the pipeline

DOXYGEN_CONF := conf/Doxyfile
DOXYREST_CONF := conf/doxyrest-config.lua

doxy:
	@echo Building...
	@doxygen $(DOXYGEN_CONF)
	@./doxyrest/doxyrest -c $(DOXYREST_CONF) 2> /dev/null
	@make -s -C ./source html

clean:
	@echo Removing documentation related files...
	@rm -rf ../docs/ ./build/ ./source/rst

all: clean doxy
