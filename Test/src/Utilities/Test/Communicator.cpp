#include "gtest/gtest.h"

#include "TBTK/Test/Communicator.h"

int main(int argc, char **argv){
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
