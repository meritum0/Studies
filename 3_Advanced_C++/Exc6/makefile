CXX =  /opt/llvm/19.1.4/bin/clang++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -Wno-experimental-header-units

# source files
HEADERS = memory unordered_map queue iostream cstdint regex vector
PCH = $(HEADERS:=.pch)
PCH_FLAGS = $(foreach pch,$(PCH),-fmodule-file=$(pch))
MODULES = counter.cppm counter_collection.cppm io.cppm main.cppm
OBJECTS = $(MODULES:.cppm=.o)
MODULES_OBJ = $(MODULES:.cppm=.pcm)

TARGET = counters

# Default target
all: $(PCH) $(TARGET)

# headers
%.pch:
	$(CXX) $(CXXFLAGS) -Wno-pragma-system-header-outside-header -xc++-system-header --precompile $* -o $@



# precompile modules depended on counter
%.pcm: %.cppm
	$(CXX) $(CXXFLAGS) -fprebuilt-module-path=. --precompile $(PCH_FLAGS) $< -o $@

# Compile
%.o: %.pcm
	$(CXX) -std=c++20 -O2 -Wall -Wextra -fprebuilt-module-path=. -c $< -o $@

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $^ -o $@


# dependencies
counter_collection.pcm: counter.pcm
io.pcm: counter_collection.pcm counter.pcm
main.pcm: counter_collection.pcm counter.pcm io.pcm

clean:
	rm -f *.o *.pcm *.pch $(TARGET)

.PHONY: all clean

