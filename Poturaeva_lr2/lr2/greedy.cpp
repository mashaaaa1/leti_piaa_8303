#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <string.h>

using namespace std;

struct edge//ðåáðî
{
	char str;//íà÷àëî
	char end;//êîíåö
	double heft;//âåñ
};

bool cmp(edge first, edge second)//ñðàâíåíèå ýëåìåíòîâ
{
	return first.heft < second.heft;
}

class Graph//ãðàô
{
private:
	vector <edge> graph;//ãðàô
	vector <char> result;//êîíå÷íûé ïóòü
	vector <char> viewingtop;//ïðîñìîòðåííûå âåðøèíû
	char source;//íà÷àëüíàÿ âåðøèíà
	char finish;//êîíå÷íàÿ

public:
	Graph()//èíèöèàëèçàöèÿ ãðàôà
	{
		ofstream out;
		char* cur = new char[200];
		ifstream dir;
		dir.open("path");
		dir >> cur;
		strcat(cur, "/Source/graph.dot");
		out.open(cur);
		out.open(cur, ofstream::app);
		out.clear();
		out << "digraph MyGraph {\n";
		string tmp;
                getline(cin,tmp);
                source=tmp[0];
                finish=tmp[2];
                while (getline(cin,tmp) && tmp!="")
                {
                        edge elem;
                        elem.str = tmp[0];
                        //if(!(cin >> elem.end))
                        //      break;
                        //if(!(cin >> elem.heft))
                        //      break;
                        elem.end=tmp[2];
                        elem.heft=stod(tmp.substr(4));
                        graph.push_back(elem); //добавление ребра
                }
		sort(graph.begin(), graph.end(), cmp);//ñîðòèðóåì ýëåìåíòû
		for (auto i : graph) {
			//cout << i.str << " " << i.end << " " << i.heft << endl;
			out << "    " << i.str << " -> " << i.end << " [label=" << i.heft << "];\n";
		}
		out << "}\n";
		system("g++ ./Source/visual.cpp -o show");
		delete[] cur;
	}

	bool isViewing(char value)//ïðîâåðêà íà ïðîñîìòðåííóþ âåðøèíó
	{
		for (size_t i = 0; i < viewingtop.size(); i++)
			if (viewingtop.at(i) == value)
				return true;
		return false;
	}

	void initSearch()//íà÷àëî ïîèñêà
	{
		if (source != finish)
			Search(source);
	}

	bool Search(char value)//ïîèñê ìèí ïóòè
	{
		if (value == finish)//åñëè âåðøèíà ðàâíà êîíå÷íîé
		{
			result.push_back(value);//äîáàâëÿåì â result
			return true;
		}
		viewingtop.push_back(value);//åñëè íåò òî äîáàâëÿåì â ñïèñîê ïðîñîìòðåííûõ âåðøèí
		for (size_t i(0); i < graph.size(); i++)//ïðîõîäèì ïî ãðàôó
		{
			if (value == graph.at(i).str)//åñëè âåðèøíà ðàâíà íà÷àëüíîé âåðøèíå ðåáðà
			{
				if (isViewing(graph.at(i).end))//åñëè êîíåö ðåáðà ïðîñìîòðåí òî ïåðåõîäèì ê ñëåäóþùåé èòåðàöèè öèêëà
					continue;
				result.push_back(graph.at(i).str); //èíà÷å ïîìåùàåì íà÷àëî ðåáðà â result
				bool flag = Search(graph.at(i).end); //çàïóñêàåì ïîèñê ïî êîíå÷íîé âåðøèíå
				if (flag)//åñëè íàøëè íóæíûé ïóòü
					return true;
				result.pop_back();//èíà÷å óäàëÿåì ðàññìàòðèâàåìóþ âåðøèíó èç result
			}
		}
		return false; //åñëè ïóòü íå íàéäåí
	}

void Print()//ïå÷àòü ðåçóëüòàòà
	{
		cout<<"Минимальный путь:";
		for (size_t i(0); i < result.size(); i++)
			cout << result.at(i);
		cout<<endl;
	}
};

int main()
{
	Graph element;
	element.initSearch();
	element.Print();
	system("g++ ./Source/visual.cpp -o show");
	return 0;
}
