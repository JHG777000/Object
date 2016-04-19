//
//  Circle.c
//  Object
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

#include <math.h>

use_class(Shape) ;

define_data_set(CircleData, float radius ; ) ;

start_method(InitCircle, argfromlist(pos_x, obj_float) argfromlist(pos_y, obj_float) argfromlist(radius, obj_float) arg(cls, obj_class))

 cm(obj, InitShape, noargs) ;

 store_set(CircleData) ;

 m(obj, set pos, pos_x, pos_y) ;

 m(obj, set radius, radius) ;

 m(obj, compute area, noargs) ;

end_method

start_method(DeinitCircle, arg(cls, obj_class))

 cm(obj, DeinitShape, noargs) ;

 destroy_set(CircleData) ;

end_method

start_method(set_radius, arg(radius,obj_float))

 get_set(CircleData)->radius = radius ;

end_method

start_method(get_radius, arg(radius,obj_float*))

 *radius = get_set(CircleData)->radius ;

end_method

start_method(compute_area, )

 m(obj, set area, get_set(CircleData)->radius * get_set(CircleData)->radius * M_PI) ;

end_method

new_class(Circle) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitCircle) ;
    
    make_method_deinit(DeinitCircle) ;
    
    make_method_mask(get_radius, get radius) ;
    
    make_method_mask(set_radius, set radius) ;
    
    make_method_mask(compute_area, compute area) ;
}
