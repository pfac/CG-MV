#ifdef _DEBUG

#ifndef __ALLOC_OVERLOADERON_H__
#define __ALLOC_OVERLOADERON_H__

//
// If AllocOverLoaderOn.h is included in a header file, 
// don't forget to include AllocOverLoaderOff.h at the end of this one
//

#include "MemoryManager.h"

// New operator overload
inline void* operator new(std::size_t size, const char* file, int line)
{
	return MemoryManager::get().allocate(size, file, line, false);
}

// New[] operator overload
inline void* operator new[](std::size_t size, const char* file, int line)
{
	return MemoryManager::get().allocate(size, file, line, true);
}

// Delete operator overload
inline void operator delete(void* ptr) throw()
{
    MemoryManager::get().desallocate(ptr, false);
}

// Delete[] operator overload
inline void operator delete[](void* ptr) throw()
{
    MemoryManager::get().desallocate(ptr, true);
}

// Delete operator overload
inline void operator delete(void* ptr, const char* file, int line) throw()
{
    MemoryManager::get().nextDelete(file, line);
    MemoryManager::get().desallocate(ptr, false);
}

// Delete[] operator overload
inline void operator delete[](void* ptr, const char* file, int line)
{
    MemoryManager::get().nextDelete(file, line);
    MemoryManager::get().desallocate(ptr, true);
}


#endif // __ALLOC_OVERLOADERON_H__

//
// Definition of macros being used to automate the tracking
// /!\ leave anti-reinclusion directives
//
#ifndef new
    #define new    new(__FILE__, __LINE__)
#define delete MemoryManager::get().nextDelete(__FILE__, __LINE__), delete
#endif

#endif // _DEBUG
