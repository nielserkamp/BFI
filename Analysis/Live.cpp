#include "Live.h"


Live::Live() {
	this->address_ = 0;
}

Live::Live(uint32_t address) {
	this->address_ = address;
}

Live::~Live() {
	
}

std::vector<LocationPtr> Live::getLocations() {
	return locations_;
}

void Live::setAddress(uint32_t address) {
	this->address_ = address;
}

void Live::addLocation(LocationPtr &location) {
	this->locations_.push_back(location);
}

bool Live::containsLocation(LocationPtr &location) {
	for(size_t i = 0; i < this->locations_.size(); i++) {
		if(location->compare(this->locations_[i].get()) == 0) {
			return true;
		}
	}
	
	return false;
}

void Live::removeAllLocations() {
	while(this->locations_.begin() != this->locations_.end()) {
		this->locations_.erase(this->locations_.begin());
	}
}

void Live::removeLocation(const LocationPtr &location) {
	for(int i = this->locations_.size() - 1; i >= 0; i--) {
		if(location->compare(this->locations_[i].get()) == 0) {
			this->locations_.erase(this->locations_.begin() + i);
			return;
		}
	}
}

uint32_t Live::getAddress() {
	return this->address_;
}

