#include "Player.h";
#include <utility>;
#include <iostream>;
#include <string>;

Player::Player(int num) {
	playerNumber = num;
	score = 0;
	numberOfTigers = 7;
	numberOfCrocodiles = 2;
	tigersPlacedOnJungles = 0;
	currentTile = NULL;
	board = NULL;
}

Player::Player(int num, Board* &b) {
	playerNumber = num;
	board = b;
	numberOfTigers = 7;
	numberOfCrocodiles = 2;
	currentTile = NULL;
	score = 0;
	tigersPlacedOnJungles = 0;
}

int Player::getScore() {
	return score;
}

int Player::getNumberOfTigers() {
	return numberOfTigers;
}

int Player::getNumberOfCrocodiles() {
	return numberOfCrocodiles;
}

Board* Player::getBoard() {
	return board;
}

void Player::setCurrentTile(Tile* &tile) {
	currentTile = tile;
}

Tile* Player::getCurrentTile() {
	return currentTile;
}

string Player::placeTile(pair<pair<int,int>,int> position_orientation){
	pair<int,int> position = position_orientation.first;
	int orientation = position_orientation.second;
	//set tile's position to the given position
	currentTile->setPosition(position.first, position.second); //set the position of the tile to be placed
	//rotate tile until it is in given orientation
	for (int i = 0; i < orientation / 90; i++) {
		currentTile->rotateCCW();
	}
	//once the tile is in the given orientation, add it to the board
	board->addTile(currentTile);
	return " " + to_string(position.first) + " " + to_string(position.second) + " " + to_string(orientation);
}

void Player::placeTiger(int position) {
	if (currentTile->getFeatAt(position)->getType() == 'J') {
		tigersPlacedOnJungles += 1;
	}
	int legalPlacement = currentTile->minInTilePlacement(position);
	currentTile->getFeatAt(position)->addTiger(playerNumber);
	numberOfTigers--;
}

void Player::placeCrocodile() {
	currentTile->addCrocodile();
	numberOfCrocodiles--;
}

void Player::placeGoat() {
	currentTile->addGoat();
	numberOfGoats--;
}

void Player::makeDumbMove() {
	vector<pair<pair<int, int>, int>> possibleTilePlacements = board->possibleTilePlacements(currentTile);
	if (possibleTilePlacements.size() > 0) {
		placeTile(possibleTilePlacements[0]);
		currentTile->print();
		if (numberOfTigers > 0) {
			vector<int> possibleTigerPlacements = currentTile->possibleTigerPlacements();
			if (possibleTigerPlacements.size() > 0) {
				placeTiger(possibleTigerPlacements[0]);
				cout << "PLACED TIGER AT " << possibleTigerPlacements[0] << endl << endl;
			}
			else {
				cout << "NO LEGAL TIGER PLACEMENTS" << endl << endl;
			}
		}
		else {
			cout << "NO TIGERS LEFT" << endl << endl;
		}
	}
	else {
		cout << "NO LEGAL TILE PLACEMENTS" << endl << endl;
	}
}

string Player::makeSmarterMove() {
	string s = "";
	pair<pair<int, int>, int>* bestPlacement = board->bestTilePlacement(currentTile);
	if (bestPlacement != NULL) {
		int x = bestPlacement->first.first;
		int y = bestPlacement->first.second;
		int or = bestPlacement->second;
		s += "PLACED " + currentTile->getName() + " AT " + to_string(x) + " " + to_string(y) + " " + to_string(or);
		placeTile(make_pair(make_pair(x,y),or));
		cout << "Player " << playerNumber << "'s turn:" << endl << endl;
		currentTile->print();
		cout << endl;
		if (numberOfTigers > 0) {
			int bestTigerPlacement = currentTile->bestTigerPlacement();
			if (bestTigerPlacement != -1) {
				if (currentTile->getFeatAt(bestTigerPlacement)->getType() == 'J' && tigersPlacedOnJungles == 2) {
					cout << "Already placed 2 tigers on jungles, was going to place at " << bestTigerPlacement << endl << endl;
					if (currentTile->getPossiblePoints() < 0 && currentTile->canPlaceCrocodile() && numberOfCrocodiles > 0) {
						placeCrocodile();
						cout << "PLACED CROCODILE" << endl << endl;
						s += " CROCODILE";
					}
					else {
						s += " NONE";
					}
				}
				else {
					placeTiger(bestTigerPlacement);
					cout << "PLACED TIGER AT " << currentTile->getFeatAt(bestTigerPlacement)->getType() << " " << currentTile->minInTilePlacement(bestTigerPlacement) << endl << endl;
					s += " TIGER " + to_string(currentTile->minInTilePlacement(bestTigerPlacement));
				}
			}
			else {
				cout << "NO LEGAL TIGER PLACEMENTS" << endl << endl;
				if (currentTile->getPossiblePoints() < 0 && currentTile->canPlaceCrocodile() && numberOfCrocodiles > 0) {
					placeCrocodile();
					cout << "PLACED CROCODILE" << endl << endl;
					s += " CROCODILE";
				}
				else {
					s += " NONE";
				}
			}
		}
		else {
			cout << "NO TIGERS LEFT" << endl << endl;
			if (currentTile->getPossiblePoints() < 0 && currentTile->canPlaceCrocodile() && numberOfCrocodiles > 0) {
				placeCrocodile();
				cout << "PLACED CROCODILE" << endl << endl;
				s += " CROCODILE";
			}
			else {
				s += " NONE";
			}
		}
	}
	else {
		s = "TILE " + currentTile->getName() + " UNPLACEABLE PASSED";
		cout << "NO LEGAL TILE PLACEMENTS" << endl << endl;
		s += " NONE";
	}
	return s;
}

void Player::getTigersBack() {
	int p1Tigers = 0;
	int p2Tigers = 0;
	currentTile->getNfeat()->checkIfScored(p1Tigers, p2Tigers);
	if (playerNumber == 1) {
		numberOfTigers += p1Tigers;
	}
	else if (playerNumber == 2) {
		numberOfTigers += p1Tigers;
	}
	currentTile->getEfeat()->checkIfScored(p1Tigers, p2Tigers);
	if (playerNumber == 1) {
		numberOfTigers += p1Tigers;
	}
	else if (playerNumber == 2) {
		numberOfTigers += p1Tigers;
	}
	currentTile->getSfeat()->checkIfScored(p1Tigers, p2Tigers);
	if (playerNumber == 1) {
		numberOfTigers += p1Tigers;
	}
	else if (playerNumber == 2) {
		numberOfTigers += p1Tigers;
	}
	currentTile->getWfeat()->checkIfScored(p1Tigers, p2Tigers);
	if (playerNumber == 1) {
		numberOfTigers += p1Tigers;
	}
	else if (playerNumber == 2) {
		numberOfTigers += p1Tigers;
	}
	//if the tile just placed has a den, and is complete, pick up tigers if applicable
	int x = currentTile->getX();
	int y = currentTile->getY();
	if (currentTile->getCfeat() != NULL) {
		if ( currentTile->getCfeat()->getType() == 'X' && board->tileIsSurrounded(currentTile) ) {
			if (currentTile->getCfeat()->hasP1Tiger() && playerNumber == 1) {
				numberOfTigers += 1;
			}
			else if (currentTile->getCfeat()->hasP2Tiger() && playerNumber == 2) {
				numberOfTigers += 1;
			}
		}
	}
	//do the same for all surrounding tiles
	Tile* curr;
	if (board->tileAt(x, y + 1) != NULL) {
		curr = board->tileAt(x, y + 1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x + 1, y + 1) != NULL) {
		curr = board->tileAt(x + 1, y + 1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x+1, y) != NULL) {
		curr = board->tileAt(x+1, y);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x+1, y-1) != NULL) {
		curr = board->tileAt(x+1, y - 1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x, y-1) != NULL) {
		curr = board->tileAt(x, y - 1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x-1, y - 1) != NULL) {
		curr = board->tileAt(x-1, y-1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x-1, y) != NULL) {
		curr = board->tileAt(x-1, y);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
	if (board->tileAt(x-1, y + 1) != NULL) {
		curr = board->tileAt(x-1, y + 1);
		if (curr->getCfeat() != NULL) {
			if (curr->getCfeat()->getType() == 'X' && board->tileIsSurrounded(curr)) {
				if (curr->getCfeat()->hasP1Tiger() && playerNumber == 1) {
					numberOfTigers += 1;
				}
				else if (curr->getCfeat()->hasP2Tiger() && playerNumber == 2) {
					numberOfTigers += 1;
				}
			}
		}
	}
}

int Player::getPlayerNumber() {
	return playerNumber;
}