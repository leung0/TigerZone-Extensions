package com.tigerzone.fall2016.animals;

import com.tigerzone.fall2016.area.Area;

public class Deer extends Prey {
    @Override
    public void addToArea(Area area){ area.addAnimal(this);}

    @Override
    public boolean isDeer() {
        return true;
    }

    @Override
    public boolean isBoar() {
        return false;
    }

    @Override
    public boolean isBuffalo() {
        return false;
    }
	
	@Override
	public boolean isGoat() {
		return false;
	}
}
