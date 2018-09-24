INC_DIR:=./inlcude/network ./include/utils ./include /cppjieba
SRC_DIR:=./src ./src/utils ./src/network
HPP_FILS:=$(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/*.h)
OBJS:=$(wildcard ./src/utils/*.cpp) $(wildcard ./src/network/*.cpp) $(wildcard ./src/*.cpp)
LIBS:= -ljson -lpthread
CC:=g++
CPPFLAGS:= -std=c++11 -g -Wno-deprecated
INC_FILE:=$(addprefix -I,$(INC_DIR))
EXE:=./bin
$(EXE):$(OBJS)
		$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(INC_FILE) $(LIBS)
clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
