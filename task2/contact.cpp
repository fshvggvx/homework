#include"contact.h"
void add(vector<contact>& list)
{
	contact a;
	while (1)
	{
		int flag = 0;
		cout << "请输入姓名(此处输入#可退出)    ：";
		getline(cin, a.name);
		if (a.name == "#")
			break;
		for (auto& b : list)

			if (a.name == b.name)
			{
				cout << "重复姓名，请重试" << endl;
				flag++;
				break;
			}
		if (flag)
			continue;
		cout << "请输入电话号码(此处输入#可退出)：";
		getline(cin, a.phone);
		if (a.phone == "#")
			break;
		cout << "请输入QQ或邮箱(此处输入#可退出)：";
		getline(cin, a.qq);
		if (a.qq == "#")
			break;
		list.push_back(a);
	}
}
void result(vector<contact>& list)
{
	cout << "**********通讯录**********\n";
	if (list.empty())
	{
		cout << "您的通迅录空空如也";
		return;
	}
	for (auto& a : list)
	{
		cout<< "." << "姓名:" << a.name << "\t "
			<< "电话号码：" << a.phone << "\t "
			<< "QQ或邮箱：" << a.qq << endl;
	}
}
void find(contact& b, vector<contact>& list)
{
	int flag = 0;
	for (auto& a : list)
		if (a.name == b.name)
		{
			cout << "姓名：" << a.name << "\t "
				<< "电话号码：" << a.phone << "\t "
				<< "QQ或邮箱：" << a.qq << endl;
			flag++;
		}
	if (flag)
		cout << "成功找到联系人" << endl;
	if (!flag)
		cout << "未找到联系人" << endl;

}
void deletename(contact& b, vector<contact>& list)
{
	for (auto& a : list)
	{
		int i = 0;
		if (a.name == b.name)
		{
			list.erase(list.begin() + i);
			cout << "成功删除联系人\n";
			break;
		}
		i++;
	}
	cout << "该联系人不存在\n";
}
void get(vector<contact>& list)
{
	ifstream ifs("contact.txt");
	string line;
	contact a;
	if (ifs.is_open())
		while (getline(ifs, line))
		{
			size_t p1 = line.find('|');
			size_t p2 = line.find('|', p1 + 1);
			a.name = line.substr(0, p1);
			a.phone = line.substr(p1 + 1, p2 - p1 - 1);
			a.qq = line.substr(p2 + 1);
			list.push_back(a);
		}
}
void put(vector<contact>& list)
{
	ofstream ofs("contact.txt");
	for (auto& a : list)
	{
		ofs <<"姓名:"<< a.name << " | "
			<<"电话号码:" << a.phone << "|"
			<<"QQ或邮箱:" << a.qq << endl;
	}
}