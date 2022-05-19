#!/usr/bin/perl

use strict;
use warnings;

sub printHex {
    my ($fh,$array) = @_;
    my $array_ptr = $#{$array}; # Start the pointer by pointing to the end of the array
    while($array_ptr >= 0) {
        my $x = sprintf("%08x",$array->[$array_ptr]);
        if (length($x) > 8) {print $fh substr($x,8)}
        else                {print $fh $x}
        $array_ptr--;
    }
    print $fh "\n";
}
my $input_file   = $ARGV[0];
my $output_file1 = $ARGV[1];
my $output_file2 = $ARGV[2];

open my $ifh,  '<', $input_file   or die "Could not open file ${input_file}\n";
open my $ofh1, '>', $output_file1 or die "Could not open file ${output_file1}\n";
open my $ofh2, '>', $output_file2 or die "Could not open file ${output_file2}\n";

my $ptr = 0;
my @scratch_pad = [];
while (<$ifh>) {
    if ($ptr == 2) {$ptr = 0;print $ofh1 "@scratch_pad\n";printHex($ofh2,\@scratch_pad)}
    #if ($_ =~ /^(\s*[-]*)\s*(\d+)\s*$/) {$scratch_pad[3-$ptr] = "${1}${2}"}
    if ($_ =~ /^(\s*[-]*)\s*(\d+)\s*$/) {$scratch_pad[$ptr] = "${1}${2}"}
    $ptr++;
}
print $ofh1 "@scratch_pad\n";
printHex($ofh2,\@scratch_pad);
