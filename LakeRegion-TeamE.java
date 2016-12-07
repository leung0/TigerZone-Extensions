package entities;

import java.util.ArrayList;
import java.util.LinkedHashSet;

/**
 * A Lake Region. Collection of terrain that exist on a board.
 */
public class LakeRegion extends Region
{

	// Lake specific properties
	protected boolean isLakeEnd;
	protected int numEndsNeeded;
	protected int endsNeeded;

	// Constructor

	/**
	 * LakeRegion is an object of the board that describes cities regions.
	 * Use this if there is no starting terrain.
	 * @param aTerrain Single terrain that is included in the region.
	 * @return LakeRegion
	 */
	public LakeRegion(Terrain aTerrain) {
		// Region ID becomes the terrain's ID
		theRegionID  = aTerrain.getTerrainID();
		theTerrains  = new ArrayList<Terrain>();
		theTigers    = new ArrayList<TigerObject>();
		theType      = 'L';
		isCompleted = false;
		theAnimals   = new ArrayList<Animal>();
		theCrocodiles = new ArrayList<CrocodileObject>();
		theGoats = new ArrayList<GoatObject>();
		tileList	= new LinkedHashSet<Integer>();

		// Add and update meepels and shields
		addTerrain(aTerrain, theRegionID);

		int num = aTerrain.getTileConnections().size();
		numEndsNeeded = adjust(num);
	}

	//ACCESSORSS 

	/*
	 * Gets the "potential" score of a region based on what is currently inside of the region. 
	 * A completed region is based on 2 * number of tiles times (1 + number of unique animals)
	 * 
	 * NOTE: Number of unique animals is adjusted for by crocodiles eating prey (with a min being 0)
	 * NOTE: This is specific to lake regions!! Crocodiles are adjusted differently for trails.
	 */
	@Override
	public int getPotential() { 
		int potential = 0;
		int unique = getUniqueAnimals();
		int numCrocs = getNumOfCrocs();
		int numTiles = getTileListSize();

		int adjustedUnique = Math.max(0,unique - numCrocs);

		if(isCompleted) { 
			potential = (2 * numTiles) * (1 + adjustedUnique); 
		} 
		else {
			potential = (1 * numTiles) * ( 1 + adjustedUnique);
		}
		return potential;
	}

	// MUTATORS

	//this function will mark a lake region as complete if it finds it to be
	public void markComplete() {

		//if the lake region isn't an end piece, 
		if(!isLakeEnd) { 

			/*
			 * The logic for this section is associated with having solely connecting pieces
			 * create a complete trail (think rectangle). What this does is it goes through 
			 * each terrain associated with a region. It'll add in the terrain's connecting
			 * points and remove them if a "connection" is made by the other terrain's 
			 * connecting points (e.g. 1 to 5 or 3 to 7). If by the end of iterations we have
			 * any left over, we know there's a gap.
			 * 
			 * This is really only an issue for the lakes with 3 connecting points.
			 */

			//initialze with fist terrain
			ArrayList<Integer> checker = new ArrayList<Integer>();
			checker.addAll(theTerrains.get(0).getTileConnections());

			//for all other terrains
			for (int i = 1; i < theTerrains.size(); i++) { 

				//get the connecting points and adjust for the orientation of the specific tile
				ArrayList<Integer> terrainConnect = theTerrains.get(i).getTileConnections();
				//				int adjustment = 2 * theTerrains.get(i).getOrientation();

				//for each connection
				for (Integer spot : terrainConnect) {

					//if connection is there, remove it; else, add in the connecting point
					if(checker.get(checker.size()-1) == (Integer)Math.floorMod(spot + 4,8)) {
						checker.remove((Integer)Math.floorMod(spot + 4,8));
					}
					checker.add((Integer)Math.floorMod(spot, 8));
				}
			}
			if(checker.isEmpty()) isCompleted = true;
		} //else the lake is an end piece
		else { 

			/*
			 * The logic for this is that the number of "end" lakes must exceed the number
			 * of ends needed to complete. The number of ends needed to complete is based 
			 * on the fact that a lake may need either 1, 2, 3, or 4 end pieces to complete.
			 * The total number of ends needed is added for each individual number of ends needed
			 * minus 2 for the connection made.
			 */
			int numEnds = 0;
			for (int i = 0; i < theTerrains.size(); i++) { 
				if(((LakeTerrain) theTerrains.get(i)).isEndOfLake()) { 
					numEnds++;
				}			
			}
			if (numEnds > numEndsNeeded) isCompleted = true;
		}

	}

	/**
	 * This version of addTerrain() will add all terrains provided through an ArrayList.
	 * This makes use of the overloaded addTerrain() for single cases.
	 * @param aTerrains Set of terrain that is included in the region
	 * @param regionID All terrains' new region ID
	 */
	public void addTerrain(ArrayList<Terrain> aTerrains, int regionID) {

		int neededSize = aTerrains.size();
		for (int i = 0; i < neededSize; i++) {
			Terrain terrain = aTerrains.get(i);
			this.addTerrain(terrain, regionID);

			//adjust number of ends needed
			int num = terrain.getTileConnections().size();
			if (terrain instanceof LakeTerrain && !((LakeTerrain) terrain).isEndOfLake()) numEndsNeeded += adjust(num) - 2;

		}

		//mark it complete if so and reset its potential accordingly
		markComplete();
		setPotential(getPotential());
	}

	/**
	 * This version of addTerrain() will add an individual terrain.
	 * @param aTerrain a single terrain that is included in the region
	 * @param regionID The terrain's new region ID
	 */
	public void addTerrain(Terrain aTerrain, int regionID) {
		if(aTerrain instanceof LakeTerrain) { 

			// Add terrain
			aTerrain.setRegionID(regionID);
			theTerrains.add(aTerrain);
			tileList.add(aTerrain.getTileID()); //for the number of tiles
			if (!isLakeEnd) isLakeEnd = ((LakeTerrain) aTerrain).isEndOfLake(); //change to lake end if necessary

			// Add tigers, if any
			if (aTerrain.hasTiger() == true) {
				theTigers.add(aTerrain.getTiger());
			}

			// Add animals, if any 
			if (((LakeTerrain) aTerrain).hasAnimal() == true) {
				Animal theAnimal = ((LakeTerrain) aTerrain).getAnimal();			
				theAnimals.add(theAnimal);
			}

			//add crocodiles, if any
			if (((LakeTerrain) aTerrain).hasCrocodile() == true) {
				theCrocodiles.add(((LakeTerrain) aTerrain).getCrocodile());
			}

			//readjust minimum for valid Tiger/Crocodile placements
			recentMin = aTerrain.getZoneMin();
		}
	}
	//METHODS 

	/*
	 * This function will adjust the number of ends needed to complete a connecting
	 * lake region. If the number of connecting points of a lake region is 1, it 
	 * needs 1 end to finish. If 3, then 2. If 5, then 3. If 8, then 4. 
	 */
	public int adjust(int num) { 
		switch(num) {
		case 1: return 1;  
		case 3: return 2; 
		case 5: return 3; 
		case 8: return 4; 
		default: return num;
		}
	}

	@Override
	public String toString() {

		int regionID = theRegionID;
		char regionType = theType;
		int numOfTigers = getNumOfTigers();
		int numOfTerrains = getNumOfTerrains();
		int numOfCrocs = getNumOfCrocs();
		int numOfAnimals = Math.max(0,getNumOfAnimals()[0] - getNumOfAnimals()[1]);
		int numOfUniqueAnimals = getUniqueAnimals();
		String outTrailEnd = (isLakeEnd) ? "end" : "connecting";

		return "The " + outTrailEnd + " region " + regionID + " of type " + regionType + " has " +
		numOfTigers + " Tiger(s), " + numOfCrocs + " crocodile(s), " + numOfAnimals + " animal(s), " + numOfUniqueAnimals + " unique, and " + numOfTerrains + " Terrain(s).";
	}

	/**
	 * DO NOT USE, testing only. LakeRegion is an object of the board that describes cities regions.
	 * Use this if there is no starting terrain.
	 * @param aRegionID A unique ID derived from the tile and region
	 * @return LakeRegion
	 */
	public LakeRegion(int aRegionID) {
		theRegionID  = aRegionID;
		theTerrains  = new ArrayList<Terrain>();
		theTigers    = new ArrayList<TigerObject>();
		isCompleted = false;
		theType      = 'L';
		theAnimals   = null;
	}

}