#pragma once

#include <functional>
#include <map>
#include <memory>


template <class IdentifierType, class ProductType>
class DefaultFactoryError
{
public:
	class Exception : public std::exception
	{
	public:
		Exception(const IdentifierType &id) : m_unknownId(id) { }
		virtual const char *what() const { return "Unknown Factory Type"; }
		const IdentifierType GetId() { return m_unknownId; }
	private:
		IdentifierType m_unknownId;
	};
protected:
	static void onUnknownType(const IdentifierType &id)
	{
		throw Exception(id);
	}
};

template 
< 
	class AbstractProduct, 
	typename IdentifierType,
	template <typename, class> class FactoryErrorPolicy	= DefaultFactoryError	
>
class Factory : FactoryErrorPolicy< IdentifierType, AbstractProduct >
{
public:
	typedef std::unique_ptr< AbstractProduct > ProductType;
	typedef std::function<ProductType(void)> ProductCreator;
	bool Register(const IdentifierType &id, ProductCreator creator)
	{
		return m_associations.insert(AssocMap::value_type(id, creator)).second;
	}

	bool UnRegister(const IdentifierType &id)
	{
		return m_associations.erase(id) == 1;
	}

	ProductType CreateObject(const IdentifierType &id)
	{
		auto it = m_associations.find(id);
		if (it != m_associations.end())
			return (it->second)();
		else
			onUnknownType(id);
	}

private:
	typedef std::map< IdentifierType, ProductCreator > AssocMap;
	
private:
	AssocMap m_associations;
};

