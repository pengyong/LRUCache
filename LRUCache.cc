#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <typename K , typename V>
struct Node
{
	K key;
	V data;
	struct Node *prev,*next;
};

template <typename K , typename V>
class LRUCache
{
public:
	LRUCache(){
		LRUCache(512);
	}

	LRUCache(size_t size){
		list_size = size;
		lru_list = new Node<K,V>[list_size];
		if(NULL == lru_list){
			cerr<<"alloc memory failed"<<endl;
		}
		head = new Node<K,V>;
		tail = new Node<K,V>;
		head->prev = NULL;
		head->next = tail;
		tail->prev = head;
		tail->next = NULL;
		for(int i = 0 ; i < list_size ; ++i){
			free_list.push_back(lru_list+i);
		}
	}

	V get(K key){
		list_map_iterator iter = list_map.find(key);
		if(iter != list_map.end()){
			Node<K,V> *node = iter->second;
			detach(node);
			attach(node);
			return node->data;
		}else{
			return V();
		}
	}

	bool put(K key , V value){
		list_map_iterator iter = list_map.find(key);
		if(iter != list_map.end()){
			//exist,update the node position
			Node<K,V> *node = iter->second;
			node->data = value;
			detach(node);
			attach(node);
		}else{
			if(free_list.empty()){
				//there is no free node
				Node<K,V> *node = tail->prev;
				node->key = key;
				node->data = value;
				detach(node);
				attach(node);
				list_map.insert(pair<K,Node<K,V>*>(key,node));
			}else{
				//use a free node
				Node<K,V> *node = free_list.back();
				free_list.pop_back();
				node->key = key;
				node->data = value;
				attach(node);
				list_map.insert(pair<K,Node<K,V>*>(key,node));
			}
		}
	}

	~LRUCache(){
		delete head;
		delete tail;
		delete []lru_list;
	}

private:
	void detach(Node<K,V> *node){
		node->prev->next = node->next;
		node->next->prev = node->prev;
		node->prev = NULL;
		node->next = NULL;
	}

	void attach(Node<K,V> *node){
		node->next = head->next;
		head->next->prev = node;
		node->prev = head;
		head->next = node;
	}

private:
	size_t 				list_size;
	Node<K,V> 			*lru_list;
	Node<K,V> 			*head,*tail;
	map<K,Node<K,V>* > 	list_map;
	vector<Node<K,V>* > free_list;
	typedef typename map<K,Node<K,V>* >::iterator list_map_iterator;
};

int main()
{
	LRUCache<int,string> lru(10);
	lru.put(10,"pengyong");
	cout<<"get 10: "<<lru.get(10)<<" get 11: "<<lru.get(11)<<endl;
    return 0;
}
