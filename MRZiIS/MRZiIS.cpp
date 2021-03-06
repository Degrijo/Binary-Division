// MRZiIS.cpp : Лабораторная работа 1 по МРЗвИС вариант №6. Алгоритм вычисления целочисленного частного пары 4-разрядных чисел делением без восстановления частичного остатка.
//Автор: Яценко Ярослав, гр.721701

#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <conio.h>
#include "windows.h"


using namespace std;
vector<bool> transfer(int number);
vector<bool> sum(vector <bool> a, vector <bool> b);
vector<bool> invert(vector<bool> a);
vector<vector<vector<bool>>> division(vector <bool> a, vector <bool> b);
vector<vector<bool>> shift(vector <bool> p, vector <bool> a);
int number_digit = 5;

int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Лабораторная работа 1 по МРЗвИС вариант №6. Алгоритм вычисления целочисленного частного пары 4-разрядных чисел делением без восстановления частичного остатка. Сделана Яценко Ярославом" << endl;
	cout << "Размер первого вектора" << endl;
	int size;
	cin >> size;
	cout << "Введите первый вектор (целые числа)" << endl;
	vector <int> a;
	for (int i = 0; i < size; i++) {
		int digit;
		cin >> digit;
		a.push_back(digit);
	}
	cout << "Размер второго вектора" << endl;
	cin >> size;
	cout << "Введите второй вектор (целый числа)" << endl;
	vector <int> b;
	for (int i = 0; i < size; i++) {
		int digit;
		cin >> digit;
		b.push_back(digit);
	}
	if (a.size() == b.size()) {
		vector<vector<vector<bool>>> table;
		for (int i = 0; i < (number_digit - 2 + a.size()) * 2; i++) {
			table.push_back({});
			for (int j = 0; j < number_digit * 2; j++) table[i].push_back({});
		}
		for (int i = 0; i < a.size(); i++) {
			vector<vector<bool>> p = division(transfer(a[i]), transfer(b[i]))[0];
			vector<vector<bool>> rez = division(transfer(a[i]), transfer(b[i]))[1];
			table[2*i][0] = p[0];
			table[2*i + 1][0] = rez[0];
			for (int j = 1; j < p.size()-1; j++) {
				if (j % 2) {
					table[i*2 + j - 1][j] = p[j];
					table[i*2 + j][j] = rez[j];
				}
				else {
					table[i*2 + j - 2][j] = p[j];
					table[i*2 + j - 1][j] = rez[j];
				}
			}
			table[(i + number_digit - 2)*2][p.size() - 1] = p[p.size() - 1];
			table[(i + number_digit - 2) * 2 + 1][rez.size() - 1] = rez[rez.size() - 1];
		}
		cout << "       " << "start" << "     ";
		for (int i = 0; i < int(table[0].size()/2)-1; i++) cout << "shift" << "     " << "adding" << "    ";
		cout << "finish" << endl;
		for (int i = 0; i < table.size(); i += 2) {
			_getch();
			cout << "tact " << int(i/2) + 1 << " ";
			for (int j = 0; j < table[i].size(); j++) {
				if (table[i][j].size() == 0) cout << "          ";
				else {
					cout << "P = ";
					for (bool dig : table[i][j]) cout << dig;
					cout << " ";
				}
			}
			cout << endl << "       ";
			for (int j = 0; j < table[i].size(); j++) {
				if (table[i+1][j].size() == 0) cout << "          ";
				else {
					cout << "A = ";
					for (bool dig : table[i+1][j]) cout << dig;
					if (table[i + 1][j].size() == 5) cout << " ";
					else cout << "  ";
				}
			}
			_getch();
			cout << endl;
		}
	}
	else cout << "Вектора имеют разный размер" << endl;
}

vector<bool> transfer(int number) { // Перевод числа из десятичной системы в двоичную. Яценко Я. С.
	vector <bool> rez;
	bool sign = 0;
	if (number < 0) {
		sign = 1;
		number *= -1;
	}
	while (number >= 1) {
		rez.push_back(number % 2);
		if (number % 2 == 1) {
			number -= 1;
		}
		number /= 2;
	}
	while (rez.size() <= number_digit-1) {
		rez.push_back(0);
	}
	reverse(rez.begin(), rez.end());
	if (sign) rez = invert(rez);
	return rez;
}

vector<bool> invert(vector<bool> a) { // Перевод числа в двоичной системе из прямого кода в дополнительный и наоборот. Яценко Я. С.
	for (int i = 0; i < a.size(); i++) a[i]=!a[i];
	vector<bool> one;
	for (int i = 0; i < number_digit - 1; i++) one.push_back(0);
	one.push_back(1);
	return sum(a, one);
}

vector<bool> sum(vector <bool> a, vector <bool> b) { // Сумма чисел в двоичном представлении. Яценко Я. С.
	vector<bool> rez;
	int carry = 0;
	int count = 0;
	if (a.size() == b.size()) {
		for (int i = a.size() - 1; i >= 0; i--) {
			count += int(a[i]) + int(b[i]) + carry;
			carry = 0;
			if ((count == 2)||(count == 3)) {
				carry += 1;
				count -= 2;
			}
			rez.push_back(count);
			count = 0;

		}
		reverse(rez.begin(), rez.end());
	}
	else cout << "Вектора имеют разный размер" << endl;
	return rez;
}

vector<vector<bool>> shift(vector <bool> p, vector <bool> a) { // Сдвиг влево на одну позицию. Яценко Я. С.
	p.erase(p.begin());
	p.push_back(a[1]);
	a.erase(a.begin()+1);
	return { p, a };
}

vector<vector<vector<bool>>> division(vector <bool> a, vector <bool> b) { // Реализация деления. Яценко Я. С.
	if (a.size() == b.size()) {
		vector<vector<bool>> p;
		vector<vector<bool>> rez = { a };
		vector<bool> temp;
		for (int i = 0; i < number_digit; i++) temp.push_back(0);
		p.push_back(temp);
		p.push_back(shift(p[p.size()-1], a)[0]);
		rez.push_back(shift(p[p.size()-1], a)[1]);
		for (int i = 0; i < number_digit - 2; i++) {
			if (p[p.size()-1][0]) p.push_back(sum(p[p.size()-1], b));
			else p.push_back(sum(p[p.size() - 1], invert(b)));
			rez.push_back(rez[rez.size()-1]);
			rez[rez.size()-1].push_back(!p[rez.size()-1][0]);
			p.push_back(shift(p[p.size()-1], rez[rez.size()-1])[0]);
			rez.push_back(shift(p[p.size()-1], rez[rez.size()-1])[1]);
		}
		if (p[p.size() - 1][0]) p.push_back(sum(p[p.size() - 1], b));
		else p.push_back(sum(p[p.size() - 1], invert(b)));
		rez.push_back(rez[rez.size() - 1]);
		rez[rez.size() - 1].push_back(!p[rez.size() - 1][0]);
		if (p[p.size() - 1][0]) p.push_back(sum(p[p.size() - 1], b));
		else p.push_back(p[p.size()-1]);
		rez.push_back(rez[rez.size()-1]);
		return { p, rez };
	}
}
