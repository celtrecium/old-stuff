: println print "\n" print drop drop ;
: bool if drop 1 else drop 0 then ;

: fact
  dup 1 > if
    dup 1 - fact *
  else
    drop 1
  then ;

: ++ dup @ 1 + swap ! ;

: test
  while dup 10 < do
    swap
    dup
    
    while dup 10 < do
      dup print " " print
      1 +
    loop

    drop
    swap 1 +
    "\n" print
  loop

  drop drop ;
