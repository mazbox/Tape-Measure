/**
 *  clipboard.cpp
 *
 *  Created by Marek Bereza on 06/09/2011.
 */

#include "clipboard.h"
#include <AppKit/AppKit.h>


void copyToClipboard(string s) {
	NSString *str = [NSString stringWithCString:s.c_str() 
										  encoding:[NSString defaultCStringEncoding]];
	
	NSPasteboard *pasteBoard = [NSPasteboard generalPasteboard];
	[pasteBoard declareTypes:[NSArray arrayWithObjects:NSStringPboardType, nil] owner:nil];
	[pasteBoard setString:str forType:NSStringPboardType];
}