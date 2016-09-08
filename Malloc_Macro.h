//
// Created by Ido Ilani on 28/08/2016.
//

#ifndef FINALPROJECT_MALLOC_MACRO_H
#define FINALPROJECT_MALLOC_MACRO_H

#include <stdlib.h>
#define MALLOC_MACRO(name,type,size)\
            name=(type)malloc(size);\
            if(name==NULL){\
                res=ALLOC_FAIL;\
                goto fail;\
            }
#define FREE_MACRO(name)\
        if(name!=NULL){\
            free(name);\
        }\
        name=NULL;\

#define FUNC_MACRO(method)\
	res = method;\
	if(SUCCESS != res){\
		goto fail;\
	}
#endif //FINALPROJECT_MALLOC_MACRO_H
