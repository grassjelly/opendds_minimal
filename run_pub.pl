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

my $pub_opts = " -DCPSConfigFile config/rtps.ini";

my $Publisher = PerlDDS::create_process ("build/publisher", " $pub_opts");

print $Publisher->CommandLine() . "\n";
$Publisher->Spawn ();
print STDOUT "\nRunning Publisher\n\n";

my $PublisherResult = $Publisher->WaitKill (300);
if ($PublisherResult != 0) {
    print STDERR "ERROR: publisher returned $PublisherResult \n";
    $status = 1;
}

exit $status;
