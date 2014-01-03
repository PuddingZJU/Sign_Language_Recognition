#ifndef ZZ_AN_ANY_CLASS_THAT_USE_VOID_STAR__H___
#define ZZ_AN_ANY_CLASS_THAT_USE_VOID_STAR__H___
#pragma once
/**
 * @author  Thinapple (turingmc@gmail.com)
 */
/**
 * @brief  This is an "any class" just like the boost::any, but it uses void*.
 *
 *   As you think, it is not type-safe, but it need less infomation about the Value that it holds
 * 	For example, it does not copy the Value, so a copy-constructor is not needed, which
 * 	is what I want. 
 */
class ZAny
{
public:
	template <typename TypeValue>
	static TypeValue & zany_cast(ZAny & any){return *static_cast<TypeValue*>(any.m_value);}
	template <typename TypeValue>
	static const TypeValue & zany_cast(const ZAny & any){return *static_cast<TypeValue*>(any.m_value);}
public:
	ZAny(): m_value(0){};
	template <typename TypeValue>
	ZAny(TypeValue & value): m_value(&value){};
	template <typename TypeValue>
	ZAny & operator = (TypeValue & value)
	{
		m_value = &value;
		return *this;
	}
	template <typename TypeValue>
	ZAny & operator = (const TypeValue & value)
	{
		m_value = const_cast<TypeValue *>(&value);
		return *this;
	}
private:
	void * m_value;
};

#endif
