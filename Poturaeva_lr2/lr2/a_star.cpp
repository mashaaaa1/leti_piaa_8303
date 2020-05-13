#include <iostream> 
#include <vector> 
#include <string> 
#include <string.h> 
#include <cmath> 
#include <fstream> 
#include <cstdlib>
using namespace std;

struct edge     //ребро графа
{
	char str; //начало
	char end; //конец
	double heft;//вес
};

struct step     //возможные пути
{
	string path; //путь
	double length;//длина
};

class Graph //граф
{
private:
	vector <edge> graph; //изначальный граф
	vector <step> result; //конечный путь
	vector <char> viewingtop;
	char source; //начало графа
	char finish; //конец графа

public:
	Graph() //инициализация графа
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
			//	break;
			//if(!(cin >> elem.heft))
			//	break;
			elem.end=tmp[2];
			elem.heft=stod(tmp.substr(4));
			graph.push_back(elem); //добавление ребра
		}
		for (auto i : graph) {
			//cout << i.str << " " << i.end << " " << i.heft << endl;
			out << "    " << i.str << " -> " << i.end << " [label=" << i.heft << "];\n";
		}
		out << "}\n";
		system("g++ ./Source/visual.cpp -o show");
		delete[] cur;
		string buf = ""; //для ребра
		buf += source;
		for (size_t i(0); i < graph.size(); i++)
		{
			if (graph.at(i).str == source)
			{
				buf += graph.at(i).end;
				result.push_back({ buf, graph.at(i).heft });//добавляем все ребра начинающиеся с source
				buf.resize(1); //возвращаем размер 1
			}
		}
		viewingtop.push_back(source); //добавляем первую вершмну как просмотренную
	}

	size_t MinSearch() //возвращает индекс минимального элемента из непросмотренных
	{
		double min = 10000;//недостижимый минимум
		size_t tmp;//индекс мин элемента
		for (size_t i(0); i < result.size(); i++)//поиск мин элемента среди вершин result
		{
			if (result.at(i).length + abs(finish - result.at(i).path.back()) < min)//сравниваем значение с минимумом
			{
				if (isViewing(result.at(i).path.back()))//проверяем просмотрена вершина или нет
				{
					result.erase(result.begin() + i); //если уже просматривали конец ребра то удаляем этот элемент из result
				}
				else
				{
					min = result.at(i).length + abs(finish - result.at(i).path.back());//устанавливаем новый минимум
					tmp = i;//индекс элемента
				}
			}
		}
		return tmp;
	}

	bool isViewing(char val)//проверка на просомтренную вершину
	{
		for (size_t i = 0; i < viewingtop.size(); i++)
			if (viewingtop.at(i) == val)
				return true;
		return false;
	}

	void Search() //поиск мин пути
	{
		while (true)
		{
			size_t min = MinSearch();//запускаем поиск мин элемента
			if (result.at(min).path.back() == finish) //если нашли путь до конечной вершины
			{
				cout <<"Минимальный путь:"<< result.at(min).path<<endl;//выводим результат
				return;
			}
			for (size_t i(0); i < graph.size(); i++)//проходимся по элементам графа
			{
				if (graph.at(i).str == result.at(min).path.back())//если начало ребра равно концу мин ребра
				{
					string buf = result.at(min).path;//помещаем в buf мин элемент
					buf += graph.at(i).end;//доавляем конец текущего ребра
					result.push_back({ buf, graph.at(i).heft + result.at(min).length });//помещаем это в result
				}
			}
			viewingtop.push_back(result.at(min).path.back());//добавлем конечную вершину в просмотренные
			result.erase(result.begin() + min);//удаляем мин из result
		}
	}
};

int main()
{
	Graph elem;
	elem.Search();
	system("g++ ./Source/visual.cpp -o show");
	return 0;
}

