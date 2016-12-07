package com.tigerzone.fall2016.animals;

import com.tigerzone.fall2016.area.Area;
import com.tigerzone.fall2016.gamesystem.Player;

public class Goat extends Prey {
      private Player owner;

      public Goat() {}

      public Goat(Player owner) {
        this.owner = owner;
      {

      @Override
      public boolean isGoat() {
        return true;
      }

      @Override
      public boolean isDeer() {
          return false;
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
      public boolean placeableInDen() {
          return false;
      }

      @Override
      public boolean placeableInJungle() {
          return false;
      }

      @Override
      public boolean placeableInTrail() {
          return true;
      }

      @Override
      public boolean placeableInLake() {
          return true;
      }

      @Override
      public void placeInArea(Area area) {
          area.placePredator(this);
      }

      @Override
      public void addToArea(Area area){ area.addAnimal(this);}

      public Player getOwner(){
          return this.owner;
      }

      public boolean hasOwner() {
          if(this.owner!=null) {
              return true;
          } else {
              return false;
          }
      }
}
