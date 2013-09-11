# Data types
@ts = ('char', 'int', 'long', 'float', 'double');

# Read template
open F, 'sparTemplate.h';
@ls = <F>;
close F;

# Print template headers
open G, '>'.'../spar.h';
foreach $l (@ls)
{
	if( $l =~ /^\s*\#include/ )
	{
		print G $l;
	}
}

print G "\n";
print G '// Do not edit!'."\n";
print G '// Automatically-generated file from sparTemplate.h'."\n";

$x = join('',@ls);
if( $x =~ /(\/*[^\r\n]*[\r\n]*typedef[^\{]+\{[^\}]+\}[^\r\n]*[\r\n]*)/ )
{
	$td = $1;
}

# Read functions
foreach my $l (@ls)
{
	if( $l =~ /^\s*(void|int|double|sparType|spar)\s*\*?\s*(spar[^\(]*?)\s*\(/ )
	{
		$f = $2;
		push(@fs, $2);
		$l =~ s/([\r\n]+)$/\;$1/g;
		push(@gs, $ll.$l);
	}
	$ll = $l;
}

# Append structs and function headers
foreach my $t (@ts)
{
	$l = $td.join('', @gs);
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
		$l =~ s/\"$f(\s+)/'"'.$nf.$1/eg;
		$l =~ s/(\s+)$f(\s*\()/$1.$nf.$2/eg;
	}
	print G "\n".$l;
}

foreach my $t (@ts)
{
	# Read template
	open F, 'sparTemplate.h';
	@ls = <F>;
	close F;

	# Remove structs
	$x = join('',@ls);
	$x =~ s/\/*[^\r\n]*[\r\n]*typedef[^\{]+\{[^\}]+\}[^\r\n]*[\r\n]*//g;
	@ls = split(/(?<=\n)/, $x);

	foreach my $l (@ls)
	{
		if( $l =~ /\s*\/\/\s*main/ ||  $l =~ /\s*int\s*main/ )
		{
			$l = '';
			@ls = ();
		}
		$l =~ s/^\s*\#include.*[\r\n]*//g;
		$l =~ s/\/\/\s*Arbitrary data type\s*[\r\n]*//g;
		$l =~ s/\#define\s+sparType.*[\r\n]*//g;
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
			$l =~ s/\"$f(\s+)/'"'.$nf.$1/eg;
			$l =~ s/(\s+)$f(\s*\()/$1.$nf.$2/eg;
		}
		print G $l;
	}
}

close G;

