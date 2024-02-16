#include "Building.h"
#include "Factory.h"
#include <vector>

Factory::Factory(std::vector<int> outputResourceIDs)
	: outputResourceIDs(outputResourceIDs) {}

bool Factory::linkBuildings(Building outputBuilding, Building inputBuilding) {
	if (outputBuilding.outputsAvailable() > 0 && inputBuilding.inputsAvailable() > 0) {
		outputBuilding.linkToEmptyOutput(inputBuilding);
		inputBuilding.linkToEmptyInput(outputBuilding);
	}
}