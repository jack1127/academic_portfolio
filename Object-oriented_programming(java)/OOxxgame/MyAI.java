import java.io.*;
import java.util.*;

public class MyAI {
	private OOxxGame game;
	public String x_reverse_map = "ABCDEFGHI";
	public MyAI(OOxxGame game) {
		this.game = game;
	}

	public int getBlock_int(String coordinate){  //integer_version of getBlock in OOxxGame.java
		int x = Integer.parseInt(game.map.get(coordinate.substring(0, 1)));
		int y = Integer.parseInt(coordinate.substring(1, 2));
		if(((x==1)||(x==4)||(x==7))&&((y==1)||(y==4)||(y==7)))
			return 1;
		else if(((x==1)||(x==4)||(x==7))&&((y==2)||(y==5)||(y==8)))
			return 2;
		else if(((x==1)||(x==4)||(x==7))&&((y==3)||(y==6)||(y==9)))
			return 3;
		else if(((x==2)||(x==5)||(x==8))&&((y==1)||(y==4)||(y==7)))
			return 4;
		else if(((x==2)||(x==5)||(x==8))&&((y==2)||(y==5)||(y==8)))
			return 5;
		else if(((x==2)||(x==5)||(x==8))&&((y==3)||(y==6)||(y==9)))
			return 6;
		else if(((x==3)||(x==6)||(x==9))&&((y==1)||(y==4)||(y==7)))
			return 7;
		else if(((x==3)||(x==6)||(x==9))&&((y==2)||(y==5)||(y==8)))
			return 8;
		else if(((x==3)||(x==6)||(x==9))&&((y==3)||(y==6)||(y==9)))
			return 9;
		else
			return 0;
	}

	public void drawConquer(int cnt, int x, int y){ //if conquer
		game.board[cnt+1][2*x]   = game.board[cnt+1][2*x].substring(0, 4*y)   + "X       X" + game.board[cnt+1][2*x].substring(4*y+9, game.board[cnt+1][2*x].length());
		game.board[cnt+1][2*x+1] = game.board[cnt+1][2*x+1].substring(0, 4*y) + "  X   X  " + game.board[cnt+1][2*x+1].substring(4*y+9, game.board[cnt+1][2*x+1].length());
		game.board[cnt+1][2*x+2] = game.board[cnt+1][2*x+2].substring(0, 4*y) + "    X    " + game.board[cnt+1][2*x+2].substring(4*y+9, game.board[cnt+1][2*x+2].length());
		game.board[cnt+1][2*x+3] = game.board[cnt+1][2*x+3].substring(0, 4*y) + "  X   X  " + game.board[cnt+1][2*x+3].substring(4*y+9, game.board[cnt+1][2*x+3].length());
		game.board[cnt+1][2*x+4] = game.board[cnt+1][2*x+4].substring(0, 4*y) + "X       X" + game.board[cnt+1][2*x+4].substring(4*y+9, game.board[cnt+1][2*x+4].length());
	}
	public void drawStep(int cnt, int x, int y, int inblock){ // process steps 
		for(int i = 0; i <= 19; i++){
			game.board[cnt+1][i] = new String(game.board[cnt][i]);
		}
	    game.board[cnt+1][2*x] = game.board[cnt+1][2*x].substring(0, 4*y) + "X" + game.board[cnt+1][2*x].substring(4*y+1, 39);
		game.previous_block[cnt+1] = 3 * ((x - 1) % 3) + ((y - 1) % 3) + 1;
		System.out.println(" ");
		System.out.print("AI play at : ");
		System.out.print(x_reverse_map.charAt(x-1));
		System.out.println(y);
		x = (inblock / 3) * 3 + 1;
		y = (inblock % 3) * 3 + 1;
		if(game.partWin(x, y)){ //check "partWin"
			game.conquer[inblock] = -1;
			drawConquer(cnt, x, y);
		}
		else if(game.filled_num[inblock]==9){  //check "filled up"
			game.conquer[inblock] = 2;
		}
		if(game.conquer[game.previous_block[cnt+1]-1] != 0) //check "free-step"
			game.previous_block[cnt+1] = -1;
	}

	public void AI_run(String coordinate, int cnt){
		boolean moved = false;
		int[] xlist = {1,1,1,4,4,4,7,7,7}; // A, D, G
		int[] ylist = {1,4,7,1,4,7,1,4,7};
		
		// represent a 3*3 matrix as a list
		int[] xcorlist = {0,0,0,1,1,1,2,2,2};  
		int[] ycorlist = {0,1,2,0,1,2,0,1,2};

		int inblock = getBlock_int(coordinate)-1;
		int max_X_num = -1;
		int max_X_num_idx = -1;
		
		if(game.conquer[inblock] != 0){    //have a free step and decide to play in the block with the most Xs
			for(int i = 1; i < 9; i++){
				if(game.conquer[(inblock+i)%9] == 0 && game.AI_filled_num[(inblock+i)%9] > max_X_num){
					max_X_num = game.AI_filled_num[(inblock+i)%9];
					max_X_num_idx = (inblock+i)%9;
				}
			}
			inblock = max_X_num_idx;
			//System.out.println("INBLOCK : "+inblock);
		}
				
        //using strategy to choose a step to conquer, if not found, randomly choose a valid coordinate
      	while(!moved){
      		int tmpx, tmpy;
      		for(int i=0; i<9; i++){
      			//System.out.println("try with stragegy : " + i);
      			tmpx=xlist[inblock]+xcorlist[i];
      			tmpy=ylist[inblock]+ycorlist[i];
      			if(game.record[tmpx][tmpy]==0){
      				game.record[tmpx][tmpy]=-1;
					if(!game.partWin(( inblock / 3) * 3 + 1, ( inblock % 3) * 3 + 1)){  //not a partWin step
						game.record[tmpx][tmpy]=0;
					}
					else{  //find a partWin step
						game.filled_num[inblock] += 1;  
						game.AI_filled_num[inblock] += 1;
						drawStep(cnt, tmpx, tmpy, inblock);
						moved = true;
      					break;
      				}
      			}
      		}
      		if(moved){
      			break;
      		}
      		else{ // randomly choose a valid coordinate
      			ArrayList<Integer> list = new ArrayList<>();  // make a list representing idx of a 3*3 matrix
        		for(int i = 0; i < 9; i++) 
        			list.add(i);
				Random rand = new Random();
		      	int r,x,y;
        		while(true){
        			//System.out.println("random");
		      		r = rand.nextInt(list.size());
		      		x = xlist[inblock]+xcorlist[r];
		      		y = ylist[inblock]+ycorlist[r];
		      		if(game.record[x][y]==0){
		      			//System.out.println("found by random");
		      			game.record[x][y] = -1;
		      			game.filled_num[inblock] += 1;
						game.AI_filled_num[inblock] += 1;
		      			drawStep(cnt, x, y, inblock);
						moved = true;
		      			break;
		      		}
		      		else
		      			list.remove(r);
		      	}
	      	}
	    }  	
	}
}