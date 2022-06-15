public class IsLeapYear {
	public static boolean determine(int year){
		boolean flag=false;
		if (year%400==0){
			flag=true;
		}
		else if(year%100==0){
			flag=false;
		}
		else if(year%4==0){
			flag=true;
		}
		return flag;
	}
}