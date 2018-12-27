#import "MyImageView.h"


@implementation MyImageView

- (void)dealloc
{
    if (cgImage) {
        CGImageRelease(cgImage);
    }
}

- (void)setImage:(CGImageRef)image
{
    if (cgImage) {
        CGImageRelease(cgImage);
    }
    cgImage = image;
    CGImageRetain(cgImage);
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect
{
    CGContextRef context = [NSGraphicsContext currentContext].CGContext;
    CGContextSetInterpolationQuality(context, kCGInterpolationNone);
    CGContextDrawImage(context, CGRectMake(0, 0, 640, 480), cgImage);
}

@end

