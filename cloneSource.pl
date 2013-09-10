# Data types
@ts = ('char', 'int', 'long', 'float', 'double');

# Read template
open F, 'spar.c';
@ls = <F>;
close F;

# Read functions
foreach $l (@ls)
{
	if( $l =~ /^\s*(void|int|double|sparType|spar)\s*\*?\s*(spar[^\(]*?)\s*\(/ )
	{
		$f = $2;
		push(@fs, $2);
	}
}

foreach $t (@ts)
{
	$fn = 'spar'.ucfirst($t).'.c';
	open F, '>'.$fn;
	foreach my $l2 (@ls)
	{
		$l = $l2;
		if( $l =~ /\s*\/\/\s*main/ ||  $l =~ /\s*int\s*main/ )
		{
			$l = '';
			@ls = ();
		}
		$l =~ s/\/\/\s*Arbitrary data type\s*[\r\n]*//g;
		$l =~ s/\#define\s+sparType.*/\/\/ Do not edit\! Automatically\-generated file/g;
		$l =~ s/sparType/$t/g;
		$l =~ s/spar([\s\*\)])/'spar'.ucfirst($t).$1/eg;
		$l =~ s/(\}\s*spar)(\;)/$1.ucfirst($t).$2/eg;
		foreach $f (@fs)
		{
			if( $f =~ /^spar(.*)$/ )
			{
				$nf = 'spar'.ucfirst($t).$1;
			}
			$l =~ s/^$f(\s*\()/$nf.$1/eg;
			$l =~ s/(\s+)$f(\s*\()/$1.$nf.$2/eg;
		}
		print F $l;
	}
	close F;
}
