#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <string.h>


using namespace std;

struct edge//ребро
{
	char str;//начало
	char end;//конец
	double heft;//вес
};

bool cmp(edge first, edge second)//сравнение элементов
{
	return first.heft < second.heft;
}

class Graph//граф
{
private:
	vector <edge> graph;//граф
	vector <char> result_way;//конечный путь
	vector <char> viewingtop;//просмотренные вершины
	char source;//начальна¤ вершина
	char finish;//конечна¤

public:
	Graph()//инициализаци¤ графа
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
		getline(cin, tmp);
		source = tmp[0];
		finish = tmp[2];
		while (getline(cin, tmp) && tmp != "")
		{
			edge elem;
			elem.str = tmp[0];
			elem.end = tmp[2];
			elem.heft = stod(tmp.substr(4));
			graph.push_back(elem); //добавление ребра
		}
		sort(graph.begin(), graph.end(), cmp);//сортируем элементы
		for (auto i : graph) {
			out << "    " << i.str << " -> " << i.end << " [label=" << i.heft << "];\n";
		}
		out << "}\n";
		system("g++ ./Source/visual.cpp -o show");
		delete[] cur;
	}

	bool isViewing(char value)//проверка на просомтренную вершину
	{
		for (size_t i = 0; i < viewingtop.size(); i++)
			if (viewingtop.at(i) == value)
				return true;
		return false;
	}

	void initSearch()//начало поиска
	{
		if (source != finish)
			Search(source);
	}

	bool Search(char value)//поиск мин пути
	{
		if (value == finish)//если вершина равна конечной
		{
			result_way.push_back(value);//добавл¤ем в result
			return true;
		}
		viewingtop.push_back(value);//если нет то добавл¤ем в список просомтренных вершин
		for (size_t i(0); i < graph.size(); i++)//проходим по графу
		{
			if (value == graph.at(i).str)//если веришна равна начальной вершине ребра
			{
				if (isViewing(graph.at(i).end))//если конец ребра просмотрен то переходим к следующей итерации цикла
					continue;
				result_way.push_back(graph.at(i).str); //иначе помещаем начало ребра в result
				bool flag = Search(graph.at(i).end); //запускаем поиск по конечной вершине
				if (flag)//если нашли нужный путь
					return true;
				result_way.pop_back();//иначе удаляем рассматриваемую вершину из result
			}
		}
		return false; //если путь не найден
	}

void Print()//печать результата
	{
	cout << "ћинимальный путь:";
	for (size_t i(0); i < result_way.size(); i++)
		cout << result_way.at(i);
	cout << endl;
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