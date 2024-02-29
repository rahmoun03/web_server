NAME = Web_server

CHAB_SRC = CHEBCHOUB/main.cpp

AYOUB_SRC = AYOUB/Request.cpp AYOUB/Response.cpp

SWIRI_SRC = 
 
SRC = $(CHAB_SRC) $(AYOUB_SRC) $(SWIRI_SRC)

FLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME) : $(SRC)
	c++ $(FLAGS) $(SRC) -o $@
	./${NAME} config_file.conf

all : $(NAME)

clean :
	clear
	rm -rf $(NAME)
	rm -rf upload/*

fclean : clean

re : fclean all