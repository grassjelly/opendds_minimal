eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# -*- perl -*-

use Env (DDS_ROOT);
use lib "$DDS_ROOT/bin";
use Env (ACE_ROOT);
use lib "$ACE_ROOT/bin";
use PerlDDS::Run_Test;
use strict;

my $status = 0;

my $sub_opts = " -DCPSConfigFile config/rtps.ini";

my $Subscriber = PerlDDS::create_process ("build/subscriber", " $sub_opts");

print $Subscriber->CommandLine() . "\n";
$Subscriber->Spawn ();
print STDOUT "\nRunning Subscriber\n\n";

my $SubscriberResult = $Subscriber->WaitKill (300);
if ($SubscriberResult != 0) {
    print STDERR "ERROR: Subscriber returned $SubscriberResult \n";
    $status = 1;
}

exit $status;
