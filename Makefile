# компилятор и флаги
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude
COV      := --coverage
GTEST    := -lgtest -lgtest_main -pthread
BENCH    := -lbenchmark -lpthread

# директории
SRC_DIR    := src
INC_DIR    := include
TEST_DIR   := tests
BENCH_DIR  := benchmarks
BUILD_DIR  := build
GO_DIR     := go
REPORT_DIR := coverage_report

# файлы
# все .cpp из src
SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
# библиотечные .cpp — всё кроме main.cpp (для линковки с тестами)
LIB_SRCS := $(filter-out $(SRC_DIR)/main.cpp,$(SRCS))

LIB_OBJS     := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(LIB_SRCS))
LIB_COV_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/cov/%.o,$(LIB_SRCS))
TEST_OBJS    := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/tests/%.o,$(wildcard $(TEST_DIR)/*.cpp))
BENCH_OBJS   := $(patsubst $(BENCH_DIR)/%.cpp,$(BUILD_DIR)/bench/%.o,$(wildcard $(BENCH_DIR)/*.cpp))

APP       := app
TEST_BIN  := run_tests
BENCH_BIN := run_benchmarks

ALL_OBJS := $(LIB_OBJS) $(LIB_COV_OBJS) $(TEST_OBJS) $(BENCH_OBJS) $(BUILD_DIR)/main.o
DEPS := $(ALL_OBJS:.o=.d)

# все файлы кода для линтера
ALL_CODE := $(wildcard $(INC_DIR)/*.h $(SRC_DIR)/*.tpp $(SRC_DIR)/*.cpp $(TEST_DIR)/*.cpp $(BENCH_DIR)/*.cpp)

.PHONY: all test coverage bench lint format go-test go-coverage clean

# сборка приложения
all: $(APP)

$(APP): $(BUILD_DIR)/main.o $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -O2 -MMD -MP -c $< -o $@

# тесты
test: $(TEST_BIN)
	find $(BUILD_DIR) -name '*.gcda' -delete
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) $(LIB_COV_OBJS)
	$(CXX) $(CXXFLAGS) $(COV) $^ $(GTEST) -o $@

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(COV) -O0 -g -MMD -MP -c $< -o $@

$(BUILD_DIR)/cov/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(COV) -O0 -g -MMD -MP -c $< -o $@

# HTML-отчёт о покрытии
coverage: test
	mkdir -p $(REPORT_DIR)
	lcov --capture --directory $(BUILD_DIR) --output-file $(REPORT_DIR)/coverage.info --ignore-errors mismatch,unused,empty
	lcov --remove $(REPORT_DIR)/coverage.info '/usr/*' '*/tests/*' --output-file $(REPORT_DIR)/coverage.info --ignore-errors mismatch,unused,empty
	genhtml $(REPORT_DIR)/coverage.info --output-directory $(REPORT_DIR)/html --ignore-errors empty
	@echo ">>> Отчёт готов: $(REPORT_DIR)/html/index.html"

# бенчмарки
bench: $(BENCH_BIN)
	./$(BENCH_BIN) --benchmark_out=bench_report.json --benchmark_out_format=json

$(BENCH_BIN): $(BENCH_OBJS) $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -O3 $^ $(BENCH) -o $@

$(BUILD_DIR)/bench/%.o: $(BENCH_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -O3 -MMD -MP -c $< -o $@

# линтеры
lint:
	clang-format --dry-run --Werror $(ALL_CODE)
	cppcheck --enable=warning,performance,portability --std=c++17 -I$(INC_DIR) $(SRC_DIR) $(INC_DIR)

format:
	clang-format -i $(ALL_CODE)

# go
go-test:
	cd $(GO_DIR) && go test ./... -coverprofile=coverage.out

go-coverage: go-test
	cd $(GO_DIR) && go tool cover -html=coverage.out -o coverage.html
	
go-bench:
	cd $(GO_DIR) && go test -run=^$$ -bench=. -benchmem ./...

go-lint:
	cd $(GO_DIR) && test -z "$$(gofmt -l .)" && go vet ./...

# очистка
clean:
	rm -rf $(BUILD_DIR) $(APP) $(TEST_BIN) $(BENCH_BIN) $(REPORT_DIR)

-include $(DEPS)