#include <iostream>
#include <malloc.h>
using namespace std;

#define Free 0 //空闲状态
#define Busy 1 //已分配
#define MAX 640 

int Allocate_Memory();//分配内存
int Free_Memory(int); //回收内存
int InitList();       //初始化双向链表
void show();          //显示分配情况
int FF(int);          //首次适应算法
typedef struct Free_Partition {
	long size;   //分区大小
	long address;//分区起始地址
	int state;   //分区状态
}Free_P;

typedef struct DuLNode              //定义双向链表
{
	Free_P data;
	struct DuLNode* prior, * next; //前趋指针 后继指针
}DuLNode, * DuLinkList;

DuLinkList first; //头结点
DuLinkList last;  //尾结点

//初始化双向链表
int InitList() {
	//  NULL←[prior][first][next] <——>[prior][last(0 640 空闲）][next]→NULL 
	//初始化
	first = (DuLinkList)malloc(sizeof(DuLNode));
	last = (DuLinkList)malloc(sizeof(DuLNode));
	first->prior = NULL;
	first->next = last;
	last->prior = first;
	last->next = NULL;
	last->data.address = 0;
	last->data.size = MAX;
	last->data.state = Free;
	return 1;
}

//分配内存
int Allocate_Memory() {
	int d;//请求分配的内存大小
	cout << "请输入内存大小（单位:KB）：";
	cin >> d;
	if (d < 0 || d == 0) {
		cout << "请求分配内存大小不合理!!" << endl;
		return 0;
	}
	if (FF(d) == 1)
		cout << "内存分配成功!!" << endl;
	else
		cout << "内存分配失败!!!" << endl;
	return 1;
}

//回收内存
int Free_Memory(int d) {

	DuLinkList p = first;
	for (int i = 0; i <= d; i++)
		if (p != NULL)
			p = p->next;
		else
			return 0;

	p->data.state = Free;
	if (p->prior != first && p->prior->data.state == Free)//与前面的空闲块相连
	{
		p->prior->data.size += p->data.size;//空间扩充,合并为一个
		p->prior->next = p->next;//去掉原来被合并的p
		p->next->prior = p->prior;
		p = p->prior;
	}
	else
	{

	}
	if (p->next != last && p->next->data.state == Free)//与后面的空闲块相连
	{
		p->data.size += p->next->data.size;//空间扩充,合并为一个
		if (p->next->next != NULL) {
			p->next = p->next->next;
			p->next->next->prior = p;
		}
		else
		{
			p->next = NULL;
		}
	}
	if (p->next == last && p->next->data.state == Free)//与最后的空闲块相连
	{
		p->data.size += p->next->data.size;
		p->next = NULL;
	}

	return 1;
}

//首次适应算法
int FF(int d) {
	long size = 1;
	//初始化新空间
	DuLinkList New = (DuLinkList)malloc(sizeof(DuLNode));
	New->data.size = d;
	New->data.state = Busy; //表示此空间已分配

	DuLinkList p = first->next;

	//开始分配内存
	while (p)
	{
		//有大小刚好适合的空闲分区
		if ( p->data.state == Free && p->data.size == d) {
			p->data.state = Busy;
			return 1;
			break;
		}
		//有空闲分区并且空间大于请求大小
		if (p->data.state == Free && p->data.size > d) {
			New->prior = p->prior;
			New->next = p;
			New->data.address = p->data.address;
			p->prior->next = New;
			p->prior = New;
			p->data.address = New->data.address + New->data.size;
			p->data.size = p->data.size - d;
			return 1;
			break;
		}
		p = p->next;
	}
	return 0;
}

//显示内存分配情况
void show() {
	int d = 0;//分区号
	DuLinkList p = first->next;
	cout << "\n主存分配情况:" << endl<<endl;
	cout << "|    分区号    |    起始地址    |    分区大小    |    状态    |" << endl << endl;
	while (p)
	{
		cout << "       " << d++ << "\t\t" << p->data.address << "\t      " << p->data.size << "KB\t  ";
		if (p->data.state == Free)
			cout << "    空闲" << endl << endl;
		else
			cout << "    已分配" << endl << endl;
		p = p->next;
	}
}
int main()
{ 
    InitList(); //初始化双向链表
    int d;      //选择操作
	while (1)
	{
		show();
		cout << "请选择操作（1.分配内存 2.回收内存 0.退出）：";
		cin >> d;

		//执行操作
		if (d == 1)
			Allocate_Memory();//分配内存
		else
		{
			if (d == 2) {
				int Num;//分区号
				cout << "请输入要回收的分区号：";
				cin >> Num;
				Free_Memory(Num);
			}
			else
			{
				if (d == 0)
					break;//退出
			}
				

		}
	}

}
