#!/usr/bin/perl -w

use strict;

my @pids = glob "work*/pid.txt";

foreach my $pidfile (@pids) {
    open my $INF, "<$pidfile" or die;
    chomp(my $pid = <$INF>);
    print "killing $pid\n";
    my $ret = kill('KILL', $pid);
    die "failed!" unless $ret == 1;
    close $INF;
}
