NAME=raytracing
SRCS=main.cc
OBJS=$(SRCS:.c=.o)
CC=g++
FLAGS= -std=c++17 -Wc++11-extensions

all: $(NAME)

$(NAME): $(OBJS) *.h
	$(CC) $(FLAGS) -o $(NAME) $(OBJS)

show: $(NAME)
	./$(NAME) > /tmp/tmp.ppm
	open /tmp/tmp.ppm

.PHONY: show
