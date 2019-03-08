#ifndef BEHAVIOURTREE_H
#define BEHAVIOURTREE_H

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>

class BehaviourTree
{
public:
	/// <summary>
	/// Node class. Base class for all BT nodes
	/// </summary>
	class Node
	{
	public:
		virtual bool run() = 0;
	};

	/// <summary>
	/// Uses composite pattern, contains a list of other nodes
	/// </summary>
	class CompositeNode : public Node
	{
	public:
		const std::vector<Node *>& getChildren() const { return m_children; }
		void addChild(Node * c) { m_children.emplace_back(c); }
		void addChildren(std::initializer_list<Node *>&& newChildren) { for (Node * c : newChildren) addChild(c); }
		template <typename CONTAINER>
		void addChildren(const CONTAINER& newChildren) { for (Node * c : newChildren) addChild(c); }
	private:
		std::vector<Node *> m_children;
	protected:
		std::vector<Node *> childrenShuffled() const { std::vector<Node *> temp = m_children; std::random_shuffle(temp.begin(), temp.end()); return temp; }
	};

	/// <summary>
	/// Decision making nodes
	/// </summary>
	class Selector : public CompositeNode
	{
	public:
		virtual bool run() override
		{
			for (Node * c : getChildren())
			{
				//If one child node succeeds the entire operation succeeds
				if (c->run())
				{
					return true;
				}
			}
			//False if all children fail
			return false;
		}
	};

	/// <summary>
	/// Operates the same as a selector node but in random order
	/// instead of first to last
	/// </summary>
	class RandomSelector : public CompositeNode
	{
	public:
		virtual bool run() override
		{
			//Order of child nodes is randomised
			for (Node * c : childrenShuffled())
			{
				if (c->run())
				{
					return true;
				}
			}
			return false;
		}
	};

	/// <summary>
	/// Sequence node implementation
	/// </summary>
	class Sequence : public CompositeNode
	{
	public:
		virtual bool run() override
		{
			for (Node * c : getChildren())
			{
				//If one child node fails the run operation fails
				if (!c->run())
				{
					return false;
				}
			}
			//Return true if all children succeed
			return true;
		}
	};

	/// <summary>
	/// 
	/// </summary>
	class DecoratorNode : public Node
	{
	private:
		Node * m_child;
	protected:
		Node * getChild() const { return m_child; }
	public:
		void setChild(Node * c) { m_child = c; }
	};

	/// <summary>
	/// 
	/// </summary>
	class Inverter : public DecoratorNode
	{
	private:
		virtual bool run() override { return !getChild()->run(); }
	};

	/// <summary>
	/// 
	/// </summary>
	class Succeeder : public DecoratorNode
	{
	private:
		virtual bool run() override { getChild()->run(); return true; }
	};

	class Failer : public DecoratorNode
	{
	private:
		virtual bool run() override { getChild()->run(); return false; }
	};

	class Repeater : public DecoratorNode
	{
	private:
		int numRepeats;
		static const int NOT_FOUND = -1;
		Repeater(int num = NOT_FOUND) : numRepeats(num) {}
		virtual bool run() override
		{
			if (numRepeats == NOT_FOUND)
			{
				while (true) getChild()->run();
			}
			else
			{
				for (int i = 0; i < numRepeats; i++)
				{
					getChild()->run();
				}
				return getChild()->run();
			}
		}
	};

	/// <summary>
	/// Root node that start the tree structure
	/// </summary>
	class Root : public Node
	{
	private:
		Node * m_child;
		friend class BehaviourTree;
		void setChild(Node * c) { m_child = c; }
		virtual bool run() override { return m_child->run(); }
	};

private:
	Root * m_root;
public:
	BehaviourTree() : m_root(new Root) {}
	void setRootChild(Node * c) const { m_root->setChild(c); }
	bool run() const { return m_root->run(); }
};

#endif