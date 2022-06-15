public class PizzaOrder{
	public int count=0;
	public Pizza[] list = new Pizza[3];

	public boolean setNumberPizzas(int numberPizzas){
		if (numberPizzas > 3){
			return false;
		}
		else if (numberPizzas < 1){
			return false;
		}
		else{
			return true;
		}
	}
	public void setPizza1(Pizza pizza1){
		list[count]=pizza1;
		count++;
		return;
	}
	public void setPizza2(Pizza pizza2){
		list[count]=pizza2;
		count++;
		return;
	}
	public void setPizza3(Pizza pizza3){
		list[count]=pizza3;
		count++;
		return;
	}
	
	public double calcTotal(){
		double ans=0;
		for(int i=0; i<count; i++){
			if(list[i].size=="small"){
				ans+=10;
			}
			else if(list[i].size == "medium"){
				ans+=12;
			}
			else{
				ans+=14;
			}
			ans+=list[i].cheese*2;
			ans+=list[i].pepperoni*2;
			ans+=list[i].ham*2;
		}
		count=0;
		return ans;
	}
}