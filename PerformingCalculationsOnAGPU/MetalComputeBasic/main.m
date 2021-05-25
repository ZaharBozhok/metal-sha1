/*
See LICENSE folder for this sample’s licensing information.

Abstract:
An app that performs a simple calculation on a GPU.
*/

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import "MetalAdder.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        NSLog(@"Hello, World!");
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();

        // Create the custom object used to encapsulate the Metal code.
        // Initializes objects to communicate with the GPU.
        MetalAdder* hasher = [[MetalAdder alloc] initWithDevice:device];
        
        // Create buffers to hold data
        [hasher allocateBuffers];
        size_t to =((uint32_t)-1) / 1024;
        for(size_t i=0; i<to; i++)
        {
            [hasher prepareData:i];
            [hasher sendSHA1Command];
        }
        NSLog(@"Execution finished");
    }
    return 0;
}
