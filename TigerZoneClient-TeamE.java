package network;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.StringTokenizer;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import entities.TigerTile;
import entities.TileDeck;
import game.Game;

public class TigerZoneClient {

	/**
	 * Connect to the server and run games as instructed
	 * @param args Host name, port number, server password, username, password
	 */
	public static void main(String[] args) throws Exception {

		//if format not followed, specify
		if (args.length != 5) {
			System.err.println(
					"Usage: java TigerZoneClient <host name> <port number> <server password> <username> <password>");
			System.exit(1);
		}

		//take in administrative information
		String hostName = args[0];
		int portNumber = Integer.parseInt(args[1]);
		String serverPass = args[2];
		String userName = args[3];
		String userPass = args[4];

		//attempt connection with server specified by product owner
		try (
				//attempt connection with the Tournament TCP server
				Socket serverSocket = new Socket(hostName, portNumber);
				PrintWriter out = new PrintWriter(serverSocket.getOutputStream(), true);
				BufferedReader in = new BufferedReader(new InputStreamReader(serverSocket.getInputStream()));
				) {

			BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			String fromTourneyServer = "", fromHandler, fromAI;

			//variable declarations - information from server
			String gameID, GameA = null, GameB = null; // IDs for games
			String currentPlayerID;
			String opponentName;
			int challengeID, numChallenges;
			int roundID, numRounds;
			int countDown;
			ArrayList<String> tiles = new ArrayList<String>();
			int numTiles;

			//starting tile information
			String startingTile;
			int startingX, startingY, startingOrientation;

			// Current move
			int moveANum;
			int moveBNum;

			//tile recently placed by opponent
			String tilePlaced;
			int tilePlacedX;
			int tilePlacedY;
			int tileOrientation;
			String animal;
			int animalZone;

			// Games themselves
			Game gameA = null;
			Game gameB = null;
			// Game move count
			moveANum = 1;
			moveBNum = 1;
			// Flag for first game
			boolean firstGame = false;
			boolean secondGame = false;

			//tile AI wants to place
			String tileToPlace;
			String response;


			//continuously wait for input from server
			while ((fromTourneyServer = in.readLine()) != null) {

				//display information from server
				System.out.println("Server: " + fromTourneyServer);

				if (fromTourneyServer.equals("THIS IS SPARTA!")) { //if first message, send join request
					out.println("JOIN " + serverPass + "\r");
					System.out.println("Client: " + "JOIN " + serverPass );
				}
				else if (fromTourneyServer.equals("HELLO!")) {  //if request accepted, send authentication
					out.println("I AM " + userName + " " + userPass + "\r");
					System.out.println("Client: " + "I AM " + userName + " " + userPass + "\r");
				}
				else if (fromTourneyServer.equals("THANK YOU FOR PLAYING! GOODBYE")) { //if end of tournament, exit from this
					// Exit everything
					break;
					//return;
				}
				else { //otherwise, message must be parsed from server

					//tokenize it
					String[] tokenizedMessage = fromTourneyServer.split("\\s+");
					String command = tokenizedMessage[0];

					switch(command){

					case "NEW": //if new tournament created (might not need)
						challengeID = Integer.parseInt(tokenizedMessage[2]);
						numRounds = Integer.parseInt(tokenizedMessage[6]);
						break;

					case "BEGIN": //if round begun (might not need)
						roundID = Integer.parseInt(tokenizedMessage[2]);
						numRounds = Integer.parseInt(tokenizedMessage[4]);
						// Create games
						gameA = new Game("A");
						gameB = new Game("B");
						moveANum = 1;
						moveBNum = 1;
						firstGame = false;
						secondGame = false;
						break;

					case "YOUR": //take in opponent information
						opponentName = tokenizedMessage[4];
						gameA.setPlayers(userName, opponentName);
						gameB.setPlayers(userName, opponentName);
						break;

					case "STARTING": //take in starting tile information (hope we dont need)
						startingTile = tokenizedMessage[3];
						startingX = Integer.parseInt(tokenizedMessage[5]);
						startingY = Integer.parseInt(tokenizedMessage[6]);
						startingOrientation = Integer.parseInt(tokenizedMessage[7]);
						// Give games starting spots
						gameA.setStartTile(startingTile, startingX, startingY, startingOrientation);
						gameB.setStartTile(startingTile, startingX, startingY, startingOrientation);
						break;

					case "THE": //take in the randomized tile list
						tiles = new ArrayList<String>();
						numTiles = Integer.parseInt(tokenizedMessage[2]);

						for (int i = 0; i < numTiles; i++)
							tiles.add(tokenizedMessage[i+6]);

						// Gove games remaining tiles
						gameA.setTileStack(tiles);
						gameB.setTileStack(tiles);
						break;

					case "MATCH": //begin match
						countDown = Integer.parseInt(tokenizedMessage[3]);
						//begin two simultaneous games using information given up to this point
						break;

					case "MAKE": //send off move based on current tile
						gameID = tokenizedMessage[5];
						String tempMove = tokenizedMessage[10];
						// Set the right values for the first and second game
						if (!firstGame)
						{
							GameA = gameID;
							firstGame = true;
							if (gameID == "1")
								GameB = "2";
							else
								GameB = '1';
						}
						else if (!secondGame)
						{
							GameB = gameID;
							secondGame = true;
							moveBNum++;
							gameB.inc();
						}
						/*if (GameA == null && (tempMove.equals("1") || tempMove.equals("2"))){
							GameA = gameID;
							if (tempMove.equals("2")){
								moveANum++;
								gameA.inc();
							}
						}
						else if(GameB == null && ((tempMove.equals("1") || tempMove.equals("2")))){
							GameB = gameID;
							if (tempMove.equals("2")){
								moveBNum++;
								gameB.inc();
							}
						}*/

						tileToPlace = tokenizedMessage[12];
						response = "";
						// Pass the move to the game
						if (gameID.equals(GameA))
						{
							response = gameA.makeMove();
							// Add the starting information to the move
							response = "GAME " + GameA + " MOVE " + moveANum + " " + response;
						}
						else if (gameID.equals(GameB))
						{
							response = gameB.makeMove();
							// Add the starting information to the move
							response = "GAME " + GameB + " MOVE " + moveBNum + " " + response;
						}

						// Send our move
						out.println(response + "\r");
						System.out.println("Client: " + response );
						break;

					case "GAME": //game logic
						gameID = tokenizedMessage[1];
						currentPlayerID = tokenizedMessage[5];
						if (tokenizedMessage[2].equals("OVER") || tokenizedMessage[6].equals("FORFEITED:")) {
							//game over logic - i.e. tell AI to stop the two games
							//or forfeited game logic - ie. tell AI to stop the two games due to forfeit
							// Get the ended game
							if (gameID.equals(GameA))
							{
								GameA = null;
								if (gameA != null) gameA.endGame();
								gameA = null;
								if (gameA == null)
									system.err.println("FORFEITED: DOES NOT KNOW OUTCOME");
									gameA.endGame();
							}
							else if (gameID.equals(GameB))
							{
								GameB = null;
								if (gameB != null) gameB.endGame();
								gameB = null;
								if (gameB == null)
									system.err.println("FORFEITED: DOES NOT KNOW OUTCOME");
									gameB.endGame();
							}
						}
						else if (tokenizedMessage[6].equals("PLACED"))
						{
							//a move was made - place onto own board(s)
							tilePlaced = tokenizedMessage[7];
							tilePlacedX = Integer.parseInt(tokenizedMessage[9]);
							tilePlacedY = Integer.parseInt(tokenizedMessage[10]);
							tileOrientation = Integer.parseInt(tokenizedMessage[11]);
							animal = tokenizedMessage[12];
							animalZone = -1; // Default value

							// Check if the tiger has a zone
							if (animal.equals("TIGER"))
							{
								animalZone = Integer.parseInt(tokenizedMessage[13]);
							}

							// Place the tile in the game
							if (gameID.equals(GameA)) {
								moveANum++;
								gameA.placeTile(tilePlacedX, tilePlacedY, tileOrientation, animal,
										userName.equals(currentPlayerID), animalZone);
							} else if (gameID.equals(GameB)) {
								moveBNum++;
								gameB.placeTile(tilePlacedX, tilePlacedY, tileOrientation, animal,
										userName.equals(currentPlayerID), animalZone);
							}

						}
						else if (tokenizedMessage[6].equals("TILE"))
						{
							if (tokenizedMessage[7].equals("PASS"))
							{
								// Place the tile in the game
								if (gameID.equals(GameA)) {
									moveANum++;
									gameA.pass();
								} else if (gameID.equals(GameB)) {
									moveBNum++;
									gameB.pass();
								}
							}
							else
							{
								// When a tiger is added or retrived
								String addOrReplace = tokenizedMessage[9];
								boolean addTiger = false;
								tilePlacedX = 0;
								tilePlacedY = 0;

								if (addOrReplace.equals("RETRIEVED"))
								{
									addTiger = false;
									tilePlacedX = Integer.parseInt(tokenizedMessage[12]);
									tilePlacedY = Integer.parseInt(tokenizedMessage[13]);
								}
								else if (addOrReplace.equals("ADDED"))
								{
									addTiger = true;
									tilePlacedX = Integer.parseInt(tokenizedMessage[13]);
									tilePlacedY = Integer.parseInt(tokenizedMessage[14]);
								}

								// Place the tile in the game
								if (gameID.equals(GameA)) {
									moveANum++;
									gameA.unplaceableTile(userName.equals(currentPlayerID), addTiger,
											tilePlacedX, tilePlacedY);
								} else if (gameID.equals(GameB)) {
									moveBNum++;
									gameB.unplaceableTile(userName.equals(currentPlayerID), addTiger,
											tilePlacedX, tilePlacedY);
								}
							}
						}
						break;

					default:
						//System.out.println("DID NOT CATCH: " + fromTourneyServer);
					//break;
					}
				}
			}
			//System.out.println("While ended");
		} catch (UnknownHostException e) {
			System.err.println("Don't know about host " + hostName);
			System.exit(1);
		} catch (IOException e) {
			System.err.println("Couldn't get I/O for the connection to " +
					hostName);
			System.exit(1);
		}
	}
}