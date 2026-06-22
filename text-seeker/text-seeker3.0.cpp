#include<iostream>
using namespace std;
#include<string>
#include<fstream>
#include<Windows.h>
#include<conio.h>

bool pdlj(string lj);					//判断文件是否可以打开，参数：(文件路径)，返回值：是否打开成功（打开成功返回true）
bool pdnr(string lj);					//判断文件中是否有内容，参数：(文件路径)，返回值：是否有内容（文件中有内容返回true）
void strzch(string& str, char* ch);		//string类型的值，赋值给char[]类型
void dy(string lj);						//打印文件中的所有内容，参数：(文件路径)
void hqhs(string lj, string sr);		//获取每段指定内容，在第几行
void dy2(string lj, string sr, int ys, int djc, int jc, string color = "\033[36;40m", string color2 = "\033[32;40m");
int czgy_sl(int ys);					//查找该页的指定内容数量，参数：页数，返回值：该页的指定内容数量

//测试内容（暂时没有用到，因为会出错）
//此处改用c语言的文件读入，且限制每次读取的内容最大为120个字符（终端的x轴最大长度），想用来限制一行的内容，使它不要换行，用于准确移动光标
//但是由于水平制表符只占用1个字节，但是会根据前面的字符以8的倍数占位置，如果使用这种方式，还需要统计水平制表符的数量，和水平制表符前面的字符离8的倍数差多少字符，
//并判断真实内容的换行次数，从而更改光标移动时的y坐标，而且这里改用c语言的文件读入后，会莫名奇妙的出错，出错后，cout无法输出内容，所以暂时不使用c语言的文件读入
//如果以后还需要测试，只需要将dy2和dy21的名字互换一下，hqhs和hqhs2的函数名也互换一下，就可以了
//如果解决了bug，可以使用函数指针dy2、hqhs，然后将hqhs函数改名为hqhs1，将dy2改名为dy21，dy21改名为dy22，的方式，选择使用c语言的文件读入或c++的文件读入
//函数指针的更改，在读内容时，加入选项就行，1、2、3、4、5、6有选项了，加入7、8选项就行
void dy21(string lj, string sr, int ys, int djc, int jc, string color, string color2);
void hqhs2(string lj, string sr);


int sl = 0;								//指定内容的数量
int hswz[255];							//每处指定内容，对应的行数
int hs = 0;								//输入的文本的内容的总行数
//int cd = 120;							//获取内容的数组长度（数组的长度需不能用变量来定义，需要手动）
int yy_hs = 20;							//一页的行数
int xs_hs = 0;							//显示行数（0（不使用），1（使用））
int gb_yd = 1;							//光标移动（0（不使用），1（使用））

int main()
{
	string lj;							//记录路径
	string sr;							//记录输入内容（需要查找内容）

	//基础数据
	int ys = 1;							//当前页数
	int zys = 0;						//总页数
	int jc = 1;							//当前的指定内容，为所有指定内容中的第几处
	int gy_sl = 0;						//该页的指定内容数量（没有记录一页的变量，这里的一页为：25行）
	int djc = 0;						//当前查找的是该页的第几处指定内容
	int py = 0;							//偏移：0（无偏移）、1（左偏移，当前的指定内容在当前页面左侧（内容上面））、2（右偏移，当前指定内容在当前页面右侧）
	int yss;							//第一处指定内容的页数
	int ysw;							//最后一处指定内容的页数

	char xz = 0;						//用户的选择
	char xz2 = 0;

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
			system("cls");
			dy(lj);										//将文件中的原内容打印出来

			cout << endl;
			cout << "请输入要查找的内容" << endl;

			cin >> sr;									//输入要查找的指定内容
			if (sr == "0")								//判断是否要返回切换文件路径
			{
				system("cls");
				break;
			}


			//获取基础数据
			//（1）每处指定内容的行数（和总行数）、当前指定内容为当前页面第几处、几处
			hqhs(lj, sr);								//获取每段指定内容的位置、文本总行数
			djc = 1;									//当前为该页第几处的指定内容
			jc = 1;										//当前为第几处的指定内容

			//（2）总页数
			if (hs % yy_hs == 0)							//如果总行数能够整除25
			{
				zys = hs / yy_hs;							//则总页数为：行数÷25
			}
			else
			{
				zys = hs / yy_hs + 1;						//如果总行数不能整除25，则总页数为：行数÷25+1
			}
			
			//（3）指定内容数量
			if (sl != 0)									//如果指定内容的数量不为0
			{
				if (hswz[0] % yy_hs == 0)						//如果该行数能够整除25
				{
					ys = hswz[0] / yy_hs;						//则首页页数为：行数÷25
				}
				else
				{
					ys = hswz[0] / yy_hs + 1;						//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
				}
			}
			else
			{
				cout << "没有该内容" << endl;
				system("pause");
				continue;
			}

			//（4）获取页数首和页数尾
			if (hswz[0] % yy_hs == 0)						//如果该行数能够整除25
			{
				yss = hswz[0] / yy_hs;						//则首页页数为：行数÷25
			}
			else
			{
				yss = hswz[0] / yy_hs + 1;					//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
			}

			if (hswz[sl - 1] % yy_hs == 0)						//如果该行数能够整除25
			{
				ysw = hswz[sl - 1] / yy_hs;						//则首页页数为：行数÷25
			}
			else
			{
				ysw = hswz[sl - 1] / yy_hs + 1;					//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
			}
			

			while (1)
			{
				system("cls");
				cout << "第" << ys << "页" << endl;
				cout << endl;								//如果不想要提示内容“第几页”与内容间有换行，可以注释这行（如果注释了需要更改光标移动中的num）
				dy2(lj, sr, ys, djc, jc);
				//cout << endl;								//如果想要使用打印完后光标移动，需要注释这行（相反，如果需要使用这行，光标移动就无法使用）


				gy_sl = czgy_sl(ys);						//获取该页的指定内容数量

				//*/
				xz = _getch();							//如果输入中有内容，会跳过这次的键盘输入
				if (_kbhit())			//判断输入流文件中是否还有内容（输入一次方向键为输入两次内容）
				{
					xz2 = _getch();		//接收下一个键盘输入的内容

					if (xz != -32 || (xz2 != 72 && xz2 != 75 && xz2 != 77 && xz2 != 80))		//如果第一次输入的内容不为224，或第二次输入的内容不为：72、75、77、80
					{																			//则输入的内容不是方向键，则为游戏暂停
						//将键盘输入的内容全接收完，防止干扰下次方向判断
						//cout << "1" << endl;
						//cout << (int)xz << endl;
						//cout << (int)xz2 << endl;
						//system("pause");

						while (_kbhit())
						{
							_getch();
						}
					}
					else								//否则（输入内容的内容是方向键），则进行方向的判断
					{
						if (xz2 == 72)					//按了"↑"（翻上一页）
						{
							int pd = 0;

							if (ys != 1)				//如果不是第一页
							{
								if (gy_sl != 0)
								{
									jc -= djc - 1;		//先让jc位于当前页的第一处
									djc = 1;			//记录为该页第一处
									pd = 1;
								}
								ys--;					//切换上一页								
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 1)		//如果偏移不为左偏移（当前指定内容在右侧）
									{
										jc--;			//将jc位于当前页的最后一处
									}

									jc -= gy_sl - 1;	//让jc位于当前页的第一处
									py = 0;
								}
								else
								{
									if (pd == 1)		//如果右侧有内容，而当前页面没内容
									{
										py = 2;			//右偏移
									}
								}
							}
							else						//如果为第一页
							{
								if (gy_sl != 0)
								{
									jc -= gy_sl - 1;	//让jc位于当前页的第一处
									djc = 0;
									py = 0;
								}
							}
						}
						else if (xz2 == 80)					//按了"↓"（向后翻页）
						{
							int pd = 0;

							if (ys < zys)				//如果不是最后一页
							{
								if (gy_sl != 0)
								{
									jc += gy_sl - djc;	//先让jc位于当前页的最后一处
									djc = 1;
									pd = 1;
								}
								ys++;					//切换下一页
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 2)		//如果不为右偏移（当前指定内容就在当前页的第一处）
									{
										jc++;			//则让jc位于当前页的第一处
									}

									py = 0;
									djc = 1;
								}
								else					//如果上一页有内容，当前页没有内容
								{
									if (pd == 1)
									{
										py = 1;			//左偏移
									}
								}
							}
						}
						else if (xz2 == 75)					//如果按了"←"（切换上一个）
						{
							int pd = 0;
							if (djc == 1)				//如果当前为该页第一处指定内容
							{
								if (ys != 1)			//且如果当前页数不等于1
								{
									if (gy_sl != 0)		//如果该页有指定内容
									{
										pd = 1;
									}

									ys--;				//往前翻一页

									gy_sl = czgy_sl(ys);//重新获取一次该页的指定内容数量
									if (gy_sl != 0)		//如果（下一页没有内容，）该页有指定内容
									{
										if (py != 1)		//如果不为左偏移
										{
											jc--;
										}

										djc = gy_sl;	//第几处等于该页最后一处
										py = 0;
									}
									else				//否则
									{
										djc = 1;		//让第几处=1
										if (pd == 1)	//如果右边有内容，翻前一页后，当前没有内容
										{
											py = 2;
										}
									}
								}
							}
							else						//如果当前不为该页第一处指定内容
							{
								djc--;					//将当前的某处往前移动一个
								jc--;
							}
						}
						else if (xz2 == 77)					//如果按了"→"（切换下一个）
						{
							int pd = 0;
							if (djc == gy_sl || gy_sl == 0)	//如果当前为该页最后一处指定内容，或该页数量为0
							{
								if (ys != zys)				//且当前页数不是最后一页
								{
									if (gy_sl != 0)
									{
										pd = 1;
									}
									ys++;					//往后翻一页
									djc = 1;
									gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
									if (gy_sl != 0)			//如果翻页后，该页有指定内容
									{
										if (py != 2)			//如果不为右偏移（左侧没有指定内容，但是右侧有，指定内容本来就在右侧第一个）
										{
											jc++;				//则让jc位于当前页第一处指定内容
										}
										py = 0;
									}
									if (gy_sl == 0 && pd == 1)	//如果前一页有内容，当前没有内容
									{
										py = 1;					//左偏移
									}
								}
							}
							else							//如果该页有指定内容，且不是该页最后一处指定内容
							{
								djc++;
								jc++;
							}
						}
					}
				}
				else					//如果只有一次键盘输入内容
				{
					if (xz == '0')		//判断第一次输入的内容是否为0
					{
						break;			//退出该次查找
					}
					else if (xz == '1')				//如果为'1'（向前翻5页）
					{
						int pd = 0;					//用于判断是否有内容以判断内容偏移

						for (int i = 0;i < 5;i++)		//然后进入往前翻5页的循环
						{
							if (ys > 1)					//如果页数大于1
							{
								if (gy_sl != 0)
								{
									jc -= djc - 1;		//先让jc位于当前页的第一处
									djc = 1;			//记录为该页第一处
									pd = 1;
								}
								ys--;					//切换上一页								
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 1)		//如果偏移不为左偏移（当前指定内容在右侧）
									{
										jc--;			//将jc位于当前页的最后一处
									}

									jc -= gy_sl - 1;	//让jc位于当前页的第一处
									py = 0;
								}
							}
							else						//如果页数为1
							{
								break;					//退出循环，不需要进行翻页
							}
						}

						if (pd == 1 && gy_sl == 0)		//如果后面页数有指定内容，而检测完的最后一页没有
						{
							py = 2;						//指定内容右偏移
						}
					}
					else if (xz == '2')				//如果为'2'
					{
						int pd = 0;					//用于判断是否有内容以判断内容偏移

						for (int i = 0;i < 5;i++)		//然后进入往后翻5页的循环
						{
							if (ys < zys)				//如果页数不是最后一页
							{
								if (gy_sl != 0)
								{
									jc += gy_sl - djc;	//先让jc位于当前页的最后一处
									djc = 1;
									pd = 1;
								}
								ys++;					//切换下一页
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 2)		//如果不为右偏移（当前指定内容就在当前页的第一处）
									{
										jc++;			//则让jc位于当前页的第一处
									}

									py = 0;
									djc = 1;
								}
							}
							else						//如果页数为最后一页
							{
								break;					//退出循环，不需要进行翻页
							}
						}

						if (pd == 1 && gy_sl == 0)		//如果前面页数有指定内容，而检测完的最后一页没有
						{
							py = 1;						//指定内容左偏移
						}
					}
					else if (xz == '3')				//如果为'3'
					{
						int pd = 0;					//用于判断是否有内容以判断内容偏移

						for (int i = 0;i < 10;i++)		//然后进入往前翻10页的循环
						{
							if (ys > 1)					//如果页数大于1
							{
								if (gy_sl != 0)
								{
									jc -= djc - 1;		//先让jc位于当前页的第一处
									djc = 1;			//记录为该页第一处
									pd = 1;
								}
								ys--;					//切换上一页								
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 1)		//如果偏移不为左偏移（当前指定内容在右侧）
									{
										jc--;			//将jc位于当前页的最后一处
									}

									jc -= gy_sl - 1;	//让jc位于当前页的第一处
									py = 0;
								}
							}
							else						//如果页数为1
							{
								break;					//退出循环，不需要进行翻页
							}
						}

						if (pd == 1 && gy_sl == 0)		//如果后面页数有指定内容，而检测完的最后一页没有
						{
							py = 2;						//指定内容右偏移
						}
					}
					else if (xz == '4')				//如果为'4'
					{
						int pd = 0;					//用于判断是否有内容以判断内容偏移

						for (int i = 0;i < 10;i++)		//然后进入往后翻10页的循环
						{
							if (ys < zys)				//如果页数不是最后一页
							{
								if (gy_sl != 0)
								{
									jc += gy_sl - djc;	//先让jc位于当前页的最后一处
									djc = 1;
									pd = 1;
								}
								ys++;					//切换下一页
								gy_sl = czgy_sl(ys);	//重新获取一次该页的指定内容数量
								if (gy_sl != 0)			//如果该页有指定内容
								{
									if (py != 2)		//如果不为右偏移（当前指定内容就在当前页的第一处）
									{
										jc++;			//则让jc位于当前页的第一处
									}

									py = 0;
									djc = 1;
								}
							}
							else						//如果页数为最后一页
							{
								break;					//退出循环，不需要进行翻页
							}
						}

						if (pd == 1 && gy_sl == 0)		//如果前面页数有指定内容，而检测完的最后一页没有
						{
							py = 1;						//指定内容左偏移
						}
					}
					else if (xz == 'w')					//按了"w"
					{
						int pd_ys = 0;

						if (ys >= yss)			//如果页数大于或等于第一处指定内容的页数
						{
							if (ys == yss)
							{
								pd_ys = 1;
							}

							if (gy_sl != 0)			//如果该页指定内容数量不为0
							{
								jc -= djc;			//则几处=几处减去上面剩下的所有指定内容（为上一页的最后一处指定内容）
								djc = 1;			//记录为该页第一处

							}
							else					//如果该页指定内容数量为0
							{
								if (py != 1)	//如果不为左偏移
								{
									jc--;
								}
								djc = 1;		//记录为该页第一处
								py = 0;
							}

							if (jc == 0)
							{
								jc = 1;
							}

							if (hswz[jc - 1] % yy_hs == 0)				//如果该行数能够整除25
							{
								ys = hswz[jc - 1] / yy_hs;				//则首页页数为：行数÷25
							}
							else
							{
								ys = hswz[jc - 1] / yy_hs + 1;			//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
							}
							gy_sl = czgy_sl(ys);			//重新获取一次该页的指定内容数量

							if (pd_ys == 0)
							{
								jc -= gy_sl - 1;			//注意，因为是hswz[jc-1]，所以该页gy_sl一定不等于0，这里用于将jc回退到该页第一处指定内容
							}
							else
							{
								jc = 1;
								djc = 1;
							}
						}
					}
					else if (xz == 's')					//按了"s"
					{
						int pd_ys = 0;

						//system("pause");
						//cout << "什么情况" << endl;		//换c语言的文件读取后，会出错，这里的cout也无法输出（或无法准确输出正确的）内容
						//cout << ys << endl;
						//cout << jc << endl;
						//cout << djc << endl;
						//system("pause");

						if (ys < ysw)			//如果该页后面还有指定内容
						{
							if (ys == yss)
							{
								pd_ys = 1;
							}

							if (gy_sl != 0)				//如果该页有指定内容
							{
								jc += gy_sl - djc + 1;	//几处=该页数所有剩余指定内容数量+1
								if (jc > sl)			//如果jc大于sl（后面页数没有指定内容）
								{
									jc -= gy_sl;		//让jc的位置，退回为该页第一处的位置
								}
								djc = 1;
							}
							else						//如果该页没有指定内容
							{
								if (jc < sl)			//则，如果该处不是最后一处
								{
									if (py != 2)		//偏移不为右偏移，则jc++
									{
										jc++;
									}
									djc = 1;
									py = 0;
								}
							}

							if (hswz[jc - 1] % yy_hs == 0)				//如果该行数能够整除25
							{
								ys = hswz[jc - 1] / yy_hs;				//则首页页数为：行数÷25
							}
							else
							{
								ys = hswz[jc - 1] / yy_hs + 1;			//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
							}
						}
						else if (ys == ysw)			//如果是指定内容的最后一处的页数，则将几处和第几处重置为当前页数的第一处
						{
							jc -= djc - 1;
							djc = 1;
							py = 0;
						}
					}
					else if (xz == 'a')					//如果按了"a"
					{
						if (djc == 1)				//如果当前为该页第一处指定内容
						{
							if (ys > yss)			//且如果当前页数不为第一处指定内容的页数
							{
								if (gy_sl != 0)
								{
									if (jc > 1)
									{
										jc--;
									}
								}
								else							//如果该页没有内容
								{
									if (py != 1)				//如果不为左偏移
									{
										jc--;
									}
									py = 0;
								}

								if (hswz[jc - 1] % yy_hs == 0)				//如果该行数能够整除25
								{
									ys = hswz[jc - 1] / yy_hs;				//则首页页数为：行数÷25
								}
								else
								{
									ys = hswz[jc - 1] / yy_hs + 1;			//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
								}

								gy_sl = czgy_sl(ys);			//重新获取一次该页的指定内容数量
								djc = gy_sl;
							}
						}
						else						//如果当前不为该页第一处指定内容
						{
							djc--;					//将当前的某处往前移动一个
							jc--;
						}
					}
					else if (xz == 'd')					//如果按了"d"
					{
						if (djc == gy_sl || gy_sl == 0)	//如果当前为该页最后一处指定内容，或该页数量为0
						{
							if (ys < ysw)				//如果页数小于最后一处指定内容的页数
							{
								djc = 1;

								if (gy_sl != 0)				//如果该页数量不为0
								{
									jc++;
								}
								else						//如果该页数量为0
								{
									if (py != 2)			//如果不为右偏移
									{
										jc++;
									}
									py = 0;
								}

								if (hswz[jc - 1] % yy_hs == 0)				//如果该行数能够整除25
								{
									ys = hswz[jc - 1] / yy_hs;				//则首页页数为：行数÷25
								}
								else
								{
									ys = hswz[jc - 1] / yy_hs + 1;			//如果该指定内容的行数不能整除25，则总页数为：行数÷25+1
								}

							}
						}
						else
						{
							djc++;
							jc++;
						}
					}
					else if (xz == '5')						//启用或关闭显示行数
					{
						if (xs_hs == 0)
						{
							xs_hs = 1;
						}
						else
						{
							xs_hs = 0;
						}
					}
					else if (xz == '6')						//启用或关闭光标移动
					{
						if (gb_yd == 0)
						{
							gb_yd = 1;
						}
						else
						{
							gb_yd = 0;
						}
					}
				}

				//*/
			}
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

void strzch(string& str, char* ch)				//string类型的值，赋值给char[]类型
{
	ofstream ofs;
	ofs.open("text.txt", ios::out);
	ofs << str << endl;
	ofs.close();

	ifstream ifs;
	ifs.open("text.txt", ios::in);
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}
	ifs.getline(ch, 255);						//删除文件的内部使用这两个字符串类型转换函数，这里不能再包含删除文件函数，
	ifs.close();								//不然会：void func1(){func2()};void func2(){func1());（循环）
	remove("text.txt");
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

void hqhs(string lj, string sr)					//获取每段指定内容的行数
{
	string temp;								//用于接收文件中每一行内容
	int wz = 0;									//用于记录指定内容位于第几个字符
	int cd = sr.size();
	int czqd = 0;								//每次查找的查找起点
	hs = 0;
	sl = 0;	

	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}

	while (getline(ifs, temp))					//用string的文件读入判断内容是否为空
	{
		czqd = 0;
		hs++;

		while ((wz = temp.find(sr, czqd)) != -1)//如果某行中查找到指定内容
		{
			czqd = wz + cd;						//刷新一下下次查找的起点，下次查找的起点为：上一次的结尾处（上一次查找到的位置+指定内容的长度）

			hswz[sl] = hs;						//记录该次找到的指定内容的位置

			sl++;								//记录找到的指定内容，一共有几处
		}
	}

	ifs.close();
}

void hqhs2(string lj, string sr)				//获取每段指定内容的行数
{
	char lj2[255];
	strzch(lj, lj2);	

	char temp[120];					//用于接收文件中每一行内容（由于水平制表符占用1字节，但是会按前面的字符数，占终端0~8个字符位置（填补满8的倍数），
	string temp2;					//而终端的换行是根据位置（内容量）决定的，不是内容的字节量，所以无法靠限制每行的字节个数，阻止它换行，
	int wz = 0;						//从而准确移动光标的位置（除非限制120个字节数后，在查找里查找一下换行符统计个数，最后在光标移动时，根据这个更改y坐标）
	int cd = sr.size();
	int czqd = 0;					//每次查找的查找起点
	hs = 0;
	sl = 0;

	FILE* file;
	fopen_s(&file, lj2, "rb");
	if (!file)
	{
		fclose(file);
		return;
	}

	while (fgets(temp, sizeof(temp), file))		//用c语言的文件读入判断内容是否为空
	{

		czqd = 0;
		hs++;

		temp2 = temp;
		while ((wz = temp2.find(sr, czqd)) != -1)//如果某行中查找到指定内容
		{
			czqd = wz + cd;						//刷新一下下次查找的起点，下次查找的起点为：上一次的结尾处（上一次查找到的位置+指定内容的长度）

			hswz[sl] = hs;						//记录该次找到的指定内容的位置

			sl++;								//记录找到的指定内容，一共有几处
		}
	}

	fclose(file);
}

void dy2(string lj, string sr, int ys, int djc, int jc, string color, string color2)
{																//参数：路径,指定内容,当前页数,当前为该页的第几处指定内容,总第几处指定内容
	string temp;								//用于接收文件中每一行内容
	int wz = 0;									//用于记录指定内容位于第几个字符
	string zdnr;								//记录指定内容
	string gbys = "\033[m";						//加在结尾处的“关闭颜色”功能字符
	int cd = sr.size();
	int cd2 = 0;
	int czqd = 0;								//每次查找的查找起点
	int pdnr = 0;								//判断后面是否还有内容
	int n = 1;									//记录当前为第几处
	int x = 0;									//打印完后，光标移动到指定内容处，x坐标
	int y = 0;									//y坐标
	int ygg = 0;								//用于记录有多少行，一行输出两行或以上的，有的换更改光标移动时的y坐标
	int ygg_pd = 0;
	int wz_spzbf = 0;							//用于记录水平制表符的位置
	int tb_gs = 0;								//填补个数
	int sp_czqd = 0;

	ifstream ifs;
	ifs.open(lj, ios::in | ios::binary);
	if (!ifs.is_open())
	{
		ifs.close();
		return;
	}



	for (int i = 1;i < hs + 1;i++)						//读取文件中的每行内容
	{
		getline(ifs, temp);
		czqd = 0;
		pdnr = 0;
		tb_gs = 0;
		sp_czqd = 0;								//水平制表符的查找起点

		if (i > ys * yy_hs)
		{
			break;
		}
		if (i >= (ys - 1) * yy_hs + 1 && i <= ys * yy_hs)		//如果该行为，当前查看的页数的范围内（1~25、26~50）
		{
			if (i != hs)							//如果没有读取到最后一行
			{
				temp.erase(temp.size() - 1, 1);		//则删除读取的内容的末尾的换行符，中的\r
			}

			cd2 = temp.size();						//获取该段文字长度

			if (xs_hs == 1)							//如果使用了显示行数功能
			{
				cout << i << "：";					//在打印每行前，打印行数
				x += 5;								//光标移动的x轴需要加5
			}

			while ((wz = temp.find(sr, czqd)) != -1)//如果某行中查找到指定内容
			{
				int i;

				if (djc == n)						//如果为当前查看的某处，则字体颜色为颜色1
				{
					zdnr = color;
					x = wz + cd;					//记录一下该指定内容在终端中的x坐标
					ygg_pd = 1;
				
					while ((wz_spzbf = temp.find("\t", sp_czqd)) != -1)	//查找该行的水平制表符，如果找到水平制表符
					{
						if (wz_spzbf >= x)				//如果改水平制表符位于该指定内容后面
						{
							break;						//则退出查找水平制表符
						}
						if (wz_spzbf % 8 != 0)			//记录它会填补的个数（填补的x坐标）（水平制表符占用一个字节，所以，一定是从1开始，或9开始，位置不会为0）
						{
							tb_gs += 8 - (wz_spzbf % 8);
						}
						sp_czqd = wz_spzbf + 1;			//更新下次查找水平制表符的查找起点
					}
					x += tb_gs;							//将该指定内容的x轴，加上水平制表符填补的个数


					if (x % 120 == 0)					//统计超出一行的总行数
					{
						ygg += cd2 / 120 - 1;
					}
					else if (x / 120 > 0)
					{
						ygg += cd2 / 120;
						x = x % 120;
					}				

				}
				else								//否则，字体颜色为颜色2
				{
					zdnr = color2;
				}

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

				pdnr = 1;							//找到过内容，就记录判断内容为1
				
				czqd = wz + cd;						//且继续找，查找起点为：上一次的结尾处（上一次查找到的位置+指定内容的长度）

				n++;
			}

			if (pdnr == 1)						//如果之前找到过内容
			{
				for (int i = czqd;i < cd2;i++)	//输出指定内容后面的文字（非指定内容部分）
				{
					cout << temp[i];
				}
				cout << endl;					//如果指定内容后面没有内容，就输出一个换行
			}
			else
			{
				cout << temp << endl;			//就输出文件中的原内容
			}

			if (ygg_pd == 0 && cd2 != 0)			//如果未到该输出的指定内容（且该行字符数量不为0）
			{
				while ((wz_spzbf = temp.find("\t", sp_czqd)) != -1)	//查找该行的水平制表符，如果找到水平制表符
				{
					if (wz_spzbf % 8 != 0)			//记录它会填补的个数（填补的x坐标）（水平制表符占用一个字节，所以，一定是从1开始，或9开始，位置不会为0）
					{
						tb_gs += 8 - (wz_spzbf % 8);
					}
					sp_czqd = wz_spzbf + 1;
				}
				cd2 += tb_gs;						//将该行的长度，加上水平制表符填补的个数


				if (cd2 % 120 == 0)					//统计超出一行的总行数
				{
					ygg += cd2 / 120 - 1;
				}
				else
				{
					ygg += cd2 / 120;
				}
			}
		}
	}


	ifs.close();

	int num = 2;					//提示内容和文件内容之间的空隙（单位：行数）（如果上面dy2前面那个换行注释了，需要将这个改成1）
	int gy_sl = czgy_sl(ys);

	if (hswz[jc - 1] % yy_hs == 0)
	{
		y = yy_hs + num - 1;		//注意：光标是从0开始算的，不是1开始，所以这里要-1
	}
	else
	{
		y = hswz[jc - 1] % yy_hs + num - 1;
	}
	y += ygg;

	COORD zb = { x,y };

	//cout << x << endl;
	//cout << y << endl;
	//system("pause");

	if (gy_sl != 0 && gb_yd == 1)
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), zb);		//打印完后，将光标移动到指定内容后面
}

void dy21(string lj, string sr, int ys, int djc, int jc, string color, string color2)
{														//参数：路径,指定内容,当前页数,当前为该页的第几处指定内容,总第几处指定内容
	char lj2[255];
	strzch(lj, lj2);

	char temp[120];								//用于接收文件中每一行内容
	string temp2;

	int wz = 0;									//用于记录指定内容位于第几个字符
	string zdnr;								//记录指定内容
	string gbys = "\033[m";						//加在结尾处的“关闭颜色”功能字符
	int cd = sr.size();
	int cd2 = 0;
	int czqd = 0;								//每次查找的查找起点
	int pdnr = 0;								//判断后面是否还有内容
	int n = 1;									//记录当前为第几处
	int x = 0;									//打印完后，光标移动到指定内容处，x坐标
	int y = 0;									//y坐标

	FILE* file;
	fopen_s(&file, lj2, "rb");
	if (!file)
	{
		fclose(file);
		return;
	}

	for (int i = 1;i < hs + 1;i++)						//读取文件中的每行内容
	{
		fgets(temp, sizeof(temp), file);
		temp2 = temp;

		czqd = 0;
		pdnr = 0;

		if (i > ys * yy_hs)
		{
			break;
		}
		if (i >= (ys - 1) * yy_hs + 1 && i <= ys * yy_hs)		//如果该行为，当前查看的页数的范围内（1~25、26~50）
		{
			if (i != hs)							//如果没有读取到最后一行
			{
				temp2.erase(temp2.size() - 1, 1);	//则删除读取的内容的末尾的换行符，中的\r
			}

			cd2 = temp2.size();						//获取该段文字长度			

			if (xs_hs == 1)							//如果使用了显示行数功能
			{
				cout << i << "：";					//在打印每行前，打印行数
				x += 5;								//光标移动的x轴需要加5
			}

			while ((wz = temp2.find(sr, czqd)) != -1)//如果某行中查找到指定内容
			{
				int i;

				if (djc == n)						//如果为当前查看的某处，则字体颜色为颜色1
				{
					zdnr = color;
					x = wz + cd;					//记录一下该指定内容在终端中的x坐标
				}
				else								//否则，字体颜色为颜色2
				{
					zdnr = color2;
				}

				for (i = czqd;i < wz;i++)			//输出指定内容前面的文字（非指定内容部分）（0或上次查找的结束位置 到 查找到的内容前一个字符）
				{
					cout << temp2[i];
				}

				for (i = 0;i < cd;i++)				//将该行内容中，相同的部分加入zdnr字符串中（获取指定内容）
				{
					zdnr += temp2[wz + i];
				}
				zdnr += gbys;						//在结尾处关闭字体颜色
				cout << zdnr;						//给指定内容加上字体颜色后，打印指定内容

				pdnr = 1;							//如果找到过内容
				
				czqd = wz + cd;						//且继续找，查找起点为：上一次的结尾处（上一次查找到的位置+指定内容的长度）

				n++;
			}

			if (pdnr == 1)						//如果之前找到过内容，且后面还有内容
			{
				for (int i = czqd;i < cd2;i++)	//输出指定内容后面的文字（非指定内容部分）
				{
					cout << temp2[i];
				}
			}
			else
			{
				cout << temp2;					//就输出文件中的原内容
			}
		}

	}

	
	fclose(file);

	int num = 2;					//提示内容和文件内容之间的空隙（单位：行数）（如果上面dy2前面那个换行注释了，需要将这个改成1）
	int gy_sl = czgy_sl(ys);

	if (hswz[jc - 1] % yy_hs == 0)
	{
		y = yy_hs + num - 1;		//注意：光标是从0开始算的，不是1开始，所以这里要-1
	}
	else
	{
		y = hswz[jc - 1] % yy_hs + num - 1;
	}

	COORD zb = { x,y };

	//cout << x << endl;
	//cout << y << endl;
	//system("pause");
	if (gy_sl != 0 && gb_yd == 1)
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), zb);		//打印完后，将光标移动到指定内容后面
}

int czgy_sl(int ys)									//查找该页指定内容数量
{
	int gy_sl = 0;
	int num = (ys - 1) * yy_hs + 1;
	int num2 = ys * yy_hs;

	for (int i = 0;i < sl;i++)
	{
		if (hswz[i] >= num && hswz[i] <= num2)		//如果该行的位置，等于该页数的范围内（1~25、26~50），就记录有一个该页的指定内容
		{
			gy_sl++;
		}
	}
	return gy_sl;
}