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

make_cls_available_for(Circle) ;

define_data_set(CircleData, float radius ; ) ;

start_method(InitCircle, argfromlist(pos_x, obj_float) argfromlist(pos_y, obj_float) argfromlist(radius, obj_float))

 iclass$(obj, get_cls_for(Circle), InitShape, noargs) ;

 store_set(CircleData) ;

 $(obj, set pos, pos_x, pos_y) ;

 $(obj, set radius, radius) ;

 $(obj, compute area, noargs) ;

end_method

start_method(DeinitCircle, )

 iclass$(obj, get_cls_for(Circle), DeinitShape, noargs) ;

 destroy_set(CircleData) ;

end_method

start_method(set_radius, arg(radius,obj_float))

 get_set(CircleData)->radius = radius ;

end_method

start_method(get_radius, arg(radius,obj_float*))

 *radius = get_set(CircleData)->radius ;

end_method

start_method(compute_area, )

 $(obj, set area, get_set(CircleData)->radius * get_set(CircleData)->radius * M_PI) ;

end_method

new_class(Circle) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitCircle) ;
    
    make_method_deinit(DeinitCircle) ;
    
    make_method_mask(get_radius, get radius) ;
    
    make_method_mask(set_radius, set radius) ;
    
    make_method_mask(compute_area, compute area) ;
    
    init_cls_for(Circle) ;
}
