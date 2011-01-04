all: JITDemo

JITDemo: JITDemo.cpp
	clang++ $< -o $@ -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS \
		`llvm-config --cxxflags --ldflags --libs` -W -Wall -O3

clean:
	rm -f JITDemo
