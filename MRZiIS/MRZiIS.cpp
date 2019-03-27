// MRZiIS.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
		cout << "|" << setw(3) << "Пары" << setw(3) << "|" << setw(3) << "Сдвиг" << setw(2) << "|" << setw(1) << "Разность" << setw(1) << "|" << endl;
		cout << "|__________|__________|__________|" << endl;
		for (int pare = 0; pare < a.size(); pare++){
			vector<vector<bool>> p = division(transfer(a[pare]), transfer(b[pare]))[0];
			vector<vector<bool>> rez = division(transfer(a[pare]), transfer(b[pare]))[1];
			int a_counter = 0;
			for (int i = 0; i < p.size(); i++) {
				int key = _getch();
				for (int j = 0; j < i; j++) cout << setw(10) << "|";
				cout << "P = ";
				for (bool dig : p[i]) cout << dig;
				if (i % 3 != 2) {
					cout << endl;
					for (int j = 0; j < i; j++) cout << setw(10) << "|";
					cout << "A = ";
					for (bool dig : rez[a_counter]) cout << dig;
					a_counter++;
				}
				cout << endl;
			}
		}
		cout << "|_______________________|_______________|_______________________|" << endl;
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
		vector<vector<bool>> rez = {a};
		vector<bool> temp;
		for (int i = 0; i < number_digit; i++) temp.push_back(0);
		p.push_back(temp);
		p.push_back(shift(p[0], a)[0]);
		rez.push_back(shift(p[0], a)[1]);
		p.push_back(invert(b));
		for (int i = 1; i < 4; i++) {
			p.push_back(sum(p[3*i - 2], p[3*i - 1]));
			rez.push_back(rez[2*i - 2]);
			rez[2*i-1].push_back(!p[3*i][0]);
			p.push_back(shift(p[0], rez[2*i-1])[0]);
			rez.push_back(shift(p[0], rez[2*i-1])[1]);
			if (p[i][0]) p.push_back(b);
			else p.push_back(invert(b));
		}
		return {p, rez};
	}
}