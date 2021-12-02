.PHONY: clean dirs exe

all:clean dirs bin/ut_all exe

objects = obj/filesystem_builder.o obj/find_visitor.o obj/update_path_visitor.o


bin/ut_all: test/ut_main.cpp $(objects) test/ut_fs.h src/link.h src/file.h src/folder.h src/node.h src/null_iterator.h src/iterator.h src/utilities.h src/visitor.h 
	#g++ -Wfatal-errors -std=c++11   -o $@ $< $(objects) -lgtest -lpthread
	#g++ -Wfatal-errors -std=c++11  -o  $@ $< $(objects) -lgtest -lpthread
	g++ -Wfatal-errors -std=c++11  $< -o  $@  $(objects) -lgtest -lpthread

obj/find_visitor.o: src/find_visitor.cpp src/find_visitor.h
	g++ -Wfatal-errors -std=c++11 -c $< -o $@

obj/update_path_visitor.o: src/update_path_visitor.cpp src/update_path_visitor.h
	g++ -Wfatal-errors -std=c++11 -c $< -o $@

obj/filesystem_builder.o: src/filesystem_builder.cpp src/filesystem_builder.h
	g++ -Wfatal-errors -std=c++11 -c $< -o $@

exe:
	bin/ut_all
	
dirs:
	 mkdir -p bin
	 #mkdir -p obj
	 
clean:
	rm -f bin/*
	rm -f obj/*