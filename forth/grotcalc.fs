: calc
  "Enter first number: " print input integer
  "Enter second number: " print input integer
  "Enter operation(+, -, *, /): " print input

  dup "+" cmp if
    drop + "Sum: " swap cat
  else
    dup "-" cmp if
      drop - "Different: " swap cat
    else
      dup "*" cmp if
        drop * "Product: " swap cat
      else
        dup "/" cmp if
          drop / "Quotient: " swap cat 
        else
          drop drop drop "Unknown operation!"
        then
      then
    then
  then

  print nl
;
