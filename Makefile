# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++11 -pthread -g -O0
LDFLAGS := -pthread       
LDLIBS := -lssl -lcrypto  # Для работы класса httplib::SSLClient

# Цели
TARGET := main.exe

# Директория для сборки
BUILD_DIR := build

# Исходники и объектные файлы
SRC_DIRS := . command_line_read file_rw url_file_loader process_worker_pool logger thread_worker_pool
SRCS := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Создание структуры директорий для сборки
$(shell mkdir -p $(addprefix $(BUILD_DIR)/,$(SRC_DIRS)))

# Сборка
all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Удаление артефактов сборки
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean