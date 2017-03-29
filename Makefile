

APPLICATION = XCSF
VERSION = 0.0.1

CXX = g++
CXXFLAGS = -std=c++11 -g -O3 -Wall -fprofile-arcs -ftest-coverage -DVERSION=\"${VERSION}\" -DAPPLICATION=\"${APPLICATION}\" -I./src

LD = g++
LDFLAGS = -g
LDLIBS = 

SRC = $(shell find src -name *.cpp)
OBJ = $(SRC:%.cpp=%.o)
OBJ_COVA = $(TEST_SRC:%.cpp=%.gcda)
OBJ_COVO = $(TEST_SRC:%.cpp=%.gcdo)
BIN = ${APPLICATION}_${VERSION}.exe

app: ${OBJ}
	g++ ${LDFLAGS} -o ${BIN} $^


TEST_SRC = $(shell find tests -name *.cpp)
TEST_OBJ = $(TEST_SRC:%.cpp=%.o)
TEST_BIN = all_tests.exe



REPORT_DIR = reports

coverage-report: all-tests
	mkdir -p ${REPORT_DIR}
	./${TEST_BIN}
	lcov --base-directory . --directory . --capture --output-file i3.info
	genhtml -o report/i3-coverage i3.info

test: all_tests
	./${TEST_BIN}

all_tests: LDLIBS := -lCppUTest -lCppUTestExt	

all_tests: $(filter-out src/main.o,${OBJ}) ${TEST_OBJ}
	${LD} $(LDFLAGS) -fprofile-arcs -o ${TEST_BIN} $^ $(LDLIBS) 

$(TEST_OBJ): CXXFLAGS := -std=c++11 -Wall -I/usr/include/CppUTest -I./src -I/test

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@


clean:
	${RM} ${OBJ} ${OBJ_COVA} ${OBJ_COVO} ${BIN} ${TEST_OBJ} ${TEST_BIN}


