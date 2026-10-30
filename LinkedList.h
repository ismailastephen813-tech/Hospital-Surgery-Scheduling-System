#pragma once
/*
 *
 *  DO NOT MODIFY THIS FILE
 *
 */

#include <stdexcept>
#include <utility> 

template <typename T>
class LinkedList
{
public:
	struct Node
	{
		Node* previous = nullptr;
		Node* next = nullptr;

		T data;
	};

protected:
	Node* begin_;
	Node* end_;

public:
	LinkedList() : begin_(nullptr), end_(nullptr) {}

	Node* begin() { return begin_; }
	Node* end() { return end_; }

	void push_front(const T& item)
	{
		Node* node = new Node();
		node->data = item;

		//check if there is a node to link in-front of
		if (begin_ != nullptr)
		{
			begin_->previous = node;
			node->next = begin_;
		}
		else
		{
			end_ = node;
		}

		begin_ = node;
	}

	void push_back(const T& item)
	{
		Node* node = new Node();
		node->data = item;

		if (begin_ == nullptr && end_ == nullptr)
		{
			begin_ = node;
			end_ = node;
			return;
		}

		node->previous = end_;
		end_->next = node;

		end_ = node;
	}

	T pop_front()
	{
		if (begin_ == nullptr)
		{
			//todo: throw
			throw std::runtime_error("Attempted to pop from an empty list");
		}

		Node* node = begin_;
		const T value = node->data;
		begin_ = node->next;

		return value;
	}

	T pop_back()
	{
		if (end_ == nullptr)
		{
			//todo: throw
			throw std::runtime_error("Attempted to pop from an empty list");
		}

		Node* node = end_;
		const T value = node->data;
		begin_ = node->previous;

		return value;
	}

	bool empty() const
	{
		return begin_ == nullptr;
	}

	unsigned size() const
	{
		unsigned counter = 0;

		Node* node = begin_;
		while (node != nullptr)
		{
			++counter;
			node = node->next;
		}

		return counter;
	}
};
