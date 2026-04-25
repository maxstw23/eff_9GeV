use File::Basename;
use Getopt::Std;
use Cwd 'abs_path';     # aka realpath()

my %pid = (
    piplus     => 8,
    piminus    => 9,
    kplus      => 11,
    kminus     => 12,
    proton     => 14,
    antiproton => 15,
);

my %list_prefix = (
    piplus     => 'Piplus',
    piminus    => 'Piminus',
    kplus      => 'Kplus',
    kminus     => 'Kminus',
    proton     => 'Proton',
    antiproton => 'AntiProton',
);

my $species = lc($ARGV[0] // '');
die "Usage: perl split.pl <species>\nValid species: " . join(', ', sort keys %pid) . "\n"
    unless exists $pid{$species};

my ($list_file) = glob($list_prefix{$species} . "*.list");
die "No list file found for $species\n" unless $list_file;

my $pid_val = $pid{$species};

# Update output species in merge.sh
do {
    open my $fh, '<', 'merge.sh' or die "Cannot read merge.sh: $!";
    my $content = do { local $/; <$fh> };
    close $fh;
    $content =~ s|(cen\$c\.)\w+(\.root)|$1$species$2|g;
    open $fh, '>', 'merge.sh' or die "Cannot write merge.sh: $!";
    print $fh $content;
    close $fh;
};

print "Configured for $species: PID=$pid_val, list=$list_file\n";

# Slurp all lines before List/ is removed
open(my $inFh, '<', $list_file) or die "Cannot open $list_file: $!";
my @lines = <$inFh>;
close $inFh;
my $num = scalar @lines;
my $div = int(sqrt($num));
print "div=$div\n";

if(-e "List") { print `rm -r List \n`;}
print `mkdir List \n`;
print `rm Jobs/cen*/* \n`;

my $count = 1;
for my $eachLine (@lines) {
    if($count % $div == 1) {
        my $name = int($count / $div) + 1;
        open(list, '>', "List/$name.list") or die "Cannot write List/$name.list: $!";
    }
    $count++;
    print list $eachLine;
}

print `rm -rf Jobs \n`;
print `mkdir Jobs\n`;
for(my $u = 1; $u < 10; $u++){
    print `mkdir Jobs/cen$u \n`;
}

for (my $i=1; $i <= $num/$div+1; $i++) {
    print `echo $i \n`;
    print `star-submit-template -template submit_condor.xml -entities job=$i,pid=$pid_val \n`;
    print `sleep 1 \n`;
}
