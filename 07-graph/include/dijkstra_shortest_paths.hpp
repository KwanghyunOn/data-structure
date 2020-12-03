#ifndef __DIJKSTRA_SHORTEST_PATHS_H_
#define __DIJKSTRA_SHORTEST_PATHS_H_

#include <unordered_map>
#include <optional>
#include <tuple>
#include <queue>
/* Feel free to add more standard library headers */

#include "graph.hpp"


/* Given a vertex `v`, `dijkstra_shortest_path` is:
 * - `nullopt` if `v` is not reacheble from `src`.
 * - {`u`, `d`} where `u` is the predecessor of `v` and `d` is the distance
 *   from `src`
 */

std::unordered_map<vertex_t,
    std::optional<std::tuple<vertex_t, edge_weight_t>>>
dijkstra_shortest_path(Graph& g, vertex_t src) {
	std::unordered_map<vertex_t,
		std::optional<std::tuple<vertex_t, edge_weight_t>>> M;
	for(vertex_t v = 0; v < g.get_num_vertices(); v++)
		M[v] = std::nullopt;

	std::vector<edge_weight_t> distance(g.get_num_vertices(), 1e30);

	typedef std::tuple<edge_weight_t, vertex_t> pq_node;
	std::priority_queue<pq_node, std::vector<pq_node>, std::greater<pq_node>> pq;

	distance[src] = 0;
	pq.push({distance[src], src});
	M[src] = {-1, distance[src]};

	while(!pq.empty()) {
		auto [ d, x ] = pq.top();
		pq.pop();
		if(distance[x] > d) continue;
		for(auto &edge : g.adj_list(x)) {
			auto [ from, to, weight ] = edge;
			if(distance[to] > d + weight) {
				distance[to] = d + weight;
				pq.push({distance[to], to});
				M[to] = {x, distance[to]};
			}
		}
	}
	return M;
}

#endif // __DIJKSTRA_SHORTEST_PATHS_H_
