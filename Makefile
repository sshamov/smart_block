CXX := clang++

CXXFLAGS_debug := -g -std=c++17 -Wall -fPIC -MMD -MP
CXXFLAGS_release := -O2 -std=c++17 -Wall -fPIC -MMD -MP

ifneq ($(MODE), release)
MODE := debug
endif

BUILD_DIR := _build.$(MODE)

TEST_BIN := $(BUILD_DIR)/test
TEST_SRC := $(wildcard test/*.cpp)
TEST_SRC += $(wildcard test/std/*.cpp)
TEST_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst test/%.cpp, test.o/%.o, $(TEST_SRC)))
TEST_LIB := -lgtest -lgtest_main -lpthread
TEST_DEF := -DTEST_STRUCTURED_BINDING

PROFILE_BIN := $(BUILD_DIR)/profile
PROFILE_SRC := $(wildcard profile/*.cpp)
PROFILE_OBJ := $(addprefix $(BUILD_DIR)/, $(patsubst profile/%.cpp, profile.o/%.o, $(PROFILE_SRC)))

.PHONY: all clean test profile

all: test profile

clean:
	rm -rf $(BUILD_DIR)

test: $(TEST_BIN)

profile: $(PROFILE_BIN)

$(BUILD_DIR) \
$(BUILD_DIR)/test.o \
$(BUILD_DIR)/test.o/std \
$(BUILD_DIR)/profile.o:
	mkdir -p $@

$(TEST_BIN): $(TEST_OBJ) | $(BUILD_DIR)
	$(CXX) -o $(TEST_BIN) $(TEST_OBJ) $(TEST_LIB)

$(TEST_OBJ): Makefile | $(BUILD_DIR) $(BUILD_DIR)/test.o $(BUILD_DIR)/test.o/std
$(TEST_OBJ): $(BUILD_DIR)/test.o/%.o: test/%.cpp
	$(CXX) -c $(CXXFLAGS_$(MODE)) $(TEST_DEF) -o $@ $<

-include $(TEST_OBJ:.o=.d)

$(PROFILE_BIN): $(PROFILE_OBJ) | $(BUILD_DIR)
	$(CXX) -o $(PROFILE_BIN) $(PROFILE_OBJ)

$(BUILD_DIR)/profile.o/profile_impl_noopt.o: CXXFLAGS_release := $(subst -O2,-O0,$(CXXFLAGS_release))

$(PROFILE_OBJ): Makefile | $(BUILD_DIR) $(BUILD_DIR)/profile.o
$(PROFILE_OBJ): $(BUILD_DIR)/profile.o/%.o: profile/%.cpp
	$(CXX) -c $(CXXFLAGS_$(MODE)) -o $@ $<

-include $(PROFILE_OBJ:.o=.d)
