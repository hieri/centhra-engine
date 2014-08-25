#ifndef _CE_BASE_GROUP_H_
#define _CE_BASE_GROUP_H_

//- Standard Library -
#include <vector>

namespace ce
{
	class Group
	{
	public:
		class Member
		{
			Group *m_parentGroup;

		public:
			Member();
			virtual ~Member();

			Group *GetParentGroup() const;
			virtual void OnAdded(Group *group);
			virtual void OnRemoved(Group *group);

			friend class Group;
		};

		void Add(Member *entity, bool removeFromExistingGroup = true);
		void Remove(Member *entity);
		bool IsMember(Member *entity) const;
		std::vector<Member *> &GetMembers();

		virtual void OnMemberAdded(Member *entity);
		virtual void OnMemberRemoved(Member *entity);

	protected:
		std::vector<Member *> m_members;
	};
}

#endif
