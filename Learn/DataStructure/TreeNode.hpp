#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP
#include <CompliedEntry.h>
#include <queue>

using std::queue;

namespace System::DataStructure
{
	template<typename T>
	class BinaryTreeNode;
	template<typename T>
	class TernaryTreeNode;
	template<typename T>
	class ThreadBinaryTreeNode;

	class Tree;
};

template<typename T>
class System::DataStructure::BinaryTreeNode/* ������ */
{
public:
	T value{};
	WEAK_PTR(BinaryTreeNode<T>) pLeftChild;
	WEAK_PTR(BinaryTreeNode<T>) pRightChild;
	BinaryTreeNode
	(
		WEAK_PTR(BinaryTreeNode<T>) leftChild = nullptr, 
		WEAK_PTR(BinaryTreeNode<T>) rightChild = nullptr
	) : pLeftChild(leftChild), pRightChild(rightChild)
	{}
};
template<typename T>
class System::DataStructure::TernaryTreeNode : public BinaryTreeNode<T>/* ������ */
{
public:
	WEAK_PTR(BinaryTreeNode<T>) pParent;
	TernaryTreeNode
	(
		WEAK_PTR(BinaryTreeNode<T>) parent	= nullptr,
		WEAK_PTR(BinaryTreeNode<T>) leftChild = nullptr,
		WEAK_PTR(BinaryTreeNode<T>) rightChild= nullptr
	) : BinaryTreeNode<T>(leftChild, rightChild), pParent(parent)
	{}
};
template<typename T>
class System::DataStructure::ThreadBinaryTreeNode : public BinaryTreeNode<T>/* ���������� */
{
private:
	int tag;
public:
	ThreadBinaryTreeNode
	(
		WEAK_PTR(BinaryTreeNode<T>) leftChild = nullptr,
		WEAK_PTR(BinaryTreeNode<T>) rightChild = nullptr
	) : BinaryTreeNode<T>(leftChild, rightChild), 
		tag((leftChild ? 0 : 1) | (rightChild ? 0 : 2))
	{}
	inline bool HasPrecursor()
	{
		return (tag & 1);
	}
	inline bool HasSubsequent()
	{
		return (tag & 2);
	}
};
class System::DataStructure::Tree
{
private:
	Tree() {}
	template<typename T>
	void CreateMaxHeap(BinaryTreeNode<T>* root, T& val)
	{

	}
public:
	template<typename T>
	static BinaryTreeNode<T>* ToHeap(std::initializer_list<T> list)
	{
		BinaryTreeNode<T>* root = new BinaryTreeNode<T>;
		auto it = list.begin();
		root->value = *it;
	}
	template<typename T>
	BinaryTreeNode<T>* CreateMaxHeap(std::initializer_list<T> list)/* �����, T �������ظ�ֵ���캯�� */
	{
		VALRET_ASSERT(list.size(), nullptr);
		BinaryTreeNode<T>* root = new BinaryTreeNode<T>;
		root->value = *list.begin();
		auto it = list.begin() + 1;
		for (; it != list.end(); ++it)
		{
			CreateMaxHeap(root, *it);
		}
	}

	template<typename T, typename FLamda>
	static void LevelOrderTraversal(BinaryTreeNode<T>* root, FLamda func)/* ������� */
	{
		VOIDRET_ASSERT(root);
		queue<BinaryTreeNode<T>*> fifo;
		fifo.push(root);
		do
		{
			root = fifo.front();
			fifo.pop();
			func(root);
			if (root->pLeftChild)
			{
				fifo.push(root->pLeftChild);
			}
			if (root->pRightChild)
			{
				fifo.push(root->pRightChild);
			}
		} while (fifo.size());
	}
	template<typename T, typename FLamda/* [](BinaryTreeNode<T>*) */>
	static void PreOrderTraversal(BinaryTreeNode<T>* root, FLamda func)/* ������� */
	{
		VOIDRET_ASSERT(root);
		func(root);
		PreOrderTraversal(root->pLeftChild, func);
		PreOrderTraversal(root->pRightChild, func);
	}
	template<typename T, typename FLamda>
	static void InOrderTraversal(BinaryTreeNode<T>* root, FLamda func)/* ������� */
	{
		VOIDRET_ASSERT(root);
		InOrderTraversal(root->pLeftChild, func);
		func(root);
		InOrderTraversal(root->pRightChild, func);
	}
	template<typename T, typename FLamda>
	static void PostOrderTraversal(BinaryTreeNode<T>* root, FLamda func)/* ������� */
	{
		VOIDRET_ASSERT(root);
		PostOrderTraversal(root->pLeftChild, func);
		PostOrderTraversal(root->pRightChild, func);
		func(root);
	}
	/* 
	 * ��������������������ǰ�����򡢺���
	 * https://blog.csdn.net/qq_44869796/article/details/122526069 
	 */
	template<typename T>
	static void CreateInOrderTraversalThread(ThreadBinaryTreeNode<T>* root)/* ��������������������� */
	{
		ThreadBinaryTreeNode<T>* pre = nullptr;
		InOrderTraversal(root, [&](BinaryTreeNode<T>* item)->void
		{
			ThreadBinaryTreeNode<T>* p = (ThreadBinaryTreeNode<T>*)item;
			if (pre && pre->HasSubsequent())
			{
				pre->pRightChild = item;
			}
			if (p->HasPrecursor())
			{
				item->pLeftChild = pre;
			}
			pre = p;
		});
	}
	template<typename T, typename FLamda>
	static void ThreadInOrderTraversal(ThreadBinaryTreeNode<T>* root, FLamda func)/* ������������������ */
	{
		BinaryTreeNode<char>* p = root; /* pָ������ */
		while (p) /* ���� */
		{
			while (!((ThreadBinaryTreeNode<char>*)p)->HasPrecursor() /* && p->pLeftChild*/) /* �ɸ����һֱ�ҵ��������������� */
				p = p->pLeftChild;

			func(p);

			while (((ThreadBinaryTreeNode<char>*)p)->HasSubsequent() && p->pRightChild)
			{
				p = p->pRightChild;
				func(p);
			}
			p = p->pRightChild;   /* ��p->rchild��������(���Һ���)��pָ���Һ��ӣ�����ѭ����*/
		}
	}
};

#endif