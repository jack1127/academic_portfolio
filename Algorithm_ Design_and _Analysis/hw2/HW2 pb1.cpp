#include <array>
#include <iostream>
#include <random>
#include <stdio.h>

namespace ada {

	class Xoroshiro128 {
		public:
  			using result_type = uint32_t;
  			static constexpr result_type(min)() { return 0; }
  			static constexpr result_type(max)() { return UINT32_MAX; }
  			static inline result_type rotl(const result_type x, int k) {
    		return (x << k) | (x >> (32 - k));
  	}
  	Xoroshiro128() : Xoroshiro128(1, 2, 3, 4) {}
  	Xoroshiro128(result_type a, result_type b, result_type c, result_type d)
      	: s{a, b, c, d} {}
  	result_type operator()() {
    	const result_type result = rotl(s[0] + s[3], 7) + s[0];
    	const result_type t = s[1] << 9;
    	s[2] ^= s[0];
    	s[3] ^= s[1];
    	s[1] ^= s[2];
    	s[0] ^= s[3];
    	s[2] ^= t;
    	s[3] = rotl(s[3], 11);
    	return result;
  	}

	private:
  		std::array<result_type, 4> s;
	};

	namespace {
		int c_lead, c_team;
		Xoroshiro128 rng;
	}  // namespace

	int Init() {
  		int n;
  		uint32_t s1, s2, s3, s4;
 		std::cin >> n >> c_lead >> c_team >> s1 >> s2 >> s3 >> s4;
  		rng = Xoroshiro128(s1, s2, s3, s4);
  		return n;
	}

	int GetLeadership() { return uint64_t(rng()) * c_lead >> 32; }

	int GetTeamValue() {
  		int tmp = int(uint64_t(rng()) * c_team >> 32) + 1;
  		return int(c_team / sqrt(tmp));
	}
}

const int kN = 2000000;
int leadership[kN], team_value[kN];
long long int sum[kN];  //0 to r
int largest2[kN];
long long int DP[kN+1];
long long int DPsum[kN+5];

int find_largest(int L, int R, int leadership[], int idx){
	int k;
	if(leadership[idx] < sum[L]-sum[idx]){    //smaller than i+1
		return idx;
	}
	else if(leadership[idx] >= sum[R]-sum[idx]){  //larger than n-1
		return R;
	}
	while(R-L>1){
		k=(L+R)/2;
		if(sum[k]-sum[idx]==leadership[idx]){
			return k;
		}
		else if(sum[k]-sum[idx] > leadership[idx]){
			R=k;
		}
		else if(sum[k]-sum[idx] < leadership[idx]){
			L=k;
		}
	}
	if(L==R){
		return L;
	}
	else{
		if(sum[R] - sum[idx] <= leadership[idx]){
			return R;
		}
		else if(sum[L] - sum[idx] <= leadership[idx]){
			return L;
		}
	}
	return -1;
}

int main() {
	int n = ada::Init(); // Get N
//	printf("leadership : ");
	for (int i = 0; i < n; i++){
		leadership[i] = ada::GetLeadership();
//		printf("%d ",leadership[i]); 
    }
//  printf("\nawareness : ");  
    for (int i = 0; i < n; i++){
    	team_value[i] = ada::GetTeamValue();
//	    printf("%d ",team_value[i]);  		
    }
//  printf("\n");
    sum[0]=0;
    for(int i = 1; i < n; i++){
    	sum[i] = sum[i-1] + team_value[i];
    }
    for(int i = 0; i < n - 1; i++){
    	largest2[i]=find_largest(i+1,n-1,leadership,i);
//    	printf("largest2[%d] = %d\n",i, largest2[i]);
    }

    DP[n-1]=1;
    DPsum[n-1]=1;
    DPsum[n]=0;
    DPsum[n+1]=-1;
    for(int i = n-2; i >= 0; i--){
    	if(largest2[i]==i){
    		DP[i]=DP[i+1];
    	}
    	else{
    		if(DPsum[i+1] - DPsum[largest2[i] + 2] < 0){
    			DP[i]=(DPsum[i+1] - DPsum[largest2[i] + 2]+1000000007)%1000000007;
    		}
    		else 
    			DP[i]=(DPsum[i+1] - DPsum[largest2[i] + 2])%1000000007;
    	}
    	DPsum[i]=(DP[i]+DPsum[i+1])%1000000007;
 //   	printf("DP[%d] = %d, DPsum[%d] = %d\n", i , DP[i], i ,DPsum[i]);
    }

    printf("%lld",DP[0] % 1000000007);
}