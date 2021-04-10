#include <sys/time.h>

#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <omp.h>
using namespace std;

void measure_time(const char*, int);

template<class U>
class PermutationGenerator{
private:
	         vector<pair<bool, U>> source;
	typename vector<pair<bool, U>>::iterator sbegin = source.begin(); /**/
	typename vector<pair<bool, U>>::iterator send = source.end(); /**/
	         vector<typename vector<pair<bool, U>>::iterator> itrs;
	typename vector<typename vector<pair<bool, U>>::iterator>::iterator begin = itrs.begin(); /**/
	typename vector<typename vector<pair<bool, U>>::iterator>::iterator end = itrs.end(); /**/
	vector<U> result;
	int stage = 0;
public:
	~PermutationGenerator(){
		itrs.shrink_to_fit();
		result.shrink_to_fit();
		source.shrink_to_fit();
	}
	int all = 0;
	void AddElement(U add){
		source.push_back(make_pair(true, add));
		if(result.capacity() < source.size())result.reserve(source.size());
		if(itrs.capacity() < source.size())itrs.reserve(source.size());
		cout << "source.size: " << source.size() << ", result.capacity: " << result.capacity() << endl;
	}
	void reset(){
		source.clear();
		Initialize();
	}
	void Initialize(){
		send = source.end(); /**/
		sbegin = source.begin(); /**/
		result.clear();
		itrs.clear();
		return;
	}
	void Gen(int extract = -1){
		Initialize();
		for(typename vector<pair<bool, U>>::iterator s = source.begin(); s != source.end() && extract; s++, extract--){
			result.push_back(s->second);
			s->first = false;
			itrs.push_back(s);
		}
		begin = itrs.begin(); /**/
		end = itrs.end(); /**/
		return;
	}
	bool nextPermutation(){
		typename vector<typename vector<pair<bool, U>>::iterator>::iterator it = itrs.end();
		for( typename vector<typename vector<pair<bool, U>>::iterator>::reverse_iterator rit = itrs.rbegin(); rit != itrs.rend(); rit++){
			typename vector<pair<bool, U>>::iterator elem = *rit;
			while(!elem->first && elem != source.end()) elem++;
 			result.pop_back();
			(*rit)->first = true;
			if(elem != source.end()){
				result.push_back(elem->second);
				elem->first = false;
				it = --rit.base();
				*it = elem;
				break;
			}
		}
		if(it == itrs.end())return false;
		auto s = source.begin();
			it++;
		while(it != itrs.end()){
			while(!s->first && s != source.end())s++;
			*it = s;
			s->first = false;
			result.push_back(s->second);
			it++;
		}
		return true;
	}
	inline bool nextPermutation2(){
		typename vector<typename vector<pair<bool, U>>::iterator>::iterator rit = end;
		--rit;
		do{
			result.pop_back();
			(*rit)->first = true;
			if(*rit == send)continue;
			do ++(*rit);
			while(!(*rit)->first && (*rit) != send);
			if(*rit != send){
				result.push_back((*rit)->second);
				(*rit)->first = false;
				break;
			}
		} while(is_this_front_of_begin(&rit));
		if(rit == end)return false;
		for(++rit; rit != end; ++rit){
			*rit = sbegin;
			while(!(*rit)->first && (*rit) != send)++(*rit);
			(*rit)->first = false;
			result.push_back((*rit)->second);
		}

		return true;
	}
	inline bool is_this_front_of_begin(typename vector<typename vector<pair<bool, U>>::iterator>::iterator* it){
		if(*it == end){
			return false;
		} else if(*it == begin){
			*it = end;
			return false;
		} else {
			--(*it);
			return true;
		}
	}
	bool limited_nextPermutation(pair<bool, U>* limit){
		typename vector<typename vector<pair<bool, U>>::iterator>::iterator rit = end;
		--rit;
		do{
			result.pop_back();
			(*rit)->first = true;
			if(*rit == send)continue;
			do ++(*rit);
			while(!(*rit)->first && (*rit) != send);
			if(*rit != send){
				result.push_back((*rit)->second);
				(*rit)->first = false;
				break;
			}
		} while(is_this_front_of_begin(&rit));
		if(rit == end)return false;
		for(++rit; rit != end; ++rit){
			*rit = sbegin;
			while(!(*rit)->first && (*rit) != send)++(*rit);
			(*rit)->first = false;
			result.push_back((*rit)->second);
		}

		return true;

	}
	void PrintAllPermutation(){
		Initialize();
		AllPermutation();
	}
	void AllPermutation(){
		if(stage == (int)source.size()){
			//all++;
			//for(auto r: result) cout << r;
			//cout << endl;
			return;
		}
		for(typename vector<pair<bool, U>>::iterator s = source.begin(); s != source.end(); s++){
			if(s->first){
				stage++;
				result.push_back(s->second);
				s->first = false;
				AllPermutation();
				result.pop_back();
				s->first = true;
				stage--;
			}
		}
		return;
	}
	vector<pair<bool, U>>* Getsource(){
		return &source;
	}
	const vector<U>* GetResult (){
		return &result;
	}
	void PrintSource(){
		cout << "<<source>>: ";
		for(auto s: source)cout << s.second << ", ";
		cout << endl;
	}
	void PrintResult(){
		cout << "<<result>>: ";
		for(auto r: result)cout << r;
		cout << endl;
	}
};

int main(int argc, char *argv[]){
	//int elems[] = {0, 1, 2, 3};
	//PermutationGenerator<int*> *pg = new PermutationGenerator<int*>;
	PermutationGenerator<char> *pg = new PermutationGenerator<char>;
	
	for(const char a: string("CRYPTO")) pg->AddElement(a);
	//pg->PrintSource();

	unsigned int sec;
	int nsec;
	double d_sec;
	struct timespec start_time, end_time;

	printf("AllPrint\n");
	clock_gettime(CLOCK_REALTIME, &start_time);	
	pg->PrintAllPermutation();
	clock_gettime(CLOCK_REALTIME, &end_time);
	sec = end_time.tv_sec - start_time.tv_sec;	
	nsec = end_time.tv_nsec - start_time.tv_nsec;
	d_sec = (double)sec + (double)nsec / (1000*1000*1000);
	printf("time:%f\n", d_sec);

	//pg->Gen();
	//pg->PrintResult();
	//int num = 1;
	//while(pg->nextPermutation()){
	//	pg->PrintResult();pg2		//error
	//	for(auto r: *(pg->GetResult())) cout << *r;
	//	cout << endl;
	//	num++;
	//}
	//cout << "all: " << num << endl;
	//delete(pg);

	cout << "Permutation of \"CRYPTO\" using nextPermutation" << endl;
	measure_time("CRYPTO", 1);
	cout << "Permutation of \"CRYPTO\" using nextPermutation2" << endl;
	measure_time("CRYPTO", 2);
	cout << endl;
	
	/*
	cout << "Permutation of \"CRYPTOABC\" using nextPermutation" << endl;
	measure_time("CRYPTOABC", 1);
	cout << "Permutation of \"CRYPTOABC\" using nextPermutation2" << endl;
	measure_time("CRYPTOABC", 2);
	cout << endl;
			
	cout << "Permutation of \"CRYPTOABCD\" using nextPermutation" << endl;
	measure_time("CRYPTOABCD", 1);
	cout << "Permutation of \"CRYPTOABCD\" using nextPermutation" << endl;
	measure_time("CRYPTOABCD", 2);

	cout << "Permutation of \"CRYPTOABCDE\" using nextPermutation" << endl;
	measure_time("CRYPTOABCDE", 1);
	cout << "Permutation of \"CRYPTOABCDE\" using nextPermutation" << endl;
	measure_time("CRYPTOABCDE", 2);

	cout << "Permutation of \"CRYPTOABCDEF\" using nextPermutation" << endl;
	measure_time("CRYPTOABCDEF", 1);
	cout << "Permutation of \"CRYPTOABCDEF\" using nextPermutation" << endl;
	measure_time("CRYPTOABCDEF", 2);
	*/

	return 0;
}

void measure_time(const char* input, int vertion){
	PermutationGenerator<char> *pg = new PermutationGenerator<char>;

	unsigned int sec;
	int nsec;
	double d_sec;
	struct timespec start_time, end_time;

	for(char &s: string(input) )pg->AddElement(s);
	pg->Gen();	

	clock_gettime(CLOCK_REALTIME, &start_time);	
	if(vertion == 1) while(pg->nextPermutation());
	if(vertion == 2) while(pg->nextPermutation2());
	clock_gettime(CLOCK_REALTIME, &end_time);
	sec = end_time.tv_sec - start_time.tv_sec;	
	nsec = end_time.tv_nsec - start_time.tv_nsec;
	d_sec = (double)sec + (double)nsec / (1000*1000*1000);
	printf("time:%f\n", d_sec);
	
	delete(pg);
}
