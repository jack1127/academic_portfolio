import java.io.*;
import java.util.*;

public class TripModify{
	private FinalProject fp;
	public TripModify(FinalProject fp){
		this.fp = fp;
	}

	// Check whether input is valid
	public boolean CheckInput(String input){
		if(input.equals(""))
			return false;
	   	for(int i = 0; i < input.length(); ++i){
	   		if(Character.isDigit(input.charAt(i)))
	   			continue;
	   		else
	   			return false; // If it is not digit, return false.
		}
		if(Integer.parseInt(input) < 0) // If it is less than zero, return false.
			return false; 
		return true;
	}

	public void CopyFile(File from, File to){
		try{
			// Input & output file
			InputStreamReader read = new InputStreamReader(new FileInputStream(from),"UTF-8");
			BufferedReader br = new BufferedReader(read);
			OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(to), "UTF-8");
	    	BufferedWriter wr = new BufferedWriter(writer);
	    	String line;
	    	// Read and write
	    	while((line = br.readLine()) != null){
	    		wr.write(line + "\n");
	    		wr.flush();
	    	}
	    	br.close();
	    	wr.close();
		}catch(Exception e){
			e.printStackTrace();
		}
	}

	// To change the data upperbound back to the state it has to be.
	public void DataRecover(String code, String start, String end, int mode, String number){
		try{
			File tmp2_file = File.createTempFile("tmp2", ".csv");
			try{
				// Input & output file
				File data = new File("data.csv");
				InputStreamReader read = new InputStreamReader(new FileInputStream(data),"UTF-8");
		 		BufferedReader br = new BufferedReader(read);
		   		OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(tmp2_file), "UTF-8");
		    	BufferedWriter wr = new BufferedWriter(writer);
		    	String line;
		    	// Read and write
		    	while((line = br.readLine()) != null){
		    		String[] lst = line.split(",");  // Split the data
		    		if(lst[1].equals(code) && lst[4].equals(start) && lst[5].equals(end)){ // target line in the data
		    			String modified = "";
		    			for(int i = 0; i < 7; i++)
		    				modified += lst[i] + ",";
		    			String num = Integer.toString(Integer.parseInt(number)+Integer.parseInt(lst[7]));
		    			if(Integer.parseInt(num) < 0){  // The remaining number available for order is not enough
		    				System.out.println("剩餘人數不足\n請重新操作");
		    				System.exit(-1);
		    			}
		    			modified += num + "\n";   
		    			wr.write(modified);       // write to the temp file
		    			wr.flush();
		    			continue;
		    		}
		    		wr.write(line+"\n");   // write to the temp file
		    		wr.flush();
		    	}
		    	br.close();
		    	wr.close();
		    	this.CopyFile(tmp2_file, data);	 // Copy the file from temp file back to the original file.
			}catch(Exception e){
				e.printStackTrace();
			}

			// Erase the created temp file
		    boolean deleted = false;
		    try{
	    		deleted = tmp2_file.delete();
	    	}catch(Exception e){
	    		e.printStackTrace();
	    	}
	    	if(deleted == false)
	    		tmp2_file.deleteOnExit();
    	}catch(Exception e){
			e.printStackTrace();
		}
	}

	public void CancelOrder(String filename, String order_num){
		boolean find_order = false;
		try{
			File tmp_file = File.createTempFile("tmp", ".csv");
			try{
				// Input & output file
				File csv = new File(filename);
		   		InputStreamReader read = new InputStreamReader(new FileInputStream(csv),"UTF-8");
		 		BufferedReader br = new BufferedReader(read);
		   		OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(tmp_file), "UTF-8");
		    	BufferedWriter wr = new BufferedWriter(writer);
		    	String line;
		    	// Read and write
		    	while((line = br.readLine()) != null){
		    		String[] head = line.split(",");
		    		if(head[0].compareTo(order_num) == 0){
		    			find_order = true;
		    			String bit = line.substring(line.length()-1, line.length());  // The last bit as a flag in a data
		    			if(bit.compareTo("0") == 0){                                  // The order had already been removed
		    				System.out.println("訂單不存在，請重新操作");
		    				System.exit(-1);
		    			}
		    			String modified = line.substring(0, line.length()-1) + "0" + "\n"; // Set the bit off to represent that
		    			String travel_code = head[1];                                      // the order is removed.
		    			String date1 = head[3]; String date2 = head[4];
		    			String number = Integer.toString(Integer.parseInt(head[6]) + Integer.parseInt(head[7]));
		    			DataRecover(travel_code, date1, date2, 0, number);                 // recover the data in data.csv
		    			wr.write(modified);
		    			wr.flush();
		    			continue;
		    		}
		    		wr.write(line+"\n");
		   			wr.flush();
		    	}
		    	br.close();
		    	wr.close();
		    	if(find_order == false){
		    		System.out.println("查無此訂單，請重新操作");
		    		System.exit(-1);
		    	}
		    	else
		    		this.CopyFile(tmp_file, csv);  
			}catch(Exception e){
				e.printStackTrace();
			}

			// Erase the created temp file
			boolean deleted = false;
		    try{
	    		deleted = tmp_file.delete();
	    	}catch(Exception e){
	    		e.printStackTrace();
	    	}
	    	if(deleted == false)
	    		tmp_file.deleteOnExit();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
    
    // Change the order: increase or decrease is depends on the mode option.
    // order_num is the target order to change
    public void Change(String filename, String adult, String child, int mode, String order_num){
    	boolean find_order = false;
    	try{
    		File tmp_file = File.createTempFile("tmp", ".csv");
    		try{
    			// Input & output file
				File csv = new File(filename);
		   		InputStreamReader read = new InputStreamReader(new FileInputStream(csv),"UTF-8");
		 		BufferedReader br = new BufferedReader(read);
		   		OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(tmp_file), "UTF-8");
		    	BufferedWriter wr = new BufferedWriter(writer);
		    	String line;
		    	// Read and write
		    	while((line = br.readLine()) != null){
		    		String[] head = line.split(",");
		    		if(head[0].compareTo(order_num) == 0){
		    			find_order = true;
		    			String bit = line.substring(line.length()-1, line.length()); // The last bit as a flag in a data
		    			if(bit.compareTo("0") == 0){                                 // The order had already been removed
		    				System.out.println("訂單不存在，請重新操作");
		    				System.exit(-1);
		    			}
		    			// number of adult to adjust
		    			String adult_num = Integer.toString(Integer.parseInt(head[6]) + mode*Integer.parseInt(adult));
		    			// number of child to adjust
		    			String child_num = Integer.toString(Integer.parseInt(head[7]) + mode*Integer.parseInt(child));
		    			if(Integer.parseInt(adult_num) < 0 || Integer.parseInt(child_num) < 0){
		    				System.out.println("訂單修改失敗，退訂人數超過原預定人數");
		    				System.exit(-1);
		    			}
		    			String number = "";
		    			// number is positive or negative depends on the mode(increase or decrease)
		    			if(mode == 1)
		    				number = Integer.toString((-1)*Integer.parseInt(adult) + (-1)*Integer.parseInt(child));
		    			else
		    				number = Integer.toString(Integer.parseInt(adult) + Integer.parseInt(child));
		    			String modified = "";
		    			for(int i = 0; i < 6; i++)
		    				modified += head[i]+",";
		    			modified += adult_num + "," + child_num + "," + "1\n";
		    			String travel_code = head[1]; String date1 = head[3]; String date2 = head[4];
		    			DataRecover(travel_code, date1, date2, mode, number);
		    			wr.write(modified);
		    			wr.flush();
		    			continue;
		    		}
		    		wr.write(line+"\n");
		   			wr.flush();
		    	}
		    	br.close();
		    	wr.close();
		    	if(find_order == false){
		    		System.out.println("查無此訂單，請重新操作");
		    		System.exit(-1);
		    	}
		    	else
		    		this.CopyFile(tmp_file, csv);
		   	}catch(Exception e){
		   		e.printStackTrace();
		   	}

		   	// Erase the created temp file
			boolean deleted = false;
		    try{
	    		deleted = tmp_file.delete();
	    	}catch(Exception e){
	    		e.printStackTrace();
	    	}
	    	if(deleted == false)
	    		tmp_file.deleteOnExit();
    	}catch(Exception e){
    		e.printStackTrace();
    	}
    }

	//
	public void modify(){
		System.out.println("\n修改訂單");
		System.out.println("請輸入訂單編號，取消訂單/增加[Ｏ大人Ｏ小孩]/減少[Ｏ大人Ｏ小孩]\n");
		System.out.println("範例格式1：\n3 取消訂單\n");
		System.out.println("範例格式2：\n3 增加 3 2\n");
		System.out.println("範例格式3：\n3 減少 1 1\n");
		int times = 0;
		while(times < 3){  // You have three times chance to type the command once you have typing error.
			times += 1;	
			// Read the input from keyboard
			Scanner input_cmd = new Scanner(System.in);
			String cmd = input_cmd.nextLine();
			String[] cmds = cmd.split(" |\\ ");
			int counter = 1;
			String csv1 = "order.csv";
	   		String csv2 = "data.csv";

	   		// Check input validity
	   		if(!this.CheckInput(cmds[0])){
	   			System.out.println("Order Number Error: 請輸入正確訂單格式");
	   			continue;
	   		}
   			
   			// Input option
			if(cmds[1].compareTo("取消訂單") == 0){
				this.CancelOrder(csv1, cmds[0]);
				System.out.println("\n退訂成功，已取消您的預約記錄");
				System.exit(0);
			}
			if(cmds[1].compareTo("增加") == 0){
				// Check input validity
				if(!this.CheckInput(cmds[2]) && !this.CheckInput(cmds[3])){
					System.out.println("大人or小孩的數量有誤，請重新輸入");
					continue;
				}
				this.Change(csv1, cmds[2], cmds[3],  1, cmds[0]);
				System.out.println("訂單修改完畢");
				System.exit(0);
			}
			if(cmds[1].compareTo("減少") == 0){
				// Check input validity
				if(!this.CheckInput(cmds[2]) && !this.CheckInput(cmds[3])){
					System.out.println("大人or小孩的數量有誤，請重新輸入");
					continue;
				}
				this.Change(csv1, cmds[2], cmds[3], -1, cmds[0]);
				System.out.println("訂單修改完畢");
				System.exit(0);
			}
		}
		System.out.println("錯誤次數過多，請重新操作");
		System.exit(-1);
	}
}