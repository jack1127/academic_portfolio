public class GreenCrud {
	public static int calPopulation(int init, int days){
		int multi=1;
		int fib1=1;
		int fib2=1;
		days=days/5;
		for (int i = 2; i <= days; i++){
			if(i%2==1){
				fib1=fib1+fib2;
				multi=fib1;
			}
			else{
				fib2=fib1+fib2;
				multi=fib2;
			}
		}

		return multi*init;
	}
}