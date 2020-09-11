CFLAGS := -ggdb -std=c++11 -Wall -O2 -march=native
CFLAGS += `Magick++-config --cxxflags --cppflags`
LDFLAGS := CNecklace.cpp -lm
LDFLAGS += `Magick++-config --ldflags --libs`

all: whycode_gen

whycode_gen: whycode_gen.cpp CNecklace.cpp
	$(CXX) whycode_gen.cpp $(CFLAGS) $(LDFLAGS) -o $@ 

clean:
	rm -f *.o whycode_gen

.PHONY: clean

