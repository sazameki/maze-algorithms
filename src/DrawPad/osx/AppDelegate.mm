#import "AppDelegate.h"


@interface AppDelegate ()
@property (weak) IBOutlet NSWindow *window;
@end


@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self.window setAcceptsMouseMovedEvents:YES];
    [self.window makeFirstResponder:self.window.contentView];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end

