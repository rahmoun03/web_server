NAME = webserv

CHAB_SRC = CHEBCHOUB/main.cpp CHEBCHOUB/conf.cpp CHEBCHOUB/WebServer.cpp 

AYOUB_SRC = AYOUB/Request.cpp AYOUB/Response.cpp AYOUB/mime_types.cpp AYOUB/4xx.cpp \
			AYOUB/5xx.cpp AYOUB/methods.cpp AYOUB/Client.cpp#AYOUB/servClient.cpp

SRC = $(CHAB_SRC) $(AYOUB_SRC) $(SWIRI_SRC)

FLAGS = -Wall -Wextra -Werror -pedantic -std=c++98 -fsanitize=address -g3

$(NAME) : $(SRC)
	c++ $(FLAGS) $(SRC) -o $@

all : $(NAME)

clean :
	clear
	rm -rf $(NAME)
	rm -rf assets/upload/*

fclean : clean

re : fclean all