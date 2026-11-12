#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include <algorithm>
using namespace std;
struct student 
{   string name;
	string id;
	int score;
};
bool cmp(const student& a, const student& b)
{return a.score > b.score;
    }
void input( vector<student>&a)
{
	student b;
	while (1)
	{
		cout << "请输入学生姓名(此处输入#可以退出)：";
		cin >> b.name;
		if (b.name=="#")
			break;
		cout << "请输入学生学号：";
		cin >> b.id;
		cout << "请输入学生分数：";
		cin >> b.score;
		a.push_back(b);
	}
}
void output(vector<student>& a)
{
	for (int i = 0;i < a.size();i++)
		cout << a[i].name << "    \t" << a[i].id << "\t " << a[i].score << endl;
}
void handle(vector<student>& a,double*count,int*max,int*min,int*high, int*low)
{
	cout<<"==========成绩统计结果==========\n";
	for (int i = 0;i < a.size();i++)
	{
		(*count) += a[i].score;
		if (a[i].score >* max)
			*max = a[i].score;
		if (a[i].score <* min)
			*min = a[i].score;
		if (a[i].score >= 60)
			(*high)++;
		if (a[i].score < 60)
			(*low)++;
	}
}

int main()
{
	vector<student>a;
	input(a);
	output(a);
	double count = 0;
	int max = 0;
	int min = 100;
	int high = 0;
	int low = 0;
	handle(a, &count, &max, &min, &high, &low);

	cout << fixed << setprecision(2)
		<< "学生的平均分:" << count / a.size() << "\n"
		<< "学生的最高分:" << max << "\n"
		<< "学生的最低分:" << min << "\n"
		<< "及格的人数  :" << high << "\n"
		<< "不及格的人数:" << low << "\n";

	sort(a.begin(), a.end(), cmp);
	for (int i = 0;i < a.size();i++)
		cout << a[i].name << "    \t" << a[i].id << "\t" << a[i].score << endl;
}
