import java.io.*;
import java.util.*;

public class Rule {
	public void printRule(){
		System.out.println("Rules : ");
		System.out.println("Each turn, you mark one of the small squares.");
		System.out.println("When you get three in a row on a small board, you've won that board.");
		System.out.println("To win the game, you need to win three small boards in a line.");
		System.out.println(" ");
		System.out.println("You don't get to pick which of the nine boards to play on.");
		System.out.println("That's determined by your opponent's previous move.");
		System.out.println("Whichever square he picks, that's the corresponding board you must play in next,");
		System.out.println("and whichever square you pick will determine which corresponding board he plays on next.");
		System.out.println("If the corresponding board is full or has been conquered, the player can pick arbitrary square as you want.");
		System.out.println("Notice that first player's first step can't pick E5(center)");
		System.out.println("If you want to exit during the game, input  'EXIT' in your turn");
		System.out.println(" ");
	}
}