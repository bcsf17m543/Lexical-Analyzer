#include<iostream>
#include<fstream>
#include<string>
#include<cctype>
using namespace std;
int currstate = 0;
int lexStrt = 0;
int iwlfsv = -1;
int lfsv = -1;
int lineno = 0;
int temp = 0;
string ReadFile()
{
	string str;
	ifstream file("f1.txt");
	string temp;
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, str);
			temp = temp + str;
		}

		file.close();
	}

	else temp = "";
	return temp;
}
char nextChar(int &idx, string &mystr)
{

	char mychar = mystr[idx];
	return mychar;
}
void Skip(int& ptr)
{
	ptr = ptr + 1;
}
bool isFinal(int &ptr)
{
	if (ptr == 1 || ptr == 2 || ptr == 6 || ptr == 3 || ptr == 5)
	{
		return true;
	}
	else {
		return false;
	}
}

void ungetch(int& ptr)
{
	ptr = ptr - 1;
}
bool IsLexemeidentified(int& ptr)
{
	if (ptr != -1)
	{
		return true;
	}
	else
	{
		return false;
	}

}
string putLexeme(int &start, int& end, string& mystr)
{
	string tempo;
	/*tempo = mystr.substr(start, end);*/
	for (int i = start; i < end; i++)
	{
		tempo += mystr[i];
	}
	return tempo;


}
void putToken(int& start, int &end, int &lfsw, string &mystr)
{
	string str = putLexeme(start, end, mystr);
	string name, add_attribute;
	if (str == "int" || str == "for")
	{
		name = str;
		add_attribute = "keyword";
	}
	else
	{
		if (lfsw == 1)
		{
			name = str;

			add_attribute = "ID";
		}
		else if (lfsw == 2)
		{
			name = str;

			add_attribute = "Const";
		}
		else if (lfsw == 3)
		{
			name = str;

			add_attribute = "DL";
		}
		else if (lfsw == 5)
		{
			add_attribute = "Const";
			name = str;

		}
		else if (lfsw == 6)
		{
			add_attribute = "OP";
			name = str;

		}
	}

	ofstream outfile;
	outfile.open("lex.txt", ios_base::app);
	outfile << "< " << name << " , " << add_attribute << ">" << endl;
	outfile << endl;
}
void Reset(int& ptr, int& p2, int& p3, int& p4)
{
	ptr = -1;
	//p2=p4+1;
	p2 = p4;
	p4 = -1;
	p3 = 0;
}
void Fail()
{
	cout << "lexical error\n";
}
void Output()
{
	ifstream ofile("lex.txt");
	string out;
	if (ofile.is_open())
	{
		while (!ofile.eof())
		{
			getline(ofile, out);
			cout << out<<endl;
		}

	}
}
void getToken() //simulation of automata
{
	int forward = 0;//1
	string mystr = ReadFile();
	char c = ' ';
	while (c!='\0')
	{
		 c = nextChar(forward, mystr);
		// temp++;
		forward++;
		if (currstate == 0)
		{
			if (c == '_' || isalpha(c))
			{
				currstate = 1;
			}
			else if (c == '=')
			{
				currstate = 6;
			}
			else if (c == ';')
			{
				currstate = 3;
			}
			else if (isdigit(c))
			{
				currstate = 2;
			}
			else if (isspace(c))
			{
				Skip(lexStrt);
				if (c == '\n')
				{
					lineno++;
				}
			}
			else
			{
				Fail();
				break;
			}


		}
		else if (currstate == 1)
		{

			if (c == '_' || isalpha(c) || isdigit(c))
			{
				currstate = 1;
			}
			else
			{
				ungetch(forward);
				if (IsLexemeidentified(lfsv))
				{
					putToken(lexStrt, iwlfsv, lfsv, mystr);//changing
					Reset(lfsv, lexStrt, currstate, iwlfsv);
				}
			}

		}
		else if (currstate == 2)
		{

			if (c == '.')
			{
				currstate = 4;
			}
			else if (isdigit(c))
			{
				currstate = 2;
			}
			else
			{
				ungetch(forward);
				if (IsLexemeidentified(lfsv))
				{
					putToken(lexStrt, iwlfsv, lfsv, mystr);
					Reset(lfsv, lexStrt, currstate, iwlfsv);
				}
			}

		}
		else if (currstate == 3 || currstate == 6)
		{
			ungetch(forward);
			if (IsLexemeidentified(lfsv))
			{
				putToken(lexStrt, iwlfsv, lfsv, mystr);
				Reset(lfsv, lexStrt, currstate, iwlfsv);
			}
		}
		else if (currstate == 4)
		{

			if (isdigit(c))
			{
				currstate = 5;
			}
		}
		else if (currstate == 5)
		{

			if (isdigit(c))
			{
				currstate = 5;
			}
			else
			{
				/* ungetch and all steps */
				ungetch(forward);
				if (IsLexemeidentified(lfsv))
				{
					putToken(lexStrt, iwlfsv, lfsv, mystr);
					Reset(lfsv, lexStrt, currstate, iwlfsv);
				}
			}

		}
		if (isFinal(currstate))
		{
			iwlfsv = forward;//pehly forward tha
			lfsv = currstate;
		}


	}

}
void lexical_analyzer()
{
	getToken();
	Output();
}
int main()
{

	lexical_analyzer();
	return 0;
}