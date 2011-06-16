//
//  PlayTableView.m
//  iOSPlay
//
//  Created by Steve Hales on 9/15/10.
//  Copyright 2010 Igor's Software Laboratories. All rights reserved.
//
#import "iOSPlayAppDelegate.h"
#import "PlayTableView.h"
#import "PlayFileDetail.h"

@implementation PlayTableView


#pragma mark -
#pragma mark View lifecycle

- (void)viewDidLoad 
{
    [super viewDidLoad];
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;

	mFiles = [[NSMutableArray alloc] init];
}

/*
- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}
*/
/*
- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
*/
/*
- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}
*/
/*
- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}
*/
/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


#pragma mark -
#pragma mark Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView 
{
    // Return the number of sections.
    return 1;
}


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	NSError *err;
	
	// get path to resources
	NSString* resources = [[ NSBundle mainBundle ] resourcePath];	
	NSFileManager *filemgr = [NSFileManager defaultManager];
	NSArray *filelist = [filemgr contentsOfDirectoryAtPath: resources error:&err];
	
	int count;
	int xcount = 0;
	
	for (count = 0; count < [filelist count]; count++)
	{
		BOOL add = FALSE;
		NSString* fileName = [filelist objectAtIndex: count];
		if ([fileName hasSuffix:@".rmf"])
		{
			add = TRUE;
		}
		if ([fileName hasSuffix:@".mid"])
		{
			add = TRUE;
		}
		if (add)
		{
			xcount++;
			[mFiles addObject:[fileName mutableCopy]];
		}
	}
	
    // Return the number of rows in the section.
    return xcount;
}


- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath 
{    
    static NSString *CellIdentifier = @"CellIdentifier";
	
    // Dequeue or create a cell of the appropriate type.
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) 
	{
        cell = [[[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] autorelease];
        cell.accessoryType = UITableViewCellAccessoryNone;
    }
	
    // Configure the cell.
    cell.textLabel.text = [mFiles objectAtIndex: indexPath.row];
    return cell;
}


/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationFade];
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/


/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/


/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/


#pragma mark -
#pragma mark Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath 
{
    // Navigation logic may go here. Create and push another view controller.
	PlayFileDetail *detailViewController = [[PlayFileDetail alloc] initWithNibName:@"PlayFileDetail" bundle:nil];
	detailViewController.fileName = [mFiles objectAtIndex: indexPath.row];

	iOSPlayAppDelegate* app = (iOSPlayAppDelegate*)[[UIApplication sharedApplication] delegate];

	// Pass the selected object to the new view controller.
	[[app.tabBarController navigationController] pushViewController:detailViewController animated:YES];
	[detailViewController release];
}


#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning 
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Relinquish ownership any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload 
{
    // Relinquish ownership of anything that can be recreated in viewDidLoad or on demand.
    // For example: self.myOutlet = nil;
}


- (void)dealloc 
{
	[mFiles removeAllObjects];
	mFiles = nil;
    [super dealloc];
}


@end

