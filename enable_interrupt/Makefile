# Module name
obj-m += interrupt_en.o

# Kernel build directory
KDIR := /lib/modules/$(shell uname -r)/build

# Compilation flags (if needed)
EXTRA_CFLAGS :=  # Add any extra flags here, e.g., -DDEBUG

# Default target
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean target
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

# Load module target
load:
	sudo insmod interrupt_en.ko

# Unload module target
unload:
	sudo rmmod interrupt_en

.PHONY: all clean load unload

