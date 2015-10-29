

BOX_SIZE ?= 30
STEPS ?= 100

CXXFLAGS += -DDEBUG
CXXFLAGS += -DOPTIMISE=CACHE
CXXFLAGS += -DBOX_SIZE=$(BOX_SIZE)
CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors

SOURCES := a1.cpp getopt_pp.cpp


all: relaxation visualisation


relaxation: $(SOURCES)
	@echo -e "\n----Compiling the program"
	g++ $(CXXFLAGS) -o $@  $^

.PHONY: visualisation
visualisation: relaxation
	@echo -e "\n----Visualising the relaxation progress"
	./visualisation.sh $(BOX_SIZE) $(STEPS)





.PHONY: clean
clean:
	rm -f relaxation
