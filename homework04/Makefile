

#
# Programs
#
all:	luxcanisHost

luxcanisHost:  luxCanis.o
	$(CC) $(LDFLAGS) -o $@ $^

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm luxCanis.o
