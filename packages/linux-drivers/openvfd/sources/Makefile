default:
	obj-m += driver/openvfd.o
	$(MAKE) modules

OpenVFDService: OpenVFDService.c
	$(CC) $(CFLAGS) -Wall -w -o $@ $^ -lm -lpthread
