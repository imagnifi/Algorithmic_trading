CC=g++ -std=c++17 -ggdb3
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
  LIBS=-lgtest -lstdc++  -lpthread -lrt -lm
  OPEN=xdg-open
else
  LIBS=-lgtest -lstdc++ -pthread
  OPEN=open
endif

SRC = ./main.cpp\
        ./view/*.cpp\
        ./view/*.h\
        ./controller/*.cc\
        ./controller/*.h\
        ./model/*.cc\
        ./model/*.h\
    ./shared/parser/*.h\
    ./shared/parser/*.cc


all: clean build
	./build/trading

install: clean build
	@if [ -d ../install ]; then \
	rm -rf ../install/*; \
	else \
	mkdir ../install; \
	fi
	@-cp -R build/trading ../install

uninstall:
	@rm -rf ../install

build:
	@-mkdir ./build
	@-cp -R model view controller shared main.cpp *.pro Doxyfile ./build
	@-cd ./build; qmake src.pro
	@-make -C build
	@-cd build; rm -rf model view controller shared *.pro *.o *.cpp *.h *.ui Makefile Doxyfile

dvi:
	@doxygen Doxyfile

dist: clean
	@mkdir ../trading_dist
	@mkdir ../trading_dist/src
	@cp -rf . ../trading_dist/src
	@cd ..; tar czvf trading_dist.tgz trading_dist/*
	@mkdir ../distrib
	@mv -t ../distrib ../trading_dist.tgz 
	@rm -rf ../trading_dist

clean:
	@rm -rf *.o *.a *.out
	@rm -rf ./build ./documentations ../distrib

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n $(SRC)
	
cppcheck:
	@cppcheck --std=c++17 --language=c++ --enable=all \
        --suppress=uninitMemberVar --suppress=unusedFunction \
        --suppress=unusedPrivateFunction --suppress=constParameter \
        $(SRC)


#	--suppress=unusedFunction --suppress=unreadVariable \
#	--suppress=unusedStructMember --suppress=unusedVariable --suppress=redundantInitialization \
#	--suppress=variableScope --suppress=missingInclude --suppress=noCopyConstructor --suppress=noOperatorEq \
