;2.library procedure


(define list
  (lambda x x))


;(append) --> ()这个好像还不行
;还不能variadic
;jed课上的
(define append1
  (lambda (lst1 lst2)
    (if (and (pair? lst1) (pair? lst2))
        (letrec ((helper
              (lambda (reverse-me onto-here)
                (if (null? reverse-me)
                    onto-here
                    (helper (cdr reverse-me)
                            (cons (car reverse-me)
                                  onto-here))))))
      (helper (reverse lst1) lst2))
        (evaluationError "append expects pairs as input"))))
        
       
;;复杂一点的
;能variadic
(define append2
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
            (helper '() x))))

;;tests?
(define x '(1 2 3 4))
(define y '(a b c))
(append1 x y)
(append2 x y '(3))
