name 			= 	spectra
binary_name		=	app
CXXFLAGS		=	-std=c++20 -Wall -Wextra -Werror -O3 -Wno-unused-parameter -Wno-switch
LDFLAGS			=	-shared -fPIC

rwildcard		=	$(foreach d, $(wildcard $(1:=/*)), $(call rwildcard, $d, $2) $(filter $(subst *, %, $2), $d))
SRC				=	$(call rwildcard, src, *.cpp)
OBJ				=	$(SRC:.cpp=.o)
TEST_OBJ		=	main.cpp

LIBS 			=	sfml-graphics	\
					sfml-window		\
					sfml-system		\
					m

LDLIBS      := $(addprefix -l, $(LIBS))

all:	$(name)

%.o: %.cpp
	g++ $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<

$(name):	$(OBJ)
	g++ $(CXXFLAGS) $(LDFLAGS) -o lib$(name).so $(OBJ) $(LDLIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(name)
	rm -f $(binary_name)

re:	fclean all

test: re $(TEST_OBJ)
	g++ $(CXXFLAGS) -o app $(TEST_OBJ) -Wl,-rpath,'$$ORIGIN' -L. -l$(name) $(LDLIBS)

.PHONY: all clean fclean re