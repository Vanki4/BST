#include "BST.h"

#include <iostream>

BinarySearchTree::Node::Node(Key key, Value value, Node *parent, Node *left, Node *right) : keyValuePair(key, value), parent(parent), left(left), right(right) {}

BinarySearchTree::Node::Node(const Node &other) : keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {
    if (other.left)
    {
        left = new Node(*other.left);
        left->parent = this;
    }

    if (other.right)
    {
        right = new Node(*other.right);
        right->parent = this;
    }
}

bool BinarySearchTree::Node::operator==(const Node &other) const {
	return keyValuePair == other.keyValuePair;
}

void BinarySearchTree::Node::output_node_tree() const {
	if (left!=nullptr)
	{
		left->output_node_tree();
	}
	std::cout << "Ключ: " << keyValuePair.first << " значение: " << keyValuePair.second << std::endl;
	if (right!=nullptr)
	{
		right->output_node_tree();
	}
}

void BinarySearchTree::Node::insert(const Key &key, const Value &value) {
	if (key < keyValuePair.first)
	{
		if (left!=nullptr) left->insert(key, value);
		else left = new Node(key, value, this);
	}
	else
	{
		if (right!=nullptr) right->insert(key, value);
		else right = new Node(key, value, this);
	}
}

void BinarySearchTree::Node::erase(const Key &key) {
	Node *kon = nullptr;
	if (key < keyValuePair.first)
	{
		if (left!=nullptr) left->erase(key);
		return;
	}
	if (key > keyValuePair.first)
	{
		if (right) right->erase(key);
		return;
	}
	kon = this;
	if (kon->left==nullptr && kon->right==nullptr)
	{
		if (kon->parent!=nullptr)
		{
			if (kon->parent->left == kon) kon->parent->left = nullptr;
			else kon->parent->right = nullptr;
		}
		delete kon;
	}
	else if (kon->left==nullptr || kon->right==nullptr)
	{
		Node* tmp = nullptr;
		if (kon->left!=nullptr) tmp = kon->left;
		else tmp = kon->right;
		if (kon->parent!=nullptr)
		{
			if (kon->parent->left == kon) kon->parent->left = tmp;
			else kon->parent->right = tmp;
		}
		tmp->parent = kon->parent;
		delete kon;
	}
	else
	{
		Node *tmp = kon->right;
		while (tmp->left!=nullptr)
		{
			tmp = tmp->left;
		}
		kon->keyValuePair = tmp->keyValuePair;
		if (tmp->parent->left == tmp) tmp->parent->left = tmp->right;
		else tmp->parent->right = tmp->right;
		if (tmp->right!=nullptr) tmp->right->parent = tmp->parent;
		delete tmp;
	}
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree &other) : _size(other._size) {
	if (other._root!=nullptr) _root = new Node(*other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree &other) {
	if (this == &other) return *this;
	while(_root!=nullptr)
	{
		erase(_root->keyValuePair.first);
	}
	_root = nullptr;
	_size = 0;
	if (other._root!=nullptr) _root = new Node(*other._root);
	_size = other._size;
	return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree &&other) noexcept {
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree &&other) noexcept {
	if (this == &other) return *this;
	while(_root!=nullptr)
	{
		erase(_root->keyValuePair.first);
	}
	_root = other._root;
	_size = other._size;
	other._root = nullptr;
	other._size = 0;
	return *this;
}

BinarySearchTree::~BinarySearchTree() {
	while (_root!=nullptr)
	{
		erase(_root->keyValuePair.first);
	}
}

BinarySearchTree::Iterator::Iterator(Node *node) : _node(node) {};

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
	return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
	return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
	return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
	return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
	if (_node==nullptr) return *this;
	if (_node->right!=nullptr)
	{
		_node = _node->right;
		while (_node->left!=nullptr)
		{
			_node = _node->left;
		}
	}
	else
	{
		Node *parent = _node->parent;
		while (parent && _node == parent->right)
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
	Iterator temp(*this);
	++(*this);
	return temp;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
	if (_node==nullptr) return *this;
	if (_node->left!=nullptr)
	{
		_node = _node->left;
		while (_node->right!=nullptr)
		{
			_node = _node->right;
		}
	}
	else
	{
		Node *parent = _node->parent;
		while (parent!=nullptr && _node == parent->left)
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
	Iterator temp(*this);
	--(*this);
	return temp;
}

bool BinarySearchTree::Iterator::operator==(const Iterator &other) const {
	return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator &other) const {
	return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node *node) : _node(node) {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
	return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
	return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
	if (_node==nullptr) return *this;
	if (_node->right!=nullptr)
	{
		_node = _node->right;
		while (_node->left!=nullptr)
		{
			_node = _node->left;
		}
	}
	else
	{
		const Node *parent = _node->parent;
		while (parent!=nullptr && _node == parent->right)
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
	ConstIterator temp(*this);
	++(*this);
	return temp;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
	if (_node==nullptr) return *this;
	if (_node->left!=nullptr)
	{
		_node = _node->left;
		while (_node->right!=nullptr)
		{
			_node = _node->right;
		}
	}
	else
	{
		const Node *parent = _node->parent;
		while (parent!=nullptr && _node == parent->left)
		{
			_node = parent;
			parent = parent->parent;
		}
		_node = parent;
	}
	return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
	ConstIterator temp(*this);
	--(*this);
	return temp;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator &other) const {
	return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator &other) const {
	return _node != other._node;
}

void BinarySearchTree::insert(const Key &key, const Value &value) {
	if (_root==nullptr) _root = new Node(key, value);
	else _root->insert(key, value);
	_size++;
}

void BinarySearchTree::erase(const Key &key) {
	Iterator it = find(key);
	while (it._node != nullptr)
	{
		Node *node = it._node;
		++it;
		if (node == _root)
		{
			Node temp(0, 0);
			temp.left = _root;
			_root->parent = &temp;
			_root->erase(key);
			_root = temp.left;
			if (_root!=nullptr) _root->parent = nullptr;
		}
		else
		{
			node->erase(key);
		}
		--_size;
		it = find(key);
	}
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key &key) const {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return ConstIterator(tmp);
	}
	return cend();
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key &key) {
	Node *tmp = _root;
	while (tmp!=nullptr)
	{
		if (key < tmp->keyValuePair.first) tmp = tmp->left;
		else if (key > tmp->keyValuePair.first) tmp = tmp->right;
		else return Iterator(tmp);
	}
	return end();
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key &key) {
	Iterator first = find(key);
	if (first == end()) return {end(), end()};
	Iterator second = first;
	while (second != end() && (*second).first == key) ++second;
	return {first, second};
}

std::pair<BinarySearchTree::ConstIterator,BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key &key) const {
	ConstIterator first = find(key);
	if (first == cend()) return {cend(), cend()};
	ConstIterator second = first;
	while (second != cend() && (*second).first == key) ++second;
	return {first, second};
}

BinarySearchTree::ConstIterator BinarySearchTree::min() const {
	Node *tmp = _root;
	if (tmp==nullptr) return cend();
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::max() const {
	Node *tmp = _root;
	if (tmp==nullptr) return cend();
	while (tmp->right) tmp = tmp->right;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key &key) const {
	return find(key);
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key &key) const {
	ConstIterator it = find(key);
	if (it == cend()) return cend();
	ConstIterator res = it;
	while (it != cend() && (*it).first == key)
	{
		res = it;
		++it;
	}
	return res;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
	Node *tmp = _root;
	if (tmp==nullptr) return end();
	while (tmp->left!=nullptr) tmp = tmp->left;
	return Iterator(tmp);
}

BinarySearchTree::Iterator BinarySearchTree::end() {
	return Iterator(nullptr);
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
	Node *tmp = _root;
	if (tmp==nullptr) return cend();
	while (tmp->left) tmp = tmp->left;
	return ConstIterator(tmp);
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
	return ConstIterator(nullptr);
}

size_t BinarySearchTree::size() const {
	return _size;
}

void BinarySearchTree::output_tree() {
	if (_root) _root->output_node_tree();
}
