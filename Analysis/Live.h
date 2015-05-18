#ifndef ANALYSIS_LIVE_H
#define ANALYSIS_LIVE_H

#include <memory>
#include <vector>
#include <map>

#include "Address.h"
#include "Location/Location.h"

class Live {
public:
	Live();
	Live(Address address);
	~Live();
	
	void setAddress(Address address);
	void addLocation(LocationPtr &location);
	bool containsLocation(LocationPtr &location);
	void removeLocation(const LocationPtr &location);
	void removeAllLocations();
	
	Address getAddress();
	std::vector<LocationPtr> getLocations();
	
private:
	Address address_;
	std::vector<LocationPtr> locations_;
};

typedef std::shared_ptr<Live> LivePtr;

#endif