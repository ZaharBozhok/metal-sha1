/*
See LICENSE folder for this sample’s licensing information.

Abstract:
A class to manage all of the Metal objects this app creates.
*/

#import "MetalAdder.h"
#include "util.h"

const char maxStr[] = "4294967295";
const char partialKey[] = "53cr3t-";
size_t rowsNum = 1024;


@implementation MetalAdder
{
    id<MTLDevice> _mDevice;
    id<MTLComputePipelineState> _mSHA1;
    id<MTLCommandQueue> _mCommandQueue;
    id<MTLBuffer> _mText;
    id<MTLBuffer> _mHashOut;
}

- (instancetype) initWithDevice: (id<MTLDevice>) device
{
    self = [super init];
    if (self)
    {
        _mDevice = device;

        NSError* error = nil;

        id<MTLLibrary> defaultLibrary = [_mDevice newDefaultLibrary];
        if (defaultLibrary == nil)
        {
            NSLog(@"Failed to find the default library.");
            return nil;
        }
        
        id<MTLFunction> sha1 = [defaultLibrary newFunctionWithName:@"sha1"];
        if (sha1 == nil)
        {
            NSLog(@"Failed to find sha1 function");
            return nil;
        }
        
        _mSHA1 = [_mDevice newComputePipelineStateWithFunction: sha1 error:&error];
        if (_mSHA1 == nil)
        {
            NSLog(@"Failed to create pipeline, error %@", error);
            return nil;
        }

        _mCommandQueue = [_mDevice newCommandQueue];
        if (_mCommandQueue == nil)
        {
            NSLog(@"Failed to find the command queue.");
            return nil;
        }
    }

    return self;
}

- (void) allocateBuffers
{
    size_t textSize = (keySize + sizeof(uint8_t) + sizeof(char)) * rowsNum * sizeof(char);
    size_t hashOutSize =(40*keySize)*sizeof(char);
    _mText = [_mDevice newBufferWithLength:textSize options:MTLResourceStorageModeShared];
    _mHashOut = [_mDevice newBufferWithLength:hashOutSize options:MTLResourceStorageModeShared];
    char* text = _mText.contents;
    char* hashOut = _mHashOut.contents;
    memset(text, '\0', textSize);
    memset(hashOut, '\0', hashOutSize);
    NSLog(@"textSize = %d, allocatedSize = %d length = %d", textSize, _mText.allocatedSize, _mText.length);
    NSLog(@"hashOutSize = %d, allocatedSize = %d, length = %d", hashOutSize, _mHashOut.allocatedSize, _mHashOut.length);
}

- (void) prepareData:(int)bias
{
    my_set(_mText.contents, rowsNum, partialKey, bias * rowsNum);
    /*
    for(int i=0; i<rowsNum; i++)
    {
        NSLog(@"size = %d, data = %s", (int)text[i*keySize], &text[i*keySize+1]);
    }
     */
}

- (void) sendSHA1Command
{
    id<MTLCommandBuffer> commandBuffer = [_mCommandQueue commandBuffer];
    assert(commandBuffer != nil);
    id<MTLComputeCommandEncoder> computeEncoder = [commandBuffer computeCommandEncoder];
    assert(computeEncoder != nil);
    [self encodeSHA1Command:computeEncoder];
    [computeEncoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
    //[self verifysha];
}

- (void)encodeSHA1Command:(id<MTLComputeCommandEncoder>)computeEncoder {
    [computeEncoder setComputePipelineState:_mSHA1];
    [computeEncoder setBuffer:_mText offset:0 atIndex:0];
    [computeEncoder setBuffer:_mHashOut offset:0 atIndex:1];
    
    MTLSize gridSize = MTLSizeMake(rowsNum, 1, 1);
    
    NSUInteger threadGroupSize = _mSHA1.maxTotalThreadsPerThreadgroup;
    //NSLog(@"threadGroupSize = %lu", threadGroupSize);
    MTLSize threadgroupSize = MTLSizeMake(threadGroupSize, 1, 1);

    //Encode the compute command.
    [computeEncoder dispatchThreads:gridSize
              threadsPerThreadgroup:threadgroupSize];
}

void printsha(char* hash, char* data)
{
    char hex[41];
    for(size_t offset = 0; offset < 20; offset++)
    {
        sprintf((hex + (2 * offset)), "%02X", hash[offset] & 0xff);
    }
    printf("sha1(`%s`)=`%s`\n", data, hex);
}

- (void) verifysha
{
    char* hashOut = _mHashOut.contents;
    char* text = _mText.contents;
    for(size_t i=0, j=0; i<rowsNum*20 && j<rowsNum*keySize; i+=20, j+=keySize)
    {
        printsha(hashOut + i, &text[j+1]);
    }
}

@end
