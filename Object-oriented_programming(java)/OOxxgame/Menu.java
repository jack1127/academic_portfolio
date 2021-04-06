import java.io.*;
import java.util.*;

public class Menu {
	private OOxxGame game;
	public Menu(OOxxGame game) {
		this.game = game;
	}
	public void drawMenu(){
		game.board = new String[128][20];
		game.board[0][0]  = "    1   2   3   4   5   6   7   8   9";
		game.board[0][1]  = "  + - + - + - + - + - + - + - + - + - +";
		game.board[0][2]  = "A |   O O O   |-----------| X       X |";
		game.board[0][3]  = "  + O       O +  G A M E  +   X   X   +";
		game.board[0][4]  = "B | O       O |           |     X     |";
		game.board[0][5]  = "  + O       O +  M E N U  +   X   X   +";
		game.board[0][6]  = "C |   O O O   |-----------| X       X |";
		game.board[0][7]  = "  +-----------------------------------+";
		game.board[0][8]  = "D | X |  player   V.  S.  player  | O |";
		game.board[0][9]  = "  +-----------------------------------+";
		game.board[0][10] = "E | O |  player   V.  S.    AI    | O |";
		game.board[0][11] = "  +-----------------------------------+";
		game.board[0][12] = "F | O |           RULES           | X |";
		game.board[0][13] = "  +-----------------------------------+";
		game.board[0][14] = "G | X       X |           |   O O O   |";
		game.board[0][15] = "  +   X   X   +-----------+ O       O +";
		game.board[0][16] = "H |     X     |X   end   X| O       O |";
		game.board[0][17] = "  +   X   X   +-----------+ O       O +";
		game.board[0][18] = "I | X       X |           |   O O O   |";
		game.board[0][19] = "  + - + - + - + - + - + - + - + - + - +";
	}
}