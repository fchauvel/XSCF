#
# This file is part of XCSF.
#
# XCSF is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# XCSF is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
# License for more details.
#
# You should have received a copy of the GNU General Public License
# along with XCSF.  If not, see <http://www.gnu.org/licenses/>.
#

APPLICATION = XCSF
VERSION = 0.0.1

CXX = g++

LD = g++

SOURCES_DIR = src
MAIN = ${SRC_DIR}/main.cpp

BINARIES = bin
DIST = ${BINARIES}/dist
DEBUG = ${BINARIES}/debug

SRC = $(shell find ${SOURCES_DIR} -name *.cpp)
OBJ = $(SRC:${SOURCES_DIR}/%.cpp=${DIST}/%.o)
EXE = ${DIST}/${APPLICATION}_${VERSION}.exe

TEST_SOURCES_DIR = tests

TEST_BIN_DIR = ${DEBUG}/test

DBG_OBJ = $(filter-out %/main.o, $(SRC:${SOURCES_DIR}/%.cpp=${DEBUG}/app/%.o))
TEST_SRC = $(shell find ${TEST_SOURCES_DIR} -name *.cpp)
TEST_OBJ = $(TEST_SRC:${TEST_SOURCES_DIR}/%.cpp=${TEST_BIN_DIR}/%.o)
TEST_EXE = ${TEST_BIN_DIR}/all_tests.exe

app: CXXFLAGS := -std=c++11 -O3 -Wall --friend-injection -DVERSION=\"${VERSION}\" -DAPPLICATION=\"${A#PPLICATION}\" -I./${SOURCES}
app: directories ${OBJ}
	${CXX} ${CXXFLAGS} ${LDFLAGS} -o ${EXE} ${OBJ}

directories:
	mkdir -p ${DIST}
	mkdir -p ${TEST_BIN_DIR}
	mkdir -p ${DEBUG}/app

${DIST}/%.o: ${SOURCES_DIR}/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@


test: ${TEST_EXE}
	./${TEST_EXE} ${TESTS}

${TEST_EXE}: LDLIBS := -lCppUTest -lCppUTestExt
${TEST_EXE}: CXXFLAGS := -std=c++11 -g -O0 -Wall --friend-injection -fprofile-arcs -ftest-coverage -I/usr/include/CppUTest -I./${SOURCES_DIR} -I./${TEST_SOURCES_DIR} -DVERSION=\"${VERSION}\" -DAPPLICATION=\"${APPLICATION}\"
${TEST_EXE}: directories ${TEST_OBJ} ${DBG_OBJ}
	${LD} $(LDFLAGS) -fprofile-arcs -o ${TEST_EXE} ${TEST_OBJ} ${DBG_OBJ} $(LDLIBS) 

${DEBUG}/app/%.o: ${SOURCES_DIR}/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

${TEST_BIN_DIR}/%.o: ${TEST_SOURCES_DIR}/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

COVERAGE_DATA = i3.info
upload-coverage: ${COVERAGE_DATA}
	curl -s https://codecov.io/bash | bash


${COVERAGE_DATA}: test
	lcov --base-directory . --directory . --capture --output-file $@


RM = rm -rf
clean:
	${RM} ${BINARIES} ${COVERAGE_DATA}


