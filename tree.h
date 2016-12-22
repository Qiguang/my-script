#ifndef TREE_H_JMOQIUIC
#define TREE_H_JMOQIUIC
#include <vector>
using namespace std;

template<typename T>
class tree
{
public:
	tree ();
	virtual ~tree ();
	class iterator
	{
	public:
		iterator (iterator& other);
		virtual ~iterator ();
		iterator& operator= (iterator& other);
		iterator operator++ (int);
		iterator& operator++ ();
	
	private:
		/* data */
	};

	iterator& begin();
	iterator& end();

private:
	T data;
	vector<tree> children;
};

#endif /* end of include guard: TREE_H_JMOQIUIC */
