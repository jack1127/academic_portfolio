import java.io.*;
import java.util.*;

public class TripQuery {
	private FinalProject fp;
	public TripQuery(FinalProject fp) {
		this.fp = fp;
	}
	public void query() {
		// query the existing trips
		String[] travel_code_name = fp.get_travel_code_name();
		int[] travel_code = fp.get_travel_code();
		boolean found_travel = false;
		System.out.println("請依照下列格式輸入欲查詢的行程：\n[目的地] [出發日期]\n例如：波蘭 2020-05-20");
		for(int k=0; k<3 && !found_travel; k++){  //give user 3 opportunity
			Scanner input_data = new Scanner(System.in);
			String input = input_data.nextLine();
			String[] inputs = input.split(" |\\ ");
			if(inputs.length != 2 || inputs[1].length() != 10){
				if(k==2){
					System.out.println("輸入格式錯誤，請確認後再進行嘗試");
					System.exit(-1);
				}
				System.out.println("輸入格式錯誤，請重新操作");
				continue;	
			}
			boolean found_code = false;
			String dest_code = null;
			String dest_code_name = null;
			for (int i = 0; i < 103 && !found_code; i++) {
				String[] dest = travel_code_name[i].split("．");
				for (int j = 0; j < dest.length && !found_code; j++){
					if(dest[j].equals(inputs[0])){
						dest_code = Integer.toString(travel_code[i]);
						dest_code_name = travel_code_name[i];
						found_code = true;
					}
				}
			}

			// travel code correctness
			if(!found_code){
				if(k==2){
					System.out.println("暫無此行程，請確認後再進行嘗試");
					System.exit(-1);
				}
				System.out.println("暫無此行程，請重新輸入:");
				continue;
			}
			try{
				//read trip_data_all.csv
				File data_file = new File("trip_data_all.csv");
				InputStreamReader read = new InputStreamReader (new FileInputStream(data_file), "UTF-8"); 
				BufferedReader br = new BufferedReader(read);
				br.readLine();
				String line;
				while((line = br.readLine()) != null && !found_travel){
					String[] data = line.split(",");
							
					//find match
					if(data[1].equals(dest_code)){
						if(data[4].equals(inputs[1])){
							System.out.println("行程名稱：" + data[0]); 
							System.out.println("行程：" + dest_code_name);
							System.out.println("行程代碼：" + data[1]);
							System.out.println("價格：" + data[3]);
							System.out.println("最少出團人數：" + data[6]);
							System.out.println("最多出團人數：" + data[7]);
							System.out.println("出發日期：" + data[4]);
							System.out.println("回台抵達日期：" + data[5]);
							found_travel = true;
						}
					}
				}
				// date correctness
				if(!found_travel){
					if(k==2){
						System.out.println("暫無對應日期，請確認後再進行嘗試");
						System.exit(-1);
					}
					System.out.println("暫無對應日期，請重新輸入:");
					continue;
				}
			} catch(Exception e) {
				e.printStackTrace();
			}
		}
	}
}