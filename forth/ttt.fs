var board
var win-pos
var counter

: inv if 0 else 1 then ;

: draw-board
  0 while dup 9 < do
    dup 3 / dup 1 = swap 2 = or if
    dup 3 / dup 1 = swap 2 = or if
      "\e[D \n---+---+---\n" print
    then
      "\e[D \n---+---+---\n" print
    then
     
    dup board @@ " " swap cat " |" cat print
    1 +
  loop
  
  "\e[D \n" print
  drop ;

: init-board
  9 alloc board !
  24 alloc win-pos !

  0 while dup 9 < do
    dup " " swap board !!
    1 +      
  loop drop 
  
  0 1 2
  3 4 5
  6 7 8
  0 3 6
  1 4 7
  2 5 8
  0 4 8
  2 4 6

  23 counter ! while counter @ -1 > do
    counter @ win-pos !!
    counter @ 1 - counter !
  loop ;

: set-sym
  swap 3 * + dup board @@ " " cmp if
    board !!
    1
  else
    drop drop 0
  then ;

: get-input
  "Enter X pos: \e[K" print input integer
  "Enter Y pos: \e[K" print input integer ;

: check-input-values
  dup dup 2 > swap 0 < or if 
    0
  else 
    swap dup dup 2 > swap 0 < or if
      swap 0
    else 1 then
  then ;

: check-winner 
  0 counter ! while counter @ 8 < do
    3 counter @ *     win-pos @@ board @@
    3 counter @ * 1 + win-pos @@ board @@
    3 counter @ * 2 + win-pos @@ board @@

    cat cat dup dup dup

    "XXX" cmp swap "OOO" cmp or if
      10 counter !
      0 swap @@ swap drop
    else
      drop drop
    then

    counter @ 1 + counter !
  loop 
  
  counter @ 8 = if 0 then ; 

: game
  init-board
  draw-board
  
  while check-winner dup 0 = do
    swap dup if "X" else "O" then
    get-input 
    
    check-input-values if 
      set-sym if
        "Ok!\e[K\r" print
      else
        "The position is occupied!\e[K\r" print
        inv
      then
    else
      "The position out of range!\e[K\r" print
      drop drop drop
    then

    "\e[3A\n\e[K\n\e[K\n" print
    "\e[7A\r" print
    draw-board
    inv

    swap drop
  loop 
  
  dup " player won!\e[K\n" cat print
  swap drop ;

1 game
