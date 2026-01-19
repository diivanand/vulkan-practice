#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;

struct IRandom {
  virtual ~IRandom() = default;
  virtual int Next() = 0;
};

struct MockRandom : IRandom {
  MOCK_METHOD(int, Next, (), (override));
};

TEST(Sanity, Basic) {
  EXPECT_EQ(2 + 2, 4);
}

TEST(MockExample, DeterministicRandom) {
  MockRandom rng;
  EXPECT_CALL(rng, Next()).WillOnce(Return(42));
  EXPECT_EQ(rng.Next(), 42);
}