#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Neighbour { //структруа для хранения соседей
	char symbol; //вершина
	int bandwidth; //введнная пропускная способность
	int cur_bandwidth; //промежуточная пропускная способность
	int final_bandwidth;//итоговая пропускная способность
	Neighbour(char symbol, int bandwidth) : symbol(symbol), bandwidth(bandwidth), cur_bandwidth(bandwidth) {}
	Neighbour(char symbol) : symbol(symbol) {}
};

struct Top { //структура для хранения вершины
	char symbol; //вершина
	char prevTop; //пред вершина
	vector <Neighbour> neighbours; //вектор соседей
	Top() {}
	Top(char symbol) : symbol(symbol) {}
};

vector <Top> tops, viewingTop; //вектор вершин и просмотренных вершин

bool cmp_top(const Top& first, const Top& second) //компаратор для вершин
{
	if (first.symbol < second.symbol)
		return true;
	else
		return false;
}

bool cmp_neigh(const Neighbour& first, const Neighbour& second) //компаратор для соседей
{
	if (first.symbol < second.symbol)
		return true;
	else
		return false;
}

bool isExist(vector<Top> vector, char tmp) //проверяет вхождение веришны в вектор
{
	for (size_t i = 0; i < vector.size(); i++)
		if (vector[i].symbol == tmp)
			return true;
	return false;
}

int findIndexTop(vector<Top> vector, char tmp) //находит индекс вершины в векторе
{
	for (size_t i = 0; i < vector.size(); i++)
		if (tmp == vector[i].symbol)
			return i;
	return -1;
}

int findIndexNeigh(Top top, char tmp) //находит индекс соседа в векторе
{
	for (size_t i = 0; i < top.neighbours.size(); i++) {
		if (top.neighbours[i].symbol == tmp)
			return i;
	}
	return -1;
}
void init_Graph(size_t N) {
	char  curTop, endTop; //начальная и конечная вершины
	int bandwidth, index; //пропускная способность
	for (size_t i = 0; i < N; i++) {         
		cin >> curTop >> endTop >> bandwidth;
		Top first;//нач вершина
		if (!isExist(tops, curTop)) {                         // если начальной вершины ещё нет в векторе вершин
			first = Top(curTop);                                      // создаём её
			Neighbour neighbour = Neighbour(endTop, bandwidth);       // и её соседа
			first.neighbours.push_back(neighbour);                    // добавляем соседа в вектор соседей
			tops.push_back(first);                                    // добавляем вершину в вектор вершин
		}
		else {                                                         // если начальная вершина уже есть в векторе вершин
			Neighbour neighbour = Neighbour(endTop, bandwidth);       //  создаём её соседа
			index = findIndexTop(tops, curTop);						// находим индекс начальной вершины в векторе вершин
			tops[index].neighbours.push_back(neighbour);              // добавляем соседа в вектор соседей 
		}                                                             
		if (!isExist(tops, endTop)) {                          // если конечной вершины нет в векторе вершин
			Top second = Top(endTop);                          // создаем ее и добавляем в вектор вершин
			tops.push_back(second);                                 
		}
	}
}

int maxFlowCount(vector <Top> tops, Top estr)  //  считает максимальный поток в сквозном пути
{   
	cout << "Calculating the maximum flow." << endl;
	int flow, min, indexPrev, indexCur; // промежуточный поток, максимальный поток, индекс текущей и предыдущей вершины
	min = 10000;//недостижимый минимум
	Top cur = estr;                        // текущая вершина - сток
	while (cur.prevTop != '0') {  // пока не дошли до истока
		cout << "Current min:" << min<<endl;
		indexPrev = findIndexTop(tops, cur.prevTop);               // находим индекс вершины предыдущей текущей
		indexCur = findIndexNeigh(tops[indexPrev], cur.symbol);		// находим среди соседей предыдыщей вершины текущую вершину
		flow = tops[indexPrev].neighbours[indexCur].cur_bandwidth;			//вычисляем поток через это ребро
		cout << "Current flow:" << cur.prevTop << "->" << cur.symbol << ' ' << flow << endl;
		if (flow < min) {											//если он меньше минимума
			min = flow;											// то минимум равен текущему потоку
			cout << "New min:" << min << endl;
		}
		cur = tops[indexPrev];                                           // текущей вершиной становится предыдущая вершина
	}   
	cout << "The end of the calculating the maximum flow." << endl;
	return min;       // возвращаем минимальную пропускную способность сквозного пути
}

void recountFlow(vector <Top>& tops, Top estr, int maxFlow)  // производит пересчет потока по сквозному пути
{     
	cout << "Start of recalculation flow:" << endl;
	int indexCur, indexPrev, indexNeighCur, indexNeighPrev; //индекс текущей и предыдущей вершины, соседа текущей и сосеода предыдущей вершины
	Top cur = estr;                                      // текущая вершина - сток
	while (cur.prevTop != '0') {                                              // пока не дошли до истока
		cout << "Current top:" << cur.symbol << endl;
		indexCur = findIndexTop(tops, cur.symbol);                          // находим индекс текущей вершины 
		indexPrev = findIndexTop(tops, cur.prevTop);                     // находим индекс предыдущей веришны
		indexNeighCur = findIndexNeigh(tops[indexCur], cur.prevTop);		// находим среди соседей текущей вершины предыдущую вершину
		indexNeighPrev = findIndexNeigh(tops[indexPrev], cur.symbol);		// находим среди соседей предыдыщей вершины текущую вершину
		if (indexNeighCur == -1) {                                           // если из текущей вершины нет ребра до предыдущей вершины
			cout << "The path from " << cur.symbol << " to " << cur.prevTop << " was not found, creating an edge." << endl;
			Neighbour neighbour = Neighbour(cur.prevTop);           //создаем это ребро
			neighbour.cur_bandwidth = maxFlow;                                      // пропускная способность этого ребра инициализируется максимальной 
			neighbour.bandwidth = 0;                                       // его начальная пропускная способность равна 0
			tops[indexCur].neighbours.push_back(neighbour);                
		}
		else {          
			cout << "The path from " << cur.symbol << " to " << cur.prevTop << " was found." << endl;
			cout << "New bandwidth current top=" << tops[indexCur].neighbours[indexNeighCur].cur_bandwidth << '+' << maxFlow << '=' << tops[indexCur].neighbours[indexNeighCur].cur_bandwidth + maxFlow << endl;
			tops[indexCur].neighbours[indexNeighCur].cur_bandwidth += maxFlow; // иначе добавляем к пропускной способности ребра из текущей вершины величину максимальной
		}
		cout << "New bandwidth previous top=" << tops[indexPrev].neighbours[indexNeighPrev].cur_bandwidth << '-' << maxFlow << '=' << tops[indexPrev].neighbours[indexNeighPrev].cur_bandwidth - maxFlow << endl;
		tops[indexPrev].neighbours[indexNeighPrev].cur_bandwidth -= maxFlow; //пропускная способность предыдущей вершины до текущей уменьшается на максимальную
		cur = tops[indexPrev];                                  //текущая вершина теперь - предыдущая
	}
}

void Priority(int& index, Top cur) { //рассчитываем приоитет вершин
	int priority, min; //мин приоритет, промежуточный приоритет
	bool flag; //флаг для определения расположения символа относительно начала алфавита
	for (size_t i = 0; i < cur.neighbours.size(); i++) {   // перебор соседей текущей вершины
		if (cur.neighbours[i].cur_bandwidth > 0 && !isExist(viewingTop, cur.neighbours[i].symbol)) { // если пропускная способность пути до соседа больше нуля и сосед не находится в векторе посещенных вершин
			priority = abs(cur.neighbours[i].symbol - cur.symbol);        //расчитываем приоритет                         
			if (index == 10000 || priority < min) {                                           //если это первая иттерация цикла или текущий приоритет меньше минимума
				min = priority;                                                                       // минимум равен приоритету
				index = i;                                                                         // индекс вершины с мин разницей
				if (cur.neighbours[i].symbol < cur.symbol)                                            // если сосед рассматриваемой вершины находится ближе к началу алфавита
					flag = true;                                                                         // ставим flag=true
				else                                                                                          // иначе помечаем flag=false
					flag = false;                                                                        
			}
			else if (priority == min && cur.neighbours[i].symbol < cur.symbol && flag == false) { // если приоритет равен минимуму, то нужно проверить расположение символа относительно начала алфавита
					index = i; 
					flag = true; //если сосед находится ближе к началу алфавита
			}
		}
	}
}

int FordFalk(char source, char estr) { //поиск максимального потока и пересчет пропускных способностей вершин
	int i = findIndexTop(tops, source);//находим индекс истока
	tops[i].prevTop = '0';//устанавливаем пред символ у истока
	Top cur = tops[i];        // текущая вершина - исток
	int maxFlow,finalMax = 0; //промежуточный макс поток и итоговый
	viewingTop.push_back(cur); //помещаем исток в вектор просмотренных вершин
	cout << "Viewing Tops:";
	for (size_t i = 0; i < viewingTop.size(); i++)
		cout << viewingTop[i].symbol<<' ';
	cout << endl;
	while (true) { //пока не найдем макс поток
		int index_min = 10000; //недостижимый минимум
		Priority(index_min, cur); //выбираем вершины по приоритету
		if (index_min != 10000) {// если нашли соседа
			cout << "Priority vertex:" << cur.neighbours[index_min].symbol << endl;
			i = findIndexTop(tops, cur.neighbours[index_min].symbol);				// находим его индекс в векторе вершин
			tops[i].prevTop = cur.symbol;                                    // предыдущая вершина соседа это текущая вершина
			cur = tops[i];                                                      // сосед становится текущей вершиной
			viewingTop.push_back(cur);											//помещается в вектор просмотренных вершин
			cout << "Viewing Tops:";
			for (size_t i = 0; i < viewingTop.size(); i++)
				cout << viewingTop[i].symbol << ' ';
			cout << endl;
			if (cur.symbol == estr) {                      // если дошли до стока
				cout << "The current vertex is a estuary." << endl;
				maxFlow = maxFlowCount(tops, cur);        // рассчитываем минимальную пропускную способность
				recountFlow(tops, tops[i], maxFlow);      // производим пересчет пропускных способностей пути
				cout << "Recount final maximum flow:" << finalMax << '+' << maxFlow << '=' << finalMax + maxFlow << endl;
				finalMax += maxFlow;                      // пересчитываем итоговый максимальный поток
				cout << "Viewing tops is cleared." << endl;
				viewingTop.clear();							//очищаем вектор просмотренных вершин
				i = findIndexTop(tops, source);        // текущей вершиной становится исток
				cur = tops[i];
				viewingTop.push_back(cur);
				cout << "Viewing Tops:";
				for (size_t i = 0; i < viewingTop.size(); i++)
					cout << viewingTop[i].symbol << ' ';
				cout << endl;
			}

		}
		else {   // если не нашли соседа
			if (cur.prevTop == '0') { // если текущая вершина исток, то выходим из цикла
				cout << "The end of the search priority." << endl;
				break;
			}
			else {
				cout << "Priority vertex was not found" << endl;
				i = findIndexTop(tops, cur.prevTop);// иначе откатываемся к предыдущей вершине
				cur = tops[i];
			}
		}
	}
	for (size_t i = 0; i < tops.size(); i++) {        // рассчитываем итоговые пропускные способности через все ребра 
		for (size_t j = 0; j < tops[i].neighbours.size(); j++) {
			tops[i].neighbours[j].final_bandwidth = tops[i].neighbours[j].bandwidth - tops[i].neighbours[j].cur_bandwidth;
		}
	}
	return finalMax; //возвращаем максимальный поток
}

int main()
{
	size_t N;
	char source, estr;
	cout << "Enter information about the graph:" << endl;
	cin >> N;//количество вершин
	cin >> source;//исток
	cin >> estr;//сток
	int max = 0; //макс поток
	init_Graph(N);// заполняем граф
	cout << "Beginning of the algorithm:" << endl;
	max = FordFalk(source, estr);//находим макс поток
	sort(tops.begin(), tops.end(), cmp_top);        //сортируем вершины графа                          
	for (size_t i = 0; i < tops.size(); ++i) {
		sort(tops[i].neighbours.begin(), tops[i].neighbours.end(), cmp_neigh); //сортируем соседей каждой вершины
	}
	cout << "Result:" << endl; //выводим результат работы
	cout << max << endl;
	for (size_t i = 0; i < tops.size(); i++) {
		for (size_t j = 0; j < tops[i].neighbours.size(); j++) {
			if (tops[i].neighbours[j].bandwidth != 0) {  // Если ребро было введено изначально,а не создано во время работы алгоритма
				cout << tops[i].symbol << " " << tops[i].neighbours[j].symbol << " ";
				if (tops[i].neighbours[j].final_bandwidth >= 0)
					cout << tops[i].neighbours[j].final_bandwidth<<endl;
				else
					cout << 0 << endl;
			}
		}
	}
	return 0;
}

