/*
	Graph class
	directed v undirected
*/

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm> /* for std::find, std::sort */
#include <queue>	 // std::queue
// for MST
#include <map>
#include <unordered_map>

// Dijkstra implementation
#include "PriorityQueue.h"
#include "DisjointSet.h"

using std::cout;
using std::string;
using std::vector;

template <class T>
struct Node;

template <class T>
class Edge;

template <class T>
struct AdjNode
{
	Node<T> *node;
	float weight;
	struct AdjNode *next;
};

template <class T>
struct AdjList
{
	AdjNode<T> *head;

	AdjList() : head(nullptr) {}

	Node<T> *Remove(Node<T> *A)
	{
		AdjNode<T> *prev = head;
		AdjNode<T> *tmp = prev->next;

		while (tmp)
		{
			if (tmp->node == A)
			{
				prev->next = tmp->next;
				return tmp->node;
			}
			prev = tmp;
			tmp = tmp->next;
		}

		return NULL;
	}

	void Add(Node<T> *A, float w)
	{
		auto *N = new AdjNode<T>{A, w, head};
		head = N;
	}
};

template <class T>
struct AdjGraph
{
	int V;
	AdjList<T> *array;
};

template <class T>
struct Node
{
	int dest;
	T *data;

	friend bool operator==(const Node &n1, const Node &n2)
	{
		return (n1.data == n2.data);
	}
};

template <class T>
struct DirectedEdge
{
	Node<T> *to;
	Node<T> *from;
	float weight;

	friend bool operator==(const DirectedEdge &L1, const DirectedEdge &L2)
	{
		return (L1.from->data == L2.from->data && L1.to->data == L2.to->data);
	}

	bool operator<(const DirectedEdge &L) const
	{
		return (weight < L.weight);
	}
};

template <class T>
struct UndirectedEdge
{
	Node<T> *u;
	Node<T> *v;
	float weight;

	bool operator<(const UndirectedEdge &other) const
	{
		return weight < other.weight;
	}

	bool operator==(const UndirectedEdge &other) const
	{
		return (u == other.u && v == other.v) ||
			   (u == other.v && v == other.u);
	}
};

template <class T>
class Graph
{
	vector<UndirectedEdge<T>> edges;
	vector<Node<T> *> nodes;
	std::unordered_map<Node<T> *, AdjList<T>> data;

public:
	Graph<T>()
	{
		edges = vector<UndirectedEdge<T>>();
		nodes = vector<Node<T> *>();
	}

	Graph<T>(vector<T> d)
	{
		for (auto &i : d)
		{
			AddNode(&i);
		}
	}

	Graph<T>(vector<Node<T> *> nds)
	{
		for (auto &n : nds)
		{
			AddNode(n);
		}
	}

	Graph<T>(vector<UndirectedEdge<T>> e)
	{
		edges = e;

		for (auto &edge : e)
		{
			// AddNode(&i);
			if (!(data.find(edge.u) != data.end()))
			{
				AddNode(edge.u);
			}
			if (!(data.find(edge.v) != data.end()))
			{
				AddNode(edge.v);
			}
			AddUndirectedEdge(edge);
		}
	}

	// TODO: figure out whose responsibility it is to delete this0
	~Graph<T>()
	{
		// for (Node<T>* n: nodes)
		// {
		//     delete n;
		// }
	}

	void AddUndirectedEdge(Node<T> *A, Node<T> *B, float weight = 1.0f)
	{

		UndirectedEdge<T> e{A, B, weight};

		edges.push_back(e);

		data[A].Add(B, weight);
		data[B].Add(A, weight);
	}

	void AddDirectedEdge(Node<T> *from, Node<T> *to, float weight = 1.0f)
	{
		DirectedEdge<T> e{from, to, weight};
		edges.push_back(e);
		data[from].Add(to, weight);
	}

	void AddUndirectedEdge(const UndirectedEdge<T> &e)
	{
		if (data.find(e.u) == data.end())
			AddNode(e.u);

		if (data.find(e.v) == data.end())
			AddNode(e.v);

		data[e.u].Add(e.v, e.weight);
		data[e.v].Add(e.u, e.weight);

		edges.push_back(e);
	}

	void AddNode(T data)
	{
		Node<T> *tmp = new Node<T>();
		tmp->data = &data;
		AddNode(tmp);
	}

	void AddNode(T *data)
	{
		Node<T> *tmp = new Node<T>();
		tmp->data = data;
		AddNode(tmp);
	}

	void AddNode(Node<T> *A)
	{
		nodes.push_back(A);
		data[A] = AdjList<T>();
	}

	void RemoveNode(Node<T> *A)
	{
		if (Size() == 0)
		{
			return;
		}
		data.erase(A);
	}

	void RemoveEdge(UndirectedEdge<T> L)
	{
		if (edges.size() == 0)
		{
			return;
		}

		auto it = std::find(edges.begin(), edges.end(), L);
		edges.erase(it);

		data[L.u].Remove(L.v);
	}

	// Dijkstra implementation
	int Distance(Node<T> *start, Node<T> *finish)
	{
		if (start == finish)
			return 0;

		std::unordered_map<Node<T> *, Node<T> *> prev;
		std::unordered_map<Node<T> *, int> dist;

		Heap<Node<T> *, int> Q;

		// initialize the distance to all nodes as infinite
		for (auto &N : nodes)
		{
			dist[N] = INT_MAX;
			prev[N] = NULL;
			Q.Push(N, dist[N]);
		}

		dist[start] = 0;

		while (!Q.IsEmpty())
		{
			Node<T> *U = Q.Pop();

			AdjNode<T> *N = data[U].head;
			while (N)
			{
				int alt = dist[U] + N->weight;
				if (alt < dist[N->node])
				{
					dist[N->node] = alt;
					prev[N->node] = U;
					Q.UpdatePriority(N->node, alt);
				}
				N = N->next;
			}
		}

		int output = dist[finish];
		// unconnected, return -1
		if (output == INT_MAX)
			output = -1;
		return output;
	}

	Graph<T> MST()
	{
		Graph<T> output = Graph<T>(nodes);
		std::sort(edges.begin(), edges.end());

		DisjointSet<Node<T> *> disjointSet(nodes);

		for (const auto &edge : edges)
		{
			if (disjointSet.Find(edge.u) != disjointSet.Find(edge.v))
			{
				output.AddUndirectedEdge(edge);
				disjointSet.Union(edge.u, edge.v);
			}
		}

		return output;
	}

	bool Connected(Node<T> *A, Node<T> *B)
	{
		return (Distance(A, B) != -1);
	}

	UndirectedEdge<T> *PopEdge(UndirectedEdge<T> L)
	{
		auto it = std::find(edges.begin(), edges.end(), L);

		UndirectedEdge<T> *out = it;

		edges.erase(it);

		return out;
	}

	Node<T> *GetNode(T *data)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i]->data == data)
			{
				return nodes[i];
			}
		}

		return NULL;
	}

	vector<Node<T> *> GetNodes()
	{
		return nodes;
	}

	vector<UndirectedEdge<T>> GetEdges()
	{
		return edges;
	}

	void Save(string path);

	int Size()
	{
		return nodes.size();
	}
};