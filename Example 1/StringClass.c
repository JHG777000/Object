//
//  StringClass.c
//  Object
//
//  Created by Jacob Gordon on 11/16/15.
//  Copyright © 2015 Jacob Gordon. All rights reserved.
//

#include "obj.h"
#include <string.h>

use_class(BaseClass) ;

declare_class(StringClass) ;

start_method(my_init_method, argfromlist(string,char*))

  obj_long size = strlen(string) ;

  char* the_string = RKMem_CArray(size, char) ;

  strcpy(the_string, string) ;

  obj_long* the_size = RKMem_NewMemOfType(obj_long) ;

  *the_size = size ;

  sp(obj, string, the_string) ;

  sp(obj, size, the_size) ;

end_method

start_method(my_deinit_method,)

  free(get_pointer(obj, string)) ;

  free(get_pointer(obj, size)) ;

end_method

start_method(print_string,)

 printf("%s", get_pointer(obj, string)) ;

end_method

start_method(get_size, arg(size, obj_long*))

*size = *(obj_long*)gp(obj, size) ;

end_method

new_class(StringClass) {
    
    make_class_subclass_of(BaseClass) ;
    
    make_method_init(my_init_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_method_mask(print_string, print) ;
    
    make_method_mask(get_size, get the size of the string) ;
}