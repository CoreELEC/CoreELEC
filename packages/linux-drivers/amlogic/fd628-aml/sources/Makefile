default:
	obj-m += driver/openvfd.o
	$(MAKE) modules

FD628Service: OpenVFDService.c
	$(CC) $(CFLAGS) -Wall -w -o $@ $^ -lm -lpthread
