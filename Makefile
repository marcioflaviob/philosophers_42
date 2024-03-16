INCDIR = ./includes/

NAME = philo
FILE = philosophers.c
SRCSDIR = ./srcs/
OBJDIR = ./objs/

SOURCES = $(wildcard $(SRCSDIR)*.c)
OBJECTS = $(patsubst %.c,$(OBJDIR)%.o,$(notdir $(SOURCES)))

CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCDIR)

all: $(NAME)

$(NAME): $(OBJECTS) $(OBJDIR)philosophers.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)%.o: $(SRCSDIR)%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)philosophers.o: $(SRCSDIR)$(FILE) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus