import java.io.*;
import java.util.*;

public class OrderQuery {
	private FinalProject fp;
	public OrderQuery(FinalProject fp) {
		this.fp = fp;
	}

	// check the input format of the query
	public boolean check_order(String id, String[] arr, int cnt) {
		if(arr.length != 1) {
			if(cnt < 2)
				System.out.println("您輸入的查詢格式有誤，請重新輸入:");
			else
				System.out.println("您輸入的查詢格式有誤，請確認後再進行嘗試");
			return false;
		}
		else {
			// check the trip order's information recorded in the csv file
			try{
				File csv = new File("order.csv");
				InputStreamReader read = new InputStreamReader (new FileInputStream(csv),"UTF-8"); 
				BufferedReader br = new BufferedReader(read);
				br.readLine();
				String line;
				String[] info = null;
				boolean flag = false;
				while((line = br.readLine()) != null) {
					info = line.split(",");
					if(info[2].equals(id) && info[0].equals(arr[0]) && info[8].equals("1")) {
						flag = true;
						break;
					}
				}
				br.close();
				if(flag) {
					System.out.println("訂單編號:" + info[0] + "   使用者ID:" + info[2]);
					System.out.println(info[3] + " ~ " + info[4]);
					System.out.println("總價： " + info[5] + "元新台幣");
					System.out.println("入住人數： " + info[6] + "大人 " + info[7] + "小孩");
					return true;
				}
				else {
					if(cnt < 2)
						System.out.println("您輸入的訂單編號有誤, 請重新輸入:");
					else
						System.out.println("您輸入的訂單編號有誤, 請確認後再進行嘗試");
					return false;
				}
			} catch (Exception e){
				e.printStackTrace();
				return false;
			}
		}
	}

	// query the order
	public void query() {
		System.out.println("請依照下列格式輸入您想查詢的訂單：\n[訂單編號]\n例如：1");
		for(int i = 0; i < 3; i++) {
			Scanner input_data = new Scanner(System.in);
			String input = input_data.nextLine();
			String[] arr = input.split(" ");
			if(this.check_order(this.fp.get_id(), arr, i))
				break;
		}
	}
}