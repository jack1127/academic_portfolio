import java.io.*;
import java.util.*;

public class Exit {
	public void exitgame(){
			System.out.println("Before you exit, please give our game a integer score between 1~5");
			String score;
			Scanner input_data = new Scanner(System.in);
			score = input_data.nextLine();
			while (true){  // grading System before exit
				if (score.equals("1") || score.equals("2") || score.equals("3") || score.equals("4") || score.equals("5")){
					System.out.println("Thanks for your feedback, see you next time!");
					break;
				} 
				else{
					System.out.println("Invalid value, please give our game a integer score between 1~5");
					score = input_data.nextLine();
				}
			}
	}
}