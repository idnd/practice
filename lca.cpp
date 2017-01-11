// lca.cpp : Defines the entry point for the console application.
// least common ascessor

#include "stdafx.h"
#include <memory>
#include <random>
#include <iostream>
#include <deque>


template<class T>
class Tree {
	struct Node
	{
		T val;
		std::shared_ptr<Node> parent;
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
		Node(T val_, std::shared_ptr<Node> parent_) : val(val_), parent(parent_) {}
	};
	typedef std::shared_ptr<Node> pointer;
	pointer root = nullptr;
	pointer val_to_node(T val) {
		// returns node with selected val
		// if val not found returns zero pointer
		pointer p = root;
		while (p && p->val != val) {
			if (p->val > val) {
				p = p->left;
			}
			else {
				p = p->right;
			}
		}
		return p;
	}
	int node_height(pointer p);
	bool dfs_recursive(T val, pointer node) {
		if (!node) {
			return false;
		}
		if (node->val == val) {
			return true;
		}
		if (dfs_recursive(val, node->left)) {
			return true;
		}
		return dfs_recursive(val, node->right);
	}
	bool bfs_recursive(T val, std::deque<pointer> deq);
public:
	bool append(T val);
	void print();
	void print(pointer p);
	T lca(T u, T v);
	bool dfs(T val);
	bool bfs(T val);
};


template<class T>
bool Tree<T>::append(T val) {
	if (!root) {
		root = std::make_shared<Node>(val, nullptr);
		return true;
	}
	pointer p = root;
	while (true) {
		if (val > p->val) {
			if (p->right) {
				p = p->right;
			}
			else {
				p->right = std::make_shared<Node>(val, p);
				break;
			}
		}
		else {
			if (p->left) {
				p = p->left;
			}
			else {
				p->left = std::make_shared<Node>(val, p);
				break;
			}
		}
	}
	return true;
}


template<class T>
void Tree<T>::print(pointer p) {
	// print node subtree
	if (!p)
		return;
	print(p->left);
	std::cout << p->val;
	print(p->right);
}


template<class T>
void Tree<T>::print() {
	// print tree nodes
	print(root->left);
	std::cout << root->val;
	print(root->right);
}


template<class T>
T Tree<T>::lca(T u_, T v_) {
	// least common ancestor
	pointer u = val_to_node(u_);
	pointer v = val_to_node(v_);
	if (!u || !v) {
		// node not found
		return NULL;
	}
	int h1 = node_height(u);
	int h2 = node_height(v);
	while (h1 != h2)	{
		if (h1 > h2) {
			u = u->parent;
			h1--;
		} else {
			v = v->parent;
			h2--;
		}
	}
	
	while (u != v) {
		u = u->parent;
		v = v->parent;
	}
	return u->val;
}


template<class T>
int Tree<T>::node_height(pointer p) {
	// calc node height below root
	pointer p_ = p;
	int height = -1;
	while (p_) {
		height++;
		p_ = p_->parent;
	}
	return height;
}

template<class T>
bool Tree<T>::dfs(T val) {
	// depth-first search
	if (!root) {
		return false;
	}
	if (val == root->val) {
		return true;
	}
	if (dfs_recursive(val, root->left)) {
		return true;
	}
	return dfs_recursive(val, root->right);
}


template<class T>
bool Tree<T>::bfs(T val) {
	// bread-first search
	if (!root) {
		return false;
	}
	if (val == root->val) {
		return true;
	}
	std::deque<pointer> deq;
	deq.push_back(root->left);
	deq.push_back(root->right);
	return bfs_recursive(val, deq);
}


template<class T>
bool Tree<T>::bfs_recursive(T val, std::deque<pointer> deq) {
	if (deq.empty()) {
		return false;
	}
	pointer node = deq.front();
	deq.pop_front();
	if (node) 
	{
		if (val == node->val) {
			return true;
		}
		deq.push_back(node->left);
		deq.push_back(node->right);
	}
	return bfs_recursive(val, deq);
}


int main()
{
	Tree<int> t;
	std::vector<int> init_val{1,4,2,6};
	for (auto it = init_val.begin(); it != init_val.end(); it++) {
		t.append(*it);
	}
	t.print();
	std::cout << std::endl;
	std::cout << t.lca(21, 6) << std::endl;
	std::cout << t.bfs(21) << " " << t.bfs(6) << std::endl;
	std::cout << t.dfs(21) << " " << t.dfs(6) << std::endl;
    return 0;
}

