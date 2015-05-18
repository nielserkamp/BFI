#ifndef ADDRESS_RANGES_LIST_H
#define ADDRESS_RANGES_LIST_H

#include <vector>

#include "Address.h"

class AddressRangeList {
public:
	AddressRangeList();
	~AddressRangeList();

	static AddressRangeList *createFromFile(const char *file);

	void addRange(Address start, Address end);
	bool isInRange(Address address);

	Address startAddress();
	bool nextAddress(Address *address);
	Address endAddress();
private:
	std::vector<std::pair<Address, Address> *> rangeList_;
};

#endif