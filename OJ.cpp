#include<iostream>
using namespace std;
#pragma  warning(disable:4996)
struct Related // 相关链表
{
	int num;
	Related* next;
	Related()
	{
		next = NULL;
	}
};
struct Path  // 记录临时路径 , 第一个节点是奶牛
{
	int num;
	Path* next;
	Path()
	{
		next = NULL;
	}
};
struct Node // 奶牛和摊位
{
	bool matched;   // 是否已匹配
	bool can_be;    // 是否可以匹配
	int match_num;  // 对方的下标
	Related* related; // 边链表
	Node()
	{
		matched = false;
		can_be = true;
		match_num = -1;
		related = NULL;
	}
};

class Process // 运算时使用的全部是下标
{
private:
	Node* cow;        // 奶牛列表
    Node* stall;      // 摊位列表
	int cow_num;	  // 奶牛数目
	int stall_num;    // 摊位数目
	int result;       // 结果

	void Add_cow_node( int nth ,int num ) // 在某只奶牛上加关联
	{
		Related* p = new Related();
		p->num = num;
		if (!cow[nth].related)
		{
			cow[nth].related = p;
			return;
		}
		else
		{
			p->next = cow[nth].related;
			cow[nth].related = p;
			return;
		}
	}
	void Add_stall_node( int nth, int num ) // 在某个摊位上加关联
	{
		Related* p = new Related();
		p->num = num;
		if (!stall[nth].related)
		{
			stall[nth].related = p;
			return;
		}
		else
		{
			p->next = stall[nth].related;
			stall[nth].related = p;
			return;
		}
	}
    void Add_path( Path* &p , int num ) // 在path上添加一个结点
	{
        Path* p1 = new Path(); // 创建新结点
        p1->num = num;
		p1->next = NULL;
		if (!p)
			p = p1;
		else
		{
            Path* temp = p;
            while (temp->next)
				temp = temp->next;
			temp->next = p1;
		}
	}
	bool Already_visited( Path* &p , int index ) // stall 中的一个节点是否已经在path上
	{
		if (!p)
			return false;
		else
		{
			Path* temp = p;		
			do
			{
				temp = temp->next;
				if (temp->num == index)
					return true;
				else
					temp = temp->next;
			} while (temp);
			return false;
		}
	}
	void Del_path(Path* &p) // 退，即删除一条路上的两个节点
	{
		if (!p)
			return;
		Path* temp = p;
		Path* temp1 = p;
		while (temp->next->next)
			temp = temp->next; // temp是倒数第二个
		while ((temp1 != temp) && (temp1->next != temp))
		{
			temp1 = temp1->next;
		}
		if (temp1 == temp)
		{
			stall[temp->next->num].can_be = false;
			delete temp->next;
			delete temp;
			p = NULL;
			return;
		}
		else
		{
			stall[temp->next->num].can_be = false;
			delete temp->next;
            delete temp;			
			temp1->next = NULL;
			return;
		}
	}
public:
	Process( int N , int M )
	{
		cow = new Node[N];
		stall = new Node[M];
		cow_num = N;
		stall_num = M;
		result = 0;
		for (int i = 0; i < N; i++)
		{
			int rela_num;
			scanf("%d", &rela_num);
			for (int j = 0; j < rela_num; j++)
			{
				int num;
				scanf("%d", &num);
				Add_cow_node(i, num - 1);
				Add_stall_node(num - 1, i);
			}
		}
	}
	~Process()
	{
		for (int i = 0; i < cow_num; i++)
		{
			while (cow[i].related)
			{
				Related* temp = cow[i].related;
				cow[i].related = cow[i].related->next;
				delete temp;
			}
		}
		for (int i = 0; i < stall_num; i++)
		{
			while (stall[i].related)
			{
				Related* temp = stall[i].related;
				stall[i].related = stall[i].related->next;
				delete temp;
			}
		}
		delete[] cow;
		delete[] stall;
	}
	
	bool Find_way( int i , Path* &p ) // 寻找一条路
	{
		Related* temp = cow[i].related; // temp 指向关联边集      
		while ( temp ) // 对于其中一条未访问的边
		{ 
			if (!Already_visited(p, temp->num) && stall[temp->num].can_be)
			{
				if (!stall[temp->num].matched) // 未匹配，则加入该结点返回
				{
					Add_path(p, i); // 加入初始节点
					Add_path(p, temp->num);
					return true;
				}
				else // 已匹配过，递归进行
				{
					Add_path(p, i); // 加入初始节点
					Add_path(p, temp->num);
					if (Find_way(stall[temp->num].match_num, p))
						return true;
					else
						Del_path(p);
				}
			}
			temp = temp->next;
		}
		cow[i].can_be = false;
		return false;
		// 找不到合适的路时p是空的
	}
	void Set_way(Path* &p) // 设置一条路, p是一条增广路, 可能为空
	{
		if (!p)
			return;
		result++;
		Path* temp = p;
		while (temp)
		{
			cow[temp->num].matched = true;
			stall[temp->next->num].matched = true;
			cow[temp->num].match_num = temp->next->num;
			stall[temp->next->num].match_num = temp->num;
			temp = temp->next->next;
		}
	}
	void Free_way(Path* &p) // 释放一条路
	{
		while (p)
		{
			Path* temp = p;
			p = temp->next;
			delete temp;
		}
	}
	int Do()
	{ 
		while (1)
		{
			bool can = false;
			for (int i = 0; i < cow_num; i++)
			{
				if (cow[i].can_be && !cow[i].matched)
				{
					Path* p = NULL; 
					Find_way(i, p);
					Set_way(p);
					Free_way(p);
					can = true;
				}
			}
			if (!can)
				break;
		}
		return result;
	}
	
};

int main()
{
	int N , M;
	while (cin >> N)
	{
		cin >> M;
		Process pro( N , M );
		printf("%d\n", pro.Do());
	}
	return 0;
}