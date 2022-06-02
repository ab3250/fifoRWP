(define-library (delay)
(import
    (scheme base)
    (scheme red)    
    (chibi time))
(export delay-seconds
)
(begin
    (define (delay-seconds sec)
        (define start (current-seconds))
        (let timeloop ()    
            (if ( < (- (current-seconds) start) sec) (timeloop))))
      
            
            ))
