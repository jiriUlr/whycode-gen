CFLAGS := -ggdb -std=c++11 -Wall -O2
LDFLAGS := CNecklace.cpp

all: whycode_gen

whycode_gen: whycode_gen.cpp CNecklace.cpp
	$(CXX) whycode_gen.cpp $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f *.o whycode_gen

.PHONY: clean

