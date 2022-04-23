#ifndef PERFECT_BINARY_TREE_HPP
#define PERFECT_BINARY_TREE_HPP
#include "TreeNode.hpp"
class PerfectBinaryTree
{
public:
	/* 0 - n */
	static size_t GetHeapDepth(size_t nodeCount)
	{
		ERROR_ASSERT(nodeCount, "Input param error");
		size_t level = 0;
		nodeCount -= 1;
		size_t nextLevelCount = 2;
		while (nextLevelCount <= nodeCount)
		{
			level++;
			nodeCount -= nextLevelCount;
			nextLevelCount <<= 1;
		}
		return level;
	}
	/* n ²ãÀÛ¼ÆÇóºÍ */
	static size_t CumulativeSum(size_t level)
	{
		size_t currentLevelCount = 1;
		size_t sum = currentLevelCount;
		while (level-- > 0)
		{
			currentLevelCount <<= 1;
			sum += currentLevelCount;
		}
		return sum;
	}
};
#endif