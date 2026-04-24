use File::Basename;
use Getopt::Std;
use Cwd 'abs_path';     # aka realpath()
my $full_list = "Piplus_20234701.list";


open(inFile,"$full_list");
$num = 0;
while($eachLine = <inFile>) { $num = $num +1;}
close inFile;
$div = int(sqrt($num));
print `echo $div \n`;

if(-e "List") { print `rm -r List \n`;}
print `mkdir List \n`;
print `rm Jobs/cen*/* \n`;

$count = 1;
open(diffFile,"$full_list");
while($eachLine = <diffFile>) {
	if($count % $div == 1) {$name = int ($count / $div) +1; open(list,">List/$name.list");}
	$count =$count+1;
	print list $eachLine;
}
close diffFile;

print `rm -rf Jobs \n`;
print `mkdir Jobs\n`;
for(my $u = 1;$u<10;$u++){
	print `mkdir Jobs/cen$u \n`;
}


for (my $i=1; $i <= $num/$div+1; $i++) {
   	print `echo $i \n`;
   	print `star-submit-template -template submit_condor.xml -entities job=$i \n`;
	print `sleep 1 \n`;
}
