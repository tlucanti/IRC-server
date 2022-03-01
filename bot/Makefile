
NAME		= bot
SRCS		= main.cpp	 \
			  Bot.cpp \
			  User.cpp \
			  Message.cpp 

OBJS		= $(SRCS:.cpp=.o)
CC			= c++ -Wall -Wextra -Werror -std=c++2a

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

$(OBJS): %.o : %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re clean
