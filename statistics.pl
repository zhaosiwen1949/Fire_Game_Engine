sub GetTime {
    $_ = shift;
    my $t = shift;
    (!$t) and ($t = time);
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime($t);
    $year += 1900;
    my $yy = substr $year,2,2;
    $mon++;
    s/yyyy/$year/gi;
    s/yy/$yy/gi;
	($mon<10 and s/mm/0$mon/gi) or s/mm/$mon/gi;
	($mday<10 and s/dd/0$mday/gi) or s/dd/$mday/gi;
	($hour<10 and s/hh/0$hour/gi) or s/hh/$hour/gi;
	($min<10 and s/mi/0$min/gi) or s/mi/$min/gi;
	($sec<10 and s/ss/0$sec/gi) or s/ss/$sec/gi;
    $_;
}
sub LineCountOfFile{
	open F,"<".shift;
	my @lines=<F>;
	close F;
	my $count=@lines;
	return $count;
}
sub GetLineCountOfCore{
	my $dir=shift;
	my @files=<$dir/*>;
	my $lineCount=0;
	map{
		my $file=$_;
		$lineCount+=GetLineCountOfCore($file) unless not -d $file and ((not ($file=~/.+\.h.*/ or $file=~/.+\.c.*/ or $file=~/.+\.java/ or $file=~/.+\.mm/)) or $lineCount+=LineCountOfFile($file));
	}@files;
	print "$dir $lineCount\n";
	return $lineCount;
}
sub GetLineCountOf{
	my $dir=shift;
	my $extension=shift;
	my @files=<$dir/*>;
	my $lineCount=0;
	map{
		my $file=$_;
		$lineCount+=GetLineCountOf($file,$extension) unless not -d $file and ((not $file=~/.+\.$extension/) or $lineCount+=LineCountOfFile($file));
	}@files;
	print "$dir $lineCount\n";
	return $lineCount;
}
my $PlatformAppCount=GetLineCountOfCore("Platform/WinPlayer");
$PlatformAppCount+=GetLineCountOfCore("Platform/Win32Common");
$PlatformAppCount+=GetLineCountOfCore("Platform/OSXPlayer");
$PlatformAppCount+=GetLineCountOfCore("Platform/iPhonePlayer");
$PlatformAppCount+=GetLineCountOfCore("Platform/AndroidPlayer");
my $RuntimeLineCount=GetLineCountOfCore("Runtime");
my $LuaCodeLineCount=GetLineCountOf("Platform/AliceEdu2/EduDebug/Assets","lua");
open F,">>code_lines.txt" or die"%@";
my $time=GetTime("yyyy-mm-dd hh:mi:ss");
print F "$time Platform:$PlatformAppCount runtime:$RuntimeLineCount\n";
close F;
<STDIN>;