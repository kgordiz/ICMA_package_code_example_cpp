
CXX = g++
CXXFLAGS = -std=c++11 -Wall

SOURCES = ave_need_N1N2.cpp calc_go_contour_source.cpp cor_fft_c.cpp mode_averaging_AC_CCs.cpp Q_average_over_ensembles.cpp
HEADERS = ave_need_N1N2.h calc_go_contour_source.h cor_fft_c.h mode_averaging_AC_CCs.h Q_average_over_ensembles.h
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = my_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
