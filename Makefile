CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS= -Wall -m5307 -pipe -nostdlib
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
OBJDUMP=m68k-elf-objdump
ASM=./main/start.s process_management/process_stack.s io/serial_entry.s timer/timer_entry.s debug/dbug.s
LDFLAGS_RTX = -Trtx/rtx.ld -Wl,-Map=rtx/rtx.map
LDFLAGS_RTX_LOADER = -Trtx/rtx_loader.ld -Wl,-Map=rtx/rtx_loader.map
LDFLAGS_RTX_TEST = -Trtx/rtx_test.ld -Wl,-Map=rtx/rtx_test.map
LDFLAGS_STRESS_TEST = -Trtx/rtx_test.ld -Wl,-Map=rtx/rtx_test.map

RTX_OBJS = rtx.o dbug.o main_rtx.o memory.o messaging.o priority.o process_management.o uart.o kcd.o crt.o timer.o init.o
RTX_LOADER_OBJS = rtx.o dbug.o rtx_loader.o memory.o messaging.o priority.o process_management.o uart.o kcd.o crt.o timer.o init.o
RTX_TEST_DUMMY_OBJS = rtx.o dbug.o rtx_test_dummy.o memory.o messaging.o priority.o process_management.o kcd.o crt.o uart.o timer.o init.o
STRESS_TEST_OBJS = rtx.o dbug.o stress_testing.o memory.o messaging.o priority.o process_management.o kcd.o crt.o uart.o timer.o init.o

all: mdummy.s19 


# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.

rtx.s19: $(RTX_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS_RTX) -o rtx.bin $(ASM) $(RTX_OBJS) 
	$(OBJCPY) --output-format=srec rtx.bin rtx.s19
	$(OBJDUMP) -xdC rtx.bin > rtx.lst

rtx_loader.s19: $(RTX_LOADER_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS_RTX_LOADER) -o rtx_loader.bin $(ASM) $(RTX_LOADER_OBJS) 
	$(OBJCPY) --output-format=srec rtx_loader.bin rtx_loader.s19
	$(OBJDUMP) -xdC rtx_loader.bin > rtx_loader.lst

rtx_test_dummy.s19: $(RTX_TEST_DUMMY_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS_RTX_TEST) -o rtx_test_dummy.bin $(ASM) $(RTX_TEST_DUMMY_OBJS) 
	$(OBJCPY) --output-format=srec rtx_test_dummy.bin rtx_test_dummy.s19
	$(OBJDUMP) -xdC rtx_test_dummy.bin > rtx_test_dummy.lst
	
stress_testing.s19: $(STRESS_TEST_OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS_STRESS_TEST) -o stress_testing.bin $(ASM) $(STRESS_TEST_OBJS) 
	$(OBJCPY) --output-format=srec stress_testing.bin stress_testing.s19
	$(OBJDUMP) -xdC stress_testing.bin > stress_testing.lst

m2.s19: rtx_test_dummy.s19 rtx.s19
	python merge.py m2.s19 rtx.s19 rtx_test_dummy.s19 
	chmod u+x m2.s19

mdummy.s19: m2.s19 rtx_loader.s19
	python merge.py mdummy.s19 m2.s19 rtx_loader.s19 
	chmod u+x mdummy.s19
	
timer.o: timer/timer.c timer/timer.h
	$(CC) $(CFLAGS) -c timer/timer.c

crt.o: io/crt.c io/uart.h
	$(CC) $(CFLAGS) -c io/crt.c
	
kcd.o: io/kcd.c io/uart.h
	$(CC) $(CFLAGS) -c io/kcd.c
	
uart.o: io/uart.c io/uart.h
	$(CC) $(CFLAGS) -c io/uart.c

memory.o: memory/memory.c memory/memory.h
	$(CC) $(CFLAGS) -c memory/memory.c

messaging.o: messaging/messaging.c messaging/messaging.h
	$(CC) $(CFLAGS) -c messaging/messaging.c

process_management.o: process_management/process_management.c process_management/process_management.h
	$(CC) $(CFLAGS) -c process_management/process_management.c

priority.o: priority/priority.c priority/priority.h
	$(CC) $(CFLAGS) -c priority/priority.c

init.o: main/init.c main/init.h
	$(CC) $(CFLAGS) -c main/init.c

main_rtx.o: main/main_rtx.c 
	$(CC) $(CFLAGS) -c main/main_rtx.c
	
dbug.o: debug/dbug.c debug/dbug.h
	$(CC) $(CFLAGS) -c debug/dbug.c

rtx_loader.o: rtx/rtx_loader.c 
	$(CC) $(CFLAGS) -c rtx/rtx_loader.c

stress_testing.o: stress/stress_testing.c 
	$(CC) $(CFLAGS) -c stress/stress_testing.c
	
rtx_test_dummy.o: rtx/rtx_test_dummy.c rtx/rtx_test.h
	$(CC) $(CFLAGS) -c rtx/rtx_test_dummy.c
	
rtx.o: rtx/rtx.c rtx/rtx.h
	$(CC) $(CFLAGS) -c rtx/rtx.c
	
.c.o:
	$(CC) $(CFLAGS) -c $< 

.PHONY: clean
clean:
	rm -f *.bin *.o *.map *.lst *.s19
