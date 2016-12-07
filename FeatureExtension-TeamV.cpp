#include "Feature.h" //NEED TO CHANGE BACK!!!!
#include <iostream>
#include<algorithm>
#include <cmath>

//Feature
Feature::Feature() {} //Abstract Feature Class
bool Feature::isVisited() {  //Returns if a feature has been visted
	return visited;
}
void Feature::setVisited(bool v) { //Sets  the bool value to visited
	visited = v;
}
vector<Feature*> Feature::getConnectedFeatures() {
	return connectedFeatures;
}
void Feature::addTiger(int player) {
	switch (player) {
	case 1: hasP1tiger = true; break;
	case 2: hasP2tiger = true; break;
	}
}
void Feature::merge(Feature* const &f) {
	connectedFeatures.push_back(f);
	f->connectedFeatures.push_back(this);
	mergeFeature(f);
}
void Feature::addCrocodile() {
	hasCrocodile = true;
}
bool Feature::hasP1Tiger() {
	return hasP1tiger;
}
bool Feature::hasP2Tiger() {
	return hasP2tiger;
}


//Den
Den::Den() {
	visited = false;
	hasP1tiger = false;
	hasP2tiger = false;
	hasCrocodile = false;
}
char Den::getType() {
	return type;
}
void Den::checkIfScored(int &numberOfP1Tigers, int &numberOfP2Tigers) {}
void Den::mergeFeature(Feature* const &f) {}
bool Den::canPlaceTiger() {
	return true;
}
bool Den::canPlaceCrocodile() {
	return false;
}
int Den::getPotentialPoints() {
	return 4;
}


//Lake
Lake::Lake(bool isWall, int nWalls) {

	visited = false;
	hasP1tiger = false;
	hasP2tiger = false;
	hasCrocodile = false;

	isComplete = false;
	potentialPoints = 0;
	hasBeenCheckedForCompleteness = false;
	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;
	wall = isWall;
	numberOfWalls = nWalls;

};
Lake::Lake(bool isWall, int nWalls, char animal) {
	wall = isWall;
	numberOfWalls = nWalls;
	visited = false;


	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;
	hasCrocodile = false;
	switch (animal) {
	case 'P': hasBoar = true; break;
	case 'B': hasBuffalo = true; break;
	case 'D': hasDeer = true; break;
	case 'C': hasCrocodile = true; break;
	}
}
char Lake::getType() {
	return type;
};
void Lake::mergeFeature(Feature* const &f) {
	Lake * c = dynamic_cast<Lake *> (f);
	lakeConnected.push_back(c);
	c->lakeConnected.push_back(this);
};
//PLACE LAKE METHODS FROM SCORE DOWNWARD
void Lake::traverseLakeForScore(Lake * previousLake, Lake *startingLake, bool &finishedChecking, int &lakeSize, int &nWalls, int&nLakes, bool &hasDeer, bool &hasBuffalo, bool &hasBoar, int &numberOfCrocodiles, int &player1, int &player2) {
	if (this->wall == true) {
		lakeSize++;
		nWalls += 1;
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		hasDeer |= this->hasDeer;
		hasBuffalo |= this->hasBuffalo;
		hasBoar |= this->hasBoar;
		numberOfCrocodiles += hasCrocodile;
		numberOfGoats += hasGoat;
		return;
	}
	if (this == startingLake) {
		finishedChecking = true;
		return;
	}
	if (startingLake->wall == true && this->wall != true) {
		startingLake = this;
	}
	lakeSize++;
	nWalls += numberOfWalls;
	nLakes++;
	player1 += hasP1tiger;
	player2 += hasP2tiger;
	hasDeer |= this->hasDeer;
	hasBuffalo |= this->hasBuffalo;
	hasBoar |= this->hasBoar;
	numberOfCrocodiles += hasCrocodile;
	numberOfGoats += hasGoat;

	for (int i = 0; i<lakeConnected.size(); i++) {
		if (lakeConnected[i] != previousLake) {
			lakeConnected[i]->traverseLakeForScore(this, startingLake, finishedChecking, lakeSize, nWalls, nLakes, hasDeer, hasBuffalo, hasBoar, numberOfCrocodiles, player1, player2);
		}
		if (finishedChecking) {
			return;
		}
	}
	return;
};
bool Lake::traverseLakeForCompleteness(Lake * previousLake, Lake *startingLake,vector<Lake *>&lakesVisited, bool &finishedChecking, int &player1, int&player2) {
	if (this->wall == true) {
		if(find(lakesVisited.begin(),lakesVisited.end(),this)==lakesVisited.end()){
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		}
		lakesVisited.push_back(this);
		return true;
	}
	if (this == startingLake) {
		return true;
	}
	if (this->wall == false && (lakeConnected.size() + numberOfWalls) != 4) {
		return false;
	}

	if (startingLake->wall == true && this->wall != true) {
		startingLake = this;
	}

	bool isComplete = true;
	if(find(lakesVisited.begin(),lakesVisited.end(),this)==lakesVisited.end()){
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		}
		lakesVisited.push_back(this);

	for (int i = 0; i<lakeConnected.size(); i++) {
		if (lakeConnected[i] != previousLake) {
			isComplete &= lakeConnected[i]->traverseLakeForCompleteness(this, startingLake,lakesVisited,finishedChecking, player1, player2);
		}
		if (isComplete == false) {
			return isComplete;
		}
	}
	return isComplete;
};
bool Lake::traverseLakeForTigers(Lake *previousLake, Lake *startingLake) {
	if (this->wall == true) {
		if (hasP1tiger || hasP2tiger) {
			return false;
		}
		return true;
	}

	if (this == startingLake) {
		return true;
	}

	if (startingLake->wall == true && this->wall != true) {
		startingLake = this;
	}

	if (hasP1tiger || hasP2tiger) {
		return false;
	}
	bool isNotOwnedByAPlayer = true;
	for (int i = 0; i<lakeConnected.size(); i++) {
		if (lakeConnected[i] != previousLake) {
			isNotOwnedByAPlayer &= lakeConnected[i]->traverseLakeForTigers(this, startingLake);
		}
		if (isNotOwnedByAPlayer == false) {
			break;
		}
	}
	return isNotOwnedByAPlayer;
};
bool Lake::traverseLakeForCrocodiles(Lake *previousLake, Lake *startingLake) {
	if (this->wall == true) {
		if (this->hasCrocodile == true) {
			return false;
		}
		return true;
	}
	if (this == startingLake) {
		return true;
	}
	if (startingLake->wall == true && this->wall != true) {
		startingLake = this;
	}
	if (this->hasCrocodile == true) {
		return false;
	}
	bool isNotOwnedByCrocodile = true;
	for (int i = 0; i<lakeConnected.size(); i++) {
		if (lakeConnected[i] != previousLake) {
			isNotOwnedByCrocodile &= lakeConnected[i]->traverseLakeForCrocodiles(this, startingLake);
		}
		if (isNotOwnedByCrocodile == false) {
			break;
		}
	}
	return isNotOwnedByCrocodile;
};

bool Lake::traverseLakeForGoats(Lake *previousLake, Lake *startingLake) {
	if (this->wall == true) {
		if (this->hasGoat == true) {
			return false;
		}
		return true;
	}
	if (this == startingLake) {
		return true;
	}
	if (startingLake->wall == true && this->wall != true) {
		startingLake = this;
	}
	if (this->hasGoat == true) {
		return false;
	}
	bool isNotOwnedByGoat = true;
	for (int i = 0; i<lakeConnected.size(); i++) {
		if (lakeConnected[i] != previousLake) {
			isNotOwnedByGoat &= lakeConnected[i]->traverseLakeForGoats(this, startingLake);
		}
		if (isNotOwnedByGoat == false) {
			break;
		}
	}
	return isNotOwnedByGoat;
};

void Lake::checkIfScored(int &numberOfP1Tigers, int &numberOfP2Tigers) {
	if (hasBeenCheckedForCompleteness == false) {
		bool isComplete;
		numberOfP1Tigers = hasP1tiger;
		numberOfP2Tigers = hasP2tiger;
		bool finishedChecking = false;
		vector<Lake *> lakesVisited;
		lakesVisited.push_back(this);
		if (lakeConnected.empty()) {
			isComplete = false;
		}
		else {
			isComplete = true;
		}
		if (this->wall == false && (lakeConnected.size() + numberOfWalls) != 4) {
			isComplete = false;
		}
		for (int i = 0; i<lakeConnected.size(); i++) {
			isComplete &= lakeConnected[i]->traverseLakeForCompleteness(this, this,lakesVisited,finishedChecking, numberOfP1Tigers, numberOfP2Tigers);
			finishedChecking=false;
			if (isComplete == false) {
				break;
			}
		}
		if (!isComplete) {
			numberOfP1Tigers = 0;
			numberOfP2Tigers = 0;
		}
		hasBeenCheckedForCompleteness = true;
	}
};

bool Lake::canPlaceTiger() {
	bool placeTiger = (!hasP1tiger && !hasP2tiger);
	for (int i = 0; i<lakeConnected.size(); i++) {
		placeTiger &= lakeConnected[i]->traverseLakeForTigers(this, this);
		if (placeTiger == false) {
			break;
		}
	}
	return placeTiger;
};
bool Lake::canPlaceCrocodile() {
	bool placeCrocodile = !hasCrocodile;
	for (int i = 0; i<lakeConnected.size(); i++) {
		placeCrocodile &= lakeConnected[i]->traverseLakeForCrocodiles(this, this);
		if (placeCrocodile == false) {
			break;
		}
	}
	return placeCrocodile;

};

bool Lake::canPlaceGoat() {
	bool placeGoat = !hasGoat;
	for (int i = 0; i<lakeConnected.size(); i++) {
		placeGoat &= lakeConnected[i]->traverseLakeForGoats(this, this);
		if (placeGoat == false) {
			break;
		}
	}
	return placeGoat;

};

int Lake::getPotentialPoints() {
	int numberOfP1Tigers = hasP1tiger;
	int numberOfP2Tigers = hasP2tiger;
	bool hasDeer = this->hasDeer;
	bool hasBoar = this->hasBoar;
	bool hasBuffalo = this->hasBuffalo;
	int numberOfCrocodiles = this->hasCrocodile;
	bool finishedChecking = false;
	int nWalls = numberOfWalls;
	int nLakes = !wall;
	int lakeSize = 1;
	for (int i = 0; i<lakeConnected.size(); i++) {
		lakeConnected[i]->traverseLakeForScore(this, this, finishedChecking, lakeSize, nWalls, nLakes, hasDeer, hasBuffalo, hasBoar, numberOfCrocodiles, numberOfP1Tigers, numberOfP2Tigers);
	}
	int scoringFactor = 0;
	if (hasDeer + hasBuffalo + hasBoar - numberOfCrocodiles <= 0) {
		scoringFactor = 1;
	}
	else {
		scoringFactor = hasDeer + hasBuffalo + hasBoar - numberOfCrocodiles;
	}
	if (numberOfP1Tigers<numberOfP2Tigers) {
		scoringFactor *= -1;
	}
	if (nLakes == 0) {
		nLakes = 1;
	}
	if (nWalls == 0) {
		nWalls = 1;
	}
	if (scoringFactor < 0) {
		potentialPoints = lakeSize*scoringFactor;
	}
	else {
		potentialPoints = lakeSize*scoringFactor*ceil(double(nWalls) / double(nLakes));
	}
	return  potentialPoints;

};

//Jungle
Jungle::Jungle() {
	visited = false;
	hasP1tiger = false;
	hasP2tiger = false;
	hasCrocodile = false;
}
void Jungle::traverse(Jungle* const &current, int &completedLakes, int &p1tigers, int &p2tigers) {
	current->visited = true;
	if (current->hasP1tiger) {
		p1tigers += 1;
	}
	if (current->hasP2tiger) {
		p2tigers += 1;
	}
	for (int i = 0; i < current->getConnectedFeatures().size(); i++) {
		if (!current->getConnectedFeatures()[i]->isVisited()) {
			traverse(dynamic_cast<Jungle*>(current->getConnectedFeatures()[i]), completedLakes, p1tigers, p2tigers);
		}
	}
	current->visited = false;
}
char Jungle::getType() {
	return type;
};
void Jungle::mergeFeature(Feature* const &f) {
	Jungle* j = dynamic_cast<Jungle*>(f);
}
void Jungle::checkIfScored(int &numberOfP1Tigers, int &numberOfP2Tigers) {
	numberOfP1Tigers = 0;
	numberOfP2Tigers = 0;
}
bool Jungle::canPlaceTiger() {
	int p1tigers = 0;
	int p2tigers = 0;
	int completedLakes;
	traverse(this, completedLakes, p1tigers, p2tigers);
	if (p1tigers + p2tigers == 0) {
		return true;
	}
	else {
		return false;
	}
}
bool Jungle::canPlaceCrocodile() {
	return true;
}
int Jungle::getPotentialPoints() {
	return 0;
}


//GameTrail
GameTrail::GameTrail() {
	hasBeenCheckedForCompleteness = false;
	gameTrailEnd = false;
	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;

	hasP1tiger = false;
	hasP2tiger = false;
	hasCrocodile = false;
	visited = false;
};
GameTrail::GameTrail(char animal) {
	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;
	gameTrailEnd = false;
	hasBeenCheckedForCompleteness = false;
	potentialPoints = 0;

	hasCrocodile = false;
	hasP1tiger = false;
	hasP2tiger = false;
	visited = false;

	switch (animal) {
	case 'P': hasBoar = true; break;
	case 'B': hasBuffalo = true; break;
	case 'D': hasDeer = true; break;
	case 'C': hasCrocodile = true; break;
	}
}
GameTrail::GameTrail(bool end) {
	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;
	gameTrailEnd = end;
	hasBeenCheckedForCompleteness = false;
	potentialPoints = 0;

	hasCrocodile = false;
	hasP1tiger = false;
	hasP2tiger = false;
	visited = false;
}
GameTrail::GameTrail(char animal, bool end) {
	hasBoar = false;
	hasBuffalo = false;
	hasDeer = false;
	gameTrailEnd = end;
	hasBeenCheckedForCompleteness = false;
	potentialPoints = 0;

	hasCrocodile = false;
	hasP1tiger = false;
	hasP2tiger = false;
	visited = false;

	switch (animal) {
	case 'P': hasBoar = true; break;
	case 'B': hasBuffalo = true; break;
	case 'D': hasDeer = true; break;
	case 'C': hasCrocodile = true; break;
	}
}
char GameTrail::getType() {
	return type;
};
void GameTrail::mergeFeature(Feature * const &f) {
	GameTrail * g = dynamic_cast<GameTrail *> (f);
	gameTrailsConnected.push_back(g);
	g->gameTrailsConnected.push_back(this);
};
int GameTrail::traverseGameTrailForScore(GameTrail * previousGameTrail, GameTrail * startingGameTrail, int &numberOfGameTrailEnds, int &numberOfPrey, int &numberOfCrocodiles, int &player1, int &player2) {
	if (gameTrailEnd) {
		numberOfGameTrailEnds++;
		if (hasBoar || hasDeer || hasBuffalo || hasGoat) {
			numberOfPrey++;
		}
		numberOfCrocodiles += hasCrocodile;
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		return 1;
	}
	if (this == startingGameTrail) {
		return 0;
	}
	if (gameTrailsConnected.size() == 1) {
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		return 1;
	}

	player1 += hasP1tiger;
	player2 += hasP2tiger;
	if (this->hasBoar || this->hasDeer || this->hasBuffalo || this->hasGoat) {
		numberOfPrey++;
	}
	int roadSize = 0;
	numberOfCrocodiles += hasCrocodile;
	if (gameTrailsConnected[0] == previousGameTrail) {
		roadSize += gameTrailsConnected[1]->traverseGameTrailForScore(this, startingGameTrail, numberOfGameTrailEnds, numberOfPrey, numberOfCrocodiles, player1, player2);
	}
	else {
		roadSize += gameTrailsConnected[0]->traverseGameTrailForScore(this, startingGameTrail, numberOfGameTrailEnds, numberOfPrey, numberOfCrocodiles, player1, player2);
	}
	roadSize++;
	return roadSize;

};
void GameTrail::traverseGameTrailForCompleteness(GameTrail * previousGameTrail, GameTrail * startingGameTrail, int &numberOfGameTrailEnds, bool &isComplete, int &player1, int &player2) {
	if (gameTrailEnd) {
		numberOfGameTrailEnds++;
		player1 += hasP1tiger;
		player2 += hasP2tiger;
		return;
	}
	if (this == startingGameTrail) {
		isComplete = true;
		return;
	}
	if (gameTrailsConnected.size() == 1) {
		return;
	}

	player1 += hasP1tiger;
	player2 += hasP2tiger;
	if (gameTrailsConnected[0] == previousGameTrail) {
		gameTrailsConnected[1]->traverseGameTrailForCompleteness(this, startingGameTrail, numberOfGameTrailEnds, isComplete, player1, player2);
	}
	else {
		gameTrailsConnected[0]->traverseGameTrailForCompleteness(this, startingGameTrail, numberOfGameTrailEnds, isComplete, player1, player2);
	}
	return;

};
bool GameTrail::traverseGameTrailForTigers(GameTrail * previousGameTrail, GameTrail * startingGameTrail) {
	if (this == startingGameTrail) {
		return true;
	}
	if (gameTrailsConnected.size() == 1) {
		if (hasP1tiger || hasP2tiger) {
			return false;
		}
		return true;
	}

	if (hasP1tiger || hasP2tiger) {
		return false;
	}
	bool canPlaceTiger = true;
	if (gameTrailsConnected[0] == previousGameTrail) {
		canPlaceTiger &= gameTrailsConnected[1]->traverseGameTrailForTigers(this, startingGameTrail);
	}
	else {
		canPlaceTiger &= gameTrailsConnected[0]->traverseGameTrailForTigers(this, startingGameTrail);
	}
	return canPlaceTiger;

};
bool GameTrail::traverseGameTrailForCrocodile(GameTrail * previousGameTrail, GameTrail * startingGameTrail) {
	if (this == startingGameTrail) {
		return true;
	}
	if (gameTrailsConnected.size() == 1) {
		if (hasCrocodile) {
			return false;
		}
		return true;
	}

	if (hasCrocodile) {
		return false;
	}
	bool canPlaceCrocodile = true;
	if (gameTrailsConnected[0] == previousGameTrail) {
		canPlaceCrocodile &= gameTrailsConnected[1]->traverseGameTrailForCrocodile(this, startingGameTrail);
	}
	else {
		canPlaceCrocodile &= gameTrailsConnected[0]->traverseGameTrailForCrocodile(this, startingGameTrail);
	}
	return canPlaceCrocodile;

};

bool GameTrail::traverseGameTrailForGoat(GameTrail * previousGameTrail, GameTrail * startingGameTrail) {
	if (this == startingGameTrail) {
		return true;
	}
	if (gameTrailsConnected.size() == 1) {
		if (hasGoat) {
			return false;
		}
		return true;
	}

	if (hasGoat) {
		return false;
	}
	bool canPlaceGoat = true;
	if (gameTrailsConnected[0] == previousGameTrail) {
		canPlaceGoat &= gameTrailsConnected[1]->traverseGameTrailForGoat(this, startingGameTrail);
	}
	else {
		canPlaceGoat &= gameTrailsConnected[0]->traverseGameTrailForGoat(this, startingGameTrail);
	}
	return canPlaceGoat;
};

void GameTrail::checkIfScored(int &numberOfP1Tigers, int &numberOfP2Tigers) {
	if (hasBeenCheckedForCompleteness == false) {
		GameTrail * startingGameTrail = this;
		int numberOfGameTrailEnds = 0;
		numberOfP1Tigers = hasP1tiger;
		numberOfP2Tigers = hasP2tiger;
		bool isComplete = false;
		if (gameTrailEnd) {
			numberOfGameTrailEnds++;
		}
		for (int i = 0; i<gameTrailsConnected.size(); i++) {
			gameTrailsConnected[i]->traverseGameTrailForCompleteness(startingGameTrail, startingGameTrail, numberOfGameTrailEnds, isComplete, numberOfP1Tigers, numberOfP2Tigers);
			if (isComplete) {
				break;
			}
		}
		if (numberOfGameTrailEnds == 2) {
			isComplete = true;
		}
		if (!isComplete) {
			numberOfP1Tigers = 0;
			numberOfP2Tigers = 0;
		}
		hasBeenCheckedForCompleteness = true;
	}
};
bool GameTrail::canPlaceTiger() {
	bool canPlaceTiger = (!hasP1tiger && !hasP2tiger);
	GameTrail * startingGameTrail = this;
	for (int i = 0; i<gameTrailsConnected.size(); i++) {
		canPlaceTiger &= gameTrailsConnected[i]->traverseGameTrailForTigers(startingGameTrail, startingGameTrail);
		if (canPlaceTiger == false) {
			break;
		}
	}
	return canPlaceTiger;
};
bool GameTrail::canPlaceCrocodile() {
	bool placeCrocodile = !hasCrocodile;
	GameTrail * startingGameTrail = this;
	for (int i = 0; i<gameTrailsConnected.size(); i++) {
		placeCrocodile &= gameTrailsConnected[i]->traverseGameTrailForTigers(startingGameTrail, startingGameTrail);
		if (placeCrocodile == false) {
			break;
		}
	}
	return placeCrocodile;

};

bool GameTrail::canPlaceGoat() {
	bool placeGoat = !hasGoat;
	GameTrail * startingGameTrail = this;
	for (int i = 0; i<gameTrailsConnected.size(); i++) {
		placeGoat &= gameTrailsConnected[i]->traverseGameTrailForGoats(startingGameTrail, startingGameTrail);
		if (placeGoat == false) {
			break;
		}
	}
	return placeGoat;
};

int GameTrail::getPotentialPoints() {
	int numberOfP1Tigers = hasP1tiger;
	int numberOfP2Tigers = hasP2tiger;
	int numberOfGameTrailEnds = gameTrailEnd;
	int numberOfCrocodiles = hasCrocodile;
	int roadSize = 1;
	int numberOfPrey = hasDeer || hasCrocodile || hasBuffalo;
	for (int i = 0; i<gameTrailsConnected.size(); i++) {
		roadSize += gameTrailsConnected[i]->traverseGameTrailForScore(this, this, numberOfGameTrailEnds, numberOfPrey, numberOfCrocodiles, numberOfP1Tigers, numberOfP2Tigers);
	}
	int scoringFactor = 0;
	if (numberOfP1Tigers >= numberOfP2Tigers) {
		scoringFactor = 1 * (numberOfGameTrailEnds + 1);
	}
	else {
		scoringFactor = -1 * (numberOfGameTrailEnds + 1);
	}
	int numberOfPreyLeft = numberOfPrey - numberOfCrocodiles;
	if (numberOfPreyLeft<0) {
		numberOfPreyLeft = 0;
	}
	potentialPoints = scoringFactor*(roadSize + numberOfPrey - numberOfCrocodiles);
	return scoringFactor*(roadSize + numberOfPreyLeft);
};