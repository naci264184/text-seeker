#if 0
#include<iostream>
using namespace std;
#include<string>
#include<fstream>

bool pdlj(string lj);					//判断文件是否可以打开，参数：(文件路径)，返回值：是否打开成功（打开成功返回true）
bool pdnr(string lj);					//判断文件中是否有内容，参数：(文件路径)，返回值：是否有内容（文件中有内容返回true）
void dy(string lj);						//打印文件中的所有内容，参数：(文件路径)
void dy2(string lj, string sr, string ys = "\033[36;40m");	//打印文件中的内容，并突出指定内容的字体颜色，参数：(文件路径,指定的内容,显示指定内容的字体颜色（默认颜色为：蓝色）)

int main()
{
	string lj;							//记录路径
	string sr;							//记录输入内容（需要查找内容）

	while (1)
	{
		while (1)
		{
			cout << "请输入文本文件路径\t\t\t\t（返回和退出键为0）" << endl;
			cin >> lj;									//输入路径
			if (lj == "0")								//判断是否要退出
			{
				cout << "欢迎下次使用" << endl;
				system("pause");
				return 0;
			}

			if (pdlj(lj))								//判断路径是否正确
			{
				system("cls");
				break;
			}

			cout << "没有该路径" << endl;
			system("pause");
			system("cls");
		}

		while (!pdnr(lj))								//判断文件中是否有内容
		{
			cout << "没有内容" << endl;
			system("pause");
			system("cls");
		}

		while (1)
		{
			dy(lj);										//将文件中的原内容打印出来
			
			cout << endl;
			cout << "请输入要查找的内容" << endl;
			
			cin >> sr;									//输入要查找的指定内容
			if (sr == "0")								//判断是否要返回切换文件路径
			{
				system("cls");
				break;
			}
			
			system("cls");								//清屏
			dy2(lj, sr);								//打印突出显示指定文字后的文件内容
			
			system("pause");
			system("cls");
		}
	}

	return 0;
}

bool pdlj(string lj)							//判断文件（路径）是否可以打开
{
	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);
	if (ifs.is_open())							//如果打开成功
	{
		ifs.close();
		return true;							//返回true
	}
	else										//否则（打开失败）
	{
		ifs.close();
		return false;							//返回false
	}
}

bool pdnr(string lj)
{
	string temp;

	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);		//为防止程序崩，这里再判断一次文件是否打开成功
	if (!ifs.is_open())
	{
		ifs.close();
		return false;							//打开失败也返回false
	}

	if (getline(ifs, temp))						//如果文件中有内容
	{
		ifs.close();
		return true;							//返回true
	}
	else										//否则（文件中没有内容）
	{
		ifs.close();
		return false;							//返回false
	}
}

void dy(string lj)
{
	string temp;
	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);		//为防止程序崩，判断一下文件是否打开成功
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}

	while (getline(ifs, temp))					//打印文件中的所有内容
	{
		cout << temp << endl;
	}
}

void dy2(string lj, string sr, string ys)
{
	string temp;								//用于接收文件中每一行内容
	int wz = 0;									//用于记录指定内容位于第几个字符
	string zdnr;								//记录指定内容
	string gbys = "\033[m";						//加在结尾处的“关闭颜色”功能字符
	int cd = sr.size();
	int cd2 = 0;
	int jc = 0;
	int czqd = 0;								//每次查找的查找起点
	int pdnr = 0;								//判断后面是否还有内容

	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}

	while (getline(ifs, temp))					//将文件中每行内容打印出来
	{
		czqd = 0;
		pdnr = 0;
		while ((wz = temp.find(sr, czqd)) != -1)//如果某行中查找到指定内容
		{
			int i;
			cd2 = temp.size();
			zdnr = ys;							//每次打印完，都将zdnr更新一下，避免留下上一次查找的值，更新内容为：字体颜色前缀
			for (i = czqd;i < wz;i++)			//输出指定内容前面的文字（非指定内容部分）（0或上次查找的结束位置 到 查找到的内容前一个字符）
			{
				cout << temp[i];
			}

			for (i = 0;i < cd;i++)				//将该行内容中，相同的部分加入zdnr字符串中（获取指定内容）
			{
				zdnr += temp[wz + i];
			}
			zdnr += gbys;						//在结尾处关闭字体颜色
			cout << zdnr;						//给指定内容加上字体颜色后，打印指定内容

			pdnr = 1;							//如果找到过内容
			
			czqd = wz + cd;						//且继续找，查找起点为：上一次的结尾处（上一次查找到的位置+指定内容的长度）

			jc++;								//记录找到的指定内容，一共有几处
		}

		if (pdnr == 1)						//如果之前找到过内容，且后面还有内容
		{
			for (int i = czqd;i < cd2;i++)	//输出指定内容后面的文字（非指定内容部分）
			{
				cout << temp[i];
			}
			cout << endl;
		}
		else
		{
			cout << temp << endl;			//就输出文件中的原内容
		}

	}

	cout << endl;
	if (jc == 0)								//如果一处都没有
	{
		cout << "没有该内容" << endl;
	}
	else										//如果有至少一处，在最后一处的下一行输出查找完毕
	{
		cout << "查找完毕" << endl;
	}
}
#endif