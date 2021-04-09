	#include <stdio.h>
	#include <stdlib.h>
	#define max(x,y) (x>y?x:y)
	#define min(x,y) (x<y?x:y)
	#define max_3(a,b,c) ((a>b?a:b)>c?(a>b?a:b):c)

	int a[200000];
	int need[8];
	long long int hr;

	int main(){
		int N,s[8],K,i,j,temp,normal=1;
		scanf("%d%d%d%d",&N,&s[1],&s[2],&s[3]);
		for(i=1; i<3; i++){    //sort s[]
	  		for(j=1; j<3-i+1; j++){
	  			if(s[j] > s[j+1]){
	  				temp = s[j];
	  				s[j] = s[j+1];
	  				s[j+1] = temp; 
	  			}
	  		}
	  	}
	  	s[4]=s[1]+s[2];
	   	s[5]=s[1]+s[3];
	  	s[6]=s[2]+s[3];
	  	s[7]=s[1]+s[2]+s[3];
	  	if(s[4] <= s[3]){ // s1 + s2  <= s3
	  		temp = s[3];
	  		s[3] = s[4];
	  		s[4] = temp; 
	  	 	normal=0;  
	  	}

		for(i=0; i<N; i++){     //check times
	    	scanf("%d",&a[i]);
	    	if(s[1] >= a[i]){
				need[1]++;    		
	    	}
	    	else if(s[2] >= a[i]){
				need[2]++;    		
	    	}
	    	else if(s[3] >= a[i]){
				need[3]++;    		
	    	}
	    	else if(s[4] >= a[i]){
				need[4]++;    		
	    	}
	    	else if(s[5] >= a[i]){
				need[5]++;    		
	    	}
	    	else if(s[6] >= a[i]){
				need[6]++;    		
	    	}
	    	else if(s[7] >= a[i]){
				need[7]++;    		
	    	}
	    	else if(a[i] > s[7]){
	    		printf("-1");
	    		exit(0);
	    	}
	  	}

	  	if(need[7]>0){
	  		hr += need[7];       // s1,s2,s3 fight
		  	need[7]=0;
	  	}
	  	
	  	if(need[6]>0){
	  		hr += need[6];		//s2,s3 fight and s1 fight alone
	  		need[1]-=need[6];
	  		if(need[1]<0){
	  			need[1]=0;
	  		}
	  		need[6]=0;
	  	}
	  	
	  	if(need[5]>0){   // s1, s3 fight and s2 fight alone
		 	hr += need[5];
	  		if(need[2]-need[5]<0){
	  			need[1]-=(need[5]-need[2]);  // s2 fight for s1
	  			if(need[1]<0){
	  				need[1]=0;
	  			}
	  			need[2]=0;
	  		}
	  		else{
	  			need[2]-=need[5];
	  		}
	  		need[5]=0;
	  	}

	/*  s1+s2 might smaller than s3*/
	  	if(need[4]>0){
	  		hr += need[4];
	  		if(normal == 1){   // s1+s2 > s3  --> s3 fight alone
	  			if(need[3]-need[4]<0){
	  				if(need[2]-need[4]+need[3] < 0){
	  					need[1] -= need[4]-need[3]-need[2];
	  					if(need[1]<0) need[1]=0;
	  					need[2]=0;
	  				}
	  				else{ 
	  					need[2] -= need[4]-need[3];
	  				}
	  				need[3]=0;
	  			}	
	  			else{
	  				need[3] -= need[4];
	  			}
	  		}
	  		else if(normal == 0){  //s3 >= s1+s2
	  			//priority :　need1 and need2 > need3 > need1 or need2
	  			if(need[1] > need[2]){
	  				if(need[4] > need[2] ){ //s2 is leisure
	  					need[1]-=need[2];
	  					need[4]-=need[2];
	  					need[2]=0;
	  					if(need[1]%2 == 0){
	  						if(need[4] >= need[1]/2){   //s2 help need1 first and then need3
	  							need[4] -= need[1]/2;
	  							need[1]=0;
	  							need[3] -= need[4];
	  							if(need[3]<0) need[3]=0;
	  						}
	  						else if(need[1]/2 > need[4]){
	  							need[1] -= 2*need[4];
	  						}
	  					}
	  					else if(need[1]%2 == 1){
	  						if(need[4] >= need[1]/2){
	  							need[4] -= need[1]/2;
	  							need[1]=1;
	  							need[3] -= need[4];
	  							if(need[3]<0){
	  								need[3]=0;
	  								need[1]=0;  	
	  							}
	  						}
	  						else if(need[1]/2 > need[4]){
	  							need[1]-= 2*need[4];
	  						}
	  					}
	  				}
	  				else if(need[2] >= need[4]){
	  					need[2] -= need[4];
	  					need[1] -= need[4];
	  				}
	  			}
	  			else if(need[2] > need[1]){
	  				if(need[4] > need[1]){ // s1 is leisure
	  					need[2] -= need[1];
	  					need[4] -= need[1];
	  					need[1]=0;
	  					if(need[3] - need[4] >= 0){
	  						need[3] -= need[4];
	  					}
	  					else if(need[4] > need[3]){
	  						need[4] -= need[3];
	  						need[3]=0;
	  						need[2] -= need[4];
	  						if(need[2]<0) need[2]=0;
	  					}
	  				}
	  				else if(need[1] >= need[4]){
	  					need[1] -= need[4];
	  					need[2] -= need[4];
	  				}
	  			}
	  			else if(need[2] == need[1]){
	  				if(need[4] > need[2]){
	  					need[4] -= need[2];
	  					need[2]=0;
	  					need[1]=0;
	  					need[3] -= need[4];
	  					if(need[3] < 0) need[3]=0;
	  				}
	  				else {
	  					need[2] -= need[4];
	  					need[1] -= need[4];
	  				}
	  			}
	  		}
	  		need[4]=0;
	  	}

	  	//need[3]
	    if(need[3]>0){
	    	if(need[1] <= 0 && need[2] <= 0){  //need1,need2 are done, so they help need[3]
	 			if(need[3]%2==0)
	 				hr += need[3]/2;
	 			else
	 				hr += need[3]/2+1;
	 		}
	 		else if(need[1] < need[3] && need[2] < need[3]){  //clear the min(need1, need2) and help need3    
				if(need[1] < need[2]){
					temp = need[1];
					need[1]=0;
					need[2] -= temp;
					need[3] -= temp;
					hr += temp;
					if(need[3]%2==0){
	 					hr += need[3]/2;
	 					need[3]=0;
	 				}
	 				else{
	 					hr += need[3]/2+1;
	 					need[2] -= 1;
	 					if(need[2]<0){
	  						need[2]=0;
	  					}
	 					need[3]=0;
	 				}
				}
	 			else if (need[2] < need[1]){  
	 				hr += need[2];
	 				need[1] -= need[2];
	 				need[3] -= need[2];
	 				need[2]=0;           
	 				if(need[1]%2==0){
	 					hr += need[1]/2;
	 					need[3] -= need[1]/2;
	 					need[1]=0;
	 					if(need[3]%2==0){
	 						hr += need[3]/2;
	 						need[3]=0;
	 					}
	 					else if(need[3]%2==1){
	 						hr += need[3]/2+1;
	 						need[3]=0;
	 					}
	 				}
	 				else if(need[1]%2==1){
	 					hr += need[1]/2;
	 					need[3] -= need[1]/2;
	 					need[1]=1;
	 					if(need[3]%2==0){
	 						hr += need[3]/2;
	 						need[3]=0;
	 					}
	 					else if(need[3]%2==1){
	 						hr += need[3]/2+1;
	 						need[3]=0;
	 						need[1]=0;
	 					}
	 				}
	 			}
	 			else if(need[2] == need[1]){
	 				hr += need[2];
	 				need[3] -= need[2];
	 				need[1]=0;
	 				need[2]=0;
	 				if(need[3]%2==0){
	 					hr += need[3]/2;
	 					need[3]=0;
	 				}
	 				else if(need[3]%2==1){
	 					hr += need[3]/2+1;
	 					need[3]=0;
	 				}
	 			}
	 		}
	 		else if(need[2] > need[3] && need[3] > need[1]){
	 			hr += need[3];
	 			need[3] -= need[1];
	 			need[2] -= need[1];
	 			need[1]=0;
	 			need[2] -= need[3];
	 			if(need[2] < 0) need[2]=0;
	 		}
	 		else if(need[1] > need[3] && need[3] > need[2]){
	 			hr += need[2];
	 			need[3] -= need[2];
	 			need[1] -= need[2];
	 			need[2]=0;
	 			if(need[1]/2 > need[3]){
					hr += need[3];
					need[1] -= 2*need[3];
					need[3]=0;
	 			}
	 			else if (need[1]/2 <= need[3]){
	 				if(need[1]%2==0){
	 					hr += need[1]/2;
	 					need[3] -= need[1]/2;
	 					need[1]=0;
	 					if(need[3]%2==0){
	 						hr += need[3]/2;
	 					}
	 					else{
	 						hr += need[3]/2+1;
	 					}
	 				}
	 				else{
	 					hr += need[1]/2;
	 					need[3] -= need[1]/2;
	 					need[1]=1;
	 					if(need[3]%2 == 0){
	 						hr += need[3]/2;
	 					}
	 					else{
	 						hr += need[3]/2+1;
	 						need[1]=0;
	 					}
	 				}
	 			}
	 		}
			else{  //need3 is done only by s3
				hr += need[3];
				if(need[2] - need[3] <0){  
					need[1] -= need[3]-need[2];
					need[2]=0;
					if(need[1]<0) need[1]=0;
	 			}
	 			else{
	 				need[2] -= need[3];
	 			}
	 			need[1] -= need[3];
	 			if(need[1]<0){
	  				need[1]=0;
	  			}
	 		}
	 		need[3]=0;
	 	}

	 	if(need[2]>0){
	 		//s3 help s2
	 		if(need[2]%2==0){
				hr += need[2]/2;
				need[1] -= need[2]/2;
				if(need[1]<0){
	  				need[1]=0;
	  			}
	 		}
	 		else{
				hr += need[2]/2+1;
	 			need[1] -= need[2]/2+1;
	 			need[1]--;  // by s3
	 			if(need[1]<0){
	  				need[1]=0;
	  			}
			}
	 	}

	 	if(need[1]>0){
	 		if(need[1]%3==0){
				hr += need[1]/3;
	 		}
	 		else{
				hr += need[1]/3+1;
			}
	 	}
		
	  	printf("%lld",hr);
	  	return 0;
	}