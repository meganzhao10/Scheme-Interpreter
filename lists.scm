;More built-in functions to manipulate lists.
;refer to R5RS for specification
;using only special forms and primitives (e.g., car, cdr, cons, null?, pair?, and apply)
;and also other functions we have written in math.scm


(load "math.scm")

;caar        ;; (caar '((1 2) (3 4) (5 6) (7 8)))      ==> 1
;cadr        ;; (cadr '((1 2) (3 4) (5 6) (7 8)))      ==> (3 4)
;...                                           
;cddddr      ;; (cddddr '((1 2) (3 4) (5 6) (7 8)))    ==> ()
;
;list        ;; (list 1 2 3)                           ==> (1 2 3)               
;list?       ;; (cons 1 2)                             ==> #f
;length      ;; (length '(1 2 3))                      ==> 3
;list-ref    ;; (list-ref '(0 1 2 3 4) 3)              ==> 3 
;list-tail   ;; (list-tail '(0 1 2 3 4) 3)             ==> (3 4)
;member      ;; (member 'a '(1 2 3 a b c))             ==> (a b c)
;assq        ;; (assq 2 '((0 a) (1 b) (2 c) (3 d)))    ==> (2 c)
;append      ;; (append '(a b) '(c d))                 ==> (a b c d)
;reverse     ;; (reverse '(1 2 3))                     ==> (3 2 1)
;
;map         ;; (map (lambda (x) (* x x)) '(1 2 3))    ==> (1 4 9)
;filter      ;; (filter odd? '(1 2 3))                 ==> (1 3)
;foldl       ;; (foldl cons '() '(1 2 3))              ==> (3 2 1)
;foldr       ;; (foldr cons '() '(1 2 3))              ==> (1 2 3)

;...

(define caar
    (lambda (lst)
      (if (pair? lst)
          (if (pair? (car lst))
              (car (car lst))
              (evaluationError "caar expects pair as input for (car arg)"))
          (evaluationError "caar expects pair as input"))))

(define cadr
  (lambda (lst)
    (if (pair? lst)
        (if (pair? (cdr lst))
            (car (cdr lst))
            (evaluationError "cadr expects pair as input for (cdr arg)"))
        (evaluationError "cadr expects pair as input"))))



(define list
  (lambda x x))

            
       
(define list?
    (lambda (lst)
      (if (pair? lst)
          (list? (cdr lst))
          (null? lst))))

     

(define length
    (lambda (lst)
      (if (list? lst)
          (if (null? lst)
              0
              (+ 1 (length (cdr lst))))
          (evaluationError "length expects list as input"))))
        
      
(define list-ref
    (lambda (lst num)
      (if (and (list? lst) (>= num 0))
          (if (= num 0)
                (car lst)
                (list-ref (cdr lst) (- num 1)))
          (evaluationError "list-ref expects list and non-negative integer as input"))))



(define list-tail
    (lambda (lst num)
      (if (and (list? lst) (>= num 0))
          (if (= num 0)
                lst
                (list-tail (cdr lst) (- num 1)))
          (evaluationError "list-tail expects list and non-negative integer as input"))))


(define member
    (lambda (v lst)
      (if (list? lst)
          (cond
            ((null? lst) #f)
            ((equal? v (car lst)) lst)
            (else
             (member v (cdr lst))))
          (evaluationError "member expects list as one of the input"))))


(define assq
    (lambda (v lst)
      (if (list? lst)
          (cond
            ((null? lst) #f)
            ((eq? v (caar lst)) (car lst))
            (else (assq v (cdr lst))))
          (evaluationError "assp expects list as one of the input"))))
            
(define append
    (lambda x
        (letrec ((helper (lambda (x y)
                    (if (null? y)
                        x
                        (letrec ((helper2 (lambda (x)
                                (if (null? x)
                                    (helper (car y) (cdr y))
                                    (if (pair? x)
                                        (cons (car x) (helper2 (cdr x)))
                                        (evaluationError "append expects pairs as input")
                                        )))))
                            (helper2 x))))))
            (helper (quote ()) x))))            
           
  
(define reverse
    (lambda (lst)
      (if (list? lst)
          (letrec ((helper
                    (lambda (lst result)
                      (if (null? lst)
                          result
                          (helper (cdr lst)
                                  (cons (car lst) result))))))
            (helper lst (quote ())))
          (evaluationError "reverse expects a list as input"))))


(define map
  (lambda (f lst)
    (if (list? lst)
        (if (null? lst)
            (quote ())
            (cons (f (car lst)) (map f (cdr lst))))
        (evaluationError "map expects list as one of its input"))))



(define cadr
    (lambda (lst)
          (car (cdr lst))))

(define cdar
    (lambda (lst)
          (cdr (car lst))))

(define cddr
    (lambda (lst)
          (cdr (cdr lst))))

(define caaar
    (lambda (lst)
          (car (car (car lst)))))

(define caadr
    (lambda (lst)
          (car (car (cdr lst)))))

(define cadar
    (lambda (lst)
          (car (cdr (car lst)))))

(define caddr
    (lambda (lst)
          (car (cdr (cdr lst)))))

(define cdaar
    (lambda (lst)
          (cdr (car (car lst)))))

(define cdadr
    (lambda (lst)
          (cdr (car (cdr lst)))))

(define cddar
    (lambda (lst)
          (cdr (cdr (car lst)))))

(define cdddr
    (lambda (lst)
          (cdr (cdr (cdr lst)))))



;taken from class lecture
(define filter
  (lambda (f lst)
    (cond ((null? lst)
           (quote ()))
          ((f (car lst))
           (cons (car lst)
                  (filter f (cdr lst))))
          (else (filter f (cdr lst))))))



;taken from class lecture
(define foldl
  (lambda (f init lst)
    (if (null? lst)
        init
        (foldl f
               (f (car lst) init)
               (cdr lst)))))

;;taken from class lecture
(define foldr
  (lambda (f init lst)
    (if (null? lst)
        init
        (f (car lst)
           (foldr f init (cdr lst))))))
