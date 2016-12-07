#include "Tile.h";
#include <iostream>;
#define NA 100

Tile::Tile(string Name) {
	name = Name;
	xpos = NA;
	ypos = NA;
	if (Name == "JJJJ-") {
		N_feat = new Jungle();
		NE_feat = E_feat = SE_feat = S_feat = SW_feat = W_feat = NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "JJJJX")	 {
		N_feat = new Jungle();
		NE_feat = E_feat = SE_feat = S_feat = SW_feat = W_feat = NW_feat = N_feat;
		C_feat = new Den();
	}
	else if (Name == "JJTJX") {
		N_feat = new Jungle();
		NE_feat = E_feat = SE_feat = SW_feat = W_feat = NW_feat = N_feat;
		S_feat = new GameTrail(true);
		C_feat = new Den();
	}
	else if (Name == "TTTT-") {
		N_feat = new GameTrail(true);
		NE_feat = new Jungle();
		E_feat = new GameTrail(true);
		SE_feat = new Jungle();
		S_feat = new GameTrail(true);
		SW_feat = new Jungle();
		W_feat = new GameTrail(true);
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TJTJ-") {
		N_feat = new GameTrail();
		NE_feat = new Jungle();
		E_feat = SE_feat = NE_feat;
		S_feat = N_feat;
		SW_feat = new Jungle();
		W_feat = NW_feat = SW_feat;
		C_feat = NULL;
	}
	else if (Name == "TJJT-") {
		N_feat = new GameTrail();
		NE_feat = new Jungle();
		E_feat = SE_feat = S_feat = SW_feat = NE_feat;
		W_feat = N_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TJTT-") {
		N_feat = new GameTrail(true);
		NE_feat = new Jungle();
		E_feat = SE_feat = NE_feat;
		S_feat = new GameTrail(true);
		SW_feat = new Jungle();
		W_feat = new GameTrail(true);
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "LLLL-") {
		N_feat = new Lake(false,0);
		NE_feat = E_feat = SE_feat = S_feat = SW_feat = W_feat = NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "JLLL-") {
		N_feat = new Jungle();
		NE_feat = N_feat;
		E_feat = new Lake(false,1);
		SE_feat = S_feat = SW_feat = W_feat = E_feat;
		NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "LLJJ-") {
		N_feat = new Lake(false,2);
		NE_feat = E_feat = N_feat;
		SE_feat = new Jungle();
		S_feat = SW_feat = W_feat = NW_feat = SE_feat;
		C_feat = NULL;
	}
	else if (Name == "JLJL-") {
		N_feat = new Jungle();
		NE_feat = N_feat;
		E_feat = new Lake(false,2);
		SE_feat = new Jungle();
		S_feat = SW_feat = SE_feat;
		W_feat = E_feat;
		NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "LJLJ-") {
		N_feat = new Lake(true,3);
		NE_feat = new Jungle();
		E_feat = SE_feat = NE_feat;
		S_feat = new Lake(true,3);
		SW_feat = W_feat = NW_feat = NE_feat;
		C_feat = NULL;
	}
	else if (Name == "LJJJ-") {
		N_feat = new Lake(true,3);
		NE_feat = new Jungle();
		E_feat = SE_feat = S_feat = SW_feat = W_feat = NW_feat = NE_feat;
		C_feat = NULL;
	}
	else if (Name == "JLLJ-") {
		N_feat = new Jungle();
		NE_feat = N_feat;
		E_feat = new Lake(true,3);
		SE_feat = N_feat; 
		S_feat = new Lake(true,3);
		SW_feat = W_feat = NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "TLJT-") {
		N_feat = new GameTrail();
		NE_feat = new Jungle();
		E_feat = new Lake(true,3);
		SE_feat = S_feat = SW_feat = NE_feat;
		W_feat = N_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TLJTP") {
		N_feat = new GameTrail('P');
		NE_feat = new Jungle();
		E_feat = new Lake(true,3,'P');
		SE_feat = S_feat = SW_feat = NE_feat;
		W_feat = N_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "JLTT-") {
		N_feat = new Jungle();
		NE_feat = N_feat;
		E_feat = new Lake(true,3);
		SE_feat = N_feat;
		S_feat = new GameTrail();
		SW_feat = new Jungle();
		W_feat = S_feat;
		NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "JLTTB") {
		N_feat = new Jungle();
		NE_feat = N_feat;
		E_feat = new Lake(true,3,'B');
		SE_feat = N_feat;
		S_feat = new GameTrail('B');
		SW_feat = new Jungle();
		W_feat = S_feat;
		NW_feat = N_feat;
		C_feat = NULL;
	}
	else if (Name == "TLTJ-") {
		N_feat = new GameTrail();
		NE_feat = new Jungle();
		E_feat = new Lake(true,3);
		SE_feat = NE_feat; 
		S_feat = N_feat;
		SW_feat = new Jungle();
		W_feat = NW_feat = SW_feat;
		C_feat = NULL;
	}
	else if (Name == "TLTJD") {
		N_feat = new GameTrail('D');
		NE_feat = new Jungle();
		E_feat = new Lake(true,3,'D');
		SE_feat = NE_feat;
		S_feat = N_feat;
		SW_feat = new Jungle();
		W_feat = NW_feat = SW_feat;
		C_feat = NULL;
	}
	else if (Name == "TLLL-") {
		N_feat = new GameTrail(true);
		NE_feat = new Jungle();
		E_feat = new Lake(false,1);
		SE_feat = S_feat = SW_feat = W_feat = E_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TLTT-") {
		N_feat = new GameTrail(true);
		NE_feat = new Jungle();
		E_feat = new Lake(true,3);
		SE_feat = NE_feat;
		S_feat = new GameTrail(true);
		SW_feat = new Jungle();
		W_feat = new GameTrail(true);
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TLTTP") {
		N_feat = new GameTrail('P',true);
		NE_feat = new Jungle();
		E_feat = new Lake(true,3,'P');
		SE_feat = NE_feat;
		S_feat = new GameTrail('P',true);
		SW_feat = new Jungle();
		W_feat = new GameTrail('P',true);
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TLLT-") {
		N_feat = new GameTrail();
		NE_feat = new Jungle();
		E_feat = new Lake(false,2);
		SE_feat = S_feat = E_feat;
		SW_feat = NE_feat;
		W_feat = N_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "TLLTB") {
		N_feat = new GameTrail('B');
		NE_feat = new Jungle();
		E_feat = new Lake(false,2,'B');
		SE_feat = S_feat = E_feat;
		SW_feat = NE_feat;
		W_feat = N_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "LJTJ-") {
		N_feat = new Lake(true,3);
		NE_feat = new Jungle();
		E_feat = SE_feat = NE_feat;
		S_feat = new GameTrail(true);
		SW_feat = new Jungle();
		W_feat = NW_feat = SW_feat;
		C_feat = NULL;
	}
	else if (Name == "LJTJD") {
		N_feat = new Lake(true,3,'D');
		NE_feat = new Jungle();
		E_feat = SE_feat = NE_feat;
		S_feat = new GameTrail('D',true);
		SW_feat = new Jungle();
		W_feat = NW_feat = SW_feat;
		C_feat = NULL;
	}
	else if (Name == "TLLLC") {
		N_feat = new GameTrail('C',true);
		NE_feat = new Jungle();
		E_feat = new Lake(false,3,'C');
		SE_feat = S_feat = SW_feat = W_feat = E_feat;
		NW_feat = new Jungle();
		C_feat = NULL;
	}
	else if (Name == "LTLT-") {
		N_feat = new Lake(true,3);
		NE_feat = new Jungle();
		E_feat = new GameTrail(true);
		SE_feat = new Jungle();
		S_feat = new Lake(true,3);
		SW_feat = SE_feat;
		W_feat = E_feat;
		NW_feat = NE_feat;
		C_feat = NULL;
	}
	
	inTilePositions[0] = NW_feat;
	inTilePositions[1] = N_feat;
	inTilePositions[2] = NE_feat;
	inTilePositions[3] = W_feat;
	inTilePositions[4] = C_feat;
	inTilePositions[5] = E_feat;
	inTilePositions[6] = SW_feat;
	inTilePositions[7] = S_feat;
	inTilePositions[8] = SE_feat;


}

bool Tile::hasDen() {
	if (C_feat != NULL)
		return (C_feat->getType() == 'X');
	else
		return false;
}

void Tile::setWfeat(Feature* const &f) {
	W_feat = f;
}
  
void Tile::setNfeat(Feature* const &f) {
	N_feat = f;
}

void Tile::setEfeat(Feature* const &f) {
	E_feat = f;
}

void Tile::setSfeat(Feature* const &f) {
	S_feat = f;
}

void Tile::setPosition(int x, int y) {
	xpos = x;
	ypos = y;
}
	
string Tile::getName() {
	return name;
}

Feature* Tile::getNfeat() {
	return N_feat;
}

Feature* Tile::getNEfeat() {
	return NE_feat;
}

Feature* Tile::getEfeat() {
	return E_feat;
}

Feature* Tile::getSEfeat() {
	return SE_feat;
}

Feature* Tile::getSfeat() {
	return S_feat;
}

Feature* Tile::getSWfeat() {
	return SW_feat;
}

Feature* Tile::getWfeat() {
	return W_feat;
}

Feature* Tile::getNWfeat() {
	return NW_feat;
}

Feature* Tile::getCfeat() {
	return C_feat;
}

Feature* Tile::getFeatAt(int pos) {
	return inTilePositions[pos - 1];
}

int Tile::getX() {
	return xpos;
}

int Tile::getY() {
	return ypos;
}

void Tile::rotateCCW() {
	Feature* tmp = N_feat;
	N_feat = E_feat;
	E_feat = S_feat;
	S_feat = W_feat;
	W_feat = tmp;
	
	tmp = NE_feat;
	NE_feat = SE_feat;
	SE_feat = SW_feat;
	SW_feat = NW_feat;
	NW_feat = tmp;

	inTilePositions[0] = NW_feat;
	inTilePositions[1] = N_feat;
	inTilePositions[2] = NE_feat;
	inTilePositions[3] = W_feat;
	inTilePositions[4] = C_feat;
	inTilePositions[5] = E_feat;
	inTilePositions[6] = SW_feat;
	inTilePositions[7] = S_feat;
	inTilePositions[8] = SE_feat;
}

int Tile::minInTilePlacement(int num) {
	if (num == 1 || num == 5) {
		return num;
	}
	else {
		return minInTilePlacement(inTilePositions[num - 1], num);
	}
}

int Tile::minInTilePlacement(Feature* const &curr, int &num) {
	if (num == 1) {
		return num;
	}
	curr->setVisited(true);
	for (int i = 1; i < num; i++) {
		if (inTilePositions[i - 1] == curr) {
			if (i < num) {
				num = i;
			}
		}
	}
	for (int i = 0; i < curr->getConnectedFeatures().size(); i++) {
		if (!curr->getConnectedFeatures()[i]->isVisited()) {
			minInTilePlacement(curr->getConnectedFeatures()[i], num);
		}
	}
	curr->setVisited(false);
	return num;
}

bool Tile::canPlaceCrocodile() {
	for (int i = 1; i <= 9; i++) {
		if (getFeatAt(i) != NULL) {
			if (getFeatAt(i)->canPlaceCrocodile() == false) {
				return false;
			}
		}
	}
	return true;
}

void Tile::addCrocodile() {
	N_feat->addCrocodile();
	E_feat->addCrocodile();
	S_feat->addCrocodile();
	W_feat->addCrocodile();
}

vector<int> Tile::possibleTigerPlacements() {
	vector<int> result;
	for (int i = 1; i <= 9; i++) {
		if (getFeatAt(i) != NULL) {
			if (getFeatAt(i)->canPlaceTiger()) {
				result.push_back(i);
			}
		}
	}
	return result;
}

int Tile::bestTigerPlacement() {
	int result = -1;
	int possiblePoints;
	int currentBest = 0;
	for (int i = 1; i <= 9; i++) {
		if (getFeatAt(i) != NULL) {
			if (getFeatAt(i)->canPlaceTiger()) {
				possiblePoints = getFeatAt(i)->getPotentialPoints();
				if (possiblePoints >= currentBest || result == -1) {
					result = i;
					currentBest = possiblePoints;
				}
			}
		}
	}
	return result;
}

bool Tile::canPlaceGoat() {
	for (int i = 1; i <= 9; i++) {
		if (getFeatAt(i) != NULL) {
			if (getFeatAt(i)->canPlaceGoat() == false) {
				return false;
			}
		}
	}
	return true;
}

void Tile::addGoat() {
	N_feat->addGoat();
	E_feat->addGoat();
	W_feat->addGoat();
	S_feat->addGoat();
}

int Tile::getPossiblePoints() {
	int possiblePoints = 0;
	bool featChecked;
	for (int i = 1; i <= 9; i++) {
		if (getFeatAt(i) != NULL) {
			featChecked = false;
			for (int k = 1; k < i; k++) {
				if (getFeatAt(i) == getFeatAt(k)) {
					featChecked = true;
					break;
				}
			}
			if (featChecked != true) {
				for (int j = 0; j < getFeatAt(i)->getConnectedFeatures().size(); j++) {
					possiblePoints += getFeatAt(i)->getConnectedFeatures()[j]->getPotentialPoints();
				}
			}
		}
	}
	return possiblePoints;
}

void Tile::print() {
	cout << "NORTH: " << N_feat->getType() << endl;
	cout << "EAST: " << E_feat->getType() << endl;
	cout << "SOUTH: " << S_feat->getType() << endl;
	cout << "WEST: " << W_feat->getType() << endl;
	cout << "Center: ";
	if (C_feat == NULL) cout << "NONE";
	else cout << C_feat->getType();
	cout << endl;
	cout << "(" << xpos << "," << ypos << ")" << endl;
}