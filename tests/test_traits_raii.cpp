#include <memory>
#include <type_traits>

#include <gtest/gtest.h>

#include "glfw_initialization.h"
#include "glfw_window.h"

// =============================================================================
// Compile-time type trait tests
// =============================================================================

// Test that unique_ptr with custom deleter compiles correctly
struct MockDeleter {
    void operator()(const int* pointer) const noexcept {
        delete pointer;
    }
};

using MockUniquePtr = std::unique_ptr<int, MockDeleter>;

TEST(TypeTraits, UniquePtrWithCustomDeleter) {
    // Verify unique_ptr with custom deleter is move-only
    static_assert(std::is_move_constructible_v<MockUniquePtr>, "unique_ptr should be move constructible");
    static_assert(std::is_move_assignable_v<MockUniquePtr>, "unique_ptr should be move assignable");
    static_assert(!std::is_copy_constructible_v<MockUniquePtr>, "unique_ptr should not be copy constructible");
    static_assert(!std::is_copy_assignable_v<MockUniquePtr>, "unique_ptr should not be copy assignable");

    SUCCEED();
}

TEST(TypeTraits, DeleterIsNoexcept) {
    // Verify deleter call operator is noexcept
    static_assert(noexcept(MockDeleter{}(nullptr)), "Deleter should be noexcept");

    SUCCEED();
}

TEST(TypeTraits, GlfwInitializationCannotDuplicateTerminationOwnership) {
    static_assert(!std::is_copy_constructible_v<windowing::GlfwInitialization>);
    static_assert(!std::is_copy_assignable_v<windowing::GlfwInitialization>);
    static_assert(!std::is_move_constructible_v<windowing::GlfwInitialization>);
    static_assert(!std::is_move_assignable_v<windowing::GlfwInitialization>);
    static_assert(std::is_nothrow_destructible_v<windowing::GlfwInitialization>);

    SUCCEED();
}

TEST(TypeTraits, GlfwWindowHasUniqueTransferableOwnership) {
    static_assert(std::is_move_constructible_v<windowing::UniqueGlfwWindow>);
    static_assert(std::is_move_assignable_v<windowing::UniqueGlfwWindow>);
    static_assert(!std::is_copy_constructible_v<windowing::UniqueGlfwWindow>);
    static_assert(!std::is_copy_assignable_v<windowing::UniqueGlfwWindow>);
    static_assert(noexcept(windowing::GlfwWindowDeleter{}(nullptr)));

    SUCCEED();
}

// =============================================================================
// RAII pattern tests
// =============================================================================

class RaiiCounter {
public:
    explicit RaiiCounter(int& counter) : counter_(counter) {
        ++counter_;
    }
    ~RaiiCounter() {
        --counter_;
    }

    RaiiCounter(const RaiiCounter&) = delete;
    auto operator=(const RaiiCounter&) -> RaiiCounter& = delete;

private:
    int& counter_;
};

TEST(RAII, ConstructorIncrementsCounter) {
    int counter = 0;
    {
        RaiiCounter raii(counter);
        EXPECT_EQ(counter, 1);
    }
    EXPECT_EQ(counter, 0);
}

TEST(RAII, DestructorDecrementsCounter) {
    int counter = 0;
    auto* raii = new RaiiCounter(counter);
    EXPECT_EQ(counter, 1);
    delete raii;
    EXPECT_EQ(counter, 0);
}

TEST(RAII, ExceptionSafetyOnConstruction) {
    int counter = 0;
    EXPECT_THROW(
        {
            RaiiCounter raii(counter);
            EXPECT_EQ(counter, 1);
            throw std::runtime_error("Test exception");
        },
        std::runtime_error);
    EXPECT_EQ(counter, 0);
}

// =============================================================================
// Unique pointer deleter tests
// =============================================================================

struct TrackingDeleter {
    int* deleteCount;

    void operator()(const int* pointer) const noexcept {
        if (pointer != nullptr) {
            ++(*deleteCount);
            delete pointer;
        }
    }
};

constexpr int TRACKED_VALUE = 42;

TEST(UniquePtr, CustomDeleterIsCalled) {
    int deleteCount = 0;
    {
        std::unique_ptr<int, TrackingDeleter> pointer(new int(TRACKED_VALUE), TrackingDeleter{&deleteCount});
        EXPECT_EQ(deleteCount, 0);
    }
    EXPECT_EQ(deleteCount, 1);
}

TEST(UniquePtr, DeleterHandlesNull) {
    int deleteCount = 0;
    { std::unique_ptr<int, TrackingDeleter> pointer(nullptr, TrackingDeleter{&deleteCount}); }
    EXPECT_EQ(deleteCount, 0);
}

TEST(UniquePtr, ReleaseDoesNotCallDeleter) {
    int deleteCount = 0;
    int* rawPointer = nullptr;
    {
        std::unique_ptr<int, TrackingDeleter> pointer(new int(TRACKED_VALUE), TrackingDeleter{&deleteCount});
        rawPointer = pointer.release();
    }
    EXPECT_EQ(deleteCount, 0);
    delete rawPointer;
}

TEST(UniquePtr, ResetCallsDeleter) {
    int deleteCount = 0;
    std::unique_ptr<int, TrackingDeleter> pointer(new int(TRACKED_VALUE), TrackingDeleter{&deleteCount});
    EXPECT_EQ(deleteCount, 0);
    pointer.reset();
    EXPECT_EQ(deleteCount, 1);
}

// =============================================================================
// NOTE: Integration tests for GLFW functionality require a display context
// and should be run separately from unit tests. See tests/integration/ for
// tests that require actual GLFW/Vulkan initialization.
// =============================================================================
