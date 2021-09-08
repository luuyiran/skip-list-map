ALL:test

CFLAGS:=-W -Wall -O3 -g -fstack-protector-all -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak

test : test.cpp skp_map.hpp
	g++ -o $@ $< $(CFLAGS) 

.PHONY:clean
clean:
	@rm -rvf test
