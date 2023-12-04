CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g3
SRC_DIR = ./Sources
EXECUTABLE = ircserv

SRCS = \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/utils.cpp \
    $(SRC_DIR)/Server.cpp \
    $(SRC_DIR)/User.cpp \
    $(SRC_DIR)/Channel.cpp \
    $(SRC_DIR)/Commands/pass.cpp \
    $(SRC_DIR)/Commands/nick.cpp \
    $(SRC_DIR)/Commands/user.cpp \
    $(SRC_DIR)/Commands/join.cpp \
    $(SRC_DIR)/Commands/kick.cpp 

OBJS = $(SRCS:.cpp=.o)

$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(EXECUTABLE)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXECUTABLE)

re: fclean all

.PHONY: all clean fclean re
