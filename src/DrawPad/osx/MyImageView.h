#import <Cocoa/Cocoa.h>


@interface MyImageView : NSView {
    CGImageRef  cgImage;
}

- (void)setImage:(CGImageRef)image;

@end

