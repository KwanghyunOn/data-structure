#ifndef __TOPOLOGICAL_SORT_H_
#define __TOPOLOGICAL_SORT_H_

#include <vector>
#include <algorithm>
#include "graph.hpp"
/* Feel free to add more standard library headers */

void dfs(vertex_t v, Graph& g, std::vector<vertex_t>& history, std::vector<bool>& vis) {
	vis[v] = true;
	for(auto &edge : g.adj_list(v)) {
		auto [ from, to, weight ] = edge;
		if(!vis[to]) dfs(to, g, history, vis);
	}
	history.push_back(v);
}

/* Return _a_ valid topologically sorted sequence of vertex descriptors */
std::vector<vertex_t> topological_sort(Graph& g) {
	std::vector<bool> vis(g.get_num_vertices(), false);
	std::vector<vertex_t> history;
	for(vertex_t v = 0; v < g.get_num_vertices(); v++)
		if(!vis[v]) dfs(v, g, history, vis);
	std::reverse(history.begin(), history.end());
	return history;
}

#endif // __TOPOLOGICAL_SORT_H_
