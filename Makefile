CXX_FLAGS := -std=c++11 -Wall -O2

whycode_gen: whycode_gen.cpp CNecklace.cpp
	$(CXX) $(CXX_FLAGS) $^ -o $@ 

clean:
	rm -f whycode_gen

clean_markers:
	rm -f *.svg

clean_all: clean clean_markers

.PHONY: clean clean_markers clean_all

