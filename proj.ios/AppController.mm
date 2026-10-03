#import <UIKit/UIKit.h>
#import "AppController.h"
#import "cocos2d.h"
#import "EAGLView.h"
#import "AppDelegate.h"
#import "RootViewController.h"

@implementation AppController

@synthesize window;
@synthesize viewController;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedAppDelegate;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];

    // Use RootViewController manage EAGLView 
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;

    // Set RootViewController to window
    if ([[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        [window addSubview: viewController.view];
    }
    else
    {
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    // Fix Retina display resolution: compute actual physical pixels so Cocos2d-x fills 100% of the screen
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGFloat scale = [[UIScreen mainScreen] scale];
    CGFloat pixelW = screenBounds.size.width * scale;
    CGFloat pixelH = screenBounds.size.height * scale;
    if (pixelW > pixelH) {
        CGFloat temp = pixelW;
        pixelW = pixelH;
        pixelH = temp;
    }
    cocos2d::CCEGLView::sharedOpenGLView()->setFrameSize(pixelW, pixelH);

    cocos2d::CCApplication::sharedApplication()->run();

    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    cocos2d::CCDirector::sharedDirector()->purgeCachedData();
}

- (void)dealloc {
    [super dealloc];
}

@end

// ==========================================
// Native iOS OBB Downloader Bridge
// ==========================================
#include "CDNDownloaderBridge.h"

static CDNProgressCallback s_progressCb = nullptr;
static CDNSuccessCallback s_successCb = nullptr;
static CDNErrorCallback s_errorCb = nullptr;

@interface CDNNativeSessionDelegate : NSObject <NSURLSessionDownloadDelegate>
@property (nonatomic, copy) NSString *destPath;
@end

@implementation CDNNativeSessionDelegate

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
      didWriteData:(int64_t)bytesWritten
 totalBytesWritten:(int64_t)totalBytesWritten
totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    float percent = (totalBytesExpectedToWrite > 0) ? (float)totalBytesWritten / (float)totalBytesExpectedToWrite : 0.0f;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (s_progressCb) {
            s_progressCb(percent, totalBytesWritten, totalBytesExpectedToWrite);
        }
    });
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
didFinishDownloadingToURL:(NSURL *)location {
    NSFileManager *fm = [NSFileManager defaultManager];
    [fm removeItemAtPath:self.destPath error:nil];
    NSError *err = nil;
    [fm moveItemAtURL:location toURL:[NSURL fileURLWithPath:self.destPath] error:&err];
    dispatch_async(dispatch_get_main_queue(), ^{
        if (err) {
            if (s_errorCb) s_errorCb([[err localizedDescription] UTF8String]);
        } else {
            if (s_successCb) s_successCb();
        }
    });
}

- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error {
    if (error) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (s_errorCb) s_errorCb([[error localizedDescription] UTF8String]);
        });
    }
}

@end

static CDNNativeSessionDelegate *s_sessionDelegate = nil;
static NSURLSession *s_urlSession = nil;

extern "C" void startNativeDownload(const char* url, const char* destinationPath,
                                    CDNProgressCallback onProgress,
                                    CDNSuccessCallback onSuccess,
                                    CDNErrorCallback onError) {
    s_progressCb = onProgress;
    s_successCb = onSuccess;
    s_errorCb = onError;

    NSString *nsUrl = [NSString stringWithUTF8String:url];
    NSString *nsDest = [NSString stringWithUTF8String:destinationPath];

    s_sessionDelegate = [[CDNNativeSessionDelegate alloc] init];
    s_sessionDelegate.destPath = nsDest;

    NSURLSessionConfiguration *cfg = [NSURLSessionConfiguration defaultSessionConfiguration];
    cfg.requestCachePolicy = NSURLRequestReloadIgnoringLocalCacheData;
    cfg.timeoutIntervalForRequest = 60.0;

    s_urlSession = [NSURLSession sessionWithConfiguration:cfg delegate:s_sessionDelegate delegateQueue:[NSOperationQueue mainQueue]];
    NSURLSessionDownloadTask *task = [s_urlSession downloadTaskWithURL:[NSURL URLWithString:nsUrl]];
    [task resume];
}

