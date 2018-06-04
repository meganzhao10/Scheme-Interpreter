(define list
  (lambda x
        x))

(define list?
    (lambda (lst)
          (cond
            ((null? lst) #t)
            ((pair? lst) 
            (flist? (cdr lst)))
            (else #f))))

(define length
    (lambda (lst)
          (length lst)))

(define caar
    (lambda (lst)
          (car (car lst))))

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

(define list-ref
    (lambda (lst num)
          (if (= num 0)
                (car lst)
                (flist-ref (cdr lst) (- num 1)))))

(define list-tail
    (lambda (lst num)
          (if (= num 0)
                lst
                (flist-tail (cdr lst) (- num 1)))))

(define member
    (lambda (v lst)
          (cond
            ((null? lst) #f)
            ((equal? v (car lst)) lst)
            (else (fmember v (cdr lst))))))

(define assq
    (lambda (v lst)
          (cond
            ((null? lst) #f)
            ((eq? v (caar lst)) (car lst))
            (else (fassq v (cdr lst))))))

(define reverse
    (lambda (lst)
          (letrec ((helper
            (lambda (lst result)
                (if (null? lst)
                    result
                    (helper (cdr lst)
                        (cons (car lst) result))))))
            (helper lst '()))))

;;doesn't work yet
;;(define ffilter
;;  (lambda (f lst)
;;      (cond ((null? lst) (quote ()))
;;                ((eqv? #t (f (car lst)))
;;                           (cons (car lst) (ffilter (cdr lst))))
;;                                     (else (ffilter (cdr lst))))))
;;                                             
;;
;;                                                        
;;
;;
