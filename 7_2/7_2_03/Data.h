#pragma once
#include <mutex>

class Data {
private:	
	int val=0;
public:
	Data() {}
	
	Data(int value):val(value) {}

	std::mutex mtx;

	int get_data() {
		return this->val;
	}

	void set_data(int value) {
		this->val = value;
	}
};