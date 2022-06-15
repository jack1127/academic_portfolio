
public class SimpleArrayList{
	public int size;
	public Integer list[];
	
	SimpleArrayList(){
		size=0;
		list = new Integer[size];
	}
	SimpleArrayList(int initialsize){
		size=initialsize;
		list = new Integer[size];
		for(int i=0; i<size; i++){
			list[i] = 0;
		}
	}

	public void add(Integer i){
		Integer tmp[] = list;
		list = new Integer[size+1];
		for(int j=0; j<size; j++){
			list[j] = tmp[j];
		}
		list[size] = i; 
		size = size + 1;
		return;
	}
	public Integer get(int index){
		if(index >= size){
			return null;
		}
		else{
			return list[index];
		}
	}
	public Integer set(int index, Integer e){
		if(index >= size){
			return null;
		}
		else{
			Integer temp;
			temp=list[index];
			list[index]=e;
			return temp;
		}
	}
	public boolean remove(int index){  
		if(index >= size){
			return false;
		}
		else if(list[index]==null){
			return false;
		}
		else{
			Integer tmp[] = list;
			size = size-1;
			list = new Integer[size];
			for(int j=0; j<index; j++){
				list[j] = tmp[j];
			}
			for(int j=index; j<size; j++){
				list[j] = tmp[j+1];
			}
			return true;
		}
	}

	public void clear(){
		size=0;
		return;
	}

	public int size(){
		return size;
	}

	public boolean retainAll(SimpleArrayList l2){
		boolean change = false;
		boolean same = false;
		for(int j=0; j<size; j++){
			same = false;
			for(int i=0; i<l2.size; i++){
				if(list[j] == l2.list[i]){
					same = true;
					break;
				}	
			}
			if (!same) {
				this.remove(j);
				change=true;
			}
		}
		if(change){
			return true;
		}
		else{
			return false;
		}
	}
	public static void main(String argv[]){
		System.out.println("=== TASK 1 ===");
		SimpleArrayList list = new SimpleArrayList();
		System.out.println(list.get(0));

		System.out.println("=== TASK 2 ===");
		list.add(2);
		list.add(5);
		list.add(8);
		list.add(1);
		list.add(12);
		System.out.println(list.get(2));

		System.out.println("=== TASK 3 ===");
		System.out.println(list.get(5));

		System.out.println("=== TASK 4 ===");
		System.out.println(list.set(2, 100));

		System.out.println("=== TASK 5 ===");
		System.out.println(list.get(2));

		System.out.println("=== TASK 6 ===");
		System.out.println(list.set(5, 100));

		System.out.println("=== TASK 7 ===");
		System.out.println(list.remove(2));

		System.out.println("=== TASK 8 ===");
		System.out.println(list.get(2));

		System.out.println("=== TASK 9 ===");
		System.out.println(list.remove(2));

		System.out.println("=== TASK 10 ===");
		System.out.println(list.get(2));

		System.out.println("=== TASK 11 ===");
		System.out.println(list.get(3));

		System.out.println("=== TASK 12 ===");
		list.clear();
		System.out.println(list.get(0));

		System.out.println("=== TASK 13 ===");
		SimpleArrayList list2 = new SimpleArrayList(5);
		System.out.println(list2.get(3));

		System.out.println("=== TASK 14 ===");
		System.out.println(list2.get(9));

		System.out.println("=== TASK 15 ===");
		for (int i = 0; i < list2.size(); i++) {
			System.out.println(list2.set(i, i));
		}
		for (int i = 0; i < 5; i++) {
			list.add(i);
		}
		System.out.println(list.retainAll(list2));

		System.out.println("=== TASK 16 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}

		System.out.println("=== TASK 17 ===");
		System.out.println(list2.remove(0));
		System.out.println(list2.remove(2));
		System.out.println(list.retainAll(list2));

		System.out.println("=== TASK 18 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}

		System.out.println("=== TASK 19 ===");
		System.out.println(list.set(1, null));
		System.out.println(list.remove(1));

		System.out.println("=== TASK 20 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}

		System.out.println("=== TASK 21 ===");
		System.out.println(list.set(1, 123));

		System.out.println("=== TASK 22 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}

		System.out.println("=== TASK 23 ===");
		System.out.println(list.remove(1));

		System.out.println("=== TASK 24 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}

		System.out.println("=== TASK 25 ===");
		list.add(null);
		System.out.println(list.remove(2));

		System.out.println("=== TASK 26 ===");
		for (int i = 0; i < list.size(); i++) {
			System.out.println(list.get(i));
		}
	}
}
