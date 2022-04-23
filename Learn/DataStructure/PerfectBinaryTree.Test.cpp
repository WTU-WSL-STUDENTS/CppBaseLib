#include "PerfectBinaryTree.hpp"
#include <memory>
/*
		 a
		/ \
	   b   c
	  / \ /
	 d  e f
	/
   g
*/

using namespace System::DataStructure;
template<typename T>
using NodeType = ThreadBinaryTreeNode<T>;

void ForeachTest()
{
	auto g = std::make_unique<NodeType<char>>();
	g->value = 'g';

	auto d = std::make_unique<NodeType<char>>(g.get());
	d->value = 'd';
	auto e = std::make_unique<NodeType<char>>();
	e->value = 'e';
	auto f = std::make_unique<NodeType<char>>();
	f->value = 'f';

	auto b = std::make_unique<NodeType<char>>(d.get(), e.get());
	b->value = 'b';
	auto c = std::make_unique<NodeType<char>>(f.get());
	c->value = 'c';

	auto a = std::make_unique<NodeType<char>>(b.get(), c.get());
	a->value = 'a';

	Tree::LevelOrderTraversal(a.get(), [](BinaryTreeNode<char> const* item)->void
	{
		printf("%c", item->value);
	});/* abcdefg */
	printf("\n");

	Tree::PreOrderTraversal(a.get(), [](BinaryTreeNode<char> const* item)->void
	{
		printf("%c", item->value);
	});/* abdgecf */
	printf("\n");
	Tree::InOrderTraversal(a.get(), [](BinaryTreeNode<char> const* item)->void
	{
		printf("%c", item->value);
	});/* gdbeafc */
	printf("\n");
	Tree::PostOrderTraversal(a.get(), [](BinaryTreeNode<char> const* item)->void
	{
		printf("%c", item->value);
	});/* gdebfca */
	printf("\n");

	Tree::CreateInOrderTraversalThread(a.get());
	Tree::ThreadInOrderTraversal(a.get(), [](BinaryTreeNode<char>* p)->void
	{
		printf("%c, %p, %p, %p\n", p->value, p, p->pLeftChild, p->pRightChild);
	});
}

int main(int argc, char** argv, char** env)
{
	using Func = void (*)();
	ForeachTest();
	for (int i = 3; i < 0xff; i++)
	{
		size_t level = PerfectBinaryTree::GetHeapDepth(i);
		printf("%zd, %zd\n", level, i - PerfectBinaryTree::CumulativeSum(level));
	}
	return 0;
}