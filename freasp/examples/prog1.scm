(defvar i 0)

(print "----- First demo (if in while cycle) -----\n\n")
   
(while (< i 11)
       (do (if (< i 5)
               (print i " less than 5\n")
               (print i " more than 5\n"))
           (set i (+ i 1))))

(set i 0)
(defvar j 0)

(print "\n----- Second demo (while in while cycle) -----\n\n")

(while (< i 4)
       (do (while (< j 4)
                  (do (print "i = " i "   j = " j "\n") (set j (+ j 1))))
           (set i (+ i 1)) (set j 0)))
