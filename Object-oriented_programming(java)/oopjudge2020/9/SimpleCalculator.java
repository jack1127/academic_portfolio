import java.text.DecimalFormat;

public class SimpleCalculator{
	private double result = 0.00;
	private int count = 0;
	private String operator;
	private double value;
	private boolean isend = false;
	public static void main(String argv[]){
		SimpleCalculator cal = new SimpleCalculator();
		String cmd = null;
		System.out.println(cal.getMsg());
		String cmd_str = "+ 5,- 2,* 5,/ 3,% 2,* D,X D,XD,, ,/ 1000000,/ 00.000,/ 0.000001,+ 1 + 1,- 1.66633,r R,r";
		String[] cmd_arr = cmd_str.split(",");
		for (int i = 0; i < cmd_arr.length; i++) {
		  try {
		    if (cal.endCalc(cmd_arr[i])) 
		      break;		
		    cal.calResult(cmd_arr[i]);
		    System.out.println(cal.getMsg());
		  } catch (UnknownCmdException e) {
		    System.out.println(e.getMessage());
		  }
		}
		System.out.println(cal.getMsg());
	}
	public void calResult(String cmd) throws UnknownCmdException{
		String[] input = cmd.split(" ");  

		if(input.length != 2)
			throw new UnknownCmdException("Please enter 1 operator and 1 value separated by 1 space");
		
		else if(!(input[0].equals("+")||input[0].equals("-")||input[0].equals("*")||input[0].equals("/"))){
			for(int i=0; i<input[1].length(); i++){
		    	int chr=input[1].charAt(i);
		      	if(chr<48 || chr>57) {
		        	if(chr != 46)
		    	  		throw new UnknownCmdException(input[0] + " is an unknown operator and " + input[1] + " is an unknown value");
		      	}
			}
			
			throw new UnknownCmdException(input[0] + " is an unknown operator");
		}
		else if(input[0].equals("/")) {
			int cnt = 0;
			for(int i = 0; i < input[1].length(); i++) {
				if(input[1].charAt(i) == 46 || input[1].charAt(i) == 48) 
					cnt += 1;
			}
			if(cnt == input[1].length())
				throw new UnknownCmdException("Can not divide by 0");
		}

		else{
			for(int i=0; i<input[1].length(); i++){
		   		int chr=input[1].charAt(i);
		  		if(chr<48 || chr>57) {
		    	   	if(chr != 46)
		   		  		throw new UnknownCmdException(input[1] + " is an unknown value"); 
		    	}
			}
		}

		this.operator = input[0];
		this.value = Double.valueOf(input[1]);
		if(this.operator.equals("+"))
			this.result += this.value;
		if(operator.equals("-"))
			this.result -= this.value;
		if(operator.equals("*"))
			this.result *= this.value;
		if(operator.equals("/"))
			this.result /= this.value;
		this.count += 1;
	}
	public String getMsg(){
		DecimalFormat formatter = new DecimalFormat("0.00");
		String res = String.valueOf(formatter.format(this.result));
		String val = String.valueOf(formatter.format(this.value));
		if(this.isend){
			return("Final result = " + res);
		}
		else if(this.count == 0) 
			return("Calculator is on. Result = " + res);
		else if(this.count == 1)
			return("Result " + this.operator + " " + val + " = " + res + ". New result = " + res); 
		else
			return("Result " + this.operator + " " + val + " = " + res + ". Updated result = " + res);
	}
	public boolean endCalc(String cmd){
		if(cmd.equals("r") || cmd.equals("R")) {
			this.isend = true;
			return true;
		}
		else
			return false;
	}
}