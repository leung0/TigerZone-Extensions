package com.tigerzone.fall2016.area;

import com.tigerzone.fall2016.animals.Goat;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;

public abstract class GoatFriendlyArea extends Area {
    private List<Goat> goatList;


    public GoatFriendlyArea() {
        this.goatList = new ArrayList<>();
    }


    public void mergeArea(GoatFriendlyArea area) {
        super.mergeArea(area);
        this.goatList.addAll(getGoatList());
    }


    /**
     * Returns true if list is not empty
     *
     * @return
     */
    private boolean hasGoatInArea() {
        return !(this.goatList.isEmpty());
    }

    /**
     * this method should be called when user is placing a Goat
     *
     * @param crocodile
     */
    @Override
    public boolean placePrey(Goat goat) {
        if (!hasGoatInArea()) {
            this.addAnimal(goat);
            return true;
        } else if (!goat.hasOwner()) {
            this.addAnimal(goat);
            return true;
        } else {
            //throw forfeit!!

            return false;
        }
    }

    /**
     * this method should be called when a Goat is added from an AreaTile
     *
     * @param goat
     */
    @Override
    public void addAnimal(Goat goat) {
        this.goatList.add(goat);
    }

    /**
     * Returns the number of Goat in this area
     *
     * @return
     */
    public int getNumOfGoat() {
        return this.goatList.size();
    }

    public List<Goat> getGoatList() {
        return goatList;
    }
}