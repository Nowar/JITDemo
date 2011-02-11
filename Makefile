DEBUG := 1
CXXFLAGS += -I./include/
ifeq ($(DEBUG), 1)
  CXXFLAGS += -W -Wall -g -O0 -DDEBUG `llvm-config --cxxflags`
else
  CXXFLAGS += -O3 -DNDEBUG `llvm-config --cxxflags`
endif
LDFLAGS += `llvm-config --ldflags --libs`

define init
	mkdir -p out/
endef

define warnDebugVersion
	@echo "***** Warning *****"
	@echo "This is built on debug version."
	@echo "Use \`make DEBUG=0' to build optimized version."
	@echo "*******************"
endef

src_files := lib/JITer.cpp \
						 tools/demo.cpp

all : out/demo

out/demo : $(src_files)
	$(init)
	clang++ $^ -o $@ $(CXXFLAGS) $(LDFLAGS)
ifeq ($(DEBUG), 1)
	$(warnDebugVersion)
endif
	

clean:
	rm -f out/*
