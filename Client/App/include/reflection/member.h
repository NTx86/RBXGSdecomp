#pragma once
#include "reflection/descriptor.h"
#include <iterator>
#include <vector>

namespace RBX
{
	namespace Reflection
	{
		class DescribedBase;
		class ClassDescriptor;

		class MemberDescriptor : public Descriptor
		{
		public:
			const Name& category;
			const ClassDescriptor& owner;
		  
		public:
			//MemberDescriptor(const MemberDescriptor&);
		protected:
			MemberDescriptor(const ClassDescriptor& owner, const char* name, const char* category)
				: Descriptor(name),
				  category(Name::declare(category, -1)),
				  owner(owner)
			{
			}
			virtual ~MemberDescriptor();
		public:
			bool isMemberOf(const DescribedBase*) const;
			bool isMemberOf(const ClassDescriptor&) const;
			//MemberDescriptor& operator=(const MemberDescriptor&);
		};

		template<typename TheDescriptor>
		class MemberDescriptorContainer
		{
		public:
			typedef std::vector<TheDescriptor*> Collection;

		public:
			class Iterator : public std::iterator<std::forward_iterator_tag, typename TheDescriptor::Describing, void>
			{
			private:
				DescribedBase* instance;
				typename Collection::const_iterator iter;
			  
			public:
				Iterator(const typename Collection::const_iterator&, DescribedBase*);
			public:
				typename TheDescriptor::Describing operator*() const;
				bool operator==(const Iterator&) const;
				bool operator!=(const Iterator&) const;
				Iterator operator++(int);
				Iterator& operator++();
			};

			class ConstIterator : public std::iterator<std::forward_iterator_tag, typename TheDescriptor::Describing, void>
			{
			private:
				const DescribedBase* instance;
				typename Collection::const_iterator iter;
		  
			public:
				ConstIterator(const typename Collection::const_iterator&, const DescribedBase*);
			public:
				typename TheDescriptor::Describing operator*() const;
				bool operator==(const ConstIterator&) const;
				bool operator!=(const ConstIterator&) const;
				ConstIterator operator++(int);
				ConstIterator& operator++();
				const TheDescriptor& getDescriptor() const;
			};

		protected:
			Collection descriptors;
			std::vector<MemberDescriptorContainer*> derivedContainers;
			MemberDescriptorContainer* base;

		public:
			//MemberDescriptorContainer(const MemberDescriptorContainer&);
		protected:
			// TODO: check if matches
			MemberDescriptorContainer(MemberDescriptorContainer* base)
				: descriptors(),
				  derivedContainers(),
				  base(base)
			{
				if (base)
				{
					boost::recursive_mutex::scoped_lock lock(sync_0());
					mergeMembers(base);
					base->derivedContainers.push_back(this);
				}
			}
		public:
			void declare(TheDescriptor*);
			typename Collection::const_iterator descriptors_begin() const;
			typename Collection::const_iterator descriptors_end() const;
			typename Collection::const_iterator findDescriptor(const Name&) const;
			Iterator findMember(const Name&, DescribedBase*) const;
			ConstIterator findConstMember(const Name&, const DescribedBase*) const;
			Iterator members_begin(DescribedBase*) const;
			ConstIterator members_begin(const DescribedBase*) const;
			Iterator members_end(DescribedBase*) const;
			ConstIterator members_end(const DescribedBase*) const;
			void mergeMembers(const MemberDescriptorContainer*);
		public:
			~MemberDescriptorContainer() {} // TODO: does not match

		public:
			static bool compare(const TheDescriptor*, const TheDescriptor*);
		};
	}
}
