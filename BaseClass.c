//
//  BaseClass.c
//  Object
//
//  Created by Jacob Gordon on 11/28/15.
//  Copyright © 2015 Jacob Gordon. All rights reserved.
//

#include "obj.h"


start_method(my_init_method,)

end_method


start_method(my_deinit_method,)

end_method

start_method(print_size_of_object_in_bytes,)

printf("%ld\n",sizeof(obj)) ;

end_method

start_class_method(print_hello_world,)

printf("Hello World!\n") ;

end_method

new_class(BaseClass) {
    
    make_method_init(my_init_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_method_mask(print_size_of_object_in_bytes, print size of the object on the stack in bytes) ;
    
    make_class_method_public(print_hello_world) ;
}