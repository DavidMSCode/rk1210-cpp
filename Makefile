#
# Makefile for RK1210 Integrator.
# Very hacked together and unsatisfactory.
# Brent Macomber & Ausitn Probe
#  21/05/2015
#



RK1210_PATH = ./
EGM_PATH = ../egm2008_c/
INC = -I$(RK1210_PATH) -I$(EGM_PATH)
LINK = -lm -lrt
OPTS =  -g -ffloat-store -std=c99
COMP = gcc

# BUILD ALL THE THINGS
all: grav rk1210_obj dr

# BUILD EGM2008 GRAVITY
GRAV_OBJ = EGM2008.o
grav:
	$(COMP) -c \
	$(INC) $(LINK) $(OPTS) \
	$(EGM_PATH)/EGM2008.c


# BUILD RK1210
rk1210_OBJ = rk1210_obj.o
rk1210_obj:
	$(COMP) -c \
	$(INC) $(LINK) $(OPTS) \
	$(RK1210_PATH)/rk1210.c

# BUILD Driver
DR_OBJS = \
	EGM2008.o \
	rk1210.o
dr:
	$(COMP) $(DR_OBJS) \
	$(INC) $(LINK) $(OPTS) \
	$(RK1210_PATH)/rk1210Test.c \
	-o test

clean:
	rm -rf *.o *.out *.h.gch test *~
