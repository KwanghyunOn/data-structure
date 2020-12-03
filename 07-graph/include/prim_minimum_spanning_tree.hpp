#ifndef __PRIM_MINIMUM_SPANNING_TREE_H_
#define __PRIM_MINIMUM_SPANNING_TREE_H_

#include <optional>
#include <vector>
/* Feel free to add more standard library headers */

/* Returns the vector of edges in the MST, or std::nullopt if MST does
 * not exist */

std::optional<edges_t>
prim_minimum_spanning_tree(Graph& g, vertex_t src) {
	edges_t MST;
	std::vector<bool> vis(g.get_num_vertices(), false);

	typedef std::tuple<edge_weight_t, vertex_t, vertex_t> pq_node;
	std::priority_queue<pq_node, std::vector<pq_node>, std::greater<pq_node>> pq;

	vis[src] = true;
	for(auto &edge : g.adj_list(src)) {
		auto [ from, to, weight ] = edge;
		pq.push({weight, from, to});
	}

	while(!pq.empty()) {
		auto [ weight, from, to ] = pq.top();
		pq.pop();
		if(vis[to]) continue;
		MST.push_back({from, to, weight});
		vis[to] = true;
		for(auto &edge : g.adj_list(to)) {
			auto [ from, to, weight ] = edge;
			pq.push({weight, from, to});
		}
	}

	if(MST.size() == g.get_num_vertices() - 1)
		return MST;
	else
		return std::nullopt;
}
#endif // __PRIM_MINIMUM_SPANNING_TREE_H_
