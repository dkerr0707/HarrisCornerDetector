//
//  main.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

#include "Application.hpp"

#include <iostream>

int main( int argc, char** argv )
{
    
    try {
        
        Application app(argc, argv);
        app.Run();
        
    }
    catch(const std::exception& e) {
        
        std::cout << e.what() << std::endl;
        
    }
    
    
    return 0;
}







