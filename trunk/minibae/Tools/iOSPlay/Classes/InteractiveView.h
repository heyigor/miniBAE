//
//  InteractiveView.h
//  iOSPlay
//
//  Created by Steve Hales on 10/9/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iOSPlayAppDelegate.h"


#define KEY_SQUARE_MAX	(6*7)

typedef struct
{
	CGRect		mBound;
	int			mKey;
	NSString*	mMidiNoteName;
	int			mPressed;	// count for multiple pressed of the same note
} KeySquare;

typedef struct
{
	unsigned char	mInstrument;
	unsigned char	mChannel;
} KeySquareSetup;


@interface InteractiveView : UIView 
{
	CGRect			square;
	BAESong			mInteractiveSong;
	KeySquare		mSquares[KEY_SQUARE_MAX];
	KeySquareSetup	mKeySetup[KEY_SQUARE_MAX];
}

void drawLinearGradient(CGContextRef context, CGRect rect, CGColorRef startColor, 
						CGColorRef  endColor);

@end
