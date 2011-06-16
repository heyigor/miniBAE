//
//  PlayFileDetail.h
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface PlayFileDetail : UIViewController 
{
	UILabel*	detailText;
	NSString*	fileName;

	UILabel*	cpuLoad;

	UISegmentedControl* mChannelControl0;
	UISegmentedControl* mChannelControl1;
	UISegmentedControl* mChannelControl2;
	UISegmentedControl* mChannelControl3;
	UISegmentedControl* mChannelControl4;
	UISegmentedControl* mChannelControl5;
	UISegmentedControl* mChannelControl6;
	UISegmentedControl* mChannelControl7;
	UISegmentedControl* mChannelControl8;
	UISegmentedControl* mChannelControl9;
	UISegmentedControl* mChannelControl10;
	UISegmentedControl* mChannelControl11;
	UISegmentedControl* mChannelControl12;
	UISegmentedControl* mChannelControl13;
	UISegmentedControl* mChannelControl14;
	UISegmentedControl* mChannelControl15;
	UISegmentedControl*	mChannelControl[16];
	
	UISlider*			mSongPosition;
	UISlider*			mMasterTempo;
	UISlider*			mMasterVolume;
	
	UISlider*			mChannelVolume0;
	UISlider*			mChannelVolume1;
	UISlider*			mChannelVolume2;
	UISlider*			mChannelVolume3;
	UISlider*			mChannelVolume4;
	UISlider*			mChannelVolume5;
	UISlider*			mChannelVolume6;
	UISlider*			mChannelVolume7;
	UISlider*			mChannelVolume8;
	UISlider*			mChannelVolume9;
	UISlider*			mChannelVolume10;
	UISlider*			mChannelVolume11;
	UISlider*			mChannelVolume12;
	UISlider*			mChannelVolume13;
	UISlider*			mChannelVolume14;
	UISlider*			mChannelVolume15;
	UISlider*			mChannelVolume[16];
}
@property (nonatomic, retain) IBOutlet UILabel *detailText;
@property (nonatomic, retain) IBOutlet UILabel *cpuLoad;
@property (nonatomic, assign) NSString *fileName;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl0;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl1;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl2;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl3;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl4;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl5;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl6;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl7;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl8;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl9;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl10;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl11;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl12;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl13;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl14;
@property (nonatomic, retain) IBOutlet UISegmentedControl* mChannelControl15;

@property (nonatomic, retain) IBOutlet UISlider* mSongPosition;
@property (nonatomic, retain) IBOutlet UISlider* mMasterTempo;
@property (nonatomic, retain) IBOutlet UISlider* mMasterVolume;

@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume0;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume1;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume2;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume3;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume4;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume5;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume6;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume7;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume8;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume9;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume10;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume11;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume12;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume13;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume14;
@property (nonatomic, retain) IBOutlet UISlider* mChannelVolume15;

- (IBAction)toggleChannelControl: (UISegmentedControl*)which;
- (IBAction)toggleChannelVolume: (UISlider*)which;

- (IBAction)changeMasterTempo;
- (IBAction)changeMasterVolume;

- (IBAction)changeSongPosition;

- (void)reset;
- (void)updateChannelVolumeUI:(int) channel vol: (float)value;
- (void)updateSongRealtimeInfo:(unsigned long)pos length:(unsigned long)length mixTimePercent:(int)mixTimePercent;

@end
