import java.io.*;
import java.util.*;
import org.json.*;


public class FinalProject {
	// private variable
	private static FinalProject instance;
	private int upper[];
	private int travel_code[];
	private String travel_code_name[];
	private String user_id;
	// Constructor
	public FinalProject() {
		this.upper = new int[16384];
		this.travel_code = new int[128];
		this.travel_code_name = new String[128];
	}

	public FinalProject(String id) {
		this.upper = new int[16384];
		this.travel_code = new int[128];
		this.travel_code_name = new String[128];
		this.user_id = id;
	}

	// Singleton design pattern
	public static FinalProject get_instance() {
		if(instance == null)
			instance = new FinalProject();
		return instance;
	}

	public static FinalProject get_instance(String id) {
		if(instance == null)
			instance = new FinalProject(id);
		return instance;
	}

	// get information of the object
	public int[] get_travel_code() {
		return this.travel_code;
	}

	public String[] get_travel_code_name() {
		return this.travel_code_name;
	}

	public String get_id() {
		return this.user_id;
	}

	// check whether the user exists
	public boolean USER_EXIST(String name) {
		try {
			File csv = new File("user_info.csv");
			InputStreamReader read = new InputStreamReader (new FileInputStream(csv),"UTF-8"); 
			BufferedReader br = new BufferedReader(read);
			br.readLine();
			String line;
			int counter = 1;
			while((line = br.readLine()) != null) {
				if(line.compareTo(Integer.toString(counter)+","+name) == 0) 
					return true;
				counter += 1;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

	public boolean USER_EXIST() {
		try {
			File read_file = new File("user_info.csv");
			InputStreamReader read = new InputStreamReader (new FileInputStream(read_file),"UTF-8"); 
			BufferedReader br = new BufferedReader(read);
			String line;
			br.readLine();
			while((line = br.readLine()) != null) {
				String[] info = line.split(",");
				if(info[0].equals(this.user_id))
					return false;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return true;
	}

	// read json file
	public void read_json() {
		try {
			File json = new File("travel_code.json");
			InputStreamReader read = new InputStreamReader (new FileInputStream(json),"UTF-8"); 
			BufferedReader br = new BufferedReader(read);
			JSONArray arr = (JSONArray) new JSONTokener(br).nextValue();
			for (int i = 0; i < 103; i++) {
				JSONObject obj = arr.getJSONObject(i);
				this.travel_code[i] = obj.getInt("travel_code");
 				this.travel_code_name[i] = obj.getString("travel_code_name");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// main
	public static void main(String[] args) {
		System.out.println("歡迎使用團體旅遊預訂系統～請輸入您的使用者ID，如果尚未註冊請打0：");
		Scanner input_ID = new Scanner(System.in);
		String id = input_ID.nextLine();
		// let the user make the registration
		if(id.equals("0")) {
			String csv = "user_info.csv";
			System.out.println("輸入姓名：");
			Scanner input_name = new Scanner(System.in);
			String user_name = input_name.nextLine();
			FinalProject fp = get_instance();
			if(fp.USER_EXIST(user_name)){
				System.out.println("已註冊過該帳戶，請註冊新的帳戶^_^");
				System.exit(-1);
			}
			try {
				File csv_file = new File(csv);
				InputStreamReader read = new InputStreamReader (new FileInputStream(csv_file),"UTF-8"); 
				BufferedReader br = new BufferedReader(read);
				int num = 0;
				br.readLine();
				while(br.readLine() != null)
					num += 1;
				br.close();
				OutputStreamWriter write = new OutputStreamWriter(new FileOutputStream(csv_file, true), "UTF-8");
				BufferedWriter bw = new BufferedWriter(write);
				String s = Integer.toString(num+1) + "," + user_name + "\n";
				write.write(s);
				write.flush();
				write.close();
				System.out.println("註冊成功！使用者ID為："+Integer.toString(num+1));
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		else {
			FinalProject fp = get_instance(id);
			if(fp.USER_EXIST()) {
				System.out.println("ID不存在，請重新登入");
				System.exit(-1);
			}
			fp.read_json();
			System.out.println("1. 查詢可報名行程\n2. 行程預定\n3. 退訂與修改\n4. 查詢訂單\n5. 離開系統");
			System.out.print("輸入選項：");
			while(true) {
				Scanner option_input = new Scanner(System.in);
				String opt = option_input.nextLine();
				// query existing trip
				if(opt.equals("1")) {
					TripQuery trip = new TripQuery(fp);
					trip.query();
					System.exit(0);
				}
				// make a trip order
				else if(opt.equals("2")) {
					TripOrder trip = new TripOrder(fp);
					trip.order();
					System.exit(0);
				}
				// modify the order
				else if(opt.equals("3")) {
					TripModify trip = new TripModify(fp);
					trip.modify();
				}
				
				// query the order
				else if(opt.equals("4")) {
					OrderQuery order = new OrderQuery(fp);
					order.query();
					System.exit(0);
				}
				// leave the system
				else if(opt.equals("5")) {
					System.exit(0);
				}
				else {
					System.out.print("沒有此選項，請重新輸入:");
				}
			}
		}
	}
}
