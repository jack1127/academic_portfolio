import java.util.*;
import java.io.*;

public class OOxxGame {
	// information of the game
	public String[][] board;
	public int[] conquer;
	public Map<String, String> map;
	public int[] previous_block;
	public int[][] record;
	public int[] filled_num;
	public int[] AI_filled_num;
	public boolean tie = false;

	// initialize through constructor
	public OOxxGame(){
		this.conquer = new int[9];
		this.record = new int[16][16];
		this.previous_block = new int[128];
		this.previous_block[0] = -1;
		this.filled_num = new int[9];
		this.AI_filled_num = new int[9];
		for (int i = 0; i < 9; i++){
			filled_num[i] = 0;
			AI_filled_num[i] = 0;
		}
	}
	
	// set up the game board we are going to use for the game
	public void startGame(){
		String vocab = "ABCDEFGHI";
		this.map = new HashMap<String, String>();
		for(int i = 1; i <= 9; i++)
			this.map.put(vocab.substring(i-1, i), String.valueOf(i));

		this.board[0][0] = "    1   2   3   4   5   6   7   8   9";

		int idx = 0;
		for(int i = 1; i <= 19; i++){
			if (i % 2 == 1)
				this.board[0][i] = "  + - + - + - + - + - + - + - + - + - +";
			if (i % 2 == 0)
				this.board[0][i] = vocab.substring(i/2-1, i/2) + " |   |   |   |   |   |   |   |   |   |";
		}
	}

	// draw the game board on the terminal
	public void printBoard(int cnt){
		System.out.println();
		for(int i = 0; i < 20; i++)
			System.out.println(this.board[cnt][i]);
		System.out.println();
	}

	// get the block according to the coordinate
	public int getBlock(String c){
		if ( (c.charAt(0) == 'A' || c.charAt(0) == 'B' || c.charAt(0) == 'C') && (c.charAt(1) == '1' || c.charAt(1) == '2' || c.charAt(1) == '3') )
			return 1;
		if ( (c.charAt(0) == 'A' || c.charAt(0) == 'B' || c.charAt(0) == 'C') && (c.charAt(1) == '4' || c.charAt(1) == '5' || c.charAt(1) == '6') )
			return 2;
		if ( (c.charAt(0) == 'A' || c.charAt(0) == 'B' || c.charAt(0) == 'C') && (c.charAt(1) == '7' || c.charAt(1) == '8' || c.charAt(1) == '9') )
			return 3;
		if ( (c.charAt(0) == 'D' || c.charAt(0) == 'E' || c.charAt(0) == 'F') && (c.charAt(1) == '1' || c.charAt(1) == '2' || c.charAt(1) == '3') )
			return 4;
		if ( (c.charAt(0) == 'D' || c.charAt(0) == 'E' || c.charAt(0) == 'F') && (c.charAt(1) == '4' || c.charAt(1) == '5' || c.charAt(1) == '6') )
			return 5;
		if ( (c.charAt(0) == 'D' || c.charAt(0) == 'E' || c.charAt(0) == 'F') && (c.charAt(1) == '7' || c.charAt(1) == '8' || c.charAt(1) == '9') )
			return 6;
		if ( (c.charAt(0) == 'G' || c.charAt(0) == 'H' || c.charAt(0) == 'I') && (c.charAt(1) == '1' || c.charAt(1) == '2' || c.charAt(1) == '3') )
			return 7;
		if ( (c.charAt(0) == 'G' || c.charAt(0) == 'H' || c.charAt(0) == 'I') && (c.charAt(1) == '4' || c.charAt(1) == '5' || c.charAt(1) == '6') )
			return 8;
		if ( (c.charAt(0) == 'G' || c.charAt(0) == 'H' || c.charAt(0) == 'I') && (c.charAt(1) == '7' || c.charAt(1) == '8' || c.charAt(1) == '9') )
			return 9;
		return -1;
	}

	// check whether a single block has a result
	public boolean partWin(int x, int y){
		if (record[x][y] != 0 && record[x][y] == record[x][y+1] && record[x][y+1] == record[x][y+2])
			return true;
		if (record[x+1][y] != 0 && record[x+1][y] == record[x+1][y+1] && record[x+1][y+1] == record[x+1][y+2])
			return true;
		if (record[x+2][y] != 0 && record[x+2][y] == record[x+2][y+1] && record[x+2][y+1] == record[x+2][y+2])
			return true;
		if (record[x][y] != 0 && record[x][y] == record[x+1][y] && record[x+1][y] == record[x+2][y])
			return true;
		if (record[x][y+1] != 0 && record[x][y+1] == record[x+1][y+1] && record[x+1][y+1] == record[x+2][y+1])
			return true;
		if (record[x][y+2] != 0 && record[x][y+2] == record[x+1][y+2] && record[x+1][y+2] == record[x+2][y+2])
			return true;
		if (record[x][y] != 0 && record[x][y] == record[x+1][y+1] && record[x+1][y+1] == record[x+2][y+2])
			return true;
		if (record[x][y+2] != 0 && record[x][y+2] == record[x+1][y+1] && record[x+1][y+1] == record[x+2][y])
			return true;
		return false;
	}

	// make a step by player
	public boolean MakeStep(String coordinate, int cnt){
		int player = cnt % 2;
		// check boundary
		if (coordinate.length() != 2) {
			System.out.println("wrong input format");
			return false;
		}

		if (coordinate.charAt(0) < 'A' || coordinate.charAt(0) > 'I') {
			System.out.println("out of boundary");
			return false;
		}

		if (coordinate.charAt(1) < '1' || coordinate.charAt(1) > '9'){
			System.out.println("out of boundary");
			return false;
		}

		// check vaildity
		if(this.previous_block[cnt] != -1 && this.previous_block[cnt] != this.getBlock(coordinate)){
			System.out.println("Invalid");
			return false;
		}
		// cannot pick E5 on first step because of game fairness
		if(cnt==0 && coordinate.equals("E5")){
			System.out.println("First step can't pick E5");
			return false;
		}
		int x = Integer.parseInt(this.map.get(coordinate.substring(0, 1)));
		int y = Integer.parseInt(coordinate.substring(1, 2));
		if(this.record[x][y] != 0){
			System.out.println("Invalid");
			return false;
		}
		for(int i = 0; i <= 19; i++){
			this.board[cnt+1][i] = new String(this.board[cnt][i]);
		}
		this.record[x][y] = (player == 0)? 1 : -1; // In record, '1' represent O while '-1' represent X
		if(player == 1)
			this.board[cnt+1][2*x] = this.board[cnt+1][2*x].substring(0, 4*y) + "X" + this.board[cnt+1][2*x].substring(4*y+1, this.board[cnt+1][2*x].length());
		else
			this.board[cnt+1][2*x] = this.board[cnt+1][2*x].substring(0, 4*y) + "O" + this.board[cnt+1][2*x].substring(4*y+1, this.board[cnt+1][2*x].length());
		this.previous_block[cnt+1] = 3 * ((x - 1) % 3) + ((y - 1) % 3) + 1;

		int inblock = this.getBlock(coordinate) - 1;
		filled_num[inblock] += 1;
		x = ( inblock / 3) * 3 + 1;
		y = ( inblock % 3) * 3 + 1;
		
		// check whether a block has a result after making a step
		if(this.partWin(x, y)){
			this.conquer[this.getBlock(coordinate)-1] = (player == 0)? 1 : -1;
			if(player == 1){
				board[cnt+1][2*x]   = board[cnt+1][2*x].substring(0, 4*y)   + "X       X" + board[cnt+1][2*x].substring(4*y+9, board[cnt+1][2*x].length());
				board[cnt+1][2*x+1] = board[cnt+1][2*x+1].substring(0, 4*y) + "  X   X  " + board[cnt+1][2*x+1].substring(4*y+9, board[cnt+1][2*x+1].length());
				board[cnt+1][2*x+2] = board[cnt+1][2*x+2].substring(0, 4*y) + "    X    " + board[cnt+1][2*x+2].substring(4*y+9, board[cnt+1][2*x+2].length());
				board[cnt+1][2*x+3] = board[cnt+1][2*x+3].substring(0, 4*y) + "  X   X  " + board[cnt+1][2*x+3].substring(4*y+9, board[cnt+1][2*x+3].length());
				board[cnt+1][2*x+4] = board[cnt+1][2*x+4].substring(0, 4*y) + "X       X" + board[cnt+1][2*x+4].substring(4*y+9, board[cnt+1][2*x+4].length());
			}
			else{
				board[cnt+1][2*x]   = board[cnt+1][2*x].substring(0, 4*y)   + "  O O O  " + board[cnt+1][2*x].substring(4*y+9, board[cnt+1][2*x].length());
				board[cnt+1][2*x+1] = board[cnt+1][2*x+1].substring(0, 4*y) + "O       O" + board[cnt+1][2*x+1].substring(4*y+9, board[cnt+1][2*x+1].length());
				board[cnt+1][2*x+2] = board[cnt+1][2*x+2].substring(0, 4*y) + "O       O" + board[cnt+1][2*x+2].substring(4*y+9, board[cnt+1][2*x+2].length());
				board[cnt+1][2*x+3] = board[cnt+1][2*x+3].substring(0, 4*y) + "O       O" + board[cnt+1][2*x+3].substring(4*y+9, board[cnt+1][2*x+3].length());
				board[cnt+1][2*x+4] = board[cnt+1][2*x+4].substring(0, 4*y) + "  O O O  " + board[cnt+1][2*x+4].substring(4*y+9, board[cnt+1][2*x+4].length());
			}
		}
		else if(filled_num[inblock]==9)
			this.conquer[inblock] = 2; //full
		
		if(conquer[this.previous_block[cnt+1]-1] != 0)
			this.previous_block[cnt+1] = -1;
		
		//System.out.print("\nBlock : ");
		//System.out.println(this.previous_block[cnt+1]);
		return true;
	}

	// player can reverse their step by pressing "r"
	public void ReverseStep(String coordinate, int cnt){
		int x = Integer.parseInt(this.map.get(coordinate.substring(0, 1)));
		int y = Integer.parseInt(coordinate.substring(1, 2));

		this.record[x][y] = 0;
		System.out.println("Remove : " + coordinate);

		x = ((this.getBlock(coordinate) - 1) / 3) * 3 + 1;
		y = ((this.getBlock(coordinate) - 1) % 3) * 3 + 1;
		if(this.partWin(x, y)){
			this.conquer[this.getBlock(coordinate)-1] = 0;
		}
		this.printBoard(cnt);
	}

	// check whether the game finishes
	public boolean endGame(){
		if(conquer[0] != 0 && conquer[0] == conquer[1] && conquer[1] == conquer[2])
			return true;
		if(conquer[3] != 0 && conquer[3] == conquer[4] && conquer[4] == conquer[5])
			return true;
		if(conquer[6] != 0 && conquer[6] == conquer[7] && conquer[7] == conquer[8])
			return true;
		if(conquer[0] != 0 && conquer[0] == conquer[3] && conquer[3] == conquer[6])
			return true;
		if(conquer[1] != 0 && conquer[1] == conquer[4] && conquer[4] == conquer[7])
			return true;
		if(conquer[2] != 0 && conquer[2] == conquer[5] && conquer[5] == conquer[8])
			return true;
		if(conquer[0] != 0 && conquer[0] == conquer[4] && conquer[4] == conquer[8])
			return true;
		if(conquer[2] != 0 && conquer[2] == conquer[4] && conquer[4] == conquer[6])
			return true;
		if(conquer[0] != 0 && conquer[1] != 0 && conquer[2] != 0 && conquer[3] != 0 && conquer[4] != 0 &&
			conquer[5] != 0 && conquer[6] != 0 && conquer[7] != 0 && conquer[8] != 0){
			tie=true;
			return true;
		}
			
		return false;
	}

	// print the hint of next step on the terminal
	public void hint(int cnt){
		if(this.previous_block[cnt] == -1)
			System.out.println("Available coordinate: [ A-I ][ 1-9 ]");
		if(this.previous_block[cnt] == 1)
			System.out.println("Available coordinate: [ A-C ][ 1-3 ]");
		if(this.previous_block[cnt] == 2)
			System.out.println("Available coordinate: [ A-C ][ 4-6 ]");
		if(this.previous_block[cnt] == 3)
			System.out.println("Available coordinate: [ A-C ][ 7-9 ]");
		if(this.previous_block[cnt] == 4)
			System.out.println("Available coordinate: [ D-F ][ 1-3 ]");
		if(this.previous_block[cnt] == 5)
			System.out.println("Available coordinate: [ D-F ][ 4-6 ]");
		if(this.previous_block[cnt] == 6)
			System.out.println("Available coordinate: [ D-F ][ 7-9 ]");
		if(this.previous_block[cnt] == 7)
			System.out.println("Available coordinate: [ G-I ][ 1-3 ]");
		if(this.previous_block[cnt] == 8)
			System.out.println("Available coordinate: [ G-I ][ 4-6 ]");
		if(this.previous_block[cnt] == 9)
			System.out.println("Available coordinate: [ G-I ][ 7-9 ]");
	}

	public static void main(String[] args) {
		String last_cor[] = new String[128];
		int cnt = 0;
		OOxxGame game = new OOxxGame();
		Menu menu = new Menu(game);
		menu.drawMenu();

		//start executing the game
		while(true){
			game.printBoard(0);
			Scanner input_data = new Scanner(System.in);
			System.out.println("Choose one of the following options : ");
			System.out.println("Input \"XO\" for player vs player mode");
			System.out.println("Input \"OO\" for player vs AI mode");
			System.out.println("Input \"OX\" for Rules explanation");
			System.out.println("Input \"XX\" for exiting the game");
			System.out.println(" ");
			System.out.print("Input your option : ");
			String cmd = input_data.nextLine();
			System.out.println();
			if(cmd.equals("XO") || cmd.equals("xo")){  //player vs player
				System.out.print("mode : player vs player");
				game.startGame();
				game.printBoard(0);
				System.out.println("Turn of player O");
				game.hint(cnt);
				System.out.print("Input the coordinate: ");
				String cor = input_data.nextLine();
				do{
					System.out.println(" ");
					if(game.MakeStep(cor, cnt)) {
						game.printBoard(cnt+1);
						if(game.endGame()){
							if(game.tie)
								System.out.println("Tie, what a close game!!!");
							else if(cnt % 2 == 0)
								System.out.println("OO Win the Game!!!");
							else
								System.out.println("XX Win the Game!!!");
							System.exit(0);
						}
						last_cor[cnt] = cor;
						cnt += 1;
						while(true) {
							if(cnt % 2==0)
								System.out.println("Turn of player O");
							else
								System.out.println("Turn of player X");
							input_data = new Scanner(System.in);
							game.hint(cnt);
							System.out.print("Input the coordinate: ");
							cor = input_data.nextLine();
							if(cor.charAt(0) == 'r' || cor.charAt(0) == 'R') {
								if(cnt > 0) {
									cnt -= 1;
									game.ReverseStep(last_cor[cnt], cnt);
								}
								else
									System.out.println("Cannot take back a move anymore");
							}
							else if(cor.equals("EXIT")){
								Exit exit = new Exit();
								System.out.println("Do you want to exit? (input Y or N)");
								String yn = input_data.nextLine();
								if(yn.equals("Y")||yn.equals("y")){
									exit.exitgame();
									System.exit(0);
								}
								else if(yn.equals("N")||yn.equals("n")){
									game.printBoard(cnt);
									continue;
								}
							}
							else
								break;
						}
					}
					else{
						while(true) {
							if(cnt % 2==0)
								System.out.println("Turn of player O");
							else
								System.out.println("Turn of player X");
							input_data = new Scanner(System.in);
							game.hint(cnt);
							System.out.print("Please input the coordinate again: ");
							cor = input_data.nextLine();
							if(cor.charAt(0) == 'r' || cor.charAt(0) == 'R') {
								if(cnt > 0) {
									cnt -= 1;
									game.ReverseStep(last_cor[cnt], cnt);
								}
								else
									System.out.println("Cannot take back a move anymore");
							}
							else if(cor.equals("EXIT")){
								Exit exit = new Exit();
								System.out.println("Do you want to exit? (input Y or N)");
								String yn = input_data.nextLine();
								if(yn.equals("Y")||yn.equals("y")){
									exit.exitgame();
									System.exit(0);
								}
								else if(yn.equals("N")||yn.equals("n")){
									game.printBoard(cnt);
									continue;
								}
							}
							else
								break;
						}
					}
				} while(!game.endGame());
			}

			// AI trace back haven't done and have a Nullpointerexception at 262, block have a pb
			else if(cmd.equals("OO") || cmd.equals("oo")){  //player vs AI
				MyAI AI = new MyAI(game);

				System.out.print("mode : player vs AI(easy)");
				game.startGame();
				game.printBoard(0);
				game.hint(cnt);
				System.out.print("Input the coordinate: ");
				String cor = input_data.nextLine();
				do{
					if(game.MakeStep(cor, cnt)) {  //player make a step
						if(game.endGame()){
							game.printBoard(cnt+1);
							if(game.tie)
								System.out.println("Tie, What a close game!!!");
							else
								System.out.println("you Win the Game!!!");	
							System.exit(0);
						}
						last_cor[cnt] = cor;
						cnt += 1;

						AI.AI_run(cor, cnt);  // AI make a step
						if(game.endGame()){
							game.printBoard(cnt+1);
							if(game.tie)
								System.out.println("Tie, What a close game!!!");
							else
								System.out.println("AI Win the Game!!!");	
							System.exit(0);
						}

						cnt += 1;
						game.printBoard(cnt);
						
						while(true) { //player traces back
							input_data = new Scanner(System.in);
							game.hint(cnt);
							System.out.print("Input the coordinate: ");
							cor = input_data.nextLine();
							if(cor.charAt(0) == 'r' || cor.charAt(0) == 'R') {
								if(cnt > 0) {
									cnt -= 2;
									game.ReverseStep(last_cor[cnt], cnt);
								}
								else
									System.out.println("Cannot take back a move anymore");
							}
							else if(cor.equals("EXIT")){
								Exit exit = new Exit();
								System.out.println("Do you want to exit? (input Y or N)");
								String yn = input_data.nextLine();
								if(yn.equals("Y")||yn.equals("y")){
									exit.exitgame();
									System.exit(0);
								}
								else if(yn.equals("N")||yn.equals("n")){
									game.printBoard(cnt);
									continue;
								}
							}
							else
								break;
						}
					}
					else{
						while(true) {
							input_data = new Scanner(System.in);
							game.hint(cnt);
							System.out.print("Please input the coordinate again: ");
							cor = input_data.nextLine();
							if(cor.charAt(0) == 'r' || cor.charAt(0) == 'R') {
								if(cnt > 0) {
									cnt -= 2;
									game.ReverseStep(last_cor[cnt], cnt);
								}
								else
									System.out.println("Cannot take back a move anymore");
							}
							else if(cor.equals("EXIT")){
								Exit exit = new Exit();
								System.out.println("Do you want to exit? (input Y or N)");
								String yn = input_data.nextLine();
								if(yn.equals("Y")||yn.equals("y")){
									exit.exitgame();
									System.exit(0);
								}
								else if(yn.equals("N")||yn.equals("n")){
									game.printBoard(cnt);
									continue;
								}
							}
							else
								break;
						}
					}
				} while(!game.endGame());
			}
			else if(cmd.equals("OX") || cmd.equals("ox")){ // print out the rule
				Rule rule = new Rule();
				rule.printRule();
				System.out.println("Input arbitrary character to go back to the menu.");
				input_data.nextLine();	
			}
			else if(cmd.equals("XX") || cmd.equals("xx")){ // leave the game
				Exit exit = new Exit();
				System.out.println("Do you want to exit? (input Y or N)");
				String yn = input_data.nextLine();
				if(yn.equals("Y")||yn.equals("y")){
					exit.exitgame();
					System.exit(0);
				}
				else if(yn.equals("N")||yn.equals("n")){
					continue;
				}
			}
		}
	}
}