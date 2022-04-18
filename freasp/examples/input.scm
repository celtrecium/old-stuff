(defun not (x)
  (if (> x 0) (return 0) (return 1)))

(defun input ()
  ((defvar ch)
   (defvar str (getchar))
   (print str)
   
   (while (not (= (set ch (getchar)) "\n"))
          ((set str (+ str ch))
           (print ch)))
   
   (print "\n")
   (return str)))
