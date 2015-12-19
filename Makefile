#
# Download and place this file into your git repository. 
# Rename it to 'Makefile' (without quotes)
#
# wget http://www.phys.uconn.edu/phys2200/downloads/Makefile.sample
# mv Makefile.sample Makefile
#
# or
#
# wget http://www.phys.uconn.edu/phys2200/downloads/Makefile.sample -O Makefile
#

EDITOR = /usr/bin/vim
INDENT = /usr/bin/indent

CC        = clang
LDFLAGS   = -O
CFLAGS    = -Weverything -Wextra -pedantic $(LDFLAGS)
LDLIBS    = $(shell gsl-config --libs)

.SUFFIXES:
.SUFFIXES:  .c .o .h

.PHONY: edit clean veryclean

target    = main

$(target) : $(target).o custom_integrand.o \
            vegas_integrand.o printers.o timer.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean : 
	rm -f *.o
	rm -f *.*~

veryclean : clean
	rm  -f $(target)

