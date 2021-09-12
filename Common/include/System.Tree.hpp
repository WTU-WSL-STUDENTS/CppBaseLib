/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 16:20:15
 * @LastEditors: like
 * @LastEditTime: 2021-08-12 11:59:49
 */
#ifndef SYSTEM_TREE_HPP
#define SYSTEM_TREE_HPP

#include <CompliedEntry.h>
// #include <vector>
#include <System.INodeList.hpp>

using namespace System;
 
template<class TKey, class TVal>
class TreeNode : INode<TKey, TVal>{};




// #ifndef TREENODE_CHILDREN_COUNT
//     template<class T>
//     class ChildNodes : public std::vector<INode<T>>
//     {
//     };
// #endif
// template<class T>
// class TreeNode : public INode<T>
// {
// private:
//     size_t childCount;
//     #ifndef TREENODE_CHILDREN_COUNT
//         ChildNodes<INode<T>*> Children;
//         #define CHILD_AT(idx) (Children[idx])
//     #else
//         INode<T>* Children[TREENODE_CHILDREN_COUNT];
//         #define CHILD_AT(idx) (Children.at(idx))
//     #endif
// public:
//     TreeNode() : childCount(0){}
//     ~TreeNode(){}
//     size_t Count(){return childCount;}
//     void GetChildren
    
//     #ifdef TREENODE_CHILDREN_COUNT
//     void GetChildren(INode<T>* (&Children)[TREENODE_CHILDREN_COUNT]){child = Children;}
//     #else
//     void GetChildren(ChildNodes<INode<T>*>& child){child = Children;}
//     #endif

//     bool Append(T data)
//     {
//         if(TREENODE_CHILDREN_COUNT - 1 > childCount)
//         {
//             CHILD_AT(childCount) = new INode(this, data);
//             childCount++;
//             return true;
//         }
//         return false;
//     }
//     bool Append(INode<T>* child)
//     {
//         if(TREENODE_CHILDREN_COUNT - 1 > childCount)
//         {
//             child->parent = this;
//             CHILD_AT(childCount) = child;
//             childCount++;
//             return true;
//         }
//         return false;  
//     }
//     bool ReplaceAt(T data, size_t index)
//     {
//         if(childCount > index)
//         {
//             delete Children[index];
//             Children[index] = new INode(this, data);
//         }
//         return false;
//     }
//     bool ReplaceAt(INode<T>* data, size_t index)
//     {
//         if(childCount > index)
//         {
//             delete Children[index];
//             data->parent = this;
//             Children[index] = data;
//             return true;
//         }
//         return false;
//     }
//     INode<T>* IndexOf(size_t index) 
//     {
//         if(TREENODE_CHILDREN_COUNT > index)
//         {
//             return Children[index];
//         }
//         return NULL;
//     }
//     INode<T>* IndexOf(size_t index) 
//     {
//         if(TREENODE_CHILDREN_COUNT > index)
//         {
//             return Children[index];
//         }
//         return NULL;
//     }
//     template<class _Lamda>
//     INode<T>* FindIf(size_t begin size_t& end/* 返回的 查找到的元素下标 */,_Lamda _lamda)
//     {
//         if(end > childCount)
//         {
//             end = childCount;
//         }
//         for(int i = begin; i < end; i++)
//         {
//             if(_lamda(Children[index]))
//             {
//                 end = i;
//                 return Children[index];
//             }
//         }
//         return NULL;
//     };
// };


#endif