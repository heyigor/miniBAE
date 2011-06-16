//
//  FirstViewController.m
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//

#import "FirstViewController.h"


@implementation FirstViewController
@synthesize playTableView;

/*
// The designated initializer. Override to perform setup that is required before the view is loaded.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil 
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) 
	{
        // Custom initialization		
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView 
{	
    [super loadView];

    printf("loadView %p\n", self);
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
    [super viewDidLoad];
    printf("viewDidLoad PlayTableView %p\n", self);
	
    self.playTableView = [[PlayTableView alloc] initWithNibName:@"PlayTableView" bundle:nil];
    [self.view addSubview:playTableView.view];
}

- (void)viewDidAppear:(BOOL)animated
{
	UINavigationController* nav = [self navigationController];
	[[[nav navigationBar] topItem] setTitle:@"Songs"];

	[super viewDidAppear:animated];
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload 
{
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
    self.playTableView = nil;
}


- (void)dealloc 
{
    [super dealloc];
}


@end
