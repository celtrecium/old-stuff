(defun abs (x)
  ((if (< x 0)
       (set x (* x -1)))
   (return x)))

(defun not (x)
  (if (> x 0) (return 0) (return 1)))

(defun line (x1 y1 x2 y2 symbol)
  ((defvar deltaX (abs (- x2 x1)))
   (defvar deltaY (abs (- y2 y1)))
   (defvar signX)
   (defvar signY)

   (if (< x1 x2)
       (set signX 1)
       (set signX -1))

   (if (< y1 y2)
       (set signY 1)
       (set signY -1))

   (defvar error (- deltaX deltaY))
   (defvar error2)

   (while (+ (not (= x1 x2)) (not (= y1 y2)))
          ((print "\e[" y1 ";" x1 "H" symbol)
           (set error2 (* error 2))
           
           (if (> error2 (- 0 deltaY))
               ((set error (- error deltaY))
                (set x1 (+ x1 signX))))

           (if (< error2 deltaX)
               ((set error (+ error deltaX))
                (set y1 (+ y1 signY))))))))
