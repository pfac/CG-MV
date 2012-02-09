/**
 * Nao e relevante
 */

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

//
// Derive this class to implement your singleton class
// Don't forget to add this class as FriendClass in your singleton class
// ex : 
//  class T : public Singleton<T>
//	{
//		friend class Singleton<T>;
//	private:
//		...
//	};
//

#include "AllocOverLoaderOn.h"

template <class T> 
class Singleton
{
private:
	// Copy prohibited
	Singleton(Singleton&);
	void operator =(Singleton&);


protected:
    // Default constructor
	Singleton() {}

    // Destructor
	virtual ~Singleton() {}


public:
    // Get single class instance
    static T* get()
	{
		static T inst;
		return &inst;
	}
};

#include "AllocOverLoaderOff.h"

#endif // __SINGLETON_H__
