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

void Example1( void ) {
    
    TestClass myobject = new_object(TestClass, 10.0) ; //new_object(Class, arguments...)
    
    m(myobject, my_first_method, 7) ; // m is method invoke: m(object, method name, arguments...)
    
    m(myobject, my_method, 10, 33) ;
    
    m(myobject, print_hello, noargs) ;
    
    free_object(myobject) ;
    
    StringClass string = new_object(StringClass, "This is my String!\n") ;
    
    m(string,print,noargs) ;
    
    obj_long size = 0 ;
    
    m(string, get the size of the string, &size) ;
    
    printf("%ld\n",size) ;
    
    m(string, print size of the object on the stack in bytes, noargs) ;
    
    if (!m(string, hello world, noargs)) { //methods by default return 1, if a method is not found 0 is returned
        
        printf("hello world: not found! \n") ;
        
        if ( get_method(string, hello world) == get_null_method  ) printf("hello world: not found! 2 \n") ; //unresolved methods are equal to get_null_method
        
    }
    
    free_object(string) ;

}

