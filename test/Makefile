CXX=mpic++
CXXFLAGS=-std=c++11 -I../include
TARGET=local_rank_test

$(TARGET):main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)
