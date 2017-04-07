

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

COVERAGE_DATA = i3.info


upload-coverage: ${COVERAGE_DATA}
	python3 tools/upload_coverage.py -u fchauvel -p XSCF --input-file ${COVERAGE_DATA}

${COVERAGE_DATA}: test
	lcov --base-directory . --directory . --capture --output-file $@

test: ${TEST_BIN}
	./${TEST_BIN} ${TESTS}

${TEST_BIN}: LDLIBS := -lCppUTest -lCppUTestExt	

${TEST_BIN}: $(filter-out src/main.o,${OBJ}) ${TEST_OBJ}
	${LD} $(LDFLAGS) -fprofile-arcs -o ${TEST_BIN} $^ $(LDLIBS) 

$(TEST_OBJ): CXXFLAGS := -std=c++11 -Wall -fprofile-arcs -ftest-coverage -I/usr/include/CppUTest -I./src -I/test

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@


clean:
	${RM} ${OBJ} ${OBJ_COVA} ${OBJ_COVO} ${BIN} ${TEST_OBJ} ${TEST_BIN} ${COVERAGE_DATA}


