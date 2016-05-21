#!/usr/bin/perl

use strict;

my $count = 0;

while (<>) {
    chomp;	
    s/^\s*(.*?)\s*$/$1/;

    if ($_ ne "") {
	++$count;
    }
}

print "Total lines: $count\n";
