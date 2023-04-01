OBJ=$(patsubst %.c,%.o,$(wildcard ./*.c))
TARGET=coortran.exe

${TARGET}:${OBJ}
	cc $^ -o $@
%.o:%.c
	cc -c $^ -o $@	

.PHONY:
clean:
	$(RM)  *.o 