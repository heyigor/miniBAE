//
//  iOSPlayAppDelegate.m
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import "iOSPlayAppDelegate.h"
#import "PlayFileDetail.h"

#include "BAEPatches.h"

@interface SongControllerDetails : NSObject
{    
	short int channel;
	short int track;
	short int controller;
	short int value;
}
@property (nonatomic, assign) short int channel;
@property (nonatomic, assign) short int track;
@property (nonatomic, assign) short int controller;
@property (nonatomic, assign) short int value;
@end
@implementation SongControllerDetails
@synthesize channel, track, controller, value;
@end

@implementation iOSPlayAppDelegate

@synthesize window;
@synthesize tabBarController;
@synthesize navController;
@synthesize currentFileDetail;

static iOSPlayAppDelegate* sInstance;

iOSPlayAppDelegate* getAppInstance()
{
	return sInstance;
}

static void PV_Task(void *reference)
{
	iOSPlayAppDelegate* app = (iOSPlayAppDelegate*)reference;
	
	if (app)
	{
		BAEMixer_ServiceStreams(app->mMixer);
		if (app->mCurrentPlaying)
		{
			// call main thread to update GUI, otherwise leaks will occur.
			[app performSelectorOnMainThread:@selector(updatePositionGUI:)
								  withObject:nil
							   waitUntilDone:NO];
		}
	}
}


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions 
{
	sInstance = self;
    // Add the tab bar controller's view to the window and display.
    //[window addSubview:tabBarController.view];

	navController = [[UINavigationController alloc] initWithRootViewController:tabBarController];
	//[[[navController navigationBar] topItem] setTitle:@"Songs"];

    [window addSubview:navController.view];	
    [window makeKeyAndVisible];

	// setup the audio session
	audioSession = [AVAudioSession sharedInstance];
	[audioSession setDelegate:self];
	
	NSError *averr = nil;
	[audioSession setCategory:AVAudioSessionCategoryPlayback error:&averr];	
	if(averr)
	{
        NSLog(@"audioSession: %@ %d %@", [averr domain], [averr code], [[averr userInfo] description]);
	}
	
	averr = nil;
	[audioSession setActive:YES error:&averr];
	if(averr)
	{
        NSLog(@"audioSession: %@ %d %@", [averr domain], [averr code], [[averr userInfo] description]);
	}
	
	averr = nil;
	[audioSession setCategory:AVAudioSessionCategorySoloAmbient error:&averr];
	if(averr)
	{
        NSLog(@"audioSession: %@ %d %@", [averr domain], [averr code], [[averr userInfo] description]);
	}	
    
	BAEResult    err;
	mCurrentPlaying = NULL;
	mMixer = BAEMixer_New();
	if (mMixer)
	{
		int pcm   = 1;
		int rmf   = BAE_MAX_VOICES - pcm;
		int level = rmf / 3;
		err = BAEMixer_Open(mMixer,
							BAE_RATE_44K,
							BAE_LINEAR_INTERPOLATION,
							BAE_USE_STEREO | BAE_USE_16,
							rmf,                                          // midi voices
							pcm,                                          // pcm voices
							level,
							FALSE);
		if (err == BAE_NO_ERROR)
		{
			BAEMixer_SetAudioTask(mMixer, PV_Task, self);
			err = BAEMixer_AddBankFromMemory(mMixer, BAE_PATCHES, BAE_PATCHES_size, &mBank);
			if (err == BAE_NO_ERROR)
			{
				// turn on nice verb
				BAEMixer_SetDefaultReverb(mMixer, BAE_REVERB_TYPE_8);
				
				//BAEMixer_TestTone(TRUE);
				BAEMixer_TestToneFrequency(LONG_TO_FIXED(1000));
			}
			else
			{
				printf("BAE Error %d\n", err);
			}
		}
		else
		{
			printf("BAE Error %d\n", err);
		}
	}
	return YES;
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)beginInterruption
{
	[audioSession setActive:NO error:nil];	
}

/* Currently the only flag is AVAudioSessionInterruptionFlags_ShouldResume. */
- (void)endInterruptionWithFlags:(NSUInteger)flags
{
	[audioSession setActive:YES error:nil];
}

- (void)endInterruption; /* endInterruptionWithFlags: will be called instead if implemented. */
{
	[audioSession setActive:YES error:nil];
}

- (void)applicationWillResignActive:(UIApplication *)application 
{
	/*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
	
 	[audioSession setActive:YES error:nil];
	BAEMixer_DisengageAudio(mMixer);
}


- (void)applicationDidBecomeActive:(UIApplication *)application 
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive.
     */
	[audioSession setActive:YES error:nil];
	BAEMixer_ReengageAudio(mMixer);
}


- (void)applicationWillTerminate:(UIApplication *)application 
{
    /*
     Called when the application is about to terminate.
     */
	BAEMixer_Delete(mMixer);
	printf("applicationWillTerminate\n");
}

#pragma mark -
#pragma mark UITabBarControllerDelegate methods

/*
// Optional UITabBarControllerDelegate method.
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
}
*/

/*
// Optional UITabBarControllerDelegate method.
- (void)tabBarController:(UITabBarController *)tabBarController didEndCustomizingViewControllers:(NSArray *)viewControllers changed:(BOOL)changed {
}
*/


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc 
{
    [tabBarController release];
	[navController release];
    [window release];
    [super dealloc];
}

// called from BAE idle task
- (void)updatePositionGUI:(NSObject*)obj
{	
	if (mCurrentPlaying)
	{
		unsigned long pos = 0;
		unsigned long len = 0;
		BAESong_GetMicrosecondPosition(mCurrentPlaying, &pos);
		BAESong_GetMicrosecondLength(mCurrentPlaying, &len);

		unsigned long cpuLoad = 0;
		BAEMixer_GetCPULoadInPercent(mMixer, &cpuLoad);
		[currentFileDetail updateSongRealtimeInfo:pos length:len mixTimePercent:(int)cpuLoad];
	}
}

- (void)setMuteChannel:(int) channel state: (BOOL) on
{
	if (mCurrentPlaying)
	{
		if (on)
		{
			BAESong_MuteChannel(mCurrentPlaying, channel);
		}
		else
		{
			BAESong_UnmuteChannel(mCurrentPlaying, channel);
		}
	}
}

- (void)setSoloChannel:(int) channel state: (BOOL) on
{
	if (mCurrentPlaying)
	{
		if (on)
		{
			BAESong_SoloChannel(mCurrentPlaying, channel);
		}
		else
		{
			BAESong_UnSoloChannel(mCurrentPlaying, channel);
		}
	}
}

- (void)getChannelVolumes:(float*)values
{
	for (int count = 0; count < 16; count++)
	{
		values[count] = [self getChannelVolume:count];
	}
}

- (float)getChannelVolume:(int) channel
{
	float volume = 1;
	if (mCurrentPlaying)
	{
		char value;
		BAESong_GetControlValue(mCurrentPlaying, channel, VOLUME_MSB, &value);
		volume = value / 127.0f;
	}
	return volume;
}

- (void)setChannelVolume:(int) channel vol: (float)value
{
	if (mCurrentPlaying)
	{
		BAESong_ControlChange(mCurrentPlaying, channel, VOLUME_MSB, 127.0f * value, 0);
	}	
}

- (void)setMasterVolume:(float) value
{
	if (mCurrentPlaying)
	{
		BAESong_SetVolume(mCurrentPlaying, FLOAT_TO_FIXED(value));
	}
}

- (void)setMasterTempo:(float) value
{
	if (mCurrentPlaying)
	{
		BAESong_SetMasterTempo(mCurrentPlaying, FLOAT_TO_FIXED(value));
	}
}


- (void)updateSlidersGUI:(NSObject*)obj
{
	SongControllerDetails* slider = (SongControllerDetails*)obj;
	
	[currentFileDetail updateChannelVolumeUI:slider.channel vol:slider.value / 127.0f];
	[slider release];
	slider = nil;
}

static void PV_SongControllerCallback(BAESong pSong, void* reference, short int channel, short int track, short int controler, short int value)
{
	iOSPlayAppDelegate* app = (iOSPlayAppDelegate*)reference;
	switch (controler)
	{
		case VOLUME_MSB:
		{
			SongControllerDetails* songDetails = [[SongControllerDetails alloc] init];
			songDetails.channel = channel;
			songDetails.track = track;
			songDetails.controller = controler;
			songDetails.value = value;
			
			// call main thread to update GUI, otherwise leaks will occur.
			[app performSelectorOnMainThread:@selector(updateSlidersGUI:)
								  withObject:songDetails
							   waitUntilDone:NO];
		}
		break;
	}
}

- (NSArray*)getSongDetails
{
	if (mCurrentPlaying)
	{
		NSMutableArray* rmfDetails = [[NSMutableArray alloc] init];
		char text[256];
		
		if (BAEUtil_GetRmfSongInfoFromFile(mCurrentSongFile, 0, TITLE_INFO, text, 255) == BAE_NO_ERROR)
		{
			if (strlen(text) == 0)
			{
				if (BAESong_GetTitle(mCurrentPlaying, text, 255))
				{
					text[0] = 0;
				}
			}
			NSString* title = [NSString stringWithUTF8String: text];
			[rmfDetails addObject:title];
		}
		
		if (BAEUtil_GetRmfSongInfoFromFile(mCurrentSongFile, 0, PERFORMED_BY_INFO, text, 255) == BAE_NO_ERROR)
		{
			NSString* performed = [NSString stringWithUTF8String: text];
			[rmfDetails addObject:performed];
		}
		
		if (BAEUtil_GetRmfSongInfoFromFile(mCurrentSongFile, 0, COPYRIGHT_INFO, text, 255) == BAE_NO_ERROR)
		{
			NSString* copyright = [NSString stringWithUTF8String: text];
			[rmfDetails addObject:copyright];
		}
		
		return rmfDetails;
	}
	return nil;
}

- (void)setSongPosition:(float)pos
{
	if (mCurrentPlaying)
	{
		unsigned long posMS = 0;
		unsigned long len = 0;
		BAESong_GetMicrosecondLength(mCurrentPlaying, &len);
		
		posMS = (len * pos);
		BAESong_SetMicrosecondPosition(mCurrentPlaying, posMS);
	}
}

- (BAEMixer)getMixer
{
	return mMixer;
}

- (void)playSong:(char*) songFile
{
	[self stopSong];
	
	// turn on nice verb
	BAEMixer_SetDefaultReverb(mMixer, BAE_REVERB_TYPE_8);
	
	mCurrentPlaying = BAESong_New(mMixer);
	strcpy(mCurrentSongFile, songFile);
	BAEResult err = BAESong_LoadRmfFromFile(mCurrentPlaying, (BAEPathName)songFile, 0, TRUE);
	if (err == BAE_NO_ERROR)
	{
		err = BAESong_Start(mCurrentPlaying, 0);
		BAESong_SetControllerCallback(mCurrentPlaying, PV_SongControllerCallback, self, VOLUME_MSB);
	}
}

- (void)stopSong
{
	if (mCurrentPlaying)
	{
		BAESong_Stop(mCurrentPlaying, FALSE);
		BAESong_Delete(mCurrentPlaying);
	}
	mCurrentPlaying = NULL;
}

@end

