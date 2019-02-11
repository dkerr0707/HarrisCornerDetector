//
//  main.cpp
//  HarrisCornerDetector
//
//  Created by David Kerr on 2019-02-05.
//  Copyright © 2019 David Kerr. All rights reserved.
//

// This is the main file. It creates the application and runs it.
// I use exceptions for error handling so the application is
// wrapped in a try catch block to ensure all exceptions are
// handled.

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
    
    return EXIT_SUCCESS;
}







