//
//  Example.c
//  Object
//
//  Created by Jacob Gordon on 4/12/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(TestClass) ;

use_class(StringClass) ;

int main(int argc, const char * argv[]) {
    
    TestClass myobject = new_object(TestClass, 10.0) ;
    
    $(myobject, my_first_method, 7) ;
    
    $(myobject, my_method, 10, 33) ;
    
    $(myobject, print_hello, noargs) ;
    
    free_object(myobject) ;
    
    StringClass string = new_object(StringClass, "This is my String!\n") ;
    
    $(string,print,noargs) ;
    
    obj_long size = 0 ;
    
    $(string, get the size of the string, &size) ;
    
    printf("%ld\n",size) ;
    
    $(string, print size of the object on the stack in bytes, noargs) ;
    
    if (!$(string, hello world, noargs)) {
        
        printf("hello world: not found! \n") ;
        
        if ( get_method(string, hello world) == get_null_method  ) printf("hello world: not found! 2 \n") ;
        
    }
    
    free_object(string) ;
    
    return 0 ;
}

