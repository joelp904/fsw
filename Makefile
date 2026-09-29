CXX?=g++
CXX_VERSION?=c++17

TARGET := fsw

OBJECTS := gen/fsw.o \
           gen/register.o \
           gen/component.o \
           gen/temperature_monitor.o \
           gen/PCDU.o \
           gen/thermal_sim.o

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf gen/

gen:
	mkdir -p gen

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) -lpthread

gen/fsw.o: fsw.cpp src/simulation/thermal_sim.hpp src/components/PCDU.hpp src/components/temperature_monitor.hpp src/primitives/component.hpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c fsw.cpp -o gen/fsw.o

gen/thermal_sim.o: src/simulation/thermal_sim.cpp src/components/PCDU.hpp src/components/temperature_monitor.hpp src/primitives/component.hpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c src/simulation/thermal_sim.cpp -o gen/thermal_sim.o

gen/PCDU.o: src/components/PCDU.cpp src/components/PCDU.hpp src/primitives/component.hpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c src/components/PCDU.cpp -o gen/PCDU.o

gen/temperature_monitor.o: src/components/temperature_monitor.cpp src/components/temperature_monitor.hpp src/primitives/component.hpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c src/components/temperature_monitor.cpp -o gen/temperature_monitor.o

gen/component.o: src/primitives/component.hpp src/primitives/component.cpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c src/primitives/component.cpp -o gen/component.o

gen/register.o: src/primitives/register.cpp src/primitives/register.hpp | gen
	$(CXX) -std=$(CXX_VERSION) -c src/primitives/register.cpp -o gen/register.o
