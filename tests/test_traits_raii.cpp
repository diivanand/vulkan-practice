#include <gtest/gtest.h>

#include <memory>
#include <type_traits>

// =============================================================================
// Compile-time type trait tests
// =============================================================================

// Test that unique_ptr with custom deleter compiles correctly
struct MockDeleter {
    void operator()(int* p) const noexcept {
        delete p;
    }
};

using MockUniquePtr = std::unique_ptr<int, MockDeleter>;

TEST(TypeTraits, UniquePtrWithCustomDeleter) {
    // Verify unique_ptr with custom deleter is move-only
    static_assert(std::is_move_constructible_v<MockUniquePtr>,
                  "unique_ptr should be move constructible");
    static_assert(std::is_move_assignable_v<MockUniquePtr>,
                  "unique_ptr should be move assignable");
    static_assert(!std::is_copy_constructible_v<MockUniquePtr>,
                  "unique_ptr should not be copy constructible");
    static_assert(!std::is_copy_assignable_v<MockUniquePtr>,
                  "unique_ptr should not be copy assignable");

    SUCCEED();
}

TEST(TypeTraits, DeleterIsNoexcept) {
    // Verify deleter call operator is noexcept
    static_assert(noexcept(MockDeleter{}(nullptr)),
                  "Deleter should be noexcept");

    SUCCEED();
}

// =============================================================================
// RAII pattern tests
// =============================================================================

class RAIICounter {
public:
    explicit RAIICounter(int& counter) : counter_(counter) {
        ++counter_;
    }
    ~RAIICounter() {
        --counter_;
    }

    RAIICounter(const RAIICounter&) = delete;
    RAIICounter& operator=(const RAIICounter&) = delete;

private:
    int& counter_;
};

TEST(RAII, ConstructorIncrementsCounter) {
    int counter = 0;
    {
        RAIICounter raii(counter);
        EXPECT_EQ(counter, 1);
    }
    EXPECT_EQ(counter, 0);
}

TEST(RAII, DestructorDecrementsCounter) {
    int counter = 0;
    auto* raii = new RAIICounter(counter);
    EXPECT_EQ(counter, 1);
    delete raii;
    EXPECT_EQ(counter, 0);
}

TEST(RAII, ExceptionSafetyOnConstruction) {
    int counter = 0;
    try {
        RAIICounter raii(counter);
        EXPECT_EQ(counter, 1);
        throw std::runtime_error("Test exception");
    } catch (const std::runtime_error&) {
        // Counter should be decremented when raii goes out of scope
    }
    EXPECT_EQ(counter, 0);
}

// =============================================================================
// Unique pointer deleter tests
// =============================================================================

struct TrackingDeleter {
    int* delete_count;

    void operator()(int* p) const noexcept {
        if (p) {
            ++(*delete_count);
            delete p;
        }
    }
};

TEST(UniquePtr, CustomDeleterIsCalled) {
    int delete_count = 0;
    {
        std::unique_ptr<int, TrackingDeleter> ptr(new int(42), TrackingDeleter{&delete_count});
        EXPECT_EQ(delete_count, 0);
    }
    EXPECT_EQ(delete_count, 1);
}

TEST(UniquePtr, DeleterHandlesNull) {
    int delete_count = 0;
    {
        std::unique_ptr<int, TrackingDeleter> ptr(nullptr, TrackingDeleter{&delete_count});
    }
    EXPECT_EQ(delete_count, 0);
}

TEST(UniquePtr, ReleaseDoesNotCallDeleter) {
    int delete_count = 0;
    int* raw = nullptr;
    {
        std::unique_ptr<int, TrackingDeleter> ptr(new int(42), TrackingDeleter{&delete_count});
        raw = ptr.release();
    }
    EXPECT_EQ(delete_count, 0);
    delete raw;
}

TEST(UniquePtr, ResetCallsDeleter) {
    int delete_count = 0;
    std::unique_ptr<int, TrackingDeleter> ptr(new int(42), TrackingDeleter{&delete_count});
    EXPECT_EQ(delete_count, 0);
    ptr.reset();
    EXPECT_EQ(delete_count, 1);
}

// =============================================================================
// NOTE: Integration tests for GLFW functionality require a display context
// and should be run separately from unit tests. See tests/integration/ for
// tests that require actual GLFW/Vulkan initialization.
// =============================================================================
