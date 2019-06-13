#include <iostream>
#include <queue>
using namespace std;

template<class K> class AVL;

template<class K>
class Node
{
	friend class AVL<K>;
private:
	K key;
	Node<K> *leftChild, *rightChild, *parent;
public:
	Node(K k)
	{
		key = k;
		leftChild = rightChild = parent = NULL;
	}
};

template<class K>
class AVL
{
private:
	Node<K> *root;
	int count; // main에서 초기값으로 입력되는 19개의 숫자는 조회하지 않도록 하는 카운트 변수
public:
	//생성자
	AVL()
	{
		root = NULL;
		count = 0;
	}

	//LL회전
	Node<K>* LL(Node<K>* node)
	{
		Node<K>* ptr;
		ptr = node->leftChild;
		node->leftChild = ptr->rightChild;
		ptr->rightChild = node;

		//기존의 부모 노드에 대한 관계 갱신
		ptr->parent = node->parent;
		if (ptr->parent->leftChild == node)
		{
			ptr->parent->leftChild = ptr;
		}
		else
		{
			ptr->parent->rightChild = ptr;
		}
		node->parent = ptr;

		return ptr;
	}
	//RR회전
	Node<K>* RR(Node<K>* node)
	{
		Node<K>* ptr;
		ptr = node->rightChild;
		node->rightChild = ptr->leftChild;
		ptr->leftChild = node;

		//기존의 부모 노드에 대한 관계 갱신
		ptr->parent = node->parent;
		if (ptr->parent->leftChild == node)
		{
			ptr->parent->leftChild = ptr;
		}
		else
		{
			ptr->parent->rightChild = ptr;
		}
		node->parent = ptr;

		return ptr;
	}
	//LR회전
	Node<K>* LR(Node<K>* node)
	{
		Node<K>* ptr;
		ptr = node->leftChild;
		node->leftChild = RR(ptr);

		return LL(node);
	}
	//RL회전
	Node<K>* RL(Node<K>* node)
	{
		Node<K>* ptr;
		ptr = node->rightChild;
		node->rightChild = LL(ptr);

		return RR(node);
	}
	// 트리의 높이를 구하기 위한 함수
	int GetH(Node<K> * node)
	{
		int leftH, //node의 왼쪽 서브트리 높이
			rightH; //오른쪽 높이

		if (node == NULL)
		{
			return 0;
		}

		leftH = GetH(node->leftChild);
		rightH = GetH(node->rightChild);

		if (leftH > rightH)
		{
			return leftH + 1;
		}
		else
		{
			return rightH + 1;
		}
	}

	int GetBF(Node<K> * node)
	{
		int lh, //왼쪽 서브트리 높이
			rh; //오른쪽 서브트리 높이

		if (node == NULL)
		{
			return 0;
		}

		lh = GetH(node->leftChild);
		rh = GetH(node->rightChild);

		return lh - rh;
	}
	// AVL 균형을 맞추기 위한 밸런싱 함수
	Node<K>* Balance(Node<K>* node)
	{
		if (node == NULL)
		{
			return node;
		}

		int bf = GetBF(node);

		if (bf > 1) //왼쪽 서브 트리 방향으로 높이가 2 이상 크다면
		{
			if (GetBF(node->leftChild) >= 0)
			{
				node = LL(node);
			}
			else
			{
				node = LR(node);
			}
		}

		if (bf < -1) //오른쪽 서브 트리 방향으로 높이가 2 이상 크다면
		{
			if (GetBF(node->rightChild) <= 0)
			{
				node = RR(node);
			}
			else
			{
				node = RL(node);
			}
		}
		return node;
	}
	//탐색 함수
	void Search(K k)
	{
		Node<K>* ptr = root;
		if (ptr == NULL)
		{
			return;
		}
		while (ptr != NULL)
		{
			if (k == ptr->key)
			{
				cout << ptr->key << endl;
				return;
			}
			else if (k < ptr->key)
			{
				cout << ptr->key << " -> ";
				ptr = ptr->leftChild;
				continue;
			}
			else if (k > ptr->key)
			{
				cout << ptr->key << " -> ";
				ptr = ptr->rightChild;
				continue;
			}

		}
		cout << endl;
		cout << "-------------해당 키가 존재하지 않습니다.-------------" << endl;
	}
	//삽입 함수
	void Add(K k)
	{
		//트리에 어떠한 노드도 존재하지 않을 때
		if (root == NULL)
		{
			root = new Node<K>(k);
			return;
		}

		Node<K>* ptr = root; // 비어있는 노드일 때까지 탐색
		Node<K>* pptr = NULL; //ptr의 부모 노드

							  //삽입 위치에 대한 탐색
		while (ptr != NULL)
		{
			pptr = ptr;
			if (k < ptr->key)
			{
				ptr = ptr->leftChild;
				continue;
			}
			if (k > ptr->key)
			{
				ptr = ptr->rightChild;
				continue;
			}
			if (k == ptr->key)
			{
				//AVL트리에 대한 전체 레벨 순회
				LevelOrder();
				cout << "-------------해당 키가 이미 존재합니다.-------------" << endl;
				return;
			}
		}
		// 탐색한 삽입 위치에 키 값 k를 가지는 노드 삽입
		if (pptr != NULL)
		{
			if (k < pptr->key)
			{
				pptr->leftChild = new Node<K>(k);
				pptr->leftChild->parent = pptr;
			}
			else if (k > pptr->key)
			{
				pptr->rightChild = new Node<K>(k);
				pptr->rightChild->parent = pptr;
			}
			else if (k == pptr->key)
			{
				LevelOrder();
				return;
			}
		}

		//AVL트리를 만들기 위해 균형 조정
		while (pptr->parent != NULL)
		{
			pptr = Balance(pptr);
			pptr = pptr->parent;
		}

		//삽입or삭제시 트리 조회시켜줘야함!!
		//이곳에 큐 사용하여 레벨순회 하도록 하자!! 삭제의 경우에도 마찬가지.
		if (count >= 17)
		{
			//AVL트리에 대한 전체 레벨 순회
			LevelOrder();
		}
		count++; //한번 Add호출될 때 마다 count변수 1증가
	}
	//삭제 함수
	void Delete(K k)
	{
		Node<K>* ptr = root; //삭제할 노드를 찾기 위한 ptr
		Node<K> * pptr; //ptr의 부모 노드
		Node<K> * tmpptr; //임시 노드

		// 삭제 대상을 저장한 노드 탐색
		while (ptr != NULL&&ptr->key != k)
		{
			pptr = ptr;
			if (k < ptr->key)
			{
				ptr = ptr->leftChild;
			}
			else
			{
				ptr = ptr->rightChild;
			}
		}

		//대상이 존재하지 않는다면,
		if (ptr == NULL)
		{
			//AVL트리에 대한 전체 레벨 순회
			LevelOrder();
			cout << "-------------해당 키가 존재하지 않습니다.-------------" << endl;
			return;
		}

		tmpptr = ptr;
		//첫번째: 삭제할 노드가 자식을 갖지 않을 경우
		if (tmpptr->leftChild == NULL&&tmpptr->rightChild == NULL)
		{
			if (pptr->leftChild == tmpptr)
			{	
				pptr->leftChild = NULL;
			}
			else
			{
				pptr->rightChild = NULL;
			}
		}

		//두번째: 하나의 자식을 갖는 경우
		else if (tmpptr->leftChild == NULL || tmpptr->rightChild == NULL)
		{
			Node<K> * ctmpptr; // tmpptr의 자식
			if (tmpptr->leftChild == NULL) //오른쪽 자식만을 갖는 경우
			{
				ctmpptr = tmpptr->rightChild;
			}
			else //왼쪽 자식만을 갖는 경우
			{
				ctmpptr = tmpptr->leftChild;
			}

			if (pptr->leftChild == tmpptr)
			{
				pptr->leftChild = ctmpptr;
				ctmpptr->parent = pptr;
			}
			else
			{
				pptr->rightChild = ctmpptr;
				ctmpptr->parent = pptr;
			}
		}

		//세번째: 두 개의 자식을 갖는 경우
		else
		{
			//lc는 지울 노드의 왼쪽 서브트리의 루트 노드를 가리킨다.
			Node<K> *lc = tmpptr->leftChild;
			if (!lc->rightChild)
			{
				tmpptr->key = lc->key;
				tmpptr->leftChild = lc->leftChild;
				delete lc;
			}
			else
			{
				//삭제할 노드를 대체할 노드 탐색
				while (lc->rightChild != NULL)
				{
					pptr = lc;
					lc = lc->rightChild;
				}
				//대체할 노드에 저장된 값을 삭제할 노드에 대입한다.
				tmpptr->key = lc->key;
				pptr->rightChild = lc->leftChild;
				delete lc;
			}
		}
		//AVL트리를 만들기 위해 균형 조정
		while (pptr->parent != NULL)
		{
			pptr = Balance(pptr);
			pptr = pptr->parent;
		}
		//AVL트리에 대한 전체 레벨 순회
		LevelOrder();
	}
	//트리 조회를 위한 레벨 순회 함수
	void LevelOrder()
	{
		queue< Node<K>* > q; //레벨 순회를 위한 큐
		q.push(root);
		while (!q.empty())
		{
			Node<K>* tmp = q.front();
			cout << tmp->key;
			if (tmp->leftChild != NULL)
			{
				q.push(tmp->leftChild);
				cout << " left: " << tmp->leftChild->key;
			}
			if (tmp->rightChild != NULL)
			{
				q.push(tmp->rightChild);
				cout << " right: " << tmp->rightChild->key;
			}
			cout << endl;
			q.pop();
		}
	}
};

int main()
{
	AVL<int> avlT;
	//주어진 초기값을 직접 AVL트리에 입력
	//19,10,46,4,14,37,55,7,12,18,28,40,51,61,21,32,49,58 순서
	avlT.Add(19);
	avlT.Add(10);
	avlT.Add(46);
	avlT.Add(4);
	avlT.Add(14);
	avlT.Add(37);
	avlT.Add(55);
	avlT.Add(7);
	avlT.Add(12);
	avlT.Add(18);
	avlT.Add(28);
	avlT.Add(40);
	avlT.Add(51);
	avlT.Add(61);
	avlT.Add(21);
	avlT.Add(32);
	avlT.Add(49);
	avlT.Add(58);

	while (1)
	{
		int sel1;
		cout << "AVL Tree" << endl;
		cout << "1. Search" << endl;
		cout << "2. Add" << endl;
		cout << "3. Delete" << endl;
		cin >> sel1;

		switch (sel1)
		{
			int sel2;
		case 1:
			cout << "Select : ";
			cin >> sel2;
			cout << "[Result]" << endl;
			avlT.Search(sel2);
			break;
		case 2:
			cout << "Add : ";
			cin >> sel2;
			cout << "[Result]" << endl;
			avlT.Add(sel2);
			break;
		case 3:
			cout << "Delete : ";
			cin >> sel2;
			cout << "[Result]" << endl;
			avlT.Delete(sel2);
			break;
		}
	}
}