// https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%94%D0%B5%D0%B9%D0%BA%D1%81%D1%82%D1%80%D1%8B

#include "stdafx.h"
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>


template<class T>
class Graph {
	struct Node;
	typedef std::shared_ptr<Node> node_pointer;
	struct Edge{
		node_pointer destination;
		int weight;
		Edge(node_pointer d, int w) : destination(d), weight(w) {}
	};
	struct Node
	{
		T val;
		std::vector<Edge> links;
		Node(T val_) : val(val_) {};

		// Dijkstra’s mark
		int mark;
		bool visited;
	};

	std::map<T, std::shared_ptr<Node>> nodes;

	void dij_init(T val);
public:
	Graph(std::initializer_list<T> links);
	bool add_links(T val, std::initializer_list<std::pair<T, int>> links);
	void dijkstra(T val);
};


template<class T>
Graph<T>::Graph(std::initializer_list<T> nodes_) {
	// constructor with initial nodes
	for (auto i = nodes_.begin(); i != nodes_.end(); i++) {
		nodes[*i] = std::make_shared<Node>(*i);
	}
}


template<class T>
bool Graph<T>::add_links(T val, std::initializer_list<std::pair<T, int>> link_list) {
	// add edges between nodes

	// check destination existance
	for (auto i = link_list.begin(); i != link_list.end(); i++) {
		if (nodes.find((*i).first) == nodes.end()) {
			return false;
		}
	}
	
	// add edges
	for (auto i = link_list.begin(); i != link_list.end(); i++) {
		// destination node val
		auto dest = nodes[(*i).first];

		nodes[val]->links.push_back(Edge(dest, (*i).second));
		nodes[(*i).first]->links.push_back(Edge(nodes[val], (*i).second));
	}
	return true;
}


/// Dijkstra’s algorithm
template<class T>
void Graph<T>::dij_init(T val) {
	for (auto i = nodes.begin(); i != nodes.end(); i++) {
		(*i).second->visited = false;
		if ((*i).first == val) {
			(*i).second->mark = 0;
		}
		else {
			(*i).second->mark = INT_MAX;
		}
	}
}


template<class T>
void Graph<T>::dijkstra(T val) {
	dij_init(val);
	for (int i = 0; i < nodes.size(); i++) {
		// elementh with min mark, without visited
		// todo: is it possible to: ?
		// auto min_mark = (*std::min_element(nodes.begin(), nodes.end(), [](auto f1, auto f2) { return f1.second->mark < f2.second->mark && !f1.second->visited;	})).second;
		auto min_mark = (*nodes.begin()).second;  
		min_mark = nullptr;
		for (auto j = nodes.begin(); j != nodes.end(); j++) {
			if (!(*j).second->visited) {
				if (min_mark == nullptr || ((*j).second->mark < min_mark->mark)) {
					min_mark = (*j).second;
				}
			}
		}
		
		min_mark->visited = true;
		for (auto j = (*min_mark).links.begin(); j != (*min_mark).links.end(); j++) {
			int new_length = (*min_mark).mark + (*j).weight;
			if (new_length < (*j).destination->mark) {
				(*j).destination->mark = new_length;
			}				
		}
	}
}


int main()
{
	Graph<int> g = {1,2,3,4,5,6};
	g.add_links(1, { { 6,14 },{ 2,7 }, {3, 9} });
	g.add_links(2, { { 3,10 },{ 4,15 } });
	g.add_links(4, { { 5,6 }, {3,11} });
	g.add_links(6, { { 5,9 }, { 3,2 } });

	g.dijkstra(1);

    return 0;
}

