package entities;

/*
 * This is the GoatObject to handle the Goat that will be placeable on a tile.
 * A typical Goat should have:
 * 		1.) an owner
 * 		2.) its region association
 */
public class GoatObject {

	//Attributes
	protected Player owner;
	protected int regionID;

	//Constructors

	/**
	 * GoatObject that is placed on a terrain/tile
	 * @param  aType String with tupe of animal
	 * @return       Animal
	 */
	public GoatObject(Player owner) {
		this.owner = owner;
		this.regionID = -1;
	}

	//Getters

	/**
	* Get Goat Owner
	* @return the Player owner of the goat
	*/
	public Player getGoatOwner() {
		return owner;
	}

	/**
	* Get Goat's Region ID
	* @return an int of the goat's region
	*/
	public int getRegionID() {
		return regionID;
	}

	//MUTATORS

	//Changes the Region ID of the Goat
	public void setRegionID(int regionID) {
		this.regionID = regionID;
	}
		
	public String toString() {
		return "This goat belongs to " + owner + " in region " + regionID;
	}
}