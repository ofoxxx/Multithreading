#pragma once

#include <atomic>
#include <memory>
#include <optional>

template<typename T>
struct CASNode
{
	using value_type = T;
	value_type m_item;
	std::atomic<CASNode*> m_next;
	CASNode(value_type&& item) : m_item(std::forward<value_type>(item)), m_next(nullptr) {}
};

template<typename T, typename A = std::allocator<CASNode<T>>>
class CASQueue
{
public:
	using node_type = CASNode<T>;
	using allocator_type = A;
	using allocator_traits = std::allocator_traits<allocator_type>;
	using value_type = T;
	using reference = value_type&;
	using const_reference = const value_type&;

	CASQueue() : m_allocator(), m_first(nullptr), m_last(nullptr) {}
	CASQueue(const A& a) : m_allocator(a), m_first(nullptr), m_last(nullptr) {}
	~CASQueue() { clear(); }

	void clear()
	{
		node_type* node = nullptr;
		while ((node = m_first.load()) != nullptr)
		{
			m_first.exchange(node->m_next);

			allocator_traits::destroy(m_allocator, node);
			allocator_traits::deallocate(m_allocator, node, 1);
		}
		m_last = nullptr;
	}

	void push_back(value_type&& v)
	{
		node_type* node = allocator_traits::allocate(m_allocator, 1);
		allocator_traits::construct(m_allocator, node, std::forward<value_type>(v));

		node_type *tail, *blank;
		do {
			blank = nullptr;
			tail = m_last.load();
			if (tail == nullptr)
				break;
		} while (!tail->m_next.compare_exchange_weak(blank, node));
		m_last.compare_exchange_weak(tail, node);

		blank = nullptr;
		m_first.compare_exchange_weak(blank, node);
	}

	std::optional<value_type> pop_front()
	{
		node_type* node;
		do {
			node = m_first.load();
			if (node == nullptr)
				return {};
		} while (!m_first.compare_exchange_weak(node, node->m_next));

		node_type* tail = node;
		m_last.compare_exchange_weak(tail, nullptr);

		value_type return_value = node->m_item;

		allocator_traits::destroy(m_allocator, node);
		allocator_traits::deallocate(m_allocator, node, 1);

		return return_value;
	}

	bool empty() const { return m_first.load() == nullptr; }

	reference front() { Node* node = m_first.load(); if (node) return node->m_item; throw; }
	const_reference front() const { Node* node = m_first.load(); if (node) return node->m_item; throw; }

	reference back() { Node* node = m_last.load(); if (node) return node->m_item; throw; }
	const_reference back() const { Node* node = m_last.load(); if (node) return node->m_item; throw; }

private:
	allocator_type m_allocator;
	std::atomic<node_type*> m_first;
	std::atomic<node_type*> m_last;
};
