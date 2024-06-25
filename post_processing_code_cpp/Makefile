CXX = g++
CXXFLAGS = -std=c++11 -Wall

SOURCES = main.cpp Q_average_over_ensembles.cpp cor_fft_c.cpp mode_averaging_AC_CCs.cpp ave_need_N1N2.cpp calc_go_contour_source.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = ICMA_pp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

