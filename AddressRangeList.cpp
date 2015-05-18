#include "AddressRangeList.h"

#include <stdlib.h>
#include <string.h>

#include <fstream>

#include "Exceptions.h"

AddressRangeList::AddressRangeList() {

}

AddressRangeList::~AddressRangeList() {

}

AddressRangeList *AddressRangeList::createFromFile(const char *file) {
	std::fstream stream(file, std::ios::in);
	AddressRangeList *result = new AddressRangeList();

	if(!stream.is_open()) {
		throw FileNotFoundException();
	}

	Address start, end;
	char buffer[1024];
	char *bufferPtr;
	while(!stream.eof()) {
		stream.getline(buffer, 1024);
		if(strlen(buffer) == 0) {
			continue;
		}

		bufferPtr = strtok(buffer, " \t\n-");
		if(bufferPtr == NULL) {
			throw InvalidAddressRangeFile();
		}
		start = strtol(bufferPtr, 0, 16);

		bufferPtr = strtok(NULL, " \t\n-");
		if(bufferPtr == NULL) {
			throw InvalidAddressRangeFile();
		}
		end = strtol(bufferPtr, 0, 16);

		result->rangeList_.push_back(new std::pair<Address, Address>(start, end));
	}

	stream.close();

	return result;
}

void AddressRangeList::addRange(Address start, Address end) {
	if(start > end) {
		throw InvalidRangeIncrement();
	}

	rangeList_.push_back(new std::pair<Address, Address>(start, end));
}

bool AddressRangeList::isInRange(Address address) {
	for(size_t i = 0; i < rangeList_.size(); i++) {
		if(address >= rangeList_[i]->first && address <= rangeList_[i]->second) {
			return true;
		}
	}

	return false;
}

Address AddressRangeList::startAddress() {
	if(rangeList_.size() == 0) {
		throw NoRangeAvailable();
	}

	Address smallestAddress = rangeList_[0]->first;
	for(size_t i = 0; i < rangeList_.size(); i++) {
		if(smallestAddress > rangeList_[i]->first) {
			smallestAddress = rangeList_[i]->first;
		}
	}
	return smallestAddress;
}

bool AddressRangeList::nextAddress(Address *address) {
	Address nextAddress = *address;
	
	if(rangeList_.size() == 0) {
		throw NoRangeAvailable();
	}

	nextAddress++;
	if(isInRange(nextAddress)) {
		*address = nextAddress;
		return true;
	}

	if(nextAddress > endAddress()) {
		return false;
	}

	Address firstLargerAddress = endAddress();
	for(size_t i = 0; i < rangeList_.size(); i++) {
		if(nextAddress < rangeList_[i]->first && firstLargerAddress > rangeList_[i]->first) {
			firstLargerAddress = rangeList_[i]->first;
		}
	}

	*address = firstLargerAddress;
	return true;
}

Address AddressRangeList::endAddress() {
	if(rangeList_.size() == 0) {
		throw NoRangeAvailable();
	}

	Address largestAddress = rangeList_[0]->second;
	for(size_t i = 0; i < rangeList_.size(); i++) {
		if(largestAddress < rangeList_[i]->second) {
			largestAddress = rangeList_[i]->second;
		}
	}
	return largestAddress;
}