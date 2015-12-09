





# The name of the geneated binary
BINARY      ?= relaxation

# If not 0 debug information will be generated and the program will work in verbose mode
DEBUG_BUILD ?=0

# If not 0, the generated binary will run in parallel mode, else it will run sequentially
VECTORISE   ?=1

# The optimisation parameter to the compiler. Possible values: 0 1 2 3 s
OPTIMISE    ?=3

# The size of the array
ARRAYSIZE   ?=1024

STEPS       ?=1000

# The number of repetitions of the benchmark run
REPETITIONS ?=100

# AB HIER FINGER WEG!
###################################################################################################
###################################################################################################
###################################################################################################
###################################################################################################
###################################################################################################
###################################################################################################

# MOX: Set default to verbose, except when explicitly set to 0
V ?= 0

# Be silent per default, but 'make V=1' will show all compiler calls.
ifeq ($(shell test $(V) -lt 1; echo $$?),0)
#ifneq ($(V),1)
Q		:= @
NULL		:= 2>/dev/null
endif




###############################################################################
#Where to build the project

BUILDDIR = builddir

# A macro to create neccesary directories
DIR_GUARD = @mkdir -p $(@D)


###############################################################################
# Source files


# VPATH is makes' search path for source- and target files
#CURDIR := $(abspath $(lastword $(MAKEFILE_LIST)))
#VPATH := $(CURDIR)
VPATH = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Find all source files
dirwild = $(foreach f,$(wildcard $1*),$(call dirwild,$f/) $f)
SOURCE_FILES = $(realpath $(filter %.c %.cpp %.cxx %.c++, $(call dirwild, $(strip $(subst :, ,$(VPATH))))))

OBJS = $(patsubst %.cpp,%.o,$(filter %.cpp, $(SOURCE_FILES)))
OBJS := $(addprefix $(BUILDDIR)/,$(OBJS))

###############################################################################
# Executables


CXX				:= g++
LD				:= g++
OBJDUMP			:= objdump
SIZE			:= size


################################################################################
# Used libraries

LDLIBS		+= -pthread
DEFS        += -DN=$(ARRAYSIZE)
DEFS        += -DREPETITIONS=$(REPETITIONS)

################################################################################
# C & C++ common flags

#Note: -isystem cannot be used with C++ files, because it implicitly adds extern "C" to each file :(
#COMMONFLAGS	+= -Os -ggdb -Wshadow $(DEFS)
COMMONFLAGS	+= -O$(OPTIMISE) $(DEFS)
COMMONFLAGS	+= -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wredundant-decls
#COMMONFLAGS	+= -Wall -Wextra -Wpedantic -Werror  -Wredundant-decls
COMMONFLAGS	+= -fno-common -ffunction-sections -fdata-sections
COMMONFLAGS	+= -MD #-Wundef
COMMONFLAGS	+= -Wno-packed-bitfield-compat # TODO: Remove this
#COMMONFLAGS += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable
COMMONFLAGS	+= -Iextern
#COMMONFLAGS	+= -Wpadded -Winline
COMMONFLAGS	+= -fopenmp
ifneq ($(DEBUG_BUILD), 0)
COMMONFLAGS	+= -ggdb -DDEBUG
endif
ifneq ($(VECTORISE), 0)
	COMMONFLAGS += -DVECTORISE
endif

###############################################################################
# C++ flags

#CXXFLAGS	+= $(COMMONFLAGS) -Wold-style-cast -std=gnu++14# -Weffc++
CXXFLAGS	+= $(COMMONFLAGS)  -std=gnu++11# -Weffc++

###############################################################################
# Linker flags

LDFLAGS		+= $(COMMONFLAGS)
##LDFLAGS		+= $(addprefix -L, $(dir $(LIBS)))
##LDFLAGS		+= -L..
LDFLAGS		+= -Wl,-Map=$(BUILDDIR)/$(@).map
LDFLAGS		+= -Wl,--gc-sections

###############################################################################
# Calculate the percentage for progress indication
# See http://stackoverflow.com/a/455390/4360539
ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@ $(MAKE) $@ --no-print-directory -f $(firstword $(MAKEFILE_LIST))
	@ #$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory  -nrf $(firstword $(MAKEFILE_LIST))  ECHO="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
ECHO = "		`expr " [\`expr $C '*' 100 / $T\`" : '.*\(....\)$$'`%]"
endif

################################################################################
###############################################################################
###############################################################################


all: begin prog

prog: $(BINARY).elf

compilerversion = $(shell $(CC) --version | head -n1)

# Print some nice information at the beginning of each compilation run
.PHONY: begin
begin:
	@echo -e "\n###################################################################################################################################\n"
	@echo -e "Starting Build of Project \"$(BINARY)\":"
	@echo -e "All output will reside in the build directory \"$(BUILDDIR)\"."
	@echo -e "The Compiler in use: $(compilerversion)"
	@echo -e "\n###################################################################################################################################\n"

$(BINARY).elf: $(OBJS)
	@echo -e  "\n$(ECHO)----Linking $^ to $@"
	$(DIR_GUARD)
	$(Q)$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(Q)$(OBJDUMP) -S $@ > $(BUILDDIR)/$(@).list
	@echo -e "\n###################################################################################################################################\n"
	@echo -e "Finishing build. The sizes of the output file chunks:"
	$(Q)$(SIZE) $@
	@echo -e "\n###################################################################################################################################\n"

$(BUILDDIR)/%.o: %.cpp
	@echo -e  "\n$(ECHO)----Compiling C++-file $< to $@"
	$(DIR_GUARD)
	$(Q)$(CXX) $(CXXFLAGS) -o $@ -c $<




#$(BUILDDIR):
#	@echo -e "\n$(ECHO)----Creating build directory $@"
#	mkdir -p $@

.PHONY: clean
clean:
	@echo -e "\n----Cleaning the project directory"
	$(Q)rm -rf $(BUILDDIR)
	$(Q)rm -f $(BINARY).elf

.PHONY: tags
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q --excmd=p .

.PHONY: valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(BINARY).elf

.PHONY: run
run:
	./$(BINARY).elf -r 128


.PHONY: selftest
selftest: all
	./$(BINARY).elf -v

.PHONY: benchmark
benchmark: all
	@echo -e  "\n$(ECHO)----Generating the Benchmark Data"
	$(Q)rm -rf benchmark_data
	./$(BINARY).elf -t $(VECTORISE) -s $(STEPS)
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 100 -i 10 -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 500 -i 10 -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 1000 -i 10 -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 10000 -i 10 -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 100 -i 10 -f -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 500 -i 10 -f -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 1000 -i 10 -f -b
#./$(BINARY).elf -t $(VECTORISE) -s 1000 -d 10000 -i 10 -f -b

.PHONY: plots
plots: benchmark
	@echo -e  "\n$(ECHO)----Plotting the Benchmark Data"
	gnuplot generate_plots.gp


-include $(OBJS:.o=.d)

# Debugging target. Use make print-FOO to print the value of variable FOO
# See http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/2007/0703/070201jg01/070201jg01.html
print-%:
	@echo $* == $($*)
endif

