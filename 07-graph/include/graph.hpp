#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

/* Feel free to add more standard library headers */

// A vertex is typed as `vertex_t`. An edge is typed as `edge_t`,
// which is associated with a source vertex, a destination vertex, and
// its weight. A list of edges is typed as `edges_t`, which is a
// vector of `edge_t`.

using vertex_t = std::size_t;
using edge_weight_t = double;
using edge_t = std::tuple<vertex_t,
                          vertex_t,
                          edge_weight_t>;
using edges_t = std::vector<edge_t>;

enum class GraphType {
UNDIRECTED,
DIRECTED
};

/*
 * We do not enforce a specific design constraints other than the
 * consturtor. You may introduce your own changes as you
 * want---implementing your own member functions or variables. You
 * will still need to ensure that your constructor works properly,
 * such that we can run the test for grading. The provided test will
 * check if the constructor is correctly designed.
 */
class Graph {
    public:
        Graph() = delete;
        Graph(const Graph&) = delete;
        Graph(Graph&&) = delete;

        /* We assume that if num_vertices is n, a graph contains n vertices
           from 0 to n-1. */
        Graph(size_t num_vertices,
              const edges_t& edges,
              GraphType type)
			: num_vertices(num_vertices), type(type) {
			this->num_vertices = num_vertices;
			graph.resize(num_vertices);

			if(type == GraphType::UNDIRECTED) {
				for(auto &edge : edges) {
					const auto& [ from, to, weight ] = edge;
					graph[from].emplace_back(from, to, weight);
					graph[to].emplace_back(to, from, weight);
				}
			} else {
				for(auto &edge : edges) {
					const auto& [ from, to, weight ] = edge;
					graph[from].emplace_back(from, to, weight);
				}
			}
        }

		size_t get_num_vertices() { return num_vertices; }
		std::vector<edge_t> adj_list(vertex_t v) { return graph[v]; }
		
    private:
		size_t num_vertices;
		std::vector<std::vector<edge_t>> graph;
		GraphType type;
};

#endif // __GRAPH_H_
