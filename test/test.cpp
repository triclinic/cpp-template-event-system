#include "test_aux.h"

#include <gtest/gtest.h>

#include "test_circular.h"
#include "test_stack.h"

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
