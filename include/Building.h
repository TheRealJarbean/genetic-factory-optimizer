#ifndef BUILDING_H
#define BUILDING_H

#include <string>
#include <vector>

namespace config {
	const unsigned int maxPossibleInputs = 4;		// Highest number of inputs a building can have
	const unsigned int maxPossibleOutputs = 3;		// Highest number of outputs a building can have
	const unsigned int maxOverclockPercentage = 200;
}

class Building {
private:
	struct Link {
		Building* connection = NULL;
		int resourceID;
		float rate;
	};
	static int nextBuildingID;
	const int buildingID;
	const int minInputs;
	const int maxInputs;
	const int minOutputs;
	const int maxOutputs;
	Link inputs[config::maxPossibleInputs];
	Link outputs[config::maxPossibleOutputs];

protected:
	int outputResourceID;
	float outputRate;

	Building(int minInputs, int maxInputs, int minOutputs, int maxOutputs);

public:
	virtual void linkToEmptyInput(Building building);
	virtual void linkToEmptyOutput(Building building);
	std::vector<Building*> getLinkedInputs() const;
	std::vector<Building*> getLinkedOutputs() const;
	int getBuildingID() const;
	int getOutputResourceID() const;
	int inputsAvailable() const;
	int outputsAvailable() const;
	float getOutputRate() const;
	bool hasValidConfiguration() const;
};

class ProductionBuilding : public Building {
private:
	int basePowerConsumption;
	int overclockPercentage;
	int recipeID;

protected:
	ProductionBuilding(int minInputs, int maxInputs, int minOutputs, 
				       int maxOutputs, int overclockPercentage, int recipeID);

public:
	void setRecipeID(int recipeID);
	int getRecipeID() const;
	void setOverclockPercentage(int percentage);
	int getOverclockPercentage() const;
};

class Constructor : public ProductionBuilding {
public:
	Constructor(int overclockPercentage, int recipeID);
};

class Assembler : public ProductionBuilding {
public:
	Assembler(int overclockPercentage, int recipeID);
};

class Manufacturer : public ProductionBuilding {
public:
	Manufacturer(int overclockPercentage, int recipeID);
};

class LogisticsBuilding : public Building {
protected:
	LogisticsBuilding(int minInputs, int maxInputs, int minOutputs, int maxOutputs);

public:
	void linkToEmptyInput(Building building);
	void linkToEmptyOutput(Building building);
};

class Splitter : public LogisticsBuilding {
public:
	Splitter();
};

class StorageContainer : public LogisticsBuilding {
public:
	StorageContainer();
};

#endif