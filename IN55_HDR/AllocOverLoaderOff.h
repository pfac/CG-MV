#ifndef __ALLOCOVERLOADEROFF_H__
#define __ALLOCOVERLOADEROFF_H__

//
// Necessary when AllocOverLoaderOn.h is included in a header, 
// AllocOverLoaderOff.h has to be included at the end of this one 
// to not disturb the behavior of the thereafter included headers
//

#undef new
#undef delete

#endif // __ALLOCOVERLOADEROFF_H__
