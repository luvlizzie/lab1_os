CXX = x86_64-w64-mingw32-g++
CXXFLAGS = -static -Wall -std=c++11
TARGETS = Main.exe Creator.exe Reporter.exe

all: $(TARGETS)

Main.exe: lab1_os/Main.cpp lab1_os/employee.h
	$(CXX) $(CXXFLAGS) lab1_os/Main.cpp -o Main.exe

Creator.exe: lab1_os/Creator.cpp lab1_os/employee.h
	$(CXX) $(CXXFLAGS) lab1_os/Creator.cpp -o Creator.exe

Reporter.exe: lab1_os/Reporter.cpp lab1_os/employee.h
	$(CXX) $(CXXFLAGS) lab1_os/Reporter.cpp -o Reporter.exe

clean:
	rm -f *.exe *.bin *.txt

run: all
	wine Main.exe

.PHONY: all clean run
