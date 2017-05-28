#pragma once

#include <algorithm>

/*!
 * @author kovlev
 */

/*!
 * @namespace stdex
 * Contains functions that helps stl containers
 */
namespace stdex {
	
	//Works on vectors
	template<typename Vector>
	void clear_ptr_vec(Vector& v) {
		while (!v.empty()) {
			delete v.back();
			v.pop_back();
		}
		v.clear();
	}
	
	//Works on lists
	template<typename List>
	void clear_ptr_list(List& l) {
		while (!l.empty()) {
			delete l.back();
			l.pop_back();
		}
		l.clear();
	}
	
	//Works on queues
	template<typename Queue>
	void clear_ptr_que(Queue& q) {
		while (!q.empty()) {
			delete q.front();
			q.pop();
		}
	}
	
	//Works on maps, whose value is a pointer
	template<typename Map>
	void clear_ptr_map(Map& m) {
		while (!m.empty()) {
			delete m.begin()->second;
			m.erase(m.begin());
		}
		m.clear();
	}
	
	//Removes element by value from a vector
	template<typename Vector, typename E>
	void remove_value_vec(Vector& v, E e) {
		//NOTE removes all elements, this might not be the preferable behaviour in all cases
		v.erase(std::remove(v.begin(), v.end(), e), v.end());
	}
}
