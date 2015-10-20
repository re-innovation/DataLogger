#ifndef _DL_TEST_MOCK_BATTERY_H_
#define _DL_TEST_MOCK_BATTERY_H_

class MockLBattery
{
	public:
		int level() { return 100;}
		bool isCharging() { return true;}
};

extern MockLBattery LBattery;

#endif