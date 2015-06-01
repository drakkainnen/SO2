#include "Zombie.h"
#include "gtest/gtest.h"

TEST(Zombie, getPosition) {
	Zombie z;
	z.setPosition(1, 2);
	auto pos = z.getPosition();

	EXPECT_EQ(pos.first, 1);
	EXPECT_EQ(pos.second, 2);
}

