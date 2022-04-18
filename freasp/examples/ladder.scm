(defvar i)
(defvar sym "#")
(defvar str sym)

(while (< i 8)
       ((print str "\n")
        (set str (+ str sym))
        (set i (+ i 1))))
