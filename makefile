# This makefile is used to compile the Funny JSON Explorer program.
build:
	g++ -o bin/fje -I./depends src/FunnyJsonExplorer.cpp
run:
	./bin/fje -f input/input.json -s tree -i default
	./bin/fje -f input/input.json -s rectangle -i default
	./bin/fje -f input/input.json -s tree -i poker
	./bin/fje -f input/input.json -s rectangle -i poker
all:
	g++ -o bin/fje -I./depends src/FunnyJsonExplorer.cpp
	./bin/fje -f input/input.json -s tree -i default
	./bin/fje -f input/input.json -s rectangle -i default
	./bin/fje -f input/input.json -s tree -i poker
	./bin/fje -f input/input.json -s rectangle -i poker