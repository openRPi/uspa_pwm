src_dir = src
inc_dir = include

CC = gcc
CFLAGS = -Wall -g

$(src_dir)/%.o: $(src_dir)/%.c 
	$(CC) $(CFLAGS) -o $@ -c $< -I$(inc_dir)

pwm: src/pwm.o src/bcm2835.o
	$(CC) $(CFLAGS) -o $@.exe $^ -I$(inc_dir)

PHONY: clean

clean:
	rm -rf *.o $(src_dir)/*.o  *~  *.exe
