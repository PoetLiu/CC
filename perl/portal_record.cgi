#!/usr/bin/perl -Tw

use utf8;
use strict;
use CGI;
use DBI;
use warnings;

my ($cgi) = new CGI;
print $cgi->header('text/html');
print $cgi->p, "Success\n";
print $cgi->end_html, "\n";

my $dbh = DBI->connect('dbi:mysql:Portal','root','liupeng') || die "Error opening database: $DBI::errstr\n";
$dbh->do("SET NAMES 'UTF8'");
my $sql = "INSERT INTO auth_record (mac, wan_mac, wan_ip, login_time, model, version) VALUES (?,?,?,?,?,?)";
my $sth = $dbh->prepare($sql);
$sth->execute($cgi->param('mac'), $cgi->param('wan_mac'), $cgi->param('wan_ip'), 
	$cgi->param('login_time'), $cgi->param('model'), $cgi->param('version'));
$sth->finish();
$dbh->disconnect || die "Failed to disconnect\n";

print $cgi->header('text/html; charset=utf-8');
print $cgi->p, "Hi, ", $cgi->param('model'), "\n";
print $cgi->p, "Success\n";
print $cgi->end_html, "\n";
