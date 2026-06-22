#include<iostream>
using namespace std;
#include<stdio.h>
#include<fstream>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<string>


void jm();
void dynr(char* nr, char* lj, int hs);


int main01()
{
	//不能向终端里复制超过一行的内容（只能复制一行），不然终端会强制停止运行
	//只能复制到文件里，在通过文件里读取

	//FILE* file;
	//file = fopen("C:\\Users\\26418\\Desktop\\text.txt", "r");

	//jm();

	//变量的定义
	char ch[1024];								//用于读取文件内容
	char* str;									//用于判断文件是否读取完
	char sr[1024];								//用于记录查找内容
	int hs = 1;									//记录查找的行数，从第一行开始
	char nr[1024];								//记录一行的内容

	char lj[1024];								//用于输入需要进行查找文字的文本文件路径

	char czms[100] = "普通查找";				//查找模式：分为普通查找、精确查找
	int jqcz = 1;								//因为这里的"1"!="1"，'\0'=='\0'返回假，'\0'!='\0'返回假，"普通查找"更不等于"普通查找"
												//所以加个变量记录查找模式，1为普通查找，2为精确查找
	int qhczcs = 0;								//记录切换查找模式的次数，当次数大于或等于1的时候就不要打印那几行话了

	//ifstream ifs;

	while (1)
	{
		cout << "初始化文件路径" << endl;
		cout << "请输入需要查找文字的文本文件路径" << endl;


		//初始化文本文件路径
		while (1)
		{	
			cin >> lj;								//输入路径

			ifstream ifs;							//这个文件流对象必须定义在循环内，不能定义在循环外
													//如果将这个变量定义在循环外，那么无论输入什么文件路径，ifs.is_open()都会判断为打开成功
			ifs.open(lj, ios::in);					//打开对应路径
			if (ifs.is_open())						//判断是否打开成功
			{
				break;								//打开成功则退出循环
			}
			cout << "没有该路径，请输入正确的文件路径" << endl;
			ifs.close();							//没有打开，就关闭文件，然后进入循环，重新输入文件路径
			system("pause");
			system("cls");
			cout << "初始化文件路径" << endl;
			cout << "请输入需要查找文字的文本文件路径" << endl;
		}
		system("cls");

		//判断文件中是否有内容
		FILE* file;											//创建文件对象

		while (1)
		{
			fopen_s(&file, lj, "rb");						//将路径设置为刚刚输入的路径，读取方式为：只读，二进制方式

			str = fgets(ch, 1024, file);					//读取一次
			if (str == NULL)								//如果文件中没有内容
			{
				cout << "文件中没有内容" << endl;
			}
			else if (str != NULL)							//如果文件中有内容
			{
				break;										//退出循环
			}
			fclose(file);									//则关闭文件
			system("pause");								//暂停系统，按任意键继续后，进入循环，重新打开文件，如果还没有内容，继续关闭文件，并暂停系统
			system("cls");
		}

		//文件中有内容
		while (1)
		{
			fclose(file);
			fopen_s(&file, lj, "rb");
			hs = 1;

			cout << "查找模式：" << czms << endl;
			cout << "当前文件路径：" << lj << endl;
			//if (czms == "普通查找")					//这里的"普通查找"可不等于"普通查找"（也许是变量和数组开辟太多，导致内存泄露？）
			if (jqcz == 1)
			{
				cout << "请输入您要查找的文字：\t\t\t（输入1切换路径，输入2切换为精确查找）" << endl;
			}
			else
			{
				cout << "请输入您要查找的文字：\t\t\t（输入1切换路径，输入2切换查找模式）" << endl;
			}
			

			cin >> sr;											//输入要进行查找的文字

			string temp;
			ofstream ofs;
			ofs.open("text.txt", ios::out);
			ofs << sr << endl;
			ofs.close();

			ifstream ifs;
			ifs.open("text.txt", ios::in);
			if (!ifs.is_open())
			{
				ifs.close();
			}
			else
			{
				getline(ifs, temp);
				ifs.close();
			}

			if (temp == "1")
			{
				system("cls");
				break;
			}
			else if (temp == "2")
			{
				if (qhczcs == 0)
				{
					cout << "您的VIP等级不足";
					Sleep(2000);
					cout << "，请充值" << endl;
					Sleep(3000);
					cout << "开玩笑的" << endl;
					Sleep(1000);
				}
				if (jqcz == 1)
				{
					strcpy_s(czms, "精确查找");
					jqcz = 2;
				}
				else if (jqcz == 2)
				{
					strcpy_s(czms, "普通查找");
					jqcz = 1;
				}
				qhczcs = 1;
				system("cls");
				continue;
			}


			system("cls");

			int i2 = 0;					//用于判断循环是否结束
			int i3 = 0;
			int cd1 = 0;				//用于记录字符串的长度
			int cd2 = temp.size();		//记录需要查找的文字，的字符串长度
			int pdwz = 1;				//判断文字内容是否相同（初始为1，1为相同，0为不相同）
			int pd = 0;					//判断文字内容是否全部相同（初始为0，1为相同，0为不相同）
			int jc = 1;					//记录当前为第几处相同内容

			while ((str = fgets(ch, 1024, file)) != NULL)			//读取一行内容（读取完后会自动停止循环）
			{
				pd = 0;
				pdwz = 1;
				cd1 = strlen(str);

				for (int i = 0;i < cd1;i++)							//遍历读取的该行内容
				{
					if (str[i] == sr[0])							//如果某个字符，等于需要查找文字的第一个字符
					{
						i2 = i;										//从找到的第一个相同字符开始一一对比
						i3 = 0;

						while (i2 < cd1 && i3 < cd2)				//则遍历后面的所有字符
						{
							if (str[i2] != sr[i3])					//如果有一个字符不相同
							{
								pdwz = 0;							//就判断为不相同
							}
							i2++;
							i3++;
						}

						if (pdwz == 1 && i3 == cd2)				//如果没有出现字符不相同的情况，而且输入的文字已经跟文本内容一一对比完
						{
							pd = 1;								//则记录已经找到相同内容的文字
							//break;								//并且退出循环，已经不需要再往下找了

							strcpy_s(nr, str);					//将该行内容拷贝进来

							cout << jc << "：" << endl;
							dynr(nr, lj, hs);					//将改行内容和路径和当前行数传入dynr函数中，调用该函数，将内容打印出来
							system("pause");
							system("cls");

							jc++;
						}
					}
				}

				if (pd == 1)							//如果找到了相同内容的文字的行数
				{
					
				}

				hs++;
			}

			if (jc == 1)
			{
				cout << "没有符合该文字的内容" << endl;
			}

			//jc--;								//它会将剩下的内容找完再退出循环，如果中间一直没有符合的内容，它就会一直++，
			//hs--;								//这里的jc只++了一次，但是hs已经++了很多次了，所以不能通过这种方法重新打印
			//cout << jc << "：" << endl;
			//dynr(nr, lj, hs);
			else
			{
				cout << "查找完毕" << endl;
			}
			system("pause");
			system("cls");
		}
	}
	return 0;
}

void jm()
{
	cout << "欢迎使用文字查找器" << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;
	cout << endl;cout << endl;cout << endl;cout << endl;cout << endl;
	cout << endl;cout << endl;cout << endl;cout << endl;cout << endl;
	cout << endl;cout << endl;cout << endl;cout << endl;cout << endl;
	cout << endl;cout << endl;cout << endl;cout << endl;cout << endl;
	cout << endl;cout << endl;cout << endl;
	system("pause");
	system("cls");
}

void dynr(char* nr, char* lj, int hs)
{
	FILE* file;
	fopen_s(&file, lj, "rb");
	char ch[1024];
	char* str;

	for (int i = 1;i < hs - 1;i++)
	{
		str = fgets(ch, 1024, file);
		if (str == NULL)
		{
			cout << "没有内容" << endl;
			system("pause");
			fclose(file);
			return;
		}
	}

	if (hs > 1)									//如果当前行数大于1（第一行上面没有内容了）
	{
		str = fgets(ch, 1024, file);
		cout << str;							//输出相同文字的上面一行内容
	}
	else if (hs == 1)
	{
		cout << endl;
	}

	str = fgets(ch, 1024, file);
	cout << str;								//输出相同文字的那一行内容

	str = fgets(ch, 1024, file);
	fclose(file);
	if (str == NULL)
	{
		return;
	}
	cout << str;								//输出相同文字的下面一行内容
}