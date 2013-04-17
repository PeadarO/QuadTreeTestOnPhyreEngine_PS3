#pragma once

#include <assert.h>

template < typename T >
class CSingleton
{
private:
	static T * m_pInstance;

protected:
	CSingleton()
	{
#ifdef _DEBUG
		assert__(!m_pInstance,__FILE__,__LINE__,__FUNCTION__,"!m_pInstance");
#endif // _DEBUG
		int offset = (int)(T *)1 - (int)(CSingleton *)(T *)1;
		m_pInstance = (T *)((int)this + offset);
	}
	~CSingleton()
	{
#ifdef _DEBUG
		assert__(m_pInstance,__FILE__,__LINE__,__FUNCTION__,"m_pInstance");
#endif // _DEBUG
		m_pInstance = 0;
	}

public:
	static T * getthis()
	{
		if (m_pInstance == 0)
			m_pInstance = new T;
		return m_pInstance;
	};
	static void destroyInstance()
	{
		if(m_pInstance) {
			delete m_pInstance;
			m_pInstance = 0;
		}
	};
};

template<typename T> T* CSingleton <T>::m_pInstance = 0;


