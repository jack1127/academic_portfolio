public class Pizza{
	public String size;
	public int cheese, pepperoni, ham;
	Pizza(){
		size="small";
		cheese=1;
		pepperoni=1;
		ham=1;
	}
	Pizza(String s, int c, int p, int h){
		size=s;
		cheese=c;
		pepperoni=p;
		ham=h;
	}
	/*public static void main(String argv[]){
		Pizza pizza = new Pizza("large", 3, 1, 5);
		System.out.println(pizza.getSize());
		System.out.println(pizza.getNumberOfCheese());
		System.out.println(pizza.getNumberOfPepperoni());
		System.out.println(pizza.getNumberOfHam());
		pizza = new Pizza();
		pizza.setSize("medium");
		pizza.setNumberOfCheese(2);
		pizza.setNumberOfPepperoni(4);
		pizza.setNumberOfHam(1);
		System.out.println(pizza.getSize());
		System.out.println(pizza.getNumberOfCheese());
		System.out.println(pizza.getNumberOfPepperoni());
		System.out.println(pizza.getNumberOfHam());
		System.out.println(pizza.calcCost());
		System.out.println(pizza.toString());
		System.out.println(pizza.equals(new Pizza("large", 2, 4, 1)));
		System.out.println(pizza.equals(new Pizza()));
		System.out.println(pizza.equals(new Pizza("medium", 2, 4, 1)));
	}*/
	public String getSize(){
		return size;
	}
	public int getNumberOfCheese(){
		return cheese;
	}
	public int getNumberOfPepperoni(){
		return pepperoni;
	}
	public int getNumberOfHam(){
		return ham;
	}
	public void setSize(String si){
		size=si;
	}
	public void setNumberOfCheese(int ch){
		cheese=ch;
	}
	public void setNumberOfPepperoni(int pe){
		pepperoni=pe;
	}
	public void setNumberOfHam(int ha){
		ham=ha;
	}
	public double calcCost(){
		double ans;
		if(size=="small"){
			ans=10;
		}
		else if(size == "medium"){
			ans=12;
		}
		else{
			ans=14;
		}
		ans+=cheese*2;
		ans+=pepperoni*2;
		ans+=ham*2;
		return ans;
	}
	public String toString(){
		String ans="";
		ans="size = "+size+", numOfCheese = "+cheese+", numOfPepperoni = "+pepperoni+", numOfHam = "+ham;
		return ans;
	}
	public Boolean equals(Pizza otherpizza){
		if(otherpizza.size!=size){
			return false;
		}
		else if(otherpizza.cheese!=cheese){
			return false;
		}
		else if(otherpizza.pepperoni!=pepperoni){
			return false;
		}
		else if(otherpizza.ham!=ham){
			return false;
		}
		return true;
	}
}