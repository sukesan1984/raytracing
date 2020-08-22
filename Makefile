NAME=raytracing
SRCS=main.cc
OBJS=$(SRCS:.c=.o)
CC=g++

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

show: $(NAME)
	./$(NAME) > /tmp/tmp.ppm
	open /tmp/tmp.ppm

.PHONY: show
