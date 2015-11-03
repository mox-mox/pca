

BOX_SIZE ?= 100
STEPS ?= 200
OPT ?= 3

CXXFLAGS += -DDEBUG
CXXFLAGS += -DOPTIMISE=CACHE
CXXFLAGS += -DBOX_SIZE=$(BOX_SIZE)
CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors
CXXFLAGS += -O${OPT}

SOURCES := a1.cpp getopt_pp.cpp


all: relaxation visualisation


relaxation: $(SOURCES)
	@echo -e "\n----Compiling the program"
	g++ $(CXXFLAGS) -o $@  $^

.PHONY: visualisation
visualisation: relaxation
	@echo -e "\n----Visualising the relaxation progress"
	./visualisation.sh $(BOX_SIZE) $(STEPS)

timing: relaxation




.PHONY: clean
clean:
	rm -f relaxation

.PHONY: clean_visualisation
clean_visualisation:
	rm -f ./data/iteration_*.jpeg
	rm -f ./data/iteration_*.dat
	rm -f ./relaxation.gif
