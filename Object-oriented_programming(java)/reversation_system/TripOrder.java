import java.io.*;
import java.util.*;

public class TripOrder {
	// csv file we will use
	private FinalProject fp;
	private String csv1 = "order.csv";
	private String csv2 = "data.csv";

	// constructor
	public TripOrder(FinalProject fp) {
		this.fp = fp;
	}

	// check whether the travel code exists
	public boolean TRAVEL_CODE_Exist(int code) {
		int[] travel_code = this.fp.get_travel_code();
		for(int i = 0; i < 103; i++) {
			if(code == travel_code[i])
				return true;
		}
		return false;
	}

	// check whether the user name exists
	public boolean NameCorrectness(String name) {
		try {
			File csv = new File("user_info.csv");
			InputStreamReader read = new InputStreamReader (new FileInputStream(csv),"UTF-8"); 
			BufferedReader br = new BufferedReader(read);
			br.readLine();
			String line;
			String user_id = this.fp.get_id();
			while((line = br.readLine()) != null) {
				if(line.compareTo(user_id+","+name) == 0)
					return true;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

	// modify the csv file recording the trips
	public void Modify_CSV_FILE(String filename, int number, String modify) {
		try {
			File tmp_file = File.createTempFile("tmp", ".csv");
			if(filename.compareTo("data.csv") == 0) {
				try {
					File csv = new File(filename);
					InputStreamReader read = new InputStreamReader(new FileInputStream(csv),"UTF-8");
					BufferedReader br = new BufferedReader(read);
					OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(tmp_file), "UTF-8");
					BufferedWriter wr = new BufferedWriter(writer);
					String line;
					while((line = br.readLine()) != null) {
						if(line.compareTo(modify) == 0){
							String[] info = modify.split(",");
							String val1 = Integer.toString(Integer.parseInt(info[6]) - number);
							String val2 = Integer.toString(Integer.parseInt(info[7]) - number);
							info[6] = val1;
							info[7] = val2;
							String modified = info[0]+","+info[1]+","+info[2]+","+info[3]+","+info[4]+","+info[5]+","+info[6]+","+info[7]+"\n";
							wr.write(modified);
							wr.flush();
							continue;
						}
						wr.write(line+"\n");
						wr.flush();
					}
					br.close();
					wr.close();
					File csv_now = new File(filename);
					br = new BufferedReader(read);
					writer = new OutputStreamWriter(new FileOutputStream(csv_now), "UTF-8");
					wr = new BufferedWriter(writer);
					read = new InputStreamReader(new FileInputStream(tmp_file),"UTF-8");
					br = new BufferedReader(read);
					while((line = br.readLine()) != null) {
						line = line + "\n";
						wr.write(line);
						wr.flush();
					}
					br.close();
					wr.close();
				} catch(Exception e) {
					e.printStackTrace();
				}
			}
			boolean deleted = false;
			try {
				deleted = tmp_file.delete();
			} catch(Exception e) {
				e.printStackTrace();
			}
			if(deleted == false)
				tmp_file.deleteOnExit();
		} catch(Exception e) {
				e.printStackTrace();
		}
	}

	// making the order
	public void order() {
		System.out.println("請依照下列格式輸入您的行程：\n[行程代碼] [您的姓名] [出發日期] [抵台日期] [大人人數] [小孩人數]\n例如：48 王小明 2020-05-17 2020-05-19 2 3\n日期格式：xxxx-xx-xx(year-month-day)");
		Scanner input_data = new Scanner(System.in);
		String input = input_data.nextLine();
		String[] inputs = input.split(" |\\ ");
		// travel code correctness
		if(!this.TRAVEL_CODE_Exist(Integer.parseInt(inputs[0]))) {
			System.out.println("行程代碼不存在，請重新操作");
			System.exit(-1);
		}
		if(!this.NameCorrectness(inputs[1])) {
			System.out.println("使用者ID與名字不符，請重新操作");
			System.exit(-1);
		}
		if(inputs.length != 6) {
			System.out.println("輸入格式錯誤，請重新操作");
			System.exit(-1);
		}
		try{
			File travel_file = new File(this.csv2);
			InputStreamReader read = new InputStreamReader (new FileInputStream(travel_file), "UTF-8"); 
			BufferedReader br = new BufferedReader(read);
			br.readLine();
			String line;
			while((line = br.readLine()) != null) {
				String[] data = line.split(",");
				if(data[1].compareTo(inputs[0]) == 0) {
					if(data[4].compareTo(inputs[2]) == 0 && data[5].compareTo(inputs[3]) == 0) {
						int adult_number = Integer.parseInt(inputs[4]);
						int child_number = Integer.parseInt(inputs[5]);
						if(adult_number + child_number > Integer.parseInt(data[7])) {
							System.out.println("預定失敗：日期：" + data[4] + " 至 " + data[5] + " 剩餘人數不足");
							System.exit(-1);
						}
						int total_price = Integer.parseInt(data[3]) * (adult_number + child_number);
						String date = data[4] + "," + data[5] + ",";
						String people_num = Integer.toString(adult_number) + "," + Integer.toString(child_number) + ",1\n";
						int counter = 1;
						try {
							File order_file = new File(this.csv1);
							InputStreamReader order = new InputStreamReader (new FileInputStream(order_file), "UTF-8");
							BufferedReader read_order = new BufferedReader(order);
							read_order.readLine();
							while(read_order.readLine() != null)
								counter += 1;
							read_order.close();
							// write into order.csv
							OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(this.csv1, true), "UTF-8");
							BufferedWriter bw = new BufferedWriter(write);
							String str = Integer.toString(counter)+","+data[2]+","+this.fp.get_id()+","+date+Integer.toString(total_price)+","+people_num;
							write.write(str);
							write.flush();
							write.close();
							this.Modify_CSV_FILE(this.csv2, adult_number+child_number, line);
							System.out.println("預定成功： " + data[2]);
							System.out.println("訂單編號:" + Integer.toString(counter) + "   使用者ID:" + this.fp.get_id());
							System.out.println(data[4] + " ~ " + data[5]);
							System.out.println("總價： " + Integer.toString(total_price) + "元新台幣");
							System.out.println("入住人數： " + Integer.toString(adult_number) + "大人 " + Integer.toString(child_number) + "小孩");
							System.out.println("謝謝光臨～有任何問題請聯絡 0963-385-827");
						} catch(Exception e) {
							e.printStackTrace();
						}
					}
				}
			}
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}