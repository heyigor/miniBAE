//
//  PlayFileDetail.m
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import "PlayFileDetail.h"
#import "iOSPlayAppDelegate.h"

@implementation PlayFileDetail

@synthesize fileName;
@synthesize detailText, cpuLoad;

@synthesize mChannelControl0, mChannelControl1, mChannelControl2, mChannelControl3, mChannelControl4, mChannelControl5, mChannelControl6, mChannelControl7;
@synthesize mChannelControl8, mChannelControl9, mChannelControl10, mChannelControl11, mChannelControl12, mChannelControl13, mChannelControl14, mChannelControl15;

@synthesize mChannelVolume0, mChannelVolume1, mChannelVolume2, mChannelVolume3, mChannelVolume4, mChannelVolume5, mChannelVolume6, mChannelVolume7;
@synthesize mChannelVolume8, mChannelVolume9, mChannelVolume10, mChannelVolume11, mChannelVolume12, mChannelVolume13, mChannelVolume14, mChannelVolume15;

@synthesize mMasterTempo, mMasterVolume, mSongPosition;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
    [super viewDidLoad];

	mChannelControl[0] = mChannelControl0;
	mChannelControl[1] = mChannelControl1;
	mChannelControl[2] = mChannelControl2;
	mChannelControl[3] = mChannelControl3;
	mChannelControl[4] = mChannelControl4;
	mChannelControl[5] = mChannelControl5;
	mChannelControl[6] = mChannelControl6;
	mChannelControl[7] = mChannelControl7;
	mChannelControl[8] = mChannelControl8;
	mChannelControl[9] = mChannelControl9;
	mChannelControl[10] = mChannelControl10;
	mChannelControl[11] = mChannelControl11;
	mChannelControl[12] = mChannelControl12;
	mChannelControl[13] = mChannelControl13;
	mChannelControl[14] = mChannelControl14;
	mChannelControl[15] = mChannelControl15;	
	
	mChannelVolume[0] = mChannelVolume0;
	mChannelVolume[1] = mChannelVolume1;
	mChannelVolume[2] = mChannelVolume2;
	mChannelVolume[3] = mChannelVolume3;
	mChannelVolume[4] = mChannelVolume4;
	mChannelVolume[5] = mChannelVolume5;
	mChannelVolume[6] = mChannelVolume6;
	mChannelVolume[7] = mChannelVolume7;
	mChannelVolume[8] = mChannelVolume8;
	mChannelVolume[9] = mChannelVolume9;
	mChannelVolume[10] = mChannelVolume10;
	mChannelVolume[11] = mChannelVolume11;
	mChannelVolume[12] = mChannelVolume12;
	mChannelVolume[13] = mChannelVolume13;
	mChannelVolume[14] = mChannelVolume14;
	mChannelVolume[15] = mChannelVolume15;	
	
	[self reset];

	iOSPlayAppDelegate* app = getAppInstance();
	app.currentFileDetail = self;
	[app playSong: [fileName UTF8String]];
	
	NSArray* rmfDetails = [app getSongDetails];
	[self.detailText setText:[NSString stringWithFormat:@"%@\n%@\n%@", 
							  [rmfDetails objectAtIndex:0],
							  [rmfDetails objectAtIndex:1],
							  [rmfDetails objectAtIndex:2]]];
	[rmfDetails release];
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];	
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning 
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload 
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewDidDisappear:(BOOL)animated
{
	[super viewDidDisappear:animated];

	iOSPlayAppDelegate* app = getAppInstance();
	app.currentFileDetail = nil;
	[app stopSong];
}

- (void)dealloc 
{
    [super dealloc];
}

- (void)reset;
{
	for (int count = 0; count < 16; count++)
	{
		mChannelControl[count].tag = count;
		mChannelControl[count].selectedSegmentIndex = 0;
		mChannelVolume[count].tag = count;
		mChannelVolume[count].value = 1;
	}
	mMasterTempo.value = 1;
	mMasterTempo.maximumValue = 2;
	mMasterVolume.value = 1;
	mSongPosition.value = 0;
}

- (IBAction)toggleChannelVolume: (UISlider*)which
{
	iOSPlayAppDelegate* app = getAppInstance();
	int ch = which.tag;
	float v = which.value;
	[app setChannelVolume:ch vol:v];
}

- (IBAction)changeMasterTempo
{
	iOSPlayAppDelegate* app = getAppInstance();
	[app setMasterTempo: mMasterTempo.value];
}

- (IBAction)changeMasterVolume
{
	iOSPlayAppDelegate* app = getAppInstance();
	[app setMasterVolume: mMasterVolume.value];
}

- (void)updateSongRealtimeInfo:(unsigned long)pos length:(unsigned long)length mixTimePercent:(int)mixTimePercent
{
	mSongPosition.value = ((float)pos / (float)length);
	
	[self.cpuLoad setText:[NSString stringWithFormat:@"CPU load %d %%", mixTimePercent]];
}

- (IBAction)changeSongPosition
{
	iOSPlayAppDelegate* app = getAppInstance();
	[app setSongPosition:mSongPosition.value];
	
	float volumes[16];
	[app getChannelVolumes:volumes];
	for (int count = 0; count < 16; count++)
	{
		int chan = mChannelControl[count].tag;
		mChannelVolume[chan].value = volumes[count];
	}
}

- (void)updateChannelVolumeUI:(int) channel vol: (float)value
{
	// put a pool around our UI changes, because this is happening at the audio thread
    //NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	for (int count = 0; count < 16; count++)
	{
		if (mChannelControl[count].tag == channel)
		{
			mChannelVolume[count].value = value;
		}
	}
    //[pool release];
}

- (IBAction)toggleChannelControl: (UISegmentedControl*)which
{
	iOSPlayAppDelegate* app = getAppInstance();
	
	int s = which.selectedSegmentIndex;
	int ch = which.tag;
	switch (s)
	{
		case 0:	// on
			[app setMuteChannel:ch state:FALSE];
			[app setSoloChannel:ch state:FALSE];
			break;
		case 1: // mute
			[app setMuteChannel:ch state:TRUE];
			[app setSoloChannel:ch state:FALSE];
			break;
		case 2: // solo
			[app setMuteChannel:ch state:FALSE];
			[app setSoloChannel:ch state:TRUE];
			break;
	}
}



@end
