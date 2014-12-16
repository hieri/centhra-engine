//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Group.h>

using namespace std;

namespace ce
{
	void Group::Add(Member *object, bool removeFromExistingGroup)
	{
		if(!IsMember(object))
		{
			if(removeFromExistingGroup && object->m_parentGroup != 0)
				object->m_parentGroup->Remove(object);

			if((object->m_parentGroup != this && removeFromExistingGroup) || object->m_parentGroup == 0)
			{
				m_members.push_back(object);
				object->m_parentGroup = this;

				OnMemberAdded(object);
				object->OnAdded(this);
			}
		}
	}
	void Group::Remove(Member *object)
	{
		vector<Member *>::iterator it = find(m_members.begin(), m_members.end(), object);
		if(it != m_members.end())
		{
			m_members.erase(it);
			object->m_parentGroup = 0;
			
			OnMemberRemoved(object);
			object->OnRemoved(this);
		}
	}
	bool Group::IsMember(Member *object) const
	{
		//TODO: Confirm if this is safe to have missing
//			return find(m_members.begin(), m_members.end(), object) != m_members.end();

		return object->m_parentGroup == this;
	}
	void Group::OnMemberAdded(Member *member)
	{
	}
	void Group::OnMemberRemoved(Member *member)
	{
	}
	vector<Group::Member *> *Group::GetMembers()
	{
		return &m_members;
	}
	
	Group::Member::Member()
	{
		m_parentGroup = 0;
	}
	Group::Member::~Member()
	{
		if(m_parentGroup)
			m_parentGroup->Remove(this);
	}
	Group *Group::Member::GetParentGroup() const
	{
		return m_parentGroup;
	}
	void Group::Member::OnAdded(Group *group)
	{
	}
	void Group::Member::OnRemoved(Group *group)
	{
	}
}
