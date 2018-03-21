
/***************************************************

	Srinidhi Goud Myadaboyina
	N12449860	
	Lab 1


***************************************************/





#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <iterator>
#include <sstream>
using namespace std;
 struct module{
 	int val;
 	vector<pair<string,int> > definitions;
 	vector<pair<string,int> > symbols;
 	vector<pair<string, int> > prog;
 };

int main() {

	string s;
	vector<string> warnings;
	map<string, int> symbol_map;
	map<string, pair<int,int> > symbol_list;
	int Nmodules;
	cin>>s;
	stringstream sss(s);
	sss >> Nmodules;
	//Nmodules = stoi(s);
	module code[Nmodules];
	int count = 0;
	//Reading the inputs 
	for(int i=0;i<Nmodules;i++) {
		code[i].val = count;
		int def;
		cin>>s;
		stringstream ss1(s);
		ss1 >> def;
		//def = stoi(s);
		while(def){
			string s1;
			cin>>s;
			cin>>s1;
			stringstream ss2(s1);
			
			//int loc = stoi(s1);
			int loc;
			ss2 >> loc;
			code[i].definitions.push_back(make_pair(s,loc));
			symbol_list[s].first++;
			def--;
		}
		cin>>s;
		stringstream ss3(s);
		ss3 >> def;
		//def = stoi(s);
		while(def){
			cin>>s;
			code[i].symbols.push_back(make_pair(s,1));
			def--;
		}
		cin>>s;
		stringstream ss4(s);
		ss4 >> def;
		//def = stoi(s);
		int store_def=def;
		while(def){
			string s1;
			cin>>s;
			cin>>s1;
			//int loc = stoi(s1);
			stringstream ss5(s1);
			int loc;
			ss5 >> loc;
			code[i].prog.push_back(make_pair(s,loc));
			def--;
		}
		count+=store_def;
	}
	count = 0;
	cout<<"Symbol Table"<<endl;
	//Pass 1
	for(int i=0;i<Nmodules;i++){
		for(int j=0;j<code[i].definitions.size();j++){
			map<string,int>:: iterator itsi = symbol_map.find(code[i].definitions[j].first);
			if(itsi==symbol_map.end()){
				symbol_list[code[i].definitions[j].first].second = i;
				if(code[i].definitions[j].second>=code[i].prog.size()){
					symbol_map[code[i].definitions[j].first] = count+0;
					cout << code[i].definitions[j].first << " => " << symbol_map[code[i].definitions[j].first] << '\t';
					cout<< "Error: Symbol "<<code[i].definitions[j].first<<"'s definition exceeds module's size. Allocated relative address as 0"<<endl;
					if(symbol_list[code[i].definitions[j].first].first>1) cout <<"Error: Symbol "<<symbol_map[code[i].definitions[j].first]<<" is multiply defined. It has been assigned the initial loc"<<endl;
				}
				else {
					symbol_map[code[i].definitions[j].first] = count+code[i].definitions[j].second;
					if(symbol_list[code[i].definitions[j].first].first>1) cout << code[i].definitions[j].first << " => " << symbol_map[code[i].definitions[j].first] << '\t' <<"Error: Symbol "<<symbol_map[code[i].definitions[j].first]<<" is multiply defined. It has been assigned the initial loc"<<endl;
					else cout << code[i].definitions[j].first << " => " << symbol_map[code[i].definitions[j].first] << endl;
				}
			}
		}
		count+=code[i].prog.size();
	}
	count = 0;
	//Pass 2
	cout<<endl<<"Memory map"<<endl;
	for(int i=0;i<Nmodules;i++){
		cout<<"+"<<count<<endl;
		int j=0;
		for(vector<pair<string,int> > :: iterator it=code[i].prog.begin();it!=code[i].prog.end();it++){
			int temp = (it->second)%1000;
			if(it->first == "A"){
				if(temp>=800){
					int temp_new = it->second;
					it->second -= temp;
					cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<'\t';
					cout<<"Error: The Absolute address "<<temp_new<<" exceeds the size of the machine. It is assigned the value of 0"<<endl;
				}
				else cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<endl;
			}
			else if(it->first == "R"){
				if(temp>=code[i].prog.size()) {
					int temp_new = it->second;
					it->second -= temp;
					cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<'\t';
					cout<<"Error: The Relative address "<<temp_new<<" exceeds the size of the module. It is assigned the value of 0"<<endl;

				}
				else {
					it->second += count;
					cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<endl;
				}
			}
			else if(it->first == "E"){
				if(temp>=code[i].symbols.size()) {
					cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<'\t';
					cout<<"Error: the external address "<<it->second<<" is too large to reference an entry in the use list. It is now considered as Immediate"<<endl;
				}
				else {
					map<string,int>:: iterator its = symbol_map.find(code[i].symbols[temp].first);
					if(its!=symbol_map.end()){
						symbol_list[its->first].first=0;
						code[i].symbols[temp].second = 0;
						it->second += its->second-temp;
						cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<endl;
					} 
					else {
						it->second -= temp;
						cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<'\t';
						code[i].symbols[temp].second = 0;
						cout<<"Error: Symbol "<<code[i].symbols[temp].first<<" is not defined. It is assigned the value 0"<<endl;
					}
				}
			} 
			else cout<<j<<":"<<'\t'<<it->first<<'\t'<<it->second<<endl;
			
			j++;
		}
		for(int z=0;z<code[i].symbols.size();z++){
			if(code[i].symbols[z].second) {
				ostringstream oss;
				string ss;
				oss << "Warning: In module "<< i <<" "<< code[i].symbols[z].first << " appeared in the use list but was not actually used";
				ss = oss.str();
				warnings.push_back(ss);
			}
		}
		count+= j;
		if(i==Nmodules-1){
			for(map<string,pair<int,int> >:: iterator it = symbol_list.begin();it!=symbol_list.end();it++){
				if(it->second.first){
					ostringstream oss;
					string ss;
					oss << "Warning: "<< it->first << " was defined in module "<< it->second.second << " but never used";
					ss = oss.str();
					warnings.push_back(ss);
				}
			}
		}

	}	
	cout<<endl;
	//Print the warnings
	for(int i=0;i<warnings.size();i++){
		cout<<warnings[i]<<endl;
	}
	return 0;
}