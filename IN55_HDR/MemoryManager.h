#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include "std.h"
#include "File.h"

class MemoryManager
{
private:
	// Default constructor
	MemoryManager();

	// Destructor
	~MemoryManager();

	// Report memory leaks
	void reportLeaks();

	// Types
	struct block
    {
        std::size_t size;  // Allocated size
		File        file;  // File containing the allocation
        int         line;  // Allocated line
        bool        tab;   // Is it an object or an array ?
    };
    typedef std::map<void*, block> blockMap;

	// members data
	std::ofstream     file;        // Output file
    blockMap          blocks;      // Allocated memory blocks
    std::stack<block> deleteStack; // Stack whose top contains the line and the file of the next desallocation
 
public:
	static MemoryManager& get();

    // Add a memory allocation
    void* allocate(std::size_t size, const File& f, int line, bool tab);

    // Withdraw a memory allocation 
    void desallocate(void* ptr, bool tab);

	// Save current desallocation informations
    void nextDelete(const File& file, int line);
};

#endif // __MEMORY_MANAGER_H__
