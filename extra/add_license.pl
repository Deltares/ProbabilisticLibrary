#!/usr/bin/perl

# script for adding license text
#

use strict;
use warnings;

my $lang = 'F';
if (scalar @ARGV)
{
    if (lc($ARGV[0]) eq 'c')
    {
        $lang = 'C';
    }
}

my $licenseFile = 'd:\checkouts\ProbabilisticLibrary\extra\license.txt';

open IN, "<$licenseFile" or die "can't open licensefile $licenseFile: $!\n";
my @license = ();
my $found_start = 0;
while (my $line = <IN>)
{
    chomp($line);
    if ($line =~ m/^extensions:/i) {$found_start = 0;}
    if ($found_start) {push @license, $line;}
    if ($lang eq 'C' and $line =~ m/^extensions: .cs/i) {$found_start = 1;}
    if ($lang eq 'F' and $line =~ m/^extensions: .f90/i) {$found_start = 1;}
}
close IN;

my @sourceFiles = ();
if ($lang eq 'F')
{
    @sourceFiles = glob('*.f90');
}
else
{
    @sourceFiles = glob('*.cpp *.h *.cs');
}

foreach my $srcFile (@sourceFiles)
{
    #
    # read all lines;
    # skip EOL
    #
    open IN, "<$srcFile";
    my $skip = 0;
    my @lines = ();
    while (my $line = <IN>)
    {
        chomp($line);
        if ($line =~ m/copyright.*deltares.*hkv.*tno/i)
        {
            $skip = 1;
        }
        else
        {
            push @lines, $line if ($skip == 0 or $line !~ m/^ *! *$/);
            $skip = 0;
        }
    }
    close IN;

    open OUT, ">$srcFile" or die "can't open file $srcFile for writing: $!\n";

    my $n = scalar @license;
    for (my $i=0 ; $i < $n ; $i++)
    {
        print OUT $license[$i] . "\n";
    }

    $n = scalar @lines;
    for (my $i=0 ; $i < $n ; $i++)
    {
        print OUT $lines[$i] . "\n";
    }
    close OUT or die "can't close file $srcFile after writing: $!\n";

}

