package CGI::Kwiki::Formatter;
$VERSION = '0.18';
use strict;
use base 'CGI::Kwiki', 'CGI::Kwiki::Privacy';
use CGI::Kwiki qw(:char_classes);

#        command_prep
sub process_order {
    return qw(
        table code function 
        header_1 header_2 header_3 header_4 header_5 header_6 
        escape_html
        lists 
        comment 
        horizontal_line
        paragraph 
        named_http_link no_http_link http_link
        no_mailto_link mailto_link
        no_wiki_link force_wiki_link wiki_link
        inline version negation
        bold italic underscore
    );
}


my $slide_num;
sub process 
   {
   $slide_num = 0;
   my ($self, $wiki_text) = @_;
   my $array = [];
   push @$array, $wiki_text;
   for my $method ($self->process_order) 
     {
     $array = $self->dispatch($array, $method);
     }
#  return $self->combine_chunks($array);
   return $self->combine_all($array);
   }

sub dispatch 
   {
   my ($self, $old_array, $method) = @_;
   return $old_array unless $self->can($method);
   my $new_array;
   for my $chunk (@$old_array) 
      {
      if (ref $chunk eq 'ARRAY') 
         {
         push @$new_array, $self->dispatch($chunk, $method);
         }
      else 
         {
         if (ref $chunk) 
            {
            push @$new_array, $chunk;
            }
         else 
            {
            push @$new_array, $self->$method($chunk);
            }
         }
      }
   return $new_array;
   }

sub combine_all
   {
   my ($self, $chunk_array) = @_;
   my $formatted_text = $self->combine_chunks ($chunk_array);
   my @new_chunk_array = $self->multi_col($formatted_text);
   return $self->combine_chunks (\@new_chunk_array);
   }

sub combine_chunks {
    my ($self, $chunk_array) = @_;
    my $formatted_text = '';
    for my $chunk (@$chunk_array) {
        $formatted_text .= 
          (ref $chunk eq 'ARRAY') ? $self->combine_chunks($chunk) :
          (ref $chunk) ? $$chunk :
          $chunk
    }
    return $formatted_text;
}

sub split_method {
    my ($self, $text, $regexp, $method) = @_;
    my $i = 0;
    map {$i++ % 2 ? \ $self->$method($_) : $_} split $regexp, $text;
}

sub user_functions {
    qw(
        SLIDESHOW_SELECTOR
        total_bullets
    );
}

sub isa_function {
    my ($self, $function) = @_;
    defined { map { ($_, 1) } $self->user_functions }->{$function} and
    $self->can($function)
}

sub function {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{\[\&(\w+\b.*?)\]},
        'function_format',
    );
}

sub function_format {
    my ($self, $text) = @_;
    my ($method, @args) = split;
    $self->isa_function($method) 
      ? $self->$method(@args)
      : "<!-- Function not supported here: $text -->\n";
}

sub total_bullets {
    my ($self) = @_;
    scalar(() = $self->database->load($self->cgi->page_id) =~ /^\* /gm);
}

sub SLIDESHOW_SELECTOR {
    my ($self) = @_;
    $slide_num = 1;
    my $page_id = $self->cgi->page_id;
    my $start = $self->loc('start');
    my $html = <<END;
<script src="javascript/SlideStart.js"></script>
<form>
${ \ CGI::popup_menu(
         -name => 'size',
         -values => [qw(640x480 800x600 1024x768 1280x1024 1600x1200), $self->loc('fullscreen')]
     )
 }
<input type="button" name="button" value="$start" onclick="startSlides()">
<input type="hidden" name="action" value="START">
<input type="hidden" name="page_id" value="$page_id">
</form>
END
    $html;
}

sub TRANSCLUDE_HTTP_BODY {
    my ($self, $url) = @_;
    require LWP::Simple;
    my $html = LWP::Simple::get($url)
      or return '';
    $html =~ s#.*<body>(.*)</body>.*#$1#is;
    \ $html;
}


#sub command_prep
#   {
#   my ($self, $text) = @_;
#   my $newtext;
#   while ($text =~ s/^(.*\n)//)
#      {
#      my $data = $1;
#      $newtext .= $data;
#      $newtext .= "\n" if ($data =~ /^::/);
#      }
#   return ($newtext);
#   }

sub multi_col
   {
   my ($self, $text) = @_;
   my @array;

#  while ($text =~ /(.*?)(^::col1\s*(\d*)\s*\n.*)/ms)
   while ($text =~ /(.*?)(^::col1(?:\s+(\d+))?(?:\s+(\d+))?\s*\n.*)/ms)
      {
      push @array, $1;
      my $twocol;
      ($twocol, $text) = $self->parse_multi_col($2, $3, $4);
      push @array, $twocol;
      }
   push @array, $text if length $text;
   return @array;
   }

sub parse_multi_col
   {
   my ($self, $text, $border, $tablewidth) = @_;
   my ($rows, $row);
   my $currcol = 0;
   my @colwidths = ();
   while ($text =~ s/^(.*)\n//)
      {
      my $data = $1;
      last if $data =~ m/^::end/;
      if ($data =~ m/^::col(\d)/)
         {
         $currcol = $1 - 1;
         push @{$rows}, [] if !$currcol;
         ($row = $rows->[@{$rows} - 1]) if !$currcol;
         next;
         }
      if ($data =~ m/^::colwidths\s*(.*)/)
         {
         @colwidths = split /\s+/, $1;
         next;
         }
      $row->[$currcol] .= $data;
      }
   return ($self->format_multi_col($rows, $border, $tablewidth, @colwidths), $text);
   }

sub format_multi_col 
   {
   my ($self, $rows, $border, $tablewidth, @colwidths) = @_;
   my $cols = 0;
   map {$cols = @{$_} if @{$_} > $cols;} (@{$rows});

   my $ave_cell_width = 100/$cols;
   if (! scalar @colwidths)
      {
      map {$colwidths[$_] = $ave_cell_width} (0..$cols-1);
      }

   $tablewidth = $tablewidth ? qq{width="$tablewidth%"} : "";
   $border = $border || "0";
   my $table = qq{<table class="multicol_table" border="$border" $tablewidth>\n};

   for my $row (@$rows) 
      {
      $table .= qq{<tr class="multicol_row" valign="top">\n};
      for my $colidx (0 .. $cols-1)
         {
         my $width = $colwidths[$colidx] ? qq{width="$colwidths[$colidx]%"} : "";
         $table .= qq{<td class="multicol_cell" $width>$row->[$colidx]</td>\n};
         }
      $table .= qq{</tr>\n};
      }
   $table .= qq{</table>\n};
   return \$table;
   }


sub table 
   {
   my ($self, $text) = @_;
   my @array;

   while ($text =~ /(.*?)(^\|[^\n]*\|\n.*)/ms) 
      {
      push @array, $1;
      my $table;
      ($table, $text) = $self->parse_table($2);
      push @array, $table;
      }
   push @array, $text if length $text;
   return @array;
   }


sub parse_table 
   {
   my ($self, $text) = @_;
   my $error = '';
   my $rows;
   while ($text =~ s/^(\|(.*)\|\n)//) 
     {
     $error .= $1;
     my $data = $2;
     my $row = [];
     for my $datum (split /\|/, $data) 
        {
        $datum =~ s/^\s*(.*?)\s*$/$1/;
        if ($datum =~ s/^<<(\S+)$//) 
           {
           my $marker = $1;
           while ($text =~ s/^(.*\n)//) 
              {
              my $line = $1;
              $error .= $line;
              if ($line eq "$marker\n") 
                 {
                 $marker = '';
                 last;
                 }
              $datum .= $line;
              }
           if (length $marker) 
              {
              return ($error, $text);
              }
           }
        push @$row, $datum;
        }
     push @$rows, $row;
     }
   return ($self->format_table($rows), $text);
   }

sub format_table {
    my ($self, $rows, $special) = @_;
    my $cols = 0;
    for (@$rows) {
        $cols = @$_ if @$_ > $cols;
    }
    my $atts = $special ? qq{border="0" width="90%"} : qq{border="1"};
    my $table = qq{<blockquote>\n<table $atts>\n};
    for my $row (@$rows) {
        $table .= qq{<tr valign="top">\n};
        for (my $i = 0; $i < @$row; $i++) {
            my $colspan = '';
            if ($i == $#{$row} and $cols - $i > 1) {
                $colspan = ' colspan="' . ($cols - $i) . '"';
            }
            my $cell = $self->escape_html($row->[$i]);
            $cell = qq{<pre>$cell</pre>\n}
              if $cell =~ /\n/;
            $cell = '&nbsp;' unless length $cell;
            $table .= qq{<td$colspan>$cell</td>\n};
        }
        $table .= qq{</tr>\n};
    }
    $table .= qq{</table></blockquote>\n};
    return \$table;
}

sub no_wiki_link {
    my ($self, $text) = @_;

    $self->split_method($text,
        qr{!([$UPPER](?=[$WIKIWORD]*[$UPPER])(?=[$WIKIWORD]*[$LOWER])[$WIKIWORD]+)},
        'no_wiki_link_format',
    );
}

sub no_wiki_link_format {
    my ($self, $text) = @_;
    return $text;
}

sub wiki_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{([$UPPER](?=[$WIKIWORD]*[$UPPER])(?=[$WIKIWORD]*[$LOWER])[$WIKIWORD]+)},
        'wiki_link_format',
    );
}

sub force_wiki_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(?<!\!)\[([$ALPHANUM\-:]+)\]},
        'wiki_link_format',
    );
}

sub wiki_link_format 
   {
   my ($self, $text) = @_;
#   my $script = $self->script;
   my $script = $self->{script_override} ? $self->{script_override} : $self->script;

   my $url = $self->escape($text);
   my $wiki_link = qq{<a href="$script?$url">$text</a>};

   if (not $self->database->exists($text)) 
      {
      $wiki_link =~ s/<a/<a class="empty"/;
      }
   elsif (not $self->is_readable($text)) 
      {
   	$url = $self->escape($self->loc("KwikiPrivatePage"));
      $wiki_link = qq{<a class="private" href="$script?$url">$text</a>};
      }

#print STDERR "CCC: wiki_link_format: script=$script url=$url text=$text wiki_link=$wiki_link :CCC\n";

   return $wiki_link;
   }

sub no_http_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(!(?:https?|ftp|irc):\S+?)}m,
        'no_http_link_format',
    );
}

sub no_http_link_format {
    my ($self, $text) = @_;
    $text =~ s#!##;
    return $text;
}

sub http_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{((?:https?|ftp|irc):\S+?(?=[),.:;]?\s|$))}m,
        'http_link_format',
    );
}

sub http_link_format {
    my ($self, $text) = @_;
#print STDERR "CCC: http_link_format text=$text :CCC\n";
    if ($text =~ /^http.*\.(?i:jpg|gif|jpeg|png)$/) {
        return $self->img_format($text);
    }
    else {
        return $self->link_format($text);
    }
}

sub no_mailto_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(![$ALPHANUM][$WORD\-\.]*@[$WORD][$WORD\-\.]+)}m,
        'no_mailto_link_format',
    );
}

sub no_mailto_link_format {
    my ($self, $text) = @_;
    $text =~ s#!##;
    return $text;
}

sub mailto_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{([$ALPHANUM][$WORD\-\.]*@[$WORD][$WORD\-\.]+)}m,
        'mailto_link_format',
    );
}

sub mailto_link_format {
    my ($self, $text) = @_;
    my $dot = ($text =~ s/\.$//) ? '.' : '';
    qq{<a href="mailto:$text">$text</a>$dot};
}

sub img_format {
    my ($self, $url) = @_;
    return qq{<img src="$url">};
}

sub link_format {
    my ($self, $text) = @_;
    $text =~ s/(^\s*|\s+(?=\s)|\s$)//g;
    my $url = $text;
    $url = $1 if $text =~ s/(.*?) +//;
    $url =~ s/https?:(?!\/\/)//;
#print STDERR "CCC: link_format text=$text url=$url:CCC\n";
    return qq{<a href="$url">$text</a>};
}

sub named_http_link {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(?<!\!)\[([^\[\]]*?(?:https?|ftp|irc):\S.*?)\]},
        'named_http_link_format',
    );
}

sub named_http_link_format {
    my ($self, $text) = @_;
    if ($text =~ m#(.*)(?:https?|ftp|irc):(\S+)(.*)#) {
        $text = "$2 $1$3";
    }
    return $self->link_format($text);
}

sub version {
    my ($self, $text) = @_;
    $text =~ s#(?<!\!)\[\#\.\#\]#$CGI::Kwiki::VERSION#g;
    return $text;
}

sub inline {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(?<!\!)\[=(.*?)\]},
        'inline_format',
    );
}

sub inline_format {
    my ($self, $text) = @_;
    "<tt>$text</tt>";
}

sub negation {
    my ($self, $text) = @_;
    $text =~ s#\!(?=\[)##g;
    return $text;
}

sub bold {
    my ($self, $text) = @_;
    $text =~ s#(?<![$WORD])\*(\S.*?\S)\*(?![$WORD])#<b>$1</b>#g;
    return $text;
}

sub italic {
    my ($self, $text) = @_;
    $text =~ s#(?<![$WORD<])/(\S.*?\S)/(?![$WORD])#<em>$1</em>#g;
    return $text;
}

sub underscore {
    my ($self, $text) = @_;
    $text =~ s#(?<![$WORD])_(\S.*?\S)_(?![$WORD])#<u>$1</u>#g;
    return $text;
}

sub code {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(^ +[^ \n].*?\n)(?-ms:(?=[^ \n]|$))}ms,
        'code_format',
    );
}

sub code_format {
    my ($self, $text) = @_;
    $self->code_postformat($self->code_preformat($text));
}

sub code_preformat {
    my ($self, $text) = @_;
    my ($indent) = sort { $a <=> $b } map { length } $text =~ /^( *)\S/mg;
    $text =~ s/^ {$indent}/ /gm;
    return $self->escape_html($text);
}

sub code_postformat {
    my ($self, $text) = @_;
    return "<blockquote><pre>$text</pre></blockquote>\n";
}

sub escape_html {
    my ($self, $text) = @_;
    $text =~ s/&/&amp;/g;
    $text =~ s/</&lt;/g;
    $text =~ s/>/&gt;/g;
    $text;
}

sub lists {
    my ($self, $text) = @_;
    my $switch = 0;
    return map {
        my $level = 0;
        my @tag_stack;
        if ($switch++ % 2) {
            my $text = '';
            my @lines = /(.*\n)/g;
            for my $line (@lines) {
                $line =~ s/^([0\*]+) //;
                my $new_level = length($1);
                my $tag = ($1 =~ /0/) ? 'ol' : 'ul';
                if ($new_level > $level) {
                    for (1..($new_level - $level)) {
                        push @tag_stack, $tag;
                        $text .= "<$tag>\n";
                    }
                    $level = $new_level;
                }
                elsif ($new_level < $level) {
                    for (1..($level - $new_level)) {
                        $tag = pop @tag_stack;
                        $text .= "</$tag>\n";
                    }
                    $level = $new_level;
                }
                $text .= "<li>$line";
            }
            for (1..$level) {
                my $tag = pop @tag_stack;
                $text .= "</$tag>\n";
            }
            $_ = $self->lists_format($text);
        }
        $_;
    }
    split m!(^[0\*]+ .*?\n)(?=(?:[^0\*]|$))!ms, $text;
}

sub lists_format {
    my ($self, $text) = @_;
    return $text;
}

sub paragraph 
   {
   my ($self, $text) = @_;
   my $switch = 0;
   return map 
      {
      unless ($switch++ % 2) {$_ = $self->paragraph_format($_);}
      $_;
      }  split m!(\n\s*\n)!ms, $text;
   }

sub paragraph_format 
   {
   my ($self, $text) = @_;
   return '' if $text =~ /^[\s\n]*$/;
   return $text if $text =~ /^<(o|u)l>/i;
   return $text if $text =~ /^::/;
   return "<p>\n$text\n</p>\n";
   }

sub horizontal_line {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{(^----+\n)}m,
        'horizontal_line_format',
    );
}

sub horizontal_line_format {
    my ($self) = @_;
    my $text = "<hr>\n";
    if ($slide_num) {
        my $page_id = $self->cgi->page_id;
	my $go = $self->loc('Go');
        $text .= qq{<a target="SlideShow" href="index.cgi?action=slides&page_id=$page_id&slide_num=$slide_num">$go</a>\n};
        $slide_num++;
    }
    return $text;
}

sub comment {
    my ($self, $text) = @_;
    $self->split_method($text,
        qr{^\# (.*)\n}m,
        'comment_line_format',
    );
}

sub comment_line_format {
    my ($self, $text) = @_;
    return "<!-- $text -->\n";
}

for my $num (1..6) {
    no strict 'refs';
    *{"header_$num"} = 
    sub {
        my ($self, $text) = @_;
        $self->split_method($text,
            qr{^={$num} (.*?)(?: ={$num})?\n}m,
            "header_${num}_format",
        );
    };
    *{"header_${num}_format"} = 
    sub {
        my ($self, $text) = @_;
        return "<h$num>$text</h$num>\n";
    };
}

1;

__END__

=head1 NAME 

CGI::Kwiki::Formatter - Formatter Base Class for CGI::Kwiki

=head1 DESCRIPTION

See installed kwiki pages for more information.

=head1 AUTHOR

Brian Ingerson <INGY@cpan.org>

=head1 COPYRIGHT

Copyright (c) 2003. Brian Ingerson. All rights reserved.

This program is free software; you can redistribute it and/or modify it
under the same terms as Perl itself.

See http://www.perl.com/perl/misc/Artistic.html

=cut
