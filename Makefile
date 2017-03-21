

APPLICATION = XCSF
VERSION = 0.0.1

CXX = g++
CXXFLAGS = -g -O3 -Wall -DVERSION=\"${VERSION}\" -DAPPLICATION=\"${APPLICATION}\" -I./src

LD = g++
LDFLAGS = -g
LDLIBS = 

SRC = $(shell find src -name *.cpp)
OBJ = $(SRC:%.cpp=%.o)
BIN = ${APPLICATION}_${VERSION}

TEST_SRC = $(shell find tests -name *.cpp)
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)
TEST_BIN = all_tests

app: ${OBJ}
	g++ ${LDFLAGS} -o ${BIN} $^


TEST_SRC = $(shell find tests -name *.cpp)
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)
TEST_BIN = all_tests


test: all_tests
	./${TEST_BIN}

all_tests: LDLIBS := -lCppUTest -lCppUTestExt	

all_tests: $(filter-out src/main.o,${OBJ}) ${TEST_OBJ}
	${LD} $(LDFLAGS) -o ${TEST_BIN} $^ $(LDLIBS) 

$(TEST_OBJ): CXXFLAGS := -Wall -I/usr/include/CppUTest -I./src

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@


clean:
	${RM} ${OBJ} ${BIN} ${TEST_OBJ} ${TEST_BIN}


