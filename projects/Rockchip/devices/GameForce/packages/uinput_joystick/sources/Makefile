#PROJECT_DIR := $(shell pwd)
#PROM	= uinput_joystick
##CXX	?= ../../../output/host/usr/bin/aarch64-rockchip-linux-gnu-g++
#
#CXXFLAGS ?= -fPIC -O3 -I$(PROJECT_DIR) -lpthread
#OBJ = ts_uart.o  common.o
#
#$(PROM): $(OBJ)	
#	$(CXX) -o $(PROM) $(OBJ) $(CXXFLAGS)
#%.o: %.cpp
#	$(CXX) -c $< -o $@ $(CXXFLAGS)
#clean:
#	@rm -rf $(OBJ) $(PROM)
#
#install:
#	sudo install -D -m 755 uinput_joystick -t /usr/bin/
#
#CFLAGS= -g -c -static -Wall
#
#ts_uart : common.c ts_uart.c
#	$(CROSS_COMPILE)gcc  $(CFLAGS) common.c -o common.o
#	$(CROSS_COMPILE)gcc  $(CFLAGS) ts_uart.c -o ts_uart.o
#	$(CROSS_COMPILE)gcc  -static ts_uart.o common.o -o ts_uart
#clean : 
#	rm -rf *.o ts_uart
