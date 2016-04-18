//
//  Example2.c
//  Object
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "obj.h"

use_static_method(ShapeFactory) ;

void Exmaple2( void ) {
    
    obj_float area = 0 ;
    
    
    AnyClass circle = NULL ;
    
    sm(ShapeFactory, "Circle", &circle, 0.0f, 0.0f, 4.0f) ;
    
    m(circle, get area, &area) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass square = NULL ;
    
    sm(ShapeFactory, "Square", &square, 0.0f, 0.0f, 2.0f) ;
    
    m(square, get area, &area) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass rectangle = NULL ;
    
    sm(ShapeFactory, "Rectangle", &rectangle, 0.0f, 0.0f, 5.0f, 2.0) ;
    
    m(rectangle, get area, &area) ;
    
    printf("Area: %f\n", area) ;

    
    sm(ShapeFactory, "Destroy") ;
    
}