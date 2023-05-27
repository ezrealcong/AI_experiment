##################################################################################################
# 编译工具链设置

TOOL_CHAIN = 
CC = g++
AR = ar

DEBUG_BUILD ?= yes

SHOW_COMMAND=yes #显示编译命令
ifeq (${SHOW_COMMAND}, yes)
QUIET :=
else
QUIET := @
endif

###################################################################################################
# 目录设置
# 工程根路径
PROJ_ROOT = $(abspath .)
# 中间文件缓存文件夹
TMP_PATH = $(abspath .)/tmp
# 当前路径
PWD_PATH = $(abspath .)

###################################################################################################
# 源文件.c
SRC := ${PROJ_ROOT}/src/*.cpp
# 展开*匹配，获取所有源文件完整路径
SRC := $(wildcard ${SRC})

###################################################################################################
# 头文件路径设置
INCLUDE_PATH += /include

###################################################################################################
# 编译选项
CFLAGS := -Wall -pedantic -std=c++0x  

ifeq ("${DEBUG_BUILD}","yes")
CFLAGS += -ggdb -DDEBUG -ggdb3 -gdwarf-2 -D_DEBUG_=1 -g
else
CFLAGS += -O3 -DNDEBUG
endif

###################################################################################################
# 生成的中间文件.o
OBJ := $(patsubst ${PROJ_ROOT}/src/%.cpp,${TMP_PATH}/%.o,${SRC})
# 头文件存放路径设置
INC := $(foreach path,${INCLUDE_PATH},-I${path})



# 生成目标
TARGET := ${PWD_PATH}/demo
# 生成目标中的详细符号信息文件
DEP_FILE := -Wl,-Map=${TMP_PATH}/$(notdir ${TARGET}).dep

.PHONY: all clean
all: ${TARGET}



${TARGET}:${OBJ}
	@echo "[Linking $@]"
	@echo " ${OBJ}  $< "
	${QUIET}${CC} ${OBJ} ${CFLAGS}  -o  $@ ${DEP_FILE} >/dev/null

${TMP_PATH}/%.o:${PROJ_ROOT}/src/%.cpp
	@echo "[Compiling $@]"
	@mkdir $(dir $@) -p
	${QUIET}${CC} -c $< -o $@ ${CFLAGS}  ${INC} -MMD -MF $(patsubst %.o,%.dep,$@) -MT '$@'

clean:
	@echo "[cleaning ${TARGET}]"
	${QUIET}rm -rf ${TARGET}
	${QUIET}rm -rf ${TMP_PATH}