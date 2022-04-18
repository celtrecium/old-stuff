(defvar PI 3.14159)


(defun pow (a b)
  ((defvar x a)
   (while (> b 1)
          ((set a (* a x))
           (set b (- b 1))))
   (return a)))

(defun abs (x)
  ((if (< x 0)
       (set x (* x -1)))
   (return x)))

(defun sqrt (x)
  ((defvar S x) (defvar a 1) (defvar b x)
   (while (> (abs (- a b)) 0.00001)
          ((set a (/ (+ a b) 2))
           (set b (/ S a))))
   (/ (+ a b) 2)))

(defun sin (x)
  ((defvar e 0.00001) (defvar stx x) (defvar sint)
   (defvar i 1) (defvar fct 1) (defvar z 1)
   (while (> (/ stx fct) e)
          ((set sint (+ sint (* z (/ stx fct))))
           (set i (+ i 2))
           (set stx (* stx x x))
           (set fct (* fct (- i 1) i))
           (set z (* z -1))))
   (return sint)))

(defun cos (x) (sqrt (- 1 (pow (sin x) 2))))

(defun fact (x)
  ((defvar ret 1)
   (while (> x 0)
          ((set ret (* ret x))
           (set x (- x 1))))
   (return ret)))

;; --------------------------------------------------------

(print "sqrt (49)   test: " (sqrt 49) "\n"
       "pow (2, 4)  test: " (pow 2 4) "\n"
       "sin (45°)   test: " (sin (* 45 (/ PI 180))) "\n"
       "cos (45°)   test: " (cos (* 45 (/ PI 180))) "\n"
       "fact (5)    test: " (fact 5) "\n")
