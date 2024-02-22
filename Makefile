NAME = Server

CHAB_SRC = CHEBCHOUB/main2.cpp

AYOUB_SRC = AYOUB/Request.cpp 

SWIRI_SRC = 
 
SRC = $(CHAB_SRC) $(AYOUB_SRC) $(SWIRI_SRC)

FLAGS = -Wall -Wextra -Werror -std=c++98

$(NAME) : $(SRC)
	c++ $(FLAGS) $(SRC) -o $@

all : $(NAME)

clean :
	rm -rf $(NAME)

fclean : clean

re : fclean all