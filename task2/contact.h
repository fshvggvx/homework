#ifndef CONTACT_H
#define CONTACT_H
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
struct contact {
	string name;
	string phone;
	string qq;
};
void add(vector<contact>& list);
void result(vector<contact>& list);
void find(contact& b, vector<contact>& list);
void deletename(contact& b, vector<contact>& list);
void get(vector<contact>& list);
void put(vector<contact>& list);
#endif
