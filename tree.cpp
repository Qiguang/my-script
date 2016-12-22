#include "tree.h"

template<typename T>
tree<T>::tree() {
	
}
template<typename T>
tree<T>::~tree() {
	
}
#if 0

template<typename T>
void tree<T>::foreach_deep_first() {
	
}
#endif
template<typename T>
typename tree<T>::iterator& tree<T>::begin() {
	return iterator();	
}
template<typename T>
typename tree<T>::iterator& tree<T>::end() {
	return iterator();	
}
/////////////////////////////////////////////////////////////
template<typename T>
tree<T>::iterator::iterator(iterator& other) {
	
}
template<typename T>
tree<T>::iterator::~iterator() {
	
}
template<typename T>
typename tree<T>::iterator& tree<T>::iterator::operator=(iterator& other) {
	return iterator();	
}
template<typename T>
typename tree<T>::iterator tree<T>::iterator::operator++(int dummy) {
	return iterator();	
}
template<typename T>
typename tree<T>::iterator& tree<T>::iterator::operator++() {
	return iterator();	
}

