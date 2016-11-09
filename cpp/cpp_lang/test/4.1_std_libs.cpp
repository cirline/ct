#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<list>
#include<map>
#include<unordered_map>
#include<iterator>
#include<algorithm>
#include<set>

using namespace std;

int main(void)
{
	/*
	 * string
	 */
	string s1 = "string1";
	string s2 = "string2";

	string compose = s1 + s2;		/* compose */
	cout << compose << endl;

	string substr = compose.substr(1, 4);
	cout << substr << endl;

	string replace = compose.replace(0, 3, "hello world");
	cout << replace << endl;

	compose[0] = toupper(compose[0]);
	cout << compose << endl;

	string ask = "yes";
	if(ask == "yes")
		cout << "ask is yes" << endl;
	else
		cout << "ask not yes" << endl;

#if 0
	string in;

	cout << "type a string: ";
	cin >> in;

	cout << "input is :" << in << endl;
	cout << "type a line: ";
	getline(cin, in);	/* clear previous '\n' */
	getline(cin, in);	/* get a line, the line will discard '\n' */
	cout << "get a line: " << in << endl;

	char c;
	while((cin >> c) && c != '.')		/* cin will discard whitespace */
		cout << "c = " << c << endl;
	cout << "cin end\n";
	while(cin.get(c) && c != '.')		/* cin.get dot not discard whitespace */
		cout << "c = " << c << endl;
	cout << "cin.get end\n";
#endif

	/*
	 * vector
	 * a sequence of elements, the elements are stored contiguously
	 */
	vector<int> vi = {1, 2, 3, 4, 5, 6};
	vi.push_back(9);	/* add new element and update size */
	for(auto x: vi)
		cout << "range-for vi = " << x << endl;

	vector<int> vc = vi;	/* elements are copied */
	for(int i = 0; i <= vc.size(); i++)
		try {
			cout << "for vc[" << i << "] = " << vc.at(i) << endl;	/* at() will throw out_of_range
										   if doesn't catch it, program will terminate */
		} catch(out_of_range) {
			cerr << "for vc[" << i << "] = " << "out of range" << endl;
		}

	/*
	 * list
	 * doubly-linked list
	 * -> list1 <--> list2 <--> list3 <--> ... <-
	 */
	list<int> li = {1, 2, 3, 4, 5, 6};
	list<int>::iterator li_itor = li.begin();

	/*
	 * map
	 */
	map<string, int> telebook = {
		{"name1", 123},
		{"name2", 456},
		{"name3", 789},
	};
	int name3_num = telebook["name3"];
	cout << "name3 number = " << name3_num << endl;

	/*
	 * unordered_map
	 * the unodered containers are optimized for lookup by using hash tables
	 */
	unordered_map<string, int> ud_telebook = {
		{"name1", 123},
		{"name2", 456},
		{"name3", 789},
	};
	cout << "name3 number = " << ud_telebook["name2"] << endl;

	/*
	 * standard containers basic operations:
	 * begin() & end(), get first and last iterator
	 * push_back(), add new element
	 * size(), container elements number
	 */
	vector<int> vi2;
	vi2.push_back(5);
	vi2.push_back(9);
	vi2.push_back(1);
	vi2.push_back(6);
	for(auto x: vi2)
		cout << "vi2 = " << x << endl;
#if 0
	sort(vi2.begin(), vi2.end());
	sort(vi2);
	for(auto x: vi2)
		cout << "vi2 = " << x << endl;
#endif
	/*
	 * iterator
	 */
	string find_s = "hello world";
	string::iterator i;
	for(i = find_s.begin(); i != find_s.end(); i++)
		if(*i == 'l')
			cout << "find 'l'" << endl;

	/*
	 * iostream_iterator
	 */
	string from = "datafrom";
	string to = "datato";
//	cout << "input from and to: ";
//	cin >> from >> to;
	cout << "from: " << from << endl;
	cout << "to: " << to << endl;
	ifstream is {from};
	istream_iterator<string> ii {is};
	istream_iterator<string> eos {};
	ofstream os {to};
	ostream_iterator<string> oo {os, "\n"};
#if 0
	vector<string> vs {ii, eos};
	for(auto x: vs)
		cout << "vs = " << x << endl;

	sort(vs.begin(), vs.end());
	unique_copy(vs.begin(), vs.end(), oo);
#else
	/*
	 * don't need keep the store duplicates
	 */
	set<string> ss {ii, eos};
	copy(ss.begin(), ss.end(), oo);
#endif

	/*
	 * algorithm
	 */
	string str1 = "hello world";
	string::iterator si = find(str1.begin(), str1.end(), 'o');
	cout << "si = " << *si << endl;

	si = find_if(str1.begin(), str1.end(), [](char c){return c == 'e'; });
	cout << "si = " << *si << endl;

	int l_count = count(str1.begin(), str1.end(), 'l');
	cout << "count('l') = " << l_count << endl;

	int o_count = count_if(str1.begin(), str1.end(), [](char &c){return c == 'o'; });
	cout << "count('o') = " << o_count << endl;

	/*
	 * string cannot use std::replace ? no, must use prefix std::
	 */
	std::replace(str1.begin(), str1.end(), 'o', '1');
	cout << "str(o->1) = " << str1 << endl;

	replace_if(str1.begin(), str1.end(), [](char &c){return c == '1';}, 'o');
	cout << "str(0->o) = " << str1 << endl;

	vector<int> odd = {1, 3, 5, 7, 9};
	vector<int> even = {0, 2, 4, 6, 8};
	vector<int> both(16);
	merge(odd.begin(), odd.end(), even.begin(), even.end(), both.begin());
	for(auto x: both)
		cout << " " << x;
	cout << endl;

	return 0;
}
