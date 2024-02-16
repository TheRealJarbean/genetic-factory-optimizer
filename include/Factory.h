#ifndef FACTORY_H
#define FACTORY_H

#include "Building.h"

#include <vector>

class Factory {
private:
	std::vector<int> outputResourceIDs;
	std::vector<Building> rootBuildings;

public:
	Factory(std::vector<int> outputResourceIDs);

	// Returns all buildings with any open input connection
	std::vector<Building*> getAllOpenBuildings();
	bool linkBuildings(Building outputBuildingID, Building inputBuildingID);
	void addBuilding(Building newBuilding);
	void removeBuilding (int buildingID);
};

#endif