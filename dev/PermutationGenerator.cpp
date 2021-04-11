#include <sys/time.h>
#include <iostream>

#include <string>
#include <queue>
#include <omp.h>
using namespace std;

FILE *fp;
void measure_time(string, int);

class PerGen{
protected:
	int _souLen = 0, _resLen = 0, _residual = 0, *_nowCnt;
	char temp;
	queue<char> _source;
	string _result;
public:
	PerGen() = delete;
	PerGen (string str, int resLen): _resLen(resLen){
		_souLen = str.size();
		_resLen = _souLen < resLen ? _souLen: resLen;
		_residual = _souLen - resLen;
		_nowCnt = (int*)calloc(_resLen, sizeof(int));
		for(int i = 0; i < _resLen; i++)_result += str[i];
		for(int i = _resLen; i < _souLen; i++)_source.push(str[i]);
	}
	~PerGen(){ free(_nowCnt); }
	bool nextPer(int pos = -2){
		pos = pos < -1 || _resLen < pos ? _resLen-1: pos;
		if(pos == -1)return false;
		if(_nowCnt[pos] >= _residual){
			push();
			pos--; _residual++;
			if(!nextPer(pos))return false;
			pos++; _residual--;
			pop();
			_nowCnt[pos] = 0;
			return true;
		}
		push(); pop();
		_nowCnt[pos]++;

		return true;
	}
	inline void push(){
		_source.push(_result[_result.size()-1]);
		_result.erase(_result.size()-1, 1);
	}
	inline void pop(){
		_result += _source.front();
		_source.pop();
	}

	string GetResult(){ return _result; }
};

class asyncPerGen : protected PerGen{
private:
	string str;
public:
	asyncPerGen() = delete;
	asyncPerGen(string input, int resLen): PerGen(input, resLen)
	{ str = input; }
	~asyncPerGen() = default;

};

int main(int argc, char *argv[]){
	fp = fopen("time-test.txt", "w");
	PerGen *pg = new PerGen("abcdef", 3);
	
	do cout << pg->GetResult() << endl;
	while(pg->nextPer());
	measure_time("CRYPTO", 6);
	measure_time("CRYPTOABC", 9);
	measure_time("CRYPTOABCD", 10);
	measure_time("CRYPTOABCDE", 11);
	measure_time("CRYPTOABCDEF", 12);
		

	return 0;
}

void measure_time(string input, int length){
	PerGen *pg = new PerGen(input, length);

	unsigned int sec;
	int nsec;
	double d_sec;
	struct timespec start_time, end_time;

	//for(char &s: string(input) )pg->AddElement(s);
	//pg->Gen();	

	clock_gettime(CLOCK_REALTIME, &start_time);	
	
	// if(vertion == 1) while(pg->nextPermutation());
	// if(vertion == 2) while(pg->nextPermutation2());
	while(pg->nextPer());
	
	clock_gettime(CLOCK_REALTIME, &end_time);
	sec = end_time.tv_sec - start_time.tv_sec;	
	nsec = end_time.tv_nsec - start_time.tv_nsec;
	d_sec = (double)sec + (double)nsec / (1000*1000*1000);
	fputs((input+": ").c_str(), fp);
	printf("time:%f\n", d_sec);
	fprintf(fp, "time:%f\n", d_sec);
	
	delete(pg);
}
