#include <iostream>
#include <vector>
#include <ctime>
using namespace std;



//Класс вставленного квадрата в виде координаты-размер квадрата
class Square {
public:
	int x;
	int y;
	int size;
	Square(int x, int y, int size) :x(x), y(y), size(size) {}
	void print(int Coeff) {
		cout << x * Coeff + 1 << " " << y * Coeff + 1 << " " << size * Coeff << endl;
	}
};

vector<Square> listSquare;     // промежуточный список квадратов
vector<Square> resultSquare;

bool start_square(int** square, int size, int& x, int& y) { //проверка можно ли вставить квадрат
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (square[i][j] == 0) {
				x = i;
				y = j;
				return true;
			}
	return false;
}

bool fill_square(int** square, int x, int y, int w, int size) { //проверка может ли существовать квадрат в переданных координатах
	if (x + w > size || y + w > size)
		return false;

	for (int i = x; i < x + w; i++)
		for (int j = y; j < y + w; j++)
			if (square[i][j] != 0)
				return false;

	return true;
}

void insert_square(int** square, int x, int y, int size, int val) { //вставка квадрата
	for (int i = x; i < x + size; i++)
		for (int j = y; j < y + size; j++)
			square[i][j] = val;
}

void delete_square(int** square, int x, int y, int size) { //удаление квадрата
	for (int i = x; i < x + size; i++)
		for (int j = y; j < y + size; j++)
			square[i][j] = 0;
}

int side_square(int x, int y, int size) { //изменение длины квадрата
	if (size - x < size - y)
		return size - x;
	else
		return size - y;
}

void backtracking(int** square, int size, int& minSq, int count) {
	if (minSq < count)//если дошли до минимума
		return;

	int x_start = 0, y_start = 0;

	if (start_square(square, size, x_start, y_start) == false) { //если квадрат можно вставить
		if (count - 1 < minSq) { //если нашли новый минимум
			minSq = count - 1;
			resultSquare = listSquare;
		}
	}
	else {
		int new_size = side_square(x_start, y_start, size); //иначе возвращаем длину квадрата
		if (new_size > size - 1) {
			new_size = size - 1;
		}
		for (int i = new_size; i > 0; i--) {
			if (fill_square(square, x_start, y_start, i, size)) {//если можем вставить квадрат
				insert_square(square, x_start, y_start, i, count);
				Square sq(x_start, y_start, i);//создаем новый квадрат
				listSquare.push_back(sq);
				backtracking(square, size, minSq, count + 1);//опять запускаем бэктрекинг
				listSquare.pop_back();//возвращаемся
				delete_square(square, x_start, y_start, i);//удаляем квадрат
			}
		}
	}
}

int main() {
	cout << "Enter square size: ";
	int N;
	cin >> N;
	int k = 0;
	int minSq = N*N + 1;//недостижимое значение количества квадратов
	unsigned int tmp1 = clock();
	for (int i = 2; i <= N; i++) { //уменьшение размера квадрата с помощью нахождения коэффицента
		if (N % i == 0) {
			k = N / i;
			N = i;
			break;
		}
	}


	int** square = new int* [N]; //создаем двумерный массив указателей и инициализиурем его нулями
	for (int i = 0; i < N; i++) {
		square[i] = new int[N];
		for (int j = 0; j < N; j++) {
			square[i][j] = 0;
		}
	}

	int size1 = (N + 1) / 2;
	int size2 = N - (N + 1) / 2;
	int count = 1;// в любом случае мы можем изначально вставить три квадрата в соответствии с его размерами
	insert_square(square, 0, 0, size1, count); count++;
	insert_square(square, 0, size1, size2, count); count++;
	insert_square(square, size1, 0, size2, count); count++;

	listSquare.push_back(Square(0, 0, size1));
	listSquare.push_back(Square(0, size1, size2));
	listSquare.push_back(Square(size1, 0, size2));

	backtracking(square, N, minSq, count);
	cout << minSq << endl;
	unsigned int tmp2 = clock();
	cout << "Time:"<<(tmp2 - tmp1)/1000.0 << endl;
	for (int i = 0; i < resultSquare.size(); i++) {
		resultSquare[i].print(k); //вывод результата работы программы на экран
	}

	for (int i = 0; i < N; i++)
		delete[] square[i];
	delete[] square;

	return 0;
}
