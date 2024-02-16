// Factory class to represent factory contrustion by genetic algorithm
#include "Building.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <vector>
#include <stdexcept>

int Building::nextBuildingID = 0;

Building::Building(int minInputs, int maxInputs, 
				   int minOutputs, int maxOutputs)
	: minInputs(minInputs), 
	  maxInputs(maxInputs), 
	  minOutputs(minOutputs), 
	  maxOutputs(maxOutputs), 
	  buildingID(nextBuildingID++) {}

int Building::inputsAvailable() const {
	int count = 0;
	for (int i = 0; i < maxInputs; i++) {
		if (inputs[i].connection == NULL) {
			count++;
		}
	}
	return count;
};

int Building::outputsAvailable() const {
	int count = 0;
	for (int i = 0; i < maxOutputs; i++) {
		if (inputs[i].connection == NULL) {
			count++;
		}
	}
	return count;
};

void Building::linkToEmptyInput(Building building) {
	if (inputsAvailable() == 0) {
		throw std::out_of_range("No inputs are available.");
	}

	for (int i = 0; i < maxInputs; i++) {
		if (inputs[i].connection == NULL) {
			inputs[i].connection = &building;
			inputs[i].resourceID = building.getOutputResourceID();
			inputs[i].rate = building.getOutputRate();
			break;
		}
	}
};

void Building::linkToEmptyOutput(Building building) {
	if (outputsAvailable() == 0) {
		throw std::out_of_range("No outputs are available.");
	}

	for (int i = 0; i < maxInputs; i++) {
		if (outputs[i].connection == NULL) {
			outputs[i].connection = &building;
			outputs[i].resourceID = getOutputResourceID();
			outputs[i].rate = getOutputRate();
			break;
		}
	}
};

std::vector<Building*> Building::getLinkedInputs() const {
	std::vector<Building*> linkedInputs;
	for (int i = 0; i < maxInputs; i++) {
		linkedInputs.push_back(inputs[i].connection);
	}

	return linkedInputs;
}

std::vector<Building*> Building::getLinkedOutputs() const {
	std::vector<Building*> linkedOutputs;
	for (int i = 0; i < maxOutputs; i++) {
		linkedOutputs.push_back(outputs[i].connection);
	}

	return linkedOutputs;
}

int Building::getOutputResourceID() const {
	return outputResourceID;
}

float Building::getOutputRate() const {
	return outputRate;
};

// A valid configuration refers to a building that:
// Has no connections (completely unused) or
// Has more used inputs, outputs than the minimum inputs, outputs
bool Building::hasValidConfiguration() const {
	int usedOutputs = maxOutputs - outputsAvailable();
	int usedInputs = maxInputs - inputsAvailable();

	bool disconnected = usedInputs == 0 && usedOutputs == 0;
	if (disconnected) {
		return true;
	}

	bool validNumInputs = usedInputs >= minInputs;
	bool validNumOutputs = usedOutputs >= minOutputs;
	if (validNumInputs && validNumOutputs) {
		return true;
	}
	
	return false;
}

int Building::getBuildingID() const {
	return buildingID;
}

ProductionBuilding::ProductionBuilding(int minInputs, int maxInputs, 
										int minOutputs, int maxOutputs, 
										int overclockPercentage, int recipeID)
: Building(minInputs, maxInputs, minOutputs, maxOutputs),
	overclockPercentage(overclockPercentage) {}

void ProductionBuilding::setRecipeID(int recipeID) {
	this -> recipeID = recipeID;
};

int ProductionBuilding::getRecipeID() const {
	return recipeID;
}

void ProductionBuilding::setOverclockPercentage(int percentage) {
	if (percentage < 0) {
		throw std::out_of_range("Overclock percentage must be >= 0.");
	}
	if (percentage > config::maxOverclockPercentage) {
		throw std::invalid_argument("Overclock percentage must be <= MAX_OVERCLOCK_PERCENTAGE");
	}
	overclockPercentage = percentage;
}

int ProductionBuilding::getOverclockPercentage() const {
	return overclockPercentage;
}

Constructor::Constructor(int overclockPercentage, int recipeID) 
	: ProductionBuilding(1, 1, 0, 1, overclockPercentage, recipeID) {}

Assembler::Assembler(int overclockPercentage, int recipeID)
	: ProductionBuilding(2, 2, 0, 1, overclockPercentage, recipeID) {}

Manufacturer::Manufacturer(int overclockPercentage, int recipeID)
	: ProductionBuilding(3, 4, 0, 1, overclockPercentage, recipeID) {}

LogisticsBuilding::LogisticsBuilding(int minInputs, int maxInputs, 
									 int minOutputs, int maxOutputs)
	: Building(minInputs, maxInputs, minOutputs, maxOutputs) {}

StorageContainer::StorageContainer()
	: LogisticsBuilding(0, 0, 1, 1) {}

Splitter::Splitter()
	: LogisticsBuilding(1, 1, 1, 3) {}