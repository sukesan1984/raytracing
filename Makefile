NAME=raytracing
SRCS=main.cc
OBJS=$(SRCS:.c=.o)
CC=g++
FLAGS=-Wall -Wno-overloaded-virtual -O3 -std=c++14

all: $(NAME)

$(NAME): $(OBJS) *.h
	$(CC) $(FLAGS) -o $(NAME) $(OBJS)

show: $(NAME)
	./$(NAME) > /tmp/tmp.ppm
	open /tmp/tmp.ppm

.PHONY: show
