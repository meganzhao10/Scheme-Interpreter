;;Requred - 2 Library procedures: =, modulo, zero?, equal?;
; Please refer to list.scm for list, and append.;

;More built-in functions with regards to arithmetic.
;using only special forms and primitives
;that we've implemented (e.g., +, -, *, /, and <=)

;;6.more arithmetic procedures
;;=        zero?             max         modulo
;;>=       positive?         min         floor
;;<        negative?         abs         ceiling
;;>        even?             gcd         truncate
;;         odd?              lcm         round

;We consulted R5RS(link in readme) for specifications of procedures

(define =
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (<= x y ) (<= y x))
        (evaluationError "= expects numbers as input"))))

;to be used later
(define not
    (lambda (x)
        (if x
            #f
            #t)))

(define >=
  (lambda (x y)
    (if (and (number? x) (number? y))
        (or (= x y) (not (<= x y)))
        (evaluationError ">= expects numbers as input"))))
         
(define <
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (not (= x y)) (<= x y))
        (evaluationError "< expects numbers as input"))))
        
    
(define >
  (lambda (x y)
    (if (and (number? x) (number? y))
        (and (not (= x y)) (>= x y))
        (evaluationError "> expects numbers as input"))))


(define zero?
  (lambda (x)
    (if (number? x)
        (= x 0)
        (evaluationError "zero? expects a number as input"))))

;Equal? recursively compares the contents of pairs, vectors, and strings
;applying eqv? on other objects such as numbers and symbols.
;since we have eq? already, we will use eq? instead as -
;"Eq? and eqv? are guaranteed to have the same behavior
;on symbols, booleans, the empty list, pairs,
;procedures, and non-empty strings and vectors."
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
        (evaluationError "positive? expects a real number as input"))))


(define negative?
  (lambda (x)
    (if (number? x)
        (< x 0)
        (evaluationError "negative? expects a real number as input"))))

;Floor returns the largest integer not larger than x.
;print in format of float number as the behavior of Dr.Racket
(define floor
  (lambda (x)
    (if (number? x)
        (if (integer? x)
            x
            (letrec ((helper
                        (lambda (x)
                          (if (positive? x)
                              (if (< x 1)
                                  0.0
                                  (+ 1 (floor (- x 1))))
                              ;;case of negative
                              (if (> x -1)
                                  -1.0
                                  (- 1 (floor (+ x 1))))))))
                (helper x)))
        (evaluationError "floor expects a real number as input"))))
 
;Ceiling returns the smallest integer not smaller than x
;print in format of float number as the behavior of Dr.Racket
(define ceiling
  (lambda (x)
    (if (number? x)
        (if (integer? x)
            x
            (letrec ((helper
                        (lambda (x)
                          (if (positive? x)
                              (if (< x 1)
                                  1.0
                                  (+ 1 (ceiling (- x 1))))
                              ;;case of negative
                              (if (> x -1)
                                  ;Dr.Racket prints -0.0
                                  ;but for the sake of clarity
                                  ;we decide to print 0.0
                                  0.0
                                  (- 1 (ceiling (+ x 1))))))))
                (helper x)))
        (evaluationError "ceiling expects a real number as input"))))

;remainder
(define modulo
  (lambda (x y)
    (if (and (integer? x) (integer? y))
        (- x (* y (floor (/ x y)))))
        (evaluationError "modulo expects integers as input")))


;Truncate returns the integer closest to x whose absolute value
;is not larger than the absolute value of x
;print in format of float number as the behavior of Dr.Racket
(define truncate
  (lambda (x)
    (if (number? x)
        (if (integer? x)
            x
            (letrec ((helper
                        (lambda (x)
                          (if (positive? x)
                              (if (< x 1)
                                  0.0
                                  (+ 1 (truncate (- x 1))))
                              ;;case of negative
                              (if (> x -1)
                                  ;Dr.Racket prints -0.0
                                  ;but for the sake of clarity
                                  ;we decide to print 0.0
                                  0.0
                                  (- 1 (truncate (+ x 1))))))))
                (helper x)))
        (evaluationError "truncate expects a real number as input"))))

;Round returns the closest integer to x,
;rounding to even when x is halfway between two integers.
;print in format of float number as the behavior of Dr.Racket
(define round
  (lambda (x)
    (if (number? x)
        (cond ((integer? x) x)
              ((> x (+ (floor x) 0.5)) (ceiling x))
              (else (floor x)))
        (evaluationError "round expects a real number as input"))))
        
;take in integer
(define even?
  (lambda (x)
    (if (integer? x)
        (zero? (modulo x 2))
        (evaluationError "even? expects a integer as input"))))

(define odd?
  (lambda (x)
    (if (integer? x)
        (not (even? x))
        (evaluationError "odd? expects a integer as input"))))


;These procedures return the maximum or minimum of their arguments.
;currently take in two arguments as default
;will try to make them variadic.
(define max
  (lambda (x y)
    (if (and (number? x) (number? y))
        (if (>= x y)
            x
            y)
        (evaluationError "max expects real numbers as input"))))
(define min
  (lambda (x y)
    (if (and (number? x) (number? y))
        (if (<= x y)
            x
            y)
        (evaluationError "min expects real numbers as input"))))


(define abs
  (lambda (x)
    (if (number? x)
        (if (positive? x)
            x
            (* -1 x))
            (evaluationError "abs a expects real numbers as input"))))

;using idea of Euclid Algorithm
;because modulo is adopted and modulo assumed integer
;will only deal with integers in this function
;will try to make this variadic,
;but currently take in two arguments as default
;greatest common divisor of their arguments.
;The result is always non-negative.
;if no inputs or inputs are 0, return 0 as of DrRacket's behavior
(define gcd
  (lambda (x y)
    (if (and (integer? x) (integer? y))
           (if (< x y)
               (if (zero? (modulo y x))
                   (abs x)
                   (gcd (modulo y x) x))
               (if (zero? (modulo x y))
                   (abs y)
                   (gcd (modulo x y) y)))
           (evaluationError "gcd expects rational numbers as input"))))

;;need to test
(define lcm
  (lambda (x y)
    (if (and (integer? x) (integer? y))
        (cond ((or (zero? x) (zero? y)) 0)
              ((zero? (modulo x y)) x)
              ((zero? (modulo y x)) y)
              (else (/ (abs (* x y)) (gcd x y))))
        (evaluationError "lcm expects rational numbers as input"))))


;;6.more arithmetic procedures
;;=        zero?             max         modulo
;;>=       positive?         min         floor
;;<        negative?         abs         ceiling
;;>        even?             gcd         truncate
;;         odd?              lcm         round


(= 3 5)
(zero? -3)
(max 7 0)
(min 3 4)
;(eq? (quote s) 4)
(eq? 3 4)
(= 3 3.0)
       