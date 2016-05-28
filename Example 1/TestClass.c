//
//  TestClass.c
//  Object
//
//  Created by Jacob Gordon on 10/10/15.
//  Copyright © 2015 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(BaseClass) ;

use_class(StringClass) ;

make_cls_available_for(TestClass) ;

define_record_type(TestClassFDS, int value ; ) ;

start_method( my_init_method, argfromlist(myfloat, obj_float) arg(cls, obj_class))

    printf("my init method: %f\n", myfloat) ;

    obj_float* myptrtofloat = RKMem_NewMemOfType(obj_float) ;

    *myptrtofloat = myfloat ;

    sp(obj, myfloat, myptrtofloat) ;

    fds = new_record(TestClassFDS) ;

    get_fds(TestClassFDS)->value = 22 ;

    make_private_object_store_available ;

    pos_m = new_record(TestClassFDS) ;

    get_pos_m(TestClassFDS)->value = 34 ;

end_method

start_method( my_deinit_method, arg(cls, obj_class) )

    obj_float *myptrtofloat = get_pointer(obj, myfloat) ;

    printf("my deinit method: %f\n", *myptrtofloat) ;

    free(myptrtofloat) ;

    free(fds) ;

    printf("my private data store value: %d\n", get_pos_m(TestClassFDS)->value) ;

    free(pos_m) ;

    destroy_private_object_store ;

    m(getobj(obj, mystring),print,noargs) ;

end_method

start_method( my_first_method, arg(myint, obj_interger) )

    printf("my first method: %d\n", myint) ;

end_method

start_method( my_private_method, arg(myint, obj_interger) )

    printf("my private method: %d\n", myint) ;

    printf("my private method fds: %d\n", get_fds(TestClassFDS)->value) ;

end_method

start_method( my_method, arg(myint,int) arg(myint2, obj_interger) )

    m(obj, my_first_method, myint) ;

    pm(obj, my_first_method, myint2) ;

    pm(obj, my_private_method, myint) ;

    pm(obj, my_private_method, myint2) ;

    StringClass string = new_object(StringClass, "My String!\n") ;

    sos(obj, mystring, string) ;

end_method

start_class_method(print_hello_world_from_test,)

   cm(obj, print_hello_world, noargs) ;

end_method

start_method(print_hello,)

   obj_class cls = get_cls_for(TestClass) ;

   cm(obj, print_hello_world_from_test, noargs) ;

end_method

new_class(TestClass) {
    
    make_class_subclass_of(BaseClass) ;
    
    make_method_init(my_init_method) ;
    
    make_method_public(my_first_method) ;
    
    make_method_public(my_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_class_method_public(print_hello_world_from_test) ;
    
    make_method_public(print_hello) ;
    
    init_cls_for(TestClass) ;
}