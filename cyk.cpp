#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include<cctype>
#include <set>
#include<algorithm>


using namespace std;

#define MAX_TERMINALS 10
#define MAX_VAR 20

string rules[MAX_VAR][MAX_TERMINALS];	//array to store the grammar rules
int number_of_rules = 0;	//number of rules/variables
int rhs_len[MAX_VAR];	//length of each rule

//utility function to print on console and store in output file
void print_values(string m, ofstream& fout)
{
	for (int i = 0; i < m.length(); i++)
	{
		if (i == m.length() - 1)
		{
			cout << m[i];
			fout << m[i];
		}
		else
		{
			cout << m[i] << ",";
			fout << m[i] << ",";
		}

	}
}

//utility function to concatenate the strings
string concatenate_strings(string a, string b)
{
	string temp = a;
	int l1 = b.length();
	int l2 = temp.length();
	for (int i = 0; i < l1; i++)
	{
		if (temp.find(b[i]) > temp.length())
		{
			temp += b[i];
		}
	}
	return temp;
}

//utility function to print sets
template <typename T>
void print_set(set<char> my_set)
{
	for (auto it = my_set.begin(); it != my_set.end(); ++it)
	{
		cout << *it << " ";
	}
}

//utility function to check whether a variable can generate a given string of variables or not
bool can_generate(string str, string rules[MAX_VAR][MAX_TERMINALS], int r, int rhs[MAX_TERMINALS])
{
	bool flag = false;

	for (int i = 0; i < r; i++)
	{
		for (int j = 1; rules[i][j]!=""; j++)
		{
			if (rules[i][j] == str)
			{
				flag = true;
				return flag;
			}
		}
	}

	return flag;
}

//function to find the possible combinations of a particular entry in table, after checking whether it can be generated
//then returning the generated combos of variables
string get_combinations(string str1, string str2,int number_of_rules, string rules[MAX_VAR][MAX_TERMINALS],int rhs_len[MAX_VAR])
{
	string result = "";
	string temp = "";
	int l1 = str1.length();
	int l2 = str2.length();
	bool check = false;

	for (int i = 0; i < l1; i++)
	{
		for (int j = 0; j < l2; j++)
		{
			temp = "";
			temp = temp + str1[i] + str2[j];

			for (int t = 0; t < number_of_rules; t++)
			{
				for (int u = 1; rules[t][u] != ""; u++)
				{
					//check = can_generate(temp, rules, number_of_rules, rhs_len);
					if (rules[t][u]==temp)
					{
						result = concatenate_strings(result, rules[t][0]);

					}
				}
			}
			
			

		}
	}
	return result;
}

//utility function to print the rules/grammar we have read in rules [][] array
void print_grammar(string rules[][MAX_TERMINALS], int n1 ,int rhs[MAX_VAR])
{
	for (int i = 0; i < n1; i++)
	{
		cout << rhs[i] << " ";
		for (int j = 0; j < rhs[i]; j++)
		{
			cout  << rules[i][j] << " ";
		}
		cout << endl;
	}
}

//function to check whether start is contained in top_value or not
bool is_member(string top_rule, string start)
{
	int l1 = top_rule.length();
	int l2 = start.length();

	for (int i = 0; i < l1; i++)
	{
		for (int j = 0; j < l2; j++)
		{
			string t = "";
			t = t + top_rule[i];
			if (t == start)
			{
				return true;
			}
		}
	}
	return false;
}

int main()
{
	cout << "console output is for debugging" << endl << endl;
	ofstream fout;
	fout.open("table.txt");
	if (!fout)
	{
		cout << "error opening table.txt file" << endl;
		return -1;
	}

	string rules[MAX_VAR][MAX_TERMINALS];	//to store the grammar
	int number_of_rules = 0;	//count the number of rules, this will also tell us how many variables
	int r = 0;	//iterator for rhs_len i.e. the array to store length of rules

	string input_string;	//string to test whether its a member or not
	int input_str_len = 0;	//lentgh of to be tested string

	string table[MAX_TERMINALS][MAX_TERMINALS];	//the final table

	
	char buffer[100];	//buffer to read the files
	string temp1;
	

	set<string> set_of_variables;
	set<char> set_of_terminals;

	string res, pr;


	string buff2;
	ifstream fin;
	//ifstream fin2;

	fin.open("CFG.txt");

	if (!fin)
	{
		cout << "error opening file CFG.txt" << endl;
		return -1;
	}

	while (fin.getline(buffer, 100)) 
	{
		temp1 = buffer;
		//cout << temp1 << endl;

		int len = temp1.length();
		//cout << len << endl;
		temp1.erase(remove_if(temp1.begin(), temp1.end(), ::isspace), temp1.end()); //removing space from end of buffer


		//lhs of rules
		int index = temp1.find("->"); //as the rhs would be next to this arrow symbol
		rules[number_of_rules][0] = temp1.substr(0, index);
		string v= temp1.substr(0, index);
		set_of_variables.insert(v); //storing the variables
		int a; //iterator

		//rhs of rules
		temp1 = temp1.substr(index + 2, temp1.length());	//storing rhs in temp1
		for (a = 0; temp1.length()!=0; a++)
		{
			
			//cout << buffer[a] << endl;
			int or_sign = temp1.find("|"); //finding the OR sign to store one rule per string index
			if ( or_sign > temp1.length()) //if it is the last part of RHS
			{
				rules[number_of_rules][a + 1] = temp1;
				temp1 = "";
			}
			else
			{
				rules[number_of_rules][a + 1] = temp1.substr(0, or_sign);
				temp1 = temp1.substr(or_sign + 1, temp1.length()); //move it ahead
			}
		}
		rhs_len[r] = a + 1;	//storing length of rhs, helps in printing/debugging
		r++;//iterator
		number_of_rules++; //storing the total number of rules



	}
	fin.close();
	//number_of_rules++;

	//printing the contents of CFG.txt
	print_grammar(rules, number_of_rules, rhs_len);

	//reading string from string.txt
	fin.open("string.txt");

	while (fin.getline( buffer, 100))
	{
		input_string = buffer;
	}

	cout << "input string: " << input_string << endl;
	fin.close();
	input_str_len = input_string.length();	//length of input_string to be tested

	//now start to make the table
	temp1 = '\0';
	string temp2;
	int p = 0;	//iterator
	int t = 0;	//iterator
	
	for (int q = 0; q < input_str_len; q++)
	{
		temp1 = "";
		temp2 = "";
		temp1 += input_string[q];	//storing string element by element

		//storing the lowest row, which would be the diagonal
		for (p = 0; p < number_of_rules; p++)
		{
			for (t = 1; t<rhs_len[p]+1; t++)
			{
				string test = rules[p][t];
				if (test == temp1)
				{
					temp2 =temp2+ rules[p][0];
					
				}
			}

		}
		table[q][q] = temp2;
		
	}

	
	//filling up rest of the table (upper triangular) to check and store the combos
	int n, l,k,j;	//iterators
	string result;
	for (k = 1; k <  input_string.length(); k++)
	{
		for (j = k; j <  input_string.length(); j++)
		{
			result = "";
			for (l = j - k; l < j; l++)
			{
				//cout << "table[j-k][l]" << table[j - k][l] << endl;
				//cout << "table[l+1][j]" << table[l+1][j] << endl;
				//pr = gen_comb(table[j - k][l], table[l + 1][j],number_of_rules,rules);
				pr = get_combinations(table[j - k][l], table[l + 1][j], number_of_rules, rules, rhs_len);	//check the combinations
				result = concatenate_strings(result, pr);
			}
			table[j - k][j] = result;
		}
	}

	cout << endl;


	//printing the table in right format for debugging
	//the following snippet of code also stores the table in table.txt
	for (int i = 0;i < input_string.length(); i++)
	{
		n = 0;
		l = input_string.length() - i - 1;
		
		for (int j = l; j < input_string.length(); j++)
		{
			if (table[n][j] != "")
			{
				cout << "{";
				fout << "{";
				print_values(table[n++][j],fout);
				cout << "}\t";
				fout << "}\t";
			}
			else
			{
				cout << "-\t";
				fout << "-\t";
				n++;
			}
			
			
		}
		
		cout << endl;
		fout << endl;
	}
	fout.close();


	//checking and storing result
	fout.open("result.txt");
	if (!fout)
	{
		cout << "Error opening result.txt" << endl;
	}

	string top_value_of_table = table[0][input_str_len-1]; 
	string start = rules[0][0];
	cout << endl;
	cout << "top is:" << top_value_of_table << endl;;
	cout << "table top rule: " << start << endl;

	//to check whether the input string is member or not
	if (is_member(top_value_of_table, start))
	{
		cout << endl;
		cout << "Member" << endl;
		fout << "Member" << endl;
	}
	else
	{
		cout << "Not Member" << endl;
		fout << "Not Member" << endl;
	}

	fout.close();


	
	system("pause");
	return 0;
}

