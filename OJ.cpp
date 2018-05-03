#include<iostream>
using namespace std;
#pragma  warning(disable:4996)
struct Related // �������
{
	int num;
	Related* next;
	Related()
	{
		next = NULL;
	}
};
struct Path  // ��¼��ʱ·�� , ��һ���ڵ�����ţ
{
	int num;
	Path* next;
	Path()
	{
		next = NULL;
	}
};
struct Node // ��ţ��̯λ
{
	bool matched;   // �Ƿ���ƥ��
	bool can_be;    // �Ƿ����ƥ��
	int match_num;  // �Է����±�
	Related* related; // ������
	Node()
	{
		matched = false;
		can_be = true;
		match_num = -1;
		related = NULL;
	}
};

class Process // ����ʱʹ�õ�ȫ�����±�
{
private:
	Node* cow;        // ��ţ�б�
    Node* stall;      // ̯λ�б�
	int cow_num;	  // ��ţ��Ŀ
	int stall_num;    // ̯λ��Ŀ
	int result;       // ���

	void Add_cow_node( int nth ,int num ) // ��ĳֻ��ţ�ϼӹ���
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
	void Add_stall_node( int nth, int num ) // ��ĳ��̯λ�ϼӹ���
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
    void Add_path( Path* &p , int num ) // ��path�����һ�����
	{
        Path* p1 = new Path(); // �����½��
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
	bool Already_visited( Path* &p , int index ) // stall �е�һ���ڵ��Ƿ��Ѿ���path��
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
	void Del_path(Path* &p) // �ˣ���ɾ��һ��·�ϵ������ڵ�
	{
		if (!p)
			return;
		Path* temp = p;
		Path* temp1 = p;
		while (temp->next->next)
			temp = temp->next; // temp�ǵ����ڶ���
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
	
	bool Find_way( int i , Path* &p ) // Ѱ��һ��·
	{
		Related* temp = cow[i].related; // temp ָ������߼�      
		while ( temp ) // ��������һ��δ���ʵı�
		{ 
			if (!Already_visited(p, temp->num) && stall[temp->num].can_be)
			{
				if (!stall[temp->num].matched) // δƥ�䣬�����ý�㷵��
				{
					Add_path(p, i); // �����ʼ�ڵ�
					Add_path(p, temp->num);
					return true;
				}
				else // ��ƥ������ݹ����
				{
					Add_path(p, i); // �����ʼ�ڵ�
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
		// �Ҳ������ʵ�·ʱp�ǿյ�
	}
	void Set_way(Path* &p) // ����һ��·, p��һ������·, ����Ϊ��
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
	void Free_way(Path* &p) // �ͷ�һ��·
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