#!/tools/xgs/perl/5.8.5/bin/perl
#!/usr/bin/expect
 
use strict;
use warnings;
use Expect;
 
 
my $i=0;
my $EMULATOR_COMMAND=$ARGV[0];
my $EXECUTABLE=$ARGV[1];
my $XCLBIN=$ARGV[2];
my $command;
 
#$command="launch_emulator -no-reboot -runtime ocl -t hw_emu -sd-card-image sd_card.img -device-family Ultrascale";
$command=$EMULATOR_COMMAND;
 
my($exp)  = new Expect; #Set exp for Expect
my($exp1) = new Expect; #Set exp for Expect
 
$exp-> spawn("$command")
or die "Cannot spawn $command: $!\n";
 
 while ($i<3) {
    if( $exp->expect(300, ':/mnt#')) {
      
    }
     
    sleep(10);
    $exp-> send( "mount /dev/mmcblk0p1 /mnt\r");
	$exp-> send( "cd /mnt\r");
	$exp-> send( "$EXECUTABLE\r");
    sleep(10);
    last;
     
    $i=$i+1;
 
  }
if( $exp-> expect(2000, "TEST PASSED")) {
    print "\nEmbedded Host Run Completed\n";
 
  } else {
        print "TEST PASSED String Not Found, Hence exiting ";
        exit(1);
  }
if( $exp-> expect(2000, "TEST FAILED")) {
    print "Embedded Host Run Failed";
    exit(1);
  }
 
if( $i < 3 ){
    printf "Linux kernel booted successfully\n";
}else{
    printf "Linux kernel boot failed\n";
    printf "\n";
        exit(1);
}
 
print "Exiting QEMU \n";
 sleep(10);
$exp-> send("\x01");
$exp-> send("x");
$exp->soft_close();
