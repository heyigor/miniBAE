//
//  SecondViewController.h
//  iOSPlay
//
//  Created by Steve Hales on 9/30/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "InteractiveView.h"

@interface SecondViewController : UIViewController 
{
	InteractiveView*	interactiveView;
}
@property (nonatomic, retain) IBOutlet InteractiveView* interactiveView;

@end
