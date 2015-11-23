
BINARY = ueberproject
DEBUG_BUILD=1
VECTORISE=1


#OPTIMISE_TRANSPOSE=1
#ifeq ($(OPTIMISE_TRANSPOSE),1)
#	DEFS		+= -DOPTIMISE_TRANSPOSE
#endif


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

BUILDDIR ::= builddir

# A macro to create neccesary directories
DIR_GUARD = @mkdir -p $(@D)


###############################################################################
# Source files


# VPATH is makes' search path for source- and target files
VPATH ::= $(CURDIR)

# Find all source files
dirwild = $(foreach f,$(wildcard $1*),$(call dirwild,$f/) $f)
SOURCE_FILES := $(realpath $(filter %.c %.cpp %.cxx %.c++, $(call dirwild, $(strip $(subst :, ,$(VPATH))))))

OBJS ::= $(patsubst %.cpp,%.o,$(filter %.cpp, $(SOURCE_FILES)))
OBJS ::= $(addprefix $(BUILDDIR)/,$(OBJS))

###############################################################################
# Executables


CXX				:= g++
LD				:= g++
OBJDUMP			:= objdump
SIZE			:= size


################################################################################
# Used libraries

LDLIBS		+= -pthread

################################################################################
# C & C++ common flags

#Note: -isystem cannot be used with C++ files, because it implicitly adds extern "C" to each file :(
#COMMONFLAGS	+= -Os -ggdb -Wshadow $(DEFS)
COMMONFLAGS	+= -Os -ggdb $(DEFS)
#COMMONFLAGS	+= -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wredundant-decls
COMMONFLAGS	+= -Wall -Wextra -Wpedantic -Werror  -Wredundant-decls
COMMONFLAGS	+= -fno-common -ffunction-sections -fdata-sections
COMMONFLAGS	+= -MD -Wundef
COMMONFLAGS	+= -Wno-packed-bitfield-compat # TODO: Remove this
COMMONFLAGS += -Wno-unused-function
COMMONFLAGS	+= -Iextern
#COMMONFLAGS	+= -Wpadded -Winline
COMMONFLAGS +=
ifneq ($(DEBUG_BUILD), 0)
	COMMONFLAGS += -DDEBUG
endif
ifneq ($(VECTORISE), 0)
	COMMONFLAGS += -DVECTORISE
endif

###############################################################################
# C++ flags

#CXXFLAGS	+= $(COMMONFLAGS) -Wold-style-cast -std=gnu++14# -Weffc++
CXXFLAGS	+= $(COMMONFLAGS)  -std=gnu++14# -Weffc++

###############################################################################
# Linker flags

##LDFLAGS		+= $(addprefix -L, $(dir $(LIBS)))
##LDFLAGS		+= -L..
LDFLAGS		+= -Wl,-Map=$(@).map
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

prog: $(BUILDDIR)/$(BINARY).elf

compilerversion = $(shell $(CC) --version | head -n1)

# Print some nice information at the beginning of each compilation run
.PHONY: begin
begin:
	@echo -e "\n###################################################################################################################################\n"
	@echo -e "Starting Build of Project \"$(BINARY)\":"
	@echo -e "All output will reside in the build directory \"$(BUILDDIR)\"."
	@echo -e "The Compiler in use: $(compilerversion)"
	@echo -e "\n###################################################################################################################################\n"

$(BUILDDIR)/$(BINARY).elf: $(OBJS)
	@echo -e  "\n$(ECHO)----Linking $^ to $@"
	$(DIR_GUARD)
	$(Q)$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(Q)$(OBJDUMP) -S $@ > $(@).list
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

.PHONY: tags
tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q --excmd=p .

.PHONY: valgrind
valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all $(BUILDDIR)/$(BINARY).elf -d 1000 -t 6 -i 10

.PHONY: selftest
selftest: all
	$(BUILDDIR)/$(BINARY).elf -v

.PHONY: benchmark
benchmark: all
	$(BUILDDIR)/$(BINARY).elf -d 100 -t  1 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 100 -t  2 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 100 -t  4 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 100 -t  8 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 100 -t 16 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 100 -t 32 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t  1 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t  2 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t  4 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t  8 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t 16 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 500 -t 32 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t  1 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t  2 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t  4 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t  8 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t 16 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 1000 -t 32 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t  1 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t  2 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t  4 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t  8 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t 16 -i 10
	$(BUILDDIR)/$(BINARY).elf -d 10000 -t 32 -i 10



-include $(OBJS:.o=.d)

# Debugging target. Use make print-FOO to print the value of variable FOO
# See http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/2007/0703/070201jg01/070201jg01.html
print-%:
	@echo $* == $($*)
endif

