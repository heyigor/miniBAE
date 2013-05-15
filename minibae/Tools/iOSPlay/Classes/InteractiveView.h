//
//  InteractiveView.h
//  iOSPlay
//
//  Created by Steve Hales on 10/9/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iOSPlayAppDelegate.h"


#define KEY_SQUARE_MAX  (6*7)

typedef struct
{
    CGRect      mBound;
    int         mKey;
    NSString*   mMidiNoteName;
    int         mPressed;   // count for multiple pressed of the same note
} KeySquare;

typedef struct
{
    unsigned char   mBank;
    unsigned char   mInstrument;
    unsigned char   mChannel;
} KeySquareSetup;

typedef struct
{
    unsigned long mTick;    // at record time
    unsigned char mCommand;
    unsigned char mData1;
    unsigned char mData2;
    unsigned char mData3;
} RecordStep;

enum 
{
    SEQ_STOPPED = 0,
    SEQ_RECORDING,
    SEQ_PLAYING
};

@interface InteractiveView : UIView 
{
    CGRect          square;
    BAESong         mInteractiveSong;
    BAEMixer        mMixer;
    KeySquare       mSquares[KEY_SQUARE_MAX];
    KeySquareSetup  mKeySetup[KEY_SQUARE_MAX];

    int             mSeqState;
    NSMutableData*  mRecording;
}

- (void)onRecord:(id)sender;
- (void)onStop:(id)sender;
- (void)onPlay:(id)sender;
- (void)setRecordButton;
- (void)setPlayButton;
- (void)setStopButton;

void drawLinearGradient(CGContextRef context, CGRect rect, CGColorRef startColor, 
                        CGColorRef  endColor);

@end
