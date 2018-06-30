default:
	obj-m += driver/kvimfan.o
	$(MAKE) modules

kvimfan_Service: kvimfan_Service.o
	$(CC) $(CFLAGS) -Wall -w -o $@ $^ -lm -lpthread
