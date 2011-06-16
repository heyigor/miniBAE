//
//  iOSPlayAppDelegate.h
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVAudioSession.h>
#import "PlayFileDetail.h"
#include "MiniBAE.h"

@interface iOSPlayAppDelegate : NSObject <UIApplicationDelegate, UITabBarControllerDelegate, AVAudioSessionDelegate> 
{
    UIWindow*				window;
    UITabBarController*		tabBarController;
	UINavigationController*	navController;

	AVAudioSession*			audioSession;

	BAEMixer				mMixer;
	BAEBankToken			mBank;
	
	BAESong					mCurrentPlaying;
	char					mCurrentSongFile[2048];
	
	PlayFileDetail*			currentFileDetail;
}

@property (nonatomic, retain) IBOutlet UIWindow* window;
@property (nonatomic, retain) IBOutlet UITabBarController* tabBarController;
@property (nonatomic, retain) IBOutlet UINavigationController* navController;
@property (nonatomic, retain) PlayFileDetail* currentFileDetail;

- (BAEMixer)getMixer;
- (void)playSong:(const char*) songFile;
- (void)stopSong;
- (void)setSongPosition:(float)pos;
- (NSArray*)getSongDetails;
- (void)updatePositionGUI:(NSObject*)obj;
- (void)setMuteChannel:(int) channel state: (BOOL) on;
- (void)setSoloChannel:(int) channel state: (BOOL) on;
- (void)getChannelVolumes:(float*)values;
- (float)getChannelVolume:(int) channel;
- (void)setChannelVolume:(int) channel vol: (float)value;
- (void)setMasterVolume:(float) value;
- (void)setMasterTempo:(float) value;

@end

//MyAppDelegate * app = (MyAppDelegate * )[[UIApplication sharedApplication] delegate];
iOSPlayAppDelegate* getAppInstance();
