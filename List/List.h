#pragma once

template <class T>
struct ListNode
{
	T value;
	ListNode<T>* head;
public :
	ListNode(T value, ListNode<T>* head)
	{
		this->value = value;
		this->head = head;
	}
};
template <class T>
class List
{
protected:
	size_t count;
	ListNode<T>* lastPtr;
public:
	List()
	{
		lastPtr = nullptr;
	}
	void Add(T t)
	{
		lastPtr = new ListNode<T>(t, lastPtr);
		++count;
	}
	bool Contain(T t)
	{
		ListNode<T>* temp = lastPtr;
		while (temp != nullptr)
		{
			if (temp->value == t)
				return true;
			temp = temp->head;
		}
		return false;
	}
	T EnumAt(int idx)
	{
		//return NULL;
		int loopCount = count - idx - 1;
		ListNode<T>* temp = lastPtr;
		while (loopCount > 0)
		{
			temp = temp->head;
			--loopCount;
		}
		return temp->value;
	}
};

