#include <limits.h>
#include "gtest/gtest.h"

#include "../camera_manager/CameraManager.hpp"

class CameraManager_gtest : public ::testing::Test {
protected:
	virtual void SetUp() {

	}

	virtual void TearDown() {

	}
};

TEST_F( CameraManager_gtest, doResize ) {
	const int x = 11;
	const int y = 3;
	CameraManager myCameraManager( x, y, x, y );
	EXPECT_EQ( x, myCameraManager.PlayerX_screen );
//test screen-wall left

//test screen-wall right

//test etc
}
