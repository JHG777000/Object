//
//  ShapeFactory.c
//  Object
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_class(Circle) ;

use_class(Square) ;

use_class(Rectangle) ;

declare_private_class(ShapeFactoryClass) ;

start_static_method(ShapeFactory, arg(shape, const char*) arg(object, AnyClass*) arg(pos_x, obj_float) arg(pos_y, obj_float) arg(value1, obj_float) arg(value2, obj_float))

 static ShapeFactoryClass factory = NULL ;

 AnyClass any_class = NULL ;

 if ( factory == NULL ) factory = new_object(ShapeFactoryClass, noargs) ;

 obj_long retval = send_object_msg(factory, shape, &any_class, pos_x, pos_y, value1, value2) ;

 if ( retval ) {
    
     if ( retval == method_returned_error ) {
         
         free_object(factory) ;
         
         factory = NULL ;
         
         return_from_method ;
     }
     
     *object = any_class ;
     
     return_from_method ;
 }

end_method

start_method(new_circle, arg(ret_circle, AnyClass*) arg(pos_x, obj_float) arg(pos_y, obj_float) arg(radius, obj_float))

 *ret_circle = new_any_object(Circle, pos_x, pos_y, radius) ;

 keep_object(obj, *ret_circle) ;

end_method

start_method(new_square, arg(ret_square, AnyClass*) arg(pos_x, obj_float) arg(pos_y, obj_float) arg(size, obj_float))

 *ret_square = new_any_object(Square, pos_x, pos_y, size) ;

 keep_object(obj, *ret_square) ;

end_method

start_method(new_rectangle, arg(ret_rectangle, AnyClass*) arg(pos_x, obj_float) arg(pos_y, obj_float) arg(width, obj_float) arg(length, obj_float))

 *ret_rectangle = new_any_object(Rectangle, pos_x, pos_y, width, length) ;

 keep_object(obj, *ret_rectangle) ;

end_method

start_method(destroy_factory,)

 return_error_from_method ;

end_method

new_private_class(ShapeFactoryClass) {
    
    make_method_mask(new_circle, Circle) ;
    
    make_method_mask(new_square, Square) ;
    
    make_method_mask(new_rectangle, Rectangle) ;
    
    make_method_mask(destroy_factory, Destroy) ;
}
