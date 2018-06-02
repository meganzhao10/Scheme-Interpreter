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
;;         , , list, and append.
;;                      max         modulo
;;                   min         floor
;;                   abs         ceiling
;;                     gcd         truncate
;;                     lcm         round


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
        (evaluationError "Input for 'zero?' has to be a number"))))

;Equal? recursively compares the contents of pairs, vectors, and strings
;applying eqv? on other objects such as numbers and symbols.
;since we have eq? already, we will use eq? instead as -
;"Eq? and eqv? are guaranteed to have the same behavior
;on symbols, booleans, the empty list, pairs,
;procedures, and non-empty strings and vectors."
;refer to R5RS 
(define equal?
  (lambda (x y)
    (if (and (pair? x) (pair? y))
        (letrec ((helper
                  (lambda (x y)
                           (if (not (equal? (car x) (car y)))
                               #f
                               (equal? (cdr x) (cdr y))))))
          (helper x y))
        (eq? x y))))


(define positive?
  (lambda (x)
    (if (number? x)
        (> x 0)
        (evaluationError "Input for 'positive?' has to be a number"))))


(define negative?
  (lambda (x)
    (if (number? x)
        (< x 0)
        (evaluationError "Input for 'negative?' has to be a number"))))

(define floor)

(define modulo
  (lambda (x y)
    (if (and (integer? x) (integer? y))
        (- x (* y (floor (/ x y)))))
        (evaluationError "Inputs for 'modulo' has to be integers")))            



;take in integer
(define even?
  (lambda (x)
    (if (integer? x)
        (zero? (modulo x 2))
        (evaluationError "Input for 'even?' has to be a integer"))))

(define odd?
  (lambda (x)
    (not (even? x))))
        

