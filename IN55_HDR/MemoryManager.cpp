#include "MemoryManager.h"

MemoryManager::MemoryManager() : file("MemoryLeaks.log")
{
    // Is file Opened?
    assert(file.is_open());

    // Writing of the memory leak file header
    file << "  ========================================" << std::endl;
    file << "            Memory leak tracker           " << std::endl;
    file << "  ========================================" << std::endl << std::endl;
}

MemoryManager::~MemoryManager()
{
    if (blocks.empty())
    {
        // No leak
        file << std::endl;
        file << "  ========================================" << std::endl;
        file << "     No leak detected, congratulations !  " << std::endl;
        file << "  ========================================" << std::endl;
    }
    else
    {
        // Leaks detected
        file << std::endl;
        file << "  ========================================" << std::endl;
        file << "       Some leaks have been detected      " << std::endl;
        file << "  ========================================" << std::endl;
        file << std::endl;

        reportLeaks();
    }
}

MemoryManager& MemoryManager::get()
{
    static MemoryManager inst;
    return inst;
}

void MemoryManager::reportLeaks()
{
	// leaks detail
    std::size_t totalSize = 0;
    for (blockMap::iterator it = blocks.begin(); it != blocks.end(); ++it)
    {
        // Add block size
        totalSize += it->second.size;

		// Writing current block informations into output file
        file << "-> 0x" << it->first 
			 << " | "   << std::setw(7) << std::setfill(' ') << static_cast<int>(it->second.size) << " bytes"
             << " | "   << it->second.file.getFileName() << " (" << it->second.line << ")" << std::endl;

        // Memory release
        free(it->first);
    }

    // Write all memory leaks
    file << std::endl << std::endl << "-- "
           << static_cast<int>(blocks.size()) << " not released block(s), "
           << static_cast<int>(totalSize)       << " bytes --"
           << std::endl;
}

void* MemoryManager::allocate(std::size_t size, const File& f, int line, bool tab)
{
    // Memory allocation
    void* ptr = malloc(size);

	// Add block to the allocated block list
    block newBlock;
    newBlock.size = size;
    newBlock.file = f;
    newBlock.line = line;
    newBlock.tab  = tab;
    blocks[ptr]   = newBlock;

    // Writing in the output file
    file << "++ Allocation    | 0x" << ptr 
		<< " | " << std::setw(7) << std::setfill(' ') << static_cast<int>(newBlock.size) << " bytes" 
		<< " | " << newBlock.file.getFileName() << " (" << newBlock.line << ")" << std::endl;

    return ptr;
}

void MemoryManager::desallocate(void* ptr, bool tab)
{
    // Find the address in the allocated blocks
    blockMap::iterator it = blocks.find(ptr);

	// If the block isn't allocated, destroy the block and return
    if (it == blocks.end())
    {
        free(ptr);
        return;
    }

	// If the allocation type does not correspond, an error is generated 
    assert(it->second.tab == tab);

	// Else, delete the block and write them in the output file
    file << "-- Desallocation | 0x" << ptr
           << " | " << std::setw(7) << std::setfill(' ') << static_cast<int>(it->second.size) << " bytes"
           << " | " << deleteStack.top().file.getFileName() << " (" << deleteStack.top().line << ")" << std::endl;
    blocks.erase(it);
    deleteStack.pop();
    
    // destroy block
    free(ptr);
}

void MemoryManager::nextDelete(const File& file, int line)
{
    block del;
    del.file = file;
    del.line = line;

    deleteStack.push(del);
}
