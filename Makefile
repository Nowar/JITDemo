CXXFLAGS += -W -Wall -O3 `llvm-config --cxxflags`
LDFLAGS += `llvm-config --ldflags --libs`

all: out/JITDemo

out/%: src/%.cpp Makefile
	mkdir -p out
	clang++ $< -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f out/*
