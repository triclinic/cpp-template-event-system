
#include "stack.h"
#include <vector>

TEST(Stack, Empty) {
    // Arrange
    StackBuffer<int, 20u> buffer;

    // Act (empty for this test)

    // Assert
    ASSERT_EQ(buffer.size(), 0);
    ASSERT_FALSE(buffer.isNotEmpty());
}

TEST(Stack, PushAbility) {
    // Arrange
    const size_t count = 10;
    StackBuffer<int, 20u> buffer;

    // Act
    for (size_t i = 0; i < count; ++i) {
        buffer.push(i);
    }

    // Assert
    ASSERT_EQ(buffer.size(), count);
    ASSERT_TRUE(buffer.isNotEmpty());
}

TEST(Stack, PopAbility) {
    // Arrange
    const size_t count = 10;
    StackBuffer<int, 20u> buffer;

    for (size_t i = 0; i < count; ++i) {
        buffer.push(i);
    }

    // Act
    for (size_t i = 0; i < count; ++i) {
        (void)buffer.pop<int>();
    }

    // Assert
    ASSERT_EQ(buffer.size(), 0);
    ASSERT_FALSE(buffer.isNotEmpty());
}

TEST(Stack, MainStackAxiom) {
    // Arrange
    const size_t count = 10;
    StackBuffer<int, 20u> buffer;
    std::vector<int> original, received;

    for (size_t i = 0; i < count; ++i) {
        buffer.push(i);
        original.push_back(count - i - 1);
    }

    // Act
    for (size_t i = 0; i < count; ++i) {
        received.push_back(buffer.pop<int>());
    }

    // Assert
    ASSERT_EQ(original, received);
}

TEST(Stack, MoveSemantics) {
    // Arrange
    StackBuffer<semantics_t, 20u> buffer;
    semantics_t received;

    buffer.push(semantics_t(42));

    // Act
    received = buffer.pop<int>();

    // Assert
    EXPECT_TRUE(received.isMoved());
    EXPECT_FALSE(received.isCopied());
}

TEST(Stack, Overflow) {
    // Arrange
    const size_t size = 10;

    const size_t count = size + 12;
    StackBuffer<int, size> buffer;

    std::vector<int> original, received;

    // Overflow generic case
    for (size_t i = 0; i < size/2 ; ++i) {
        buffer.push(42);
    }
    for (size_t i = 0; i < size/2; ++i) {
        (void)buffer.pop<int>();
    }

    for (size_t i = 0; i < count; ++i) {
        buffer.push(i);
        if (i < size)
        {
            original.push_back(size - i - 1);
        }else
        {
            original.push_back(0);
        }

    }

    // Act
    for (size_t i = 0; i < count; ++i) {
        received.push_back(buffer.pop<int>());
    }

    // Assert
    ASSERT_EQ(original, received);
    ASSERT_EQ(buffer.size(), 0);
    ASSERT_FALSE(buffer.isNotEmpty());
}

TEST(Stack, IndexAccess) {
    // Arrange
    const size_t size = 20;

    const size_t count = 10;
    StackBuffer<int, size> buffer;

    std::vector<int> original, received;

    // Index access with border crossing
    for (size_t i = 0; i < size - count/2 ; ++i) {
        buffer.push(42);
    }
    for (size_t i = 0; i < size - count/2; ++i) {
        (void)buffer.pop<int>();
    }

    for (size_t i = 0; i < count; ++i) {
        buffer.push(i);
        original.push_back(i);
    }

    // Act
    for (size_t i = 0; i < count; ++i) {
        received.push_back(buffer[i]);
    }

    // Assert
    EXPECT_EQ(original, received);
    EXPECT_EQ(buffer.size(), count);
    EXPECT_TRUE(buffer.isNotEmpty());
}
