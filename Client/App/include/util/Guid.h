#pragma once
#include <string>
#include "util/Name.h"

namespace RBX
{
	class Guid
	{
	public:
		struct Data
		{
		public:
			const RBX::Name* scope;
			int index;

		public:
			bool operator ==(const Data& other) const;
			bool operator >(const Data& other) const;
			bool operator <(const Data& other) const;

			std::string readableString(int scopeLength) const;
		};

	private:
		Data data;

	public:
		Guid();
		~Guid() {}

	public:
		bool operator ==(const Guid& other) const;
		bool operator >(const Guid& other) const;
		bool operator <(const Guid& other) const;

		void assign(Data);
		void extract(Data&) const;
		void copyDataFrom(const Guid&);
		std::string readableString(int scopeLength) const;

		Guid& operator =(const Guid&);

		static int compare(const Guid*, const Guid*, const Guid*, const Guid*);
		static int compare(const Guid*, const Guid*);
		static void generateGUID(std::string& result);
		const Data& getData() const
		{
			return this->data;
		}
	};
}
