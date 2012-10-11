#ifndef CE_ACTION_H
#define CE_ACTION_H

//- Standard Library -
#include <map>
#include <vector>

//- Centhra Engine -
#include <CE/Base.h>

namespace ce
{
	/* 	Action Map Class
	 * 	- Data structure holding action data for a class.
	 */
	template<class Type> class ActionMap
	{
		std::map<std::pair<unsigned int, unsigned short>, std::vector<void (Type::*)(bool)> > m_toggleActions;
		std::map<std::pair<unsigned int, unsigned short>, std::vector<void (Type::*)(void)> > m_popActions, m_pushActions;
		std::vector<std::pair<unsigned int, unsigned short> > m_stack;
		Type *m_target;

	public:
		ActionMap()
		{
			m_target = 0;
		}
		~ActionMap()
		{
		}
		void addPopAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(void))
		{
			bool isDefined = false;
			if(m_popActions.count(key))
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(void)>::iterator it = m_popActions[key].begin(); it != m_popActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(void)>::iterator it = m_popActions[key].begin(); it != m_popActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						isDefined = true;
						break;
					}
			if(!isDefined)
				m_popActions[key].insert(m_popActions[key].end(), action);
		}
		void addPushAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(void))
		{
			bool isDefined = false;
			if(m_pushActions.count(key))
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(void)>::iterator it = m_pushActions[key].begin(); it != m_pushActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(void)>::iterator it = m_pushActions[key].begin(); it != m_pushActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						isDefined = true;
						break;
					}
			if(!isDefined)
				m_pushActions[key].insert(m_pushActions[key].end(), action);
		}
		void addToggleAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(bool))
		{
			bool isDefined = false;
			if(m_toggleActions.count(key))
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(bool)>::iterator it = m_toggleActions[key].begin(); it != m_toggleActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(bool)>::iterator it = m_toggleActions[key].begin(); it != m_toggleActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						isDefined = true;
						break;
					}
			if(!isDefined)
				m_toggleActions[key].insert(m_toggleActions[key].end(), action);
		}
		void attach(Type *target)
		{
			detach();
			m_target = target;
		}
		void clearPopActions(std::pair<unsigned int, unsigned short> key)
		{
			m_popActions.erase(key);
		}
		void clearPushActions(std::pair<unsigned int, unsigned short> key)
		{
			m_pushActions.erase(key);
		}
		void clearToggleActions(std::pair<unsigned int, unsigned short> key)
		{
			m_toggleActions.erase(key);
		}
		void detach()
		{
			flush();
			m_target = 0;
		}
		void flush()
		{
			if(m_target)
			{
				for(std::vector<std::pair<unsigned int, unsigned short> >::iterator itA = m_stack.begin(); itA != m_stack.end(); itA++)
				{
#ifdef _MSC_VER
					for(std::vector<void (Type::*)(void)>::iterator itB = m_popActions[*itA].begin(); itB != m_popActions[*itA].end(); itB++)
#else
					for(class std::vector<void (Type::*)(void)>::iterator itB = m_popActions[*itA].begin(); itB != m_popActions[*itA].end(); itB++)
#endif
						(m_target->*(*itB))();
#ifdef _MSC_VER
					for(std::vector<void (Type::*)(bool)>::iterator itB = m_toggleActions[*itA].begin(); itB != m_toggleActions[*itA].end(); itB++)
#else
					for(class std::vector<void (Type::*)(bool)>::iterator itB = m_toggleActions[*itA].begin(); itB != m_toggleActions[*itA].end(); itB++)
#endif
						(m_target->*(*itB))(false);
				}
			}
			m_stack.clear();
		}
		Type *getTarget() const
		{
			return m_target;
		}
		void pop(unsigned int key)
		{
			for(std::vector<std::pair<unsigned int, unsigned short> >::iterator itA = m_stack.begin(); itA != m_stack.end(); itA++)
			{
				if(itA->first == key)
				{
					if(m_popActions.count(*itA))
#ifdef _MSC_VER
						for(std::vector<void (Type::*)(void)>::iterator itB = m_popActions[*itA].begin(); itB != m_popActions[*itA].end(); itB++)
#else
						for(class std::vector<void (Type::*)(void)>::iterator itB = m_popActions[*itA].begin(); itB != m_popActions[*itA].end(); itB++)
#endif
							(m_target->*(*itB))();
					if(m_toggleActions.count(*itA))
#ifdef _MSC_VER
						for(std::vector<void (Type::*)(bool)>::iterator itB = m_toggleActions[*itA].begin(); itB != m_toggleActions[*itA].end(); itB++)
#else
						for(class std::vector<void (Type::*)(bool)>::iterator itB = m_toggleActions[*itA].begin(); itB != m_toggleActions[*itA].end(); itB++)
#endif
							(m_target->*(*itB))(false);
					m_stack.erase(itA);
					break;
				}
			}
		}
		void push(unsigned int key, unsigned short modifier)
		{
			if(m_target)
			{
				bool inStack = false;
				for(std::vector<std::pair<unsigned int, unsigned short> >::iterator it = m_stack.begin(); it != m_stack.end(); it++)
					if(it->first == key)
					{
						inStack = true;
						break;
					}

				std::pair<unsigned int, unsigned short> actionKey(key, modifier);
				if(m_pushActions.count(actionKey))
#ifdef _MSC_VER
					for(std::vector<void (Type::*)(void)>::iterator it = m_pushActions[actionKey].begin(); it != m_pushActions[actionKey].end(); it++)
#else
					for(class std::vector<void (Type::*)(void)>::iterator it = m_pushActions[actionKey].begin(); it != m_pushActions[actionKey].end(); it++)
#endif
						(m_target->*(*it))();
				if(m_toggleActions.count(actionKey))
#ifdef _MSC_VER
					for(std::vector<void (Type::*)(bool)>::iterator it = m_toggleActions[actionKey].begin(); it != m_toggleActions[actionKey].end(); it++)
#else
					for(class std::vector<void (Type::*)(bool)>::iterator it = m_toggleActions[actionKey].begin(); it != m_toggleActions[actionKey].end(); it++)
#endif
						(m_target->*(*it))(true);

				if(!inStack)
					m_stack.insert(m_stack.end(), actionKey);
			}
		}
		void removePopAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(void))
		{
			if(m_popActions.count(key))
			{
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(void)>::iterator it = m_popActions[key].begin(); it != m_popActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(void)>::iterator it = m_popActions[key].begin(); it != m_popActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						m_popActions[key].erase(it);
						break;
					}
				if(!m_popActions[key].size())
					m_popActions.erase(key);
			}
		}
		void removePushAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(void))
		{
			if(m_pushActions.count(key))
			{
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(void)>::iterator it = m_pushActions[key].begin(); it != m_pushActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(void)>::iterator it = m_pushActions[key].begin(); it != m_pushActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						m_pushActions[key].erase(it);
						break;
					}
				if(!m_pushActions[key].size())
					m_pushActions.erase(key);
			}
		}
		void removeToggleAction(std::pair<unsigned int, unsigned short> key, void (Type::*action)(bool))
		{
			if(m_toggleActions.count(key))
			{
#ifdef _MSC_VER
				for(std::vector<void (Type::*)(void)>::iterator it = m_toggleActions[key].begin(); it != m_toggleActions[key].end(); it++)
#else
				for(class std::vector<void (Type::*)(void)>::iterator it = m_toggleActions[key].begin(); it != m_toggleActions[key].end(); it++)
#endif
					if((*it) == action)
					{
						m_toggleActions[key].erase(it);
						break;
					}
				if(!m_toggleActions[key].size())
					m_toggleActions.erase(key);
			}
		}
	};
}

#endif
