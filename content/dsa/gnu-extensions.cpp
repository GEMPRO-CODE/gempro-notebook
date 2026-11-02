#include <functional>
#include <utility>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <ext/pb_ds/priority_queue.hpp>
namespace pb = __gnu_pbds;

template<class K, class V = pb::null_type, class C = std::less<K>>
using OrderedTree = pb::tree<K,V,C,pb::rb_tree_tag,
	pb::tree_order_statistics_node_update>;
template<class K, class V = pb::null_type, class H = std::hash<K>>
using HashTable = pb::gp_hash_table<K,V,H>;
template<class T, class C = std::less<T>>
using Heap = pb::priority_queue<T,C,pb::pairing_heap_tag>;

void pbdsExample() {
	OrderedTree<std::pair<int,int>> s; // {valor,id}, id >= 0
	s.insert({5,0}); s.insert({5,1});
	s.order_of_key({5,-1}); // 0: quantidade < 5
	s.order_of_key({6,-1}); // 2: quantidade < 6
	auto it = s.find_by_order(1); // {5,1}; end() se k >= size()
	s.erase(it); // remove apenas esta ocorrencia
	OrderedTree<int,int> m; m[4] = 9; // mapa com rank por chave
	HashTable<int,int> h; h[3] = 7;
	HashTable<int> seen; seen.insert(3);
	Heap<int> a, b; // max-heap; std::greater<int> para minimo
	auto p = a.push(4); // Heap<int>::point_iterator
	a.modify(p,1);
	auto q = b.push(2);
	a.join(b); // transfere b para a; b fica vazio
	a.erase(q); // handle de b agora pertence a a
	a.top(); // 1
	a.pop(); // invalida p
}

#include <ext/rope>
using __gnu_cxx::rope;
using __gnu_cxx::crope; // rope<char>

void ropeExample() {
	int v[] = {10,20,30};
	rope<int> a(v,v+3);
	auto old = a; // snapshot
	auto part = a.substr(1,2);
	a.erase(1,2); // posicao, quantidade
	a.insert(0,part); // [20,30,10]; old = [10,20,30]
	a.replace(0,99); // [99,30,10]; part = [20,30]
	a[1]; // 30
	crope s("abc"); s += "def";
}

#include <bitset>
#include <tr2/dynamic_bitset>

void bitsetExample(int n, int x) { // n > 0, x >= 0
	std::bitset<100> b;
	b.set(3); b.set(70);
	for (size_t i = b._Find_first(); i < b.size(); i = b._Find_next(i)) {
		// processar i
	}
	std::tr2::dynamic_bitset<> d(n);
	d[0] = 1; d |= d << x; // subset sum
	for (size_t i = d.find_first(); i < d.size(); i = d.find_next(i)) {
		// processar i
	}
	d.resize(n+1); // preserva bits; novos bits = 0
}
