//
//  FirstViewController.h
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PlayTableView.h"

@interface FirstViewController : UIViewController 
{
	PlayTableView*	playTableView;
}
@property (nonatomic, retain) PlayTableView* playTableView;

@end
