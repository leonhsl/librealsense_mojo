# Detect OS and CPU
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
machine := $(shell sh -c "$(CC) -dumpmachine || echo unknown")

CFLAGS := -std=c11 -DLIBRS_MOJO_WRAPPER_IMPLEMENTATION -fPIC -pedantic
CXXFLAGS := -std=c++11 -fPIC -pedantic -Ofast -Wno-missing-field-initializers
CXXFLAGS += -Wno-switch -Wno-multichar

ifeq (arm-linux-gnueabihf,$(machine))
CXXFLAGS += -mfpu=neon -mfloat-abi=hard -ftree-vectorize
else
ifeq (aarch64-linux-gnu,$(machine))
CXXFLAGS += -mfpu=neon -mfloat-abi=hard -ftree-vectorize
else
CXXFLAGS += -mssse3
endif
endif

# Compute list of all *.o files that participate in librealsense_wrapper.so
OBJECTS = $(notdir $(basename $(wildcard src/*.cc)))
OBJECTS := $(addprefix obj/, $(addsuffix .o, $(OBJECTS)))

# Aliases for convenience
all: lib/librealsense_wrapper.so

install: library
	install -m755 -d /usr/local/include/librealsense_wrapper
	cp -r include/librealsense_wrapper/* /usr/local/include/librealsense_wrapper
	cp lib/librealsense_wrapper.so /usr/local/lib
	ldconfig

uninstall:
	rm -rf /usr/local/include/librealsense_wrapper
	rm /usr/local/lib/librealsense_wrapper.so
	ldconfig

clean:
	rm -rf obj
	rm -rf lib

library: lib/librealsense_wrapper.so

prepare:
	mkdir -p obj
	mkdir -p lib

lib/librealsense_wrapper.so: prepare $(OBJECTS)
	$(CXX) -std=c++11 -shared $(OBJECTS) -o $@

lib/librealsense.a: prepare $(OBJECTS)
	ar rvs $@ `find obj/ -name "*.o"`

obj/%.o: src/%.cc
	$(CXX) $< $(CXXFLAGS) -c -o $@
