;;2 Library procedures: =, modulo, zero?, equal?, list, and append.;More built-in functions with regards to arithmetic.
;using only special forms and primitives
;that we've implemented (e.g., +, -, *, /, and <=)

;;6.more arithmetic procedures
;;=        zero?             max         modulo
;;>=       positive?         min         floor
;;<        negative?         abs         ceiling
;;>        even?             gcd         truncate
;;         odd?              lcm         round

;;need to do
;;         modulo, , equal?, list, and append.
;;                      max         modulo
;;       positive?         min         floor
;;        negative?         abs         ceiling
;;        even?             gcd         truncate
;;         odd?              lcm         round


(define not
    (lambda (x)
        (if x
            #f
            #t)))

(define =
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (<= x y ) (<= y x))
        (evaluationError "Inputs for '=' have to be numbers"))))

(define >=
  (lambda (x y)
    (if (and (number? x) (number? y))
        (or (= x y) (not (<= x y)))
        (evaluationError "Inputs for '>=' have to be numbers"))))
         
(define <
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (not (= x y)) (<= x y))
        (evaluationError "Inputs for '<' have to be numbers"))))
        
    
(define >
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (not (= x y)) (>= x y))
        (evaluationError "Inputs for '>' have to be numbers"))))


(define zero?
  (lambda (x)
    (if (number? x)
        (= x 0)
        (evaluationError "Input for 'zero?' have to be a number"))))







